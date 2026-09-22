#ifndef TIMER_H
#define TIMER_H
#include <chrono>
#include <cstdint>

class Timer {
    public:
        // aliases
        using Nanos   = std::chrono::nanoseconds;
        using Micros  = std::chrono::microseconds;
        using Millis  = std::chrono::milliseconds;
        using Seconds = std::chrono::seconds;
        using Minutes = std::chrono::minutes;
        using Hours   = std::chrono::hours;

        // constructor
        Timer()
        : m_time{std::chrono::steady_clock::now()} {}

        // resets state of timer
        void restart() {
            m_time = std::chrono::steady_clock::now();
        }

        // returns time unit T since timer was last clicked/restarted/constructed
        // NOTE: keeping track of a separate timer for click/restart/constructed seemed redundant
        // because we only need to keep the most recent one so there is only m_time.
        template <typename T>
        uint64_t click() {
            auto timeNow{std::chrono::steady_clock::now()};
            // static cast to stop warning
            uint64_t timeElapsed{static_cast<uint64_t>(std::chrono::duration_cast<T>(timeNow - m_time).count())};
            restart();
            return timeElapsed;
        }

        // literally the same as click() but doesn't restart time
        template <typename T>
        uint64_t glance() const {
            auto timeNow{std::chrono::steady_clock::now()};
            return std::chrono::duration_cast<T>(timeNow - m_time).count();
        }
    private:
        std::chrono::time_point<std::chrono::steady_clock> m_time{};
};

#endif  // TIMER_H
