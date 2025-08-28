#pragma once

#include <string>

#include <babrat/storage/db_enums.hpp>

namespace babrat {

    namespace storage {

        /**
          * @brief The config offers to configure the database according to your parameters
          */
        struct DatabaseOptions {

            // The database mode is responsible for storing data in the operating system or in RAM
            DatabaseMode mode = DatabaseMode::IN_MEMORY;
            // different variants of search tree container in in-memory mode (red-black, b-plus)
            SearchTreeVariant variant = SearchTreeVariant::B_PLUS;
            // directory path to serialized db files
            std::string archivePath = "archive";
            // The file format for serialized DBs must be specified with `.` ( timestamp {format-file} )
            std::string formatFile = ".db";
            // default directory for location of wal files of the db ( from the called directory )
            std::string walPath = "wal_logs";
            // maximum size of archived db
            size_t maxArchiveSize = 100;
            // specifies whether to use wal every time data is inserted, deleted, or modified
            bool enableWal = true;
            // if enableWal is on use settings for base wal
            /*wal::WALOptions walOptions = defaultWalOptions();*/

        public:

            // configure the necessary default settings
            /*
            static wal::WALOptions defaultWalOptions()
            {
                wal::WALOptions opts;
                opts.setSegmentSize(256*256);
                opts.setSegmentCacheSize(100);
                opts.setLogFormat(wal::LogFormat::JSON);
                opts.setIsNoSync(false);
                return opts;
            }
            */

        };

    } // namespace storage

} // namespace babrat