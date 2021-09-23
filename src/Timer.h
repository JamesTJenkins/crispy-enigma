#pragma once
#include <chrono>

namespace Utilities {
    class Timer {
    public:
        Timer();
        ~Timer();

        void Start();
        void Stop();

        double ElapsedMilliseconds();
        double ElapsedSeconds();
    private:
        std::chrono::time_point<std::chrono::system_clock> startTime;
        std::chrono::time_point<std::chrono::system_clock> endTime;
        bool isRunning = false;
    };
}