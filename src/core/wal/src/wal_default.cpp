#include "../include/wal_default.h"
#include "LRUCache11.hpp"

std::uint64_t babrat::wal::WALManager::firstIndex() {

    std::shared_lock lock(mutex_);

    checkState();

    return last_index_ == 0 ? 0 : first_index_;

};

std::uint64_t babrat::wal::WALManager::lastIndex() {

    std::shared_lock<std::shared_mutex> lock(mutex_);

    checkState();

    return last_index_;
};

void babrat::wal::WALManager::write(uint64_t index, const std::vector<uint8_t> &data) {
    std::unique_lock lock(mutex_);
    checkState();
    wbatch_.clear();
    wbatch_.write(index, data);
    writePack(&wbatch_);

}

void babrat::wal::WALManager::writeBatch(wal::WriteBatch* batch) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    checkState();
    if (batch->entries.empty()){
        return;
    }

    writePack(batch);
}

std::vector<uint8_t> babrat::wal::WALManager::read(uint64_t index) {

    std::shared_lock lock(mutex_);

    checkState();

    if (index == 0 || index < first_index_ || index > last_index_) {
        std::cout << last_index_<< std::endl;
        std::cout << first_index_<< std::endl;
        throw not_found_error();
    }

    auto seg = loadSegment(index);

    auto epos = seg->epos[index - seg->index];

    std::vector<uint8_t> edata(
            seg->ebuf.data() + epos.pos,
            seg->ebuf.data() + epos.end
    );

    return format_handler_->readFormat(edata);

}


void babrat::wal::WALManager::truncateRange(std::uint64_t low_limit, std::uint64_t upper_limit) {

    std::unique_lock lock(mutex_);

    checkState();

    if (low_limit == 0 || upper_limit == 0 || last_index_ == 0 ||
        low_limit < first_index_ || upper_limit > last_index_ ||
        low_limit > upper_limit) {
        throw out_of_range_error();
    }

    if (low_limit == first_index_ && upper_limit == last_index_) {
        return;
    }

    if (upper_limit < last_index_) {
        truncateBackIndex(upper_limit);
    }

    if (low_limit > first_index_) {
        truncateFrontIndex(low_limit);
    }

}

void babrat::wal::WALManager::truncateFront(uint64_t index)  {

    std::unique_lock lock(mutex_);

    checkState();

    truncateFrontIndex(index);
}

void babrat::wal::WALManager::truncateBack(uint64_t index)  {

    std::unique_lock lock(mutex_);

    checkState();

    truncateBackIndex(index);
}

void babrat::wal::WALManager::close() {

    std::unique_lock lock(mutex_);

    if (closed_){
        if (corrupt_){
            throw corrupt_error();
        }
        throw closed_file_error();
    }

    if (file_.is_open()) {
        file_.flush();
        file_.close();
    }

    closed_ = true;

    if (corrupt_){
        throw corrupt_error();
    }

}

void babrat::wal::WALManager::cacheClear() {

    std::unique_lock lock(mutex_);

    checkState();

    clearCacheSegments();
}

babrat::wal::WALManager::WALManager(const std::string &path, const babrat::wal::Options& options):
        options_(options),
        lruCache(options.getSegmentCacheSize()),
        format_handler_(FormatFactory::create(options.getFormat())){
    initPath(path);
    load();
}

babrat::wal::WALManager::~WALManager() noexcept {
    if (!closed_) {
        close();
    }

}


std::string babrat::wal::WALManager::segmentName(uint64_t index) const {
    return std::format("{:020d}", index);
}


void babrat::wal::WALManager::sync(){

    std::unique_lock<std::shared_mutex> lock(mutex_);

    checkState();

    file_.flush();

};

