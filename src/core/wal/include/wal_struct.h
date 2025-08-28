#ifndef BABRAT_WAL_STRUCT_H
#define BABRAT_WAL_STRUCT_H
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

namespace babrat::wal   {

    struct BPos {
        size_t pos;
        size_t end;
    };

    struct Segment {

        uint64_t index;
        fs::path path;
        std::vector<uint8_t> ebuf;
        std::vector<BPos> epos;

        Segment(uint64_t idx, fs::path p) :
            index(idx),
            path(std::move(p)) {}

        ~Segment(){
            ebuf.clear();
            epos.clear();
        }

    };

    struct WriteBatch {

    private:

        struct Entry {
            uint64_t index;
            size_t size;
        };

    public:

        void write(uint64_t index, const std::vector<uint8_t>& data) {
            entries.push_back({index, data.size()});
            datas.insert(datas.end(), data.begin(), data.end());
        }

    public:

        ~WriteBatch(){
            clear();
        }

        void clear() {
            entries.clear();
            datas.clear();
        }

    public:

        std::vector<Entry> entries;
        std::vector<uint8_t> datas;

    };

}

#endif //BABRAT_WAL_STRUCT_H
