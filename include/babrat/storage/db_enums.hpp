#pragma once

#include <cstdint>

namespace babrat {

    namespace storage {

        /**
          * @brief IPersistStorage operation Mode that determines persistence behavior
          *
          * The Mode controls how and when data is serialized to persistent storage,
          * affecting both performance and durability characteristics.
          *
          * @var Mode::in_memory
          * @brief In-memory only Mode (no automatic persistence)
          *
          * All data exists only in memory while application is running
          *
          * @var Mode::file_system
          * @brief Persistent storage Mode with write-through behavior
          *
          * Maintains data durability by automatically persisting changes.
          * Supports point-in-time recovery (snapshotting)
          */
        enum class DatabaseMode : uint8_t
        {
            IN_MEMORY,
            FILE_SYSTEM
        };

        /**
          * @brief offers a different set of Search Tree
          *
          * @note expansion is possible in the future
          */
        enum class SearchTreeVariant : uint8_t
        {
            RED_BLACK = 1,
            B_PLUS = 2
        };

        /**
          * @brief defines the set of operations recorded in the journaling shaft for recovery
          *
          * @note expansion is possible in the future
          */
        enum class WalOperationType : uint8_t {
            INVALID = 0,
            INSERT = 1,
            DELETE = 2,
            UPDATE = 3
        };

        /**
          * @brief defines the set of level operations recorded in the journaling shaft for recovery
          *
          * @note expansion is possible in the future
          */
        enum class WalLevelType : uint8_t {
            INVALID = 0,
            POOL = 1, // database pool objects in container
            SCHEMA = 2, // database schema objects in container
            COLLECTION = 3, // database collection objects in container
            OBJECT = 4, // database data objects in container
        };

    } // namespace storage

} // namespace babrat

