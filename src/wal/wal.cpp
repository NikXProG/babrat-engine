
#include <babrat/wal/wal.hpp>

namespace babrat
{

    namespace wal
    {

        // Options getters
        LogFormat WALOptions::getFormat() const
        {
            return m_logFormat;
        }

        size_t WALOptions::getSegmentCacheSize() const
        {
            return m_segmentCacheSize;
        }

        size_t WALOptions::getSegmentSize() const
        {
            return m_segmentSize;
        }

        bool WALOptions::isNoSync() const
        {
            return m_isNoSync;
        }


        // Options setters
        void WALOptions::setSegmentSize(size_t segmentSize)
        {
            m_segmentSize =
                      (segmentSize <= 0) ? DEFAULT_SEGMENT_SIZE : segmentSize;
        }

        void WALOptions::setSegmentCacheSize(size_t segmentCacheSize)
        {
            m_segmentCacheSize =
                  (segmentCacheSize <= 0) ? DEFAULT_SEGMENT_CACHE_SIZE : segmentCacheSize;
        }

        void WALOptions::setLogFormat(LogFormat logFormat)
        {
            m_logFormat = logFormat;
        }

        void WALOptions::setIsNoSync(bool isNoSync)
        {
            m_isNoSync = isNoSync;
        }

    } // namespace wal

} // namespace babrat