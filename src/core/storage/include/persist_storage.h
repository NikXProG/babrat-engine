#pragma once

#include <string>
#include "storage.h"

namespace babrat {

    namespace storage {

        template<typename Key, typename Value>
        class IPersistStorage : public IStorage<Key,Value> {

        public:

            ~IPersistStorage() noexcept override = default;

        public:

            virtual bool saveTo(const std::string& file) = 0;

            virtual bool loadFrom(const std::string& file) = 0;

        public:

            virtual bool persist(std::uint64_t targetTime) = 0;

            virtual bool restore(std::uint64_t targetTime, bool withWal) = 0;

        };


    }


    using TypeColumn = babrat::Column*;

    using TypeStorage = storage::IPersistStorage<
            TypeColumn,
            std::vector<TypeColumn>>;



}
