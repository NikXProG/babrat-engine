#ifndef BABRAT_WAL_DEFAULT_H
#define BABRAT_WAL_DEFAULT_H
#include <utility>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "LRUCache11.hpp"
#include "wal_format.h"
#include <shared_mutex>

namespace babrat::wal {

    class WALManager : public WAL {

    private:

        std::unique_ptr<FormatHandler> format_handler_;

    private:

        Options options_;

    private:

        std::shared_mutex mutex_;
        fs::path path_;
        std::fstream file_;
        std::vector<std::shared_ptr<Segment>> segments_;
        uint64_t first_index_;
        uint64_t last_index_;
        WriteBatch wbatch_;
        bool closed_ = false;
        bool corrupt_ = false;

    private:

       lru11::Cache<size_t, std::shared_ptr<Segment>> lruCache;

    public:

        explicit WALManager(
                const std::string& path,
                const Options& options);

        ~WALManager() noexcept override;

    public:

        std::uint64_t firstIndex() override;

        std::uint64_t lastIndex() override;

    public:

        void write(uint64_t index, const std::vector<uint8_t>& data) override;

        void writeBatch(WriteBatch* batch) override;

        std::vector<uint8_t> read(uint64_t index) override;

    public:

        void truncateRange(std::uint64_t low_limit, std::uint64_t upper_limit) override;

        void truncateFront(uint64_t index) override;

        void truncateBack(uint64_t index) override;

    public:

        void sync() override;

    public:

        void close() override;

    public:

        void cacheClear() override;

    private:

        void writePack(WriteBatch* batch);

    private:

        std::string segmentName(uint64_t index) const;

    private:

        void cycleSegment();

        std::shared_ptr<Segment> loadSegment(uint64_t index);

        size_t findSegment(std::uint64_t index) const;

    private:

        void load();

        void loadSegmentEntries(const std::shared_ptr<Segment>& seg);

    private:

        void truncateFrontIndex(uint64_t index);

        void truncateBackIndex(uint64_t index);

    private:

        void initPath(const std::string& path);

    private:

        void clearCacheSegments();

    private:

        void checkState() const;

    };

    class WalFactory final {

    public:

        static std::unique_ptr<WAL> create() {
            return std::make_unique<WALManager>(DEFAULT_DIRECTORY, Options{});
        }

        static std::unique_ptr<WAL> create(std::string path) {
            return std::make_unique<WALManager>(path, Options{});
        }

        static std::unique_ptr<WAL> create(std::string path, const Options& options) {
            return std::make_unique<WALManager>(path, options);
        }

    };

}


#endif //BABRAT_WAL_DEFAULT_H
