#include "Timer.h"

namespace Utilities {
    Timer::Timer() {

    }

    Timer::~Timer() {

    }

    void Timer::Start() {
        startTime = std::chrono::system_clock::now();
        isRunning = true;
    }

    void Timer::Stop() {
        endTime = std::chrono::system_clock::now();
        isRunning = false;
    }

    double Timer::ElapsedMilliseconds() {
        std::chrono::time_point<std::chrono::system_clock> end;

        if (isRunning){
            end = std::chrono::system_clock::now();
        } else {
            end = endTime;
        }

        return std::chrono::duration_cast<std::chrono::milliseconds>(end - startTime).count();
    }

    double Timer::ElapsedSeconds() {
        return ElapsedMilliseconds() / 1000;
    }
}