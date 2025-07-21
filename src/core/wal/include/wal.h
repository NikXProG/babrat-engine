#ifndef BABRAT_WAL_H
#define BABRAT_WAL_H
#include "wal_exceptions.h"
#include "wal_struct.h"


constexpr size_t DEFAULT_SEGMENT_SIZE = 1024*1024;
constexpr size_t DEFAULT_SEGMENT_CACHE_SIZE = 2;
constexpr const char* DEFAULT_DIRECTORY = "wal_logs";

namespace babrat::wal {

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

        size_t getSegmentSize() const{
            return m_segmentSize;
        }

        size_t getSegmentCacheSize() const{
            return m_segmentCacheSize;
        }

        LogFormat getFormat() const{
            return m_logFormat;
        }

        bool isNoSync() const {
            return m_isNoSync;
        }

    public:

        void setSegmentSize(size_t segmentSize) {
            m_segmentSize =
                    (segmentSize <= 0) ? DEFAULT_SEGMENT_SIZE : segmentSize;
        }

        void setSegmentCacheSize(size_t segmentCacheSize) {
            m_segmentCacheSize =
                    (segmentCacheSize <= 0) ? DEFAULT_SEGMENT_CACHE_SIZE : segmentCacheSize;
        }

        void setLogFormat(LogFormat logFormat) {
            m_logFormat = logFormat;
        }

        void setIsNoSync(bool isNoSync) {
           m_isNoSync = isNoSync;
        }


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

}






#endif //BABRAT_WAL_H