void babrat::wal::WALManager::writePack(babrat::wal::WriteBatch *batch) {

    for (auto i= 0; i < batch->entries.size(); i++) {

       /*     std::cout << "Checking entry " << i << ": " << batch->entries[i].index
                      << " vs expected " << (last_index_ + uint64_t(i+1))
                      << " (last_index=" << last_index_ << ")\n";
    */

        if (batch->entries[i].index != last_index_+uint64_t(i+1)) {
            throw boundary_index_error();
        }

    }

    auto seg= segments_.back();

    if (seg->ebuf.size() > options_.getSegmentSize()) {
        cycleSegment();
        seg = segments_.back();
    }

    size_t mark = seg->ebuf.size();

    auto datas = batch->datas;

    for (auto & entry : batch->entries) {

        auto [out, epos] = format_handler_->appendEntry(seg->ebuf,
                                                        entry.index,
                                                        {datas.begin(), datas.begin() + entry.size});

        seg->ebuf = out;
        seg->epos.emplace_back(epos);
        last_index_ = entry.index;

        if (seg->ebuf.size() >= options_.getSegmentSize()) {

            file_.write(reinterpret_cast<const char*>(seg->ebuf.data() + mark),
                        seg->ebuf.size() - mark);

            cycleSegment();
            seg = segments_.back();
            mark = 0;
        }

        datas.erase(datas.begin(), datas.begin() + entry.size);
    }

    if (seg->ebuf.size() - mark > 0) {

        file_.write(reinterpret_cast<const char*>(seg->ebuf.data() + mark),
                    seg->ebuf.size() - mark);

        last_index_ = batch->entries.back().index;

    }

    if (!options_.isNoSync()){
       file_.flush();
    }

    batch->clear();
}

void babrat::wal::WALManager::cycleSegment() {

    if (file_.is_open()){
        file_.flush();
        file_.close();
    }

    auto i =  segments_.size()-1;

    lruCache.insert(i, segments_[i]);

    uint64_t new_index = last_index_ + 1;

    auto new_seg = std::make_shared<Segment>(
            new_index, path_ / segmentName(new_index));


    file_.open(new_seg->path, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);

    if (!file_.is_open()) {
        throw std::runtime_error("failed to open new segment");
    }

    segments_.emplace_back(new_seg);

}


std::shared_ptr<babrat::wal::Segment> babrat::wal::WALManager::loadSegment(uint64_t index) {

    auto last_segment =  segments_.back();
    if (index >= last_segment->index) {
        return last_segment;
    }

    size_t seg_idx = findSegment(index);

    try{
        return lruCache.get(seg_idx);
    }
    catch(const lru11::KeyNotFound&){

        auto seg = segments_[seg_idx];

        if (seg->epos.empty()) {
            loadSegmentEntries(seg);
        }

        lruCache.insert(seg_idx, seg);


        return seg;

    }
}

size_t babrat::wal::WALManager::findSegment(std::uint64_t index) const {
    auto it = std::upper_bound(
            segments_.begin(),
            segments_.end(),
            index,
            [](uint64_t idx, const auto& seg) { return idx < seg->index; }
    );
    return (it == segments_.begin()) ? 0 : std::distance(segments_.begin(), it) - 1;
}

void babrat::wal::WALManager::load(){

    std::unique_lock<std::shared_mutex> lock(mutex_);

    std::vector<fs::directory_entry> files;
    for (const auto& entry : fs::directory_iterator(path_)) {
        files.push_back(entry);
    }

    int start_idx = -1;
    int end_idx = -1;

    for (const auto& entry : files) {
        if (!entry.is_regular_file()) continue;

        const std::string name = entry.path().filename().string();
        if (name.length() < 20) continue;

        try {
            uint64_t index = std::stoull(name.substr(0, 20));
            if (index == 0) continue;

            bool is_start = (name.length() == 26) && (name.substr(20) == ".START");
            bool is_end = (name.length() == 24) && (name.substr(20) == ".END");

            if (name.length() == 20 || is_start || is_end) {

                if (is_start) {
                    start_idx = static_cast<int>(segments_.size());
                } else if (is_end && end_idx == -1) {
                    end_idx = static_cast<int>(segments_.size());
                }

                auto segment = std::make_shared<Segment>(index, entry.path());
                segments_.push_back(segment);

            }
        } catch (...) {
            continue;
        }
    }


    if (segments_.empty()) {


        // Create a new log
        auto segment = std::make_shared<Segment>(1, path_ / segmentName(1));
        segments_.emplace_back(segment);

        first_index_ = 1;
        last_index_ = 0;

        file_.open(segments_[0]->path,  std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);

        if (!file_.is_open()) {
            throw std::runtime_error("failed to create segment file");
        }

        return;
    }

    // Handle START segments
    // Open existing log. Clean up log if START or END segments exists.
    if (start_idx != -1) {

        if (end_idx != -1) {
            throw std::runtime_error("log corrupt: both START and END segments exist");
        }

        // Delete all files leading up to START
        for (int i = 0; i < start_idx; i++) {
            if (!fs::remove(segments_[i]->path)) {
                throw std::runtime_error("error deleting file");
            }
        }

        segments_.erase(
                segments_.begin(),
                segments_.begin() + start_idx
        );

        // Rename the START segment
        fs::path org_path = segments_[0]->path;
        fs::path final_path = org_path;
        final_path.replace_extension("");
        fs::rename(org_path, final_path);
        segments_[0]->path = final_path;
    }


    if (end_idx != -1) {
        // Delete all files following END
        for (size_t i = segments_.size() - 1; i > end_idx; i--) {
            if (!fs::remove(segments_[i]->path)) {
                throw std::runtime_error("error deleting file");
            }
        }

        segments_.erase(segments_.begin() + end_idx + 1, segments_.end());

        if (segments_.size() > 1 &&
            segments_[segments_.size()-2]->index == segments_.back()->index) {
            segments_[segments_.size()-2] = segments_.back();
            segments_.pop_back();
        }

        fs::path org_path = segments_.back()->path;
        fs::path final_path = org_path;
        final_path.replace_extension("");
        fs::rename(org_path, final_path);
        segments_.back()->path = final_path;
    }

    std::sort(segments_.begin(), segments_.end(),
              [](const auto& a, const auto& b)
              { return a->index < b->index; });

    first_index_ = segments_[0]->index;

    // Open last segment
    auto last_seg = segments_.back();

    file_.open(last_seg->path,  std::ios::binary |std::ios::out | std::ios::app);

    if (!file_.is_open()) {
        throw std::runtime_error("failed to open last segment");
    }

    loadSegmentEntries(last_seg);

    last_index_ = last_seg->index + last_seg->epos.size() - 1;

}

