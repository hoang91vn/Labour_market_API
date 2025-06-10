#include "forecast.hpp"

#include <numeric>
#include <cmath>

std::vector<double> forecast_sma(const std::vector<double>& series,
                                 size_t window, size_t steps) {
    std::vector<double> result(series.begin(), series.end());
    for (size_t s = 0; s < steps; ++s) {
        size_t size = result.size();
        size_t start = (size >= window) ? size - window : 0;
        double sum = 0.0;
        size_t count = 0;
        for (size_t i = start; i < size; ++i) {
            if (std::isfinite(result[i])) {
                sum += result[i];
                ++count;
            }
        }
        double next = count ? sum / count : 0.0;
        result.push_back(next);
    }
    // Return only the forecasted values
    return std::vector<double>(result.end() - steps, result.end());
}
