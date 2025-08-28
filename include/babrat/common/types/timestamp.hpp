#pragma once
#include <chrono>

namespace babrat {

    class timestamp_t {

        int64_t m_ticks;

    public:

        timestamp_t() = default;

    public:

        explicit timestamp_t(int64_t ticks = 0) : m_ticks(ticks) {}

    public:

        timestamp_t &operator=(int64_t ticks) {
            m_ticks = ticks;
            return *this;
        }

    public:

        explicit inline operator int64_t() const {
            return m_ticks;
        }

    public:

        inline bool operator==(const timestamp_t &rhs) const {
            return m_ticks == rhs.m_ticks;
        };

        inline bool operator!=(const timestamp_t &rhs) const {
            return m_ticks != rhs.m_ticks;
        };

        inline bool operator<=(const timestamp_t &rhs) const {
            return m_ticks <= rhs.m_ticks;
        };

        inline bool operator<(const timestamp_t &rhs) const {
            return m_ticks < rhs.m_ticks;
        };

        inline bool operator>(const timestamp_t &rhs) const {
            return m_ticks > rhs.m_ticks;
        };

        inline bool operator>=(const timestamp_t &rhs) const {
            return m_ticks >= rhs.m_ticks;
        };

    public:

        inline int64_t operator-(const timestamp_t &other) const {
            return m_ticks - int64_t(other.m_ticks);
        }

    public:

        static timestamp_t max() {
            return timestamp_t(std::numeric_limits<int64_t>::max());
        }

        static timestamp_t min() {
            return timestamp_t( std::numeric_limits<int64_t>::min());
        }

        static timestamp_t epoch() {
            return timestamp_t(0);
        }

    };

    inline bool isFinite(timestamp_t timestamp) {
        return timestamp != timestamp_t::max() && timestamp != timestamp_t::min();
    }

    inline timestamp_t fromEpochMs(int64_t ms) {
        return timestamp_t(ms);
    }

    inline timestamp_t getCurrentTimestamp()
    {
        auto now = std::chrono::system_clock::now();
        auto epoch_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        return fromEpochMs(epoch_ms);
    }

} // namespace babrat