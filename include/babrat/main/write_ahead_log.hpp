#pragma once

#include <mutex>

#include <babrat/storage/db_options.hpp>
#include <babrat/wal/default/wal_default.hpp>


namespace babrat {

    class WriteAheadLog {

        std::unique_ptr<wal::WAL> m_wal;

        std::mutex wal_walLock;

        std::atomic<std::uint64_t> m_walIdx;

        std::string path;

        /*Serializer serializer;*/

    public:

        explicit WriteAheadLog(const std::string& path, const wal::WALOptions& options);

    public:

        void WriteCreatePool();

        void WriteUpdatePool();

        void WriteDeletePool();

    public:

        void WriteCreateSchema();

        void WriteUpdateSchema();

        void WriteDeleteSchema();

    public:

        void WriteCreateCollection();

        void WriteUpdateCollection();

        void WriteDeleteCollection();

    public:

        void WriteInsert();

        void WriteUpdate();

        void WriteDelete();


    };

} // namespace babrat

// sdsdsad /// sdsadasdsadsa // asdsad