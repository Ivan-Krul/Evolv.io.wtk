#pragma once

#include <chrono>

struct TimeStop {
    std::chrono::high_resolution_clock::time_point beg;
    std::chrono::high_resolution_clock::time_point end;
    TimeStop() {
        beg = end = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        end = std::chrono::high_resolution_clock::now();
        float timeMS = std::chrono::duration_cast<std::chrono::nanoseconds>(end - beg).count() / 10e5f;
        printf("TimeStop: %fms\n", timeMS);
    }
};