void babrat::wal::WALManager::loadSegmentEntries(const std::shared_ptr<Segment>& seg) {


    std::ifstream file(seg->path,  std::ios::binary |std::ios::ate);

    if (!file) {
        throw std::runtime_error("Cannot open segment file: " + seg->path.string());
    }

    seg->ebuf.resize(file.tellg());
    file.seekg(0);
    if (!file.read(reinterpret_cast<char*>(seg->ebuf.data()), seg->ebuf.size())) {
        throw std::runtime_error("Failed to read segment file: " + seg->path.string());
    }

    size_t pos = 0;

    while (pos < seg->ebuf.size()) {

        std::vector<uint8_t> entry_data(seg->ebuf.begin() + pos, seg->ebuf.end());

        int n = format_handler_->loadNextEntry(entry_data);
        if (n <= 0 || pos + n > seg->ebuf.size()) {
            throw std::runtime_error("Invalid entry at position " + std::to_string(pos));
        }

        seg->epos.emplace_back(pos, pos + n);
        pos += n;
    }
}


void babrat::wal::WALManager::initPath(const std::string &path){

    std::string actual_path = path.empty() ? DEFAULT_DIRECTORY : path;

    if (!fs::exists(path)) {
        if(!fs::create_directories(path)){
            throw std::runtime_error("Failed to create directory: " + actual_path);
        }
    }

    path_ = std::move(actual_path);

};

void babrat::wal::WALManager::truncateFrontIndex(uint64_t index){

    if (index == 0 || last_index_ == 0 || index < first_index_ || index > last_index_) {
        throw out_of_range_error();
    }
    if (index == first_index_) {
        return; // nothing to truncate
    }

    size_t seg_idx = findSegment(index);
    auto s = loadSegment(index);
    if (!s) {
        throw std::runtime_error("segment not found");
    }

    // Get the slice of entries starting from the specified index
    size_t start_idx = index - s->index;
    if (start_idx >= s->epos.size()) {
        throw std::runtime_error("invalid entry index");
    }

    std::vector<BPos> epos_slice(s->epos.begin() + start_idx, s->epos.end());
    std::vector<uint8_t> ebuf_slice(s->ebuf.begin() + epos_slice[0].pos, s->ebuf.end());

    // Create a temporary file for the new segment
    fs::path temp_path = path_ / "TEMP";
    {
        std::ofstream temp_file(temp_path, std::ios::binary);

        if (!temp_file) {
            throw std::runtime_error("failed to create temp file");
        }

        temp_file.write(reinterpret_cast<const char*>(ebuf_slice.data()), ebuf_slice.size());
        if (!temp_file) {
            throw std::runtime_error("failed to write to temp file");
        }

        if (!options_.isNoSync()) {
            temp_file.flush();
        }
    }

    // Rename temp file to START file
    fs::path start_path = path_ / (segmentName(index) + ".START");
    fs::rename(temp_path, start_path);

    try {
        // Close current segment file if it's the last one
        if (seg_idx == segments_.size() - 1 && file_.is_open()) {
            if (!options_.isNoSync()) {
                file_.flush();;
            }
            file_.close();
        }

        // Delete old segments
        for (size_t i = 0; i <= seg_idx; i++) {
            fs::remove(segments_[i]->path);
        }

        // Rename START to final name
        fs::path new_path = path_ / segmentName(index);
        fs::rename(start_path, new_path);

        // Update segment info
        s->path = new_path;
        s->index = index;
        s->ebuf = std::move(ebuf_slice);
        s->epos = std::move(epos_slice);

        // Update segments list
        segments_.erase(segments_.begin(), segments_.begin() + seg_idx);

        // Reopen file if it was the last segment
        if (seg_idx == segments_.size() - 1) {
            file_.open(new_path,   std::ios::binary |std::ios::out | std::ios::app);

            if (!file_.is_open()) {
                throw std::runtime_error("failed to reopen segment file");
            }

            // Verify file size
            file_.seekp(0, std::ios::end);

            if (file_.tellp() != static_cast<std::streampos>(s->ebuf.size())) {
                throw std::runtime_error("invalid file size after truncate");
            }
        }

        // Update first index
        first_index_ = index;

        clearCacheSegments();

    } catch (...) {
        corrupt_ = true;
        throw;
    }
};

