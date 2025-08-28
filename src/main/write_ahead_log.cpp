
#include "babrat/storage/common/convert_timestamp.hpp"

#include <iostream>
#include <babrat/main/write_ahead_log.hpp>

namespace babrat {

    WriteAheadLog::WriteAheadLog(const std::string& path, const wal::WALOptions& options)
    {
        m_wal = std::make_unique<wal::WALManager>(path, options);
    }

    void WriteAheadLog::WriteCreatePool()
    {
        writeField("level", storage::WalLevelType::POOL);

        serializer.write();
    }


} // nnamespace babrat