#ifndef FORECAST_HPP
#define FORECAST_HPP

#include <vector>
#include <cstddef>

std::vector<double> forecast_sma(const std::vector<double>& series,
                                 size_t window, size_t steps);

#endif // FORECAST_HPP