void babrat::wal::WALManager::truncateBackIndex(uint64_t index) {
    if (index == 0 || last_index_ == 0 || index < first_index_ || index > last_index_) {
        throw out_of_range_error();
    }
    if (index == last_index_) {
        return; // nothing to truncate
    }

    size_t seg_idx = findSegment(index);
    auto s = loadSegment(index);

    if (!s) {
        throw std::runtime_error("segment not found");
    }

    // Get entries up to and including the specified index
    size_t end_idx = index - s->index + 1;
    if (end_idx > s->epos.size()) {
        throw std::runtime_error("invalid entry index");
    }

    std::vector<BPos> epos_slice(s->epos.begin(), s->epos.begin() + end_idx);
    std::vector<uint8_t> ebuf_slice(s->ebuf.begin(), s->ebuf.begin() + epos_slice.back().end);

    // Create a temporary file for the new segment
    fs::path temp_path = path_ / "TEMP";
    {
        std::ofstream temp_file(temp_path, std::ios::binary);
        if (!temp_file) {
            throw std::runtime_error("failed to create temp file");
        }
        temp_file.write(reinterpret_cast<const char*>(ebuf_slice.data()), ebuf_slice.size());
        if (!temp_file) {
            throw std::runtime_error("failed to write to temp file");
        }
        if (!options_.isNoSync()) {
            temp_file.flush();
        }
    }

    // Rename temp file to END file
    fs::path end_path = path_ / (segmentName(s->index) + ".END");
    fs::rename(temp_path, end_path);

    try {
        // Close current segment file
        if (file_.is_open()) {
            if (!options_.isNoSync()) {
                file_.flush();;
            }
            file_.close();
        }

        // Delete segments after the current one
        for (size_t i = seg_idx + 1; i < segments_.size(); i++) {
            fs::remove(segments_[i]->path);
        }

        // Rename END to final name
        fs::path new_path = path_ / segmentName(s->index);
        fs::rename(end_path, new_path);

        // Update segment info
        s->path = new_path;
        s->ebuf = std::move(ebuf_slice);
        s->epos = std::move(epos_slice);

        // Update segments list
        segments_.reserve(seg_idx + 1);

        // Reopen the file
        file_.open(new_path,  std::ios::binary |std::ios::out | std::ios::app);
        if (!file_.is_open()) {
            throw std::runtime_error("failed to reopen segment file");
        }

        // Verify file size
        file_.seekp(0, std::ios::end);
        if (file_.tellp() != static_cast<std::streampos>(s->ebuf.size())) {
            throw std::runtime_error("invalid file size after truncate");
        }

        // Update last index
        last_index_ = index;

        clearCacheSegments();

    } catch (...) {
        corrupt_ = true;
        throw;
    }

};

void babrat::wal::WALManager::clearCacheSegments() {
    // Clear all segments in cache
    for(size_t i = 0; i < lruCache.size(); i++){
        auto seg = lruCache.get(i);
        seg->ebuf.clear();
        seg->epos.clear();
    }

    lruCache.clear();
}


void babrat::wal::WALManager::checkState() const {
    if (corrupt_) throw corrupt_error();
    if (closed_) throw closed_file_error();
}
