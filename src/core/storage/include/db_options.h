#pragma once
#include <string>

namespace babrat {

    namespace storage {

        struct DatabaseOptions {

            // The database mode is responsible for storing data in the operating system or in RAM
            DatabaseMode mode = DatabaseMode::in_memory;
            // different variants of search tree container in in-memory mode (red-black, b-plus)
            SearchTreeVariant variant = SearchTreeVariant::b_plus;
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
            babrat::wal::Options walOptions = defaultWalOptions();

        public:

            // configure the necessary default settings
            babrat::wal::Options defaultWalOptions() const {
                babrat::wal::Options opts;
                opts.setSegmentSize(256*256);
                opts.setSegmentCacheSize(100);
                opts.setLogFormat(wal::LogFormat::PIPE);
                opts.setIsNoSync(false);
                return opts;
            }


        };


    }

}