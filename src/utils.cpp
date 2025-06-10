#include "utils.hpp"

#include <iostream>

ScopedTimer::ScopedTimer(const std::string& n) : name(n) {
    start = std::chrono::high_resolution_clock::now();
}

ScopedTimer::~ScopedTimer() {
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << name << " took " << ms << " ms" << std::endl;
}
