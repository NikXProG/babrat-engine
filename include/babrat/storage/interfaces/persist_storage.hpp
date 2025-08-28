#pragma once

#include <string>
#include "storage.hpp"

namespace babrat {

    namespace storage
    {

        /**
          * @brief The main Database Storage class inherits from the IPersistStorage interface to enforce persistence
          *
          * @tparam Key the key that the comparator must support (can be custom)
          * @tparam Value value stored under the key
          */
        template<typename Key, typename Value>
        class IPersistStorage : public IStorage<Key,Value> {

        public:

            ~IPersistStorage() noexcept override = default;

        /*
        public:

            virtual bool saveTo(const std::string& file) = 0;

            virtual bool loadFrom(const std::string& file) = 0;

        public:

            virtual bool persist(std::uint64_t targetTime) = 0;

            virtual bool restore(std::uint64_t targetTime, bool withWal) = 0;
            */

        };


    } // namespace storage

} // namespace babrat


    /*using TypeColumn = babrat::Column*;

    using TypeStorage = storage::IPersistStorage<
            TypeColumn,
            std::vector<TypeColumn>>;

    }*/
