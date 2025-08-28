#pragma once

#include "db_enums.h"
#include "in_memory_storage.h"

namespace babrat {

    namespace storage {

      /*  template<SearchTreeVariant TreeVariant = SearchTreeVariant::b_plus>
        struct InMemoryStoragePolicy {
            static constexpr SearchTreeVariant variant = TreeVariant;

            template<typename Key, typename Value, typename Compare>
            static std::unique_ptr<IStorage<Key, Value>> create_storage() {
                return std::make_unique<InMemoryStorage<Key, Value, Compare, variant>>();
            }
        };*/




    }





}

