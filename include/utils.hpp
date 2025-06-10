#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <chrono>

struct ScopedTimer {
    std::string name;
    std::chrono::high_resolution_clock::time_point start;
    ScopedTimer(const std::string& n);
    ~ScopedTimer();
};

#endif // UTILS_HPP
