#pragma once

#include <babrat/wal/wal_struct.hpp>

constexpr size_t DEFAULT_SEGMENT_SIZE = 1024*1024;
constexpr size_t DEFAULT_SEGMENT_CACHE_SIZE = 2;
constexpr const char* DEFAULT_DIRECTORY = "wal_logs";

namespace babrat  {

    namespace wal {

        enum class LogFormat {
            BINARY,
            JSON,
            PIPE
        };

        class Options {

        private:

            size_t m_segmentSize = DEFAULT_SEGMENT_SIZE;

            size_t m_segmentCacheSize  =  DEFAULT_SEGMENT_CACHE_SIZE;

            LogFormat m_logFormat = LogFormat::BINARY;

            bool m_isNoSync  = false;

        public:

            size_t getSegmentSize() const;

            size_t getSegmentCacheSize() const;

            LogFormat getFormat() const;

            bool isNoSync() const;

        public:

            void setSegmentSize(size_t segmentSize);

            void setSegmentCacheSize(size_t segmentCacheSize);

            void setLogFormat(LogFormat logFormat);

            void setIsNoSync(bool isNoSync);

        public:

            Options() = default;

            Options(const Options &) = default;
            Options &operator=(Options const &) = default;

        };

        class WAL {

        public:

            virtual ~WAL() noexcept = default;

        public:

            virtual std::uint64_t firstIndex() = 0;

            virtual std::uint64_t lastIndex() = 0;

        public:

            virtual void write(uint64_t index, const std::vector<uint8_t>& data) = 0;

            virtual void writeBatch(babrat::wal::WriteBatch* writeBatch) = 0;

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
