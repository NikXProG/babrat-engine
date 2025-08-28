#pragma once

#include <babrat/wal/wal_struct.hpp>
#include <babrat/common/babrat_api.hpp>

constexpr size_t DEFAULT_SEGMENT_SIZE = 1024*1024;
constexpr size_t DEFAULT_SEGMENT_CACHE_SIZE = 2;
constexpr const char* DEFAULT_DIRECTORY = "wal_logs";

namespace babrat  {

    namespace wal {

        enum class LogFormat {
            Binary,
            Json,
            Pipe
        };

        class WALOptions {

        private:

            size_t m_segmentSize = DEFAULT_SEGMENT_SIZE;

            size_t m_segmentCacheSize  =  DEFAULT_SEGMENT_CACHE_SIZE;

            LogFormat m_logFormat = LogFormat::Binary;

            bool m_isNoSync  = false;

        public:

            BABRAT_API size_t getSegmentSize() const;

            BABRAT_API size_t getSegmentCacheSize() const;

            BABRAT_API LogFormat getFormat() const;

            BABRAT_API bool isNoSync() const;

        public:

            BABRAT_API void setSegmentSize(size_t segmentSize);

            BABRAT_API void setSegmentCacheSize(size_t segmentCacheSize);

            BABRAT_API void setLogFormat(LogFormat logFormat);

            BABRAT_API void setIsNoSync(bool isNoSync);

        public:

            BABRAT_API WALOptions() = default;

        public:

            WALOptions(const WALOptions &) = default;

            WALOptions &operator=(WALOptions const &) = default;

        };

        class WAL {

        public:

            virtual ~WAL() noexcept = default;

        public:

            virtual std::uint64_t firstIndex() = 0;

            virtual std::uint64_t lastIndex() = 0;

        public:

            virtual void write(uint64_t index, const std::vector<uint8_t>& data) = 0;

            virtual void writeBatch(WriteBatch* writeBatch) = 0;

            virtual std::vector<uint8_t> read(uint64_t index) = 0;

        public:

            virtual void truncateRange(std::uint64_t low_limit, std::uint64_t upper_limit) = 0;

            virtual void truncateFront(std::uint64_t index) = 0;

            virtual void truncateBack(std::uint64_t index) = 0;

        public:

            virtual void sync() = 0;

        public:

            virtual void close() = 0;

        public:

            virtual void cacheClear() = 0;

        };


    } // namespace wal

} // namespace babrat
