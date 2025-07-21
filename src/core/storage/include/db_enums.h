#pragma once

namespace babrat {

    namespace storage {

        /**
        * @enum Mode
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
            in_memory,
            file_system
        };

        enum class SearchTreeVariant : uint8_t
        {
            red_black = 1,
            b_plus = 2
        };



    }

}

