#pragma once

#include "babrat/export_shared.h"

#include <babrat/wal/wal.hpp>
#include <babrat/wal/wal_format.hpp>
#include <babrat/wal/lrucache11.hpp>
#include <shared_mutex>
#include <fstream>


namespace babrat  {

    namespace wal {

        class WALManager : public WAL {

        private:

            WALOptions options_;

        private:

            std::unique_ptr<FormatHandler> format_handler_;

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

            Cache<size_t, std::shared_ptr<Segment>> lruCache;

        public:

            BABRAT_API explicit WALManager(
                    const std::string& path,
                    const WALOptions& options);

            ~WALManager() noexcept override;

        public:

            BABRAT_API std::uint64_t firstIndex() override;

            BABRAT_API std::uint64_t lastIndex() override;

        public:

            BABRAT_API void write(uint64_t index, const std::vector<uint8_t>& data) override;

            BABRAT_API void writeBatch(WriteBatch* batch) override;

            BABRAT_API std::vector<uint8_t> read(uint64_t index) override;

        public:

            BABRAT_API void truncateRange(std::uint64_t low_limit, std::uint64_t upper_limit) override;

            BABRAT_API void truncateFront(uint64_t index) override;

            BABRAT_API void truncateBack(uint64_t index) override;

        public:

            BABRAT_API void sync() override;

        public:

            BABRAT_API void close() override;

        public:

            BABRAT_API void cacheClear() override;

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

    } // namespace wal

} // namespace babrat
