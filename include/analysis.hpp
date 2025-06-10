#ifndef ANALYSIS_HPP
#define ANALYSIS_HPP

#include <vector>

enum class Series { Unemployment, Openings };

std::vector<double> percent_change(const std::vector<double>& series);
std::vector<double> moving_average(const std::vector<double>& series, size_t window);
std::vector<bool> flag_outliers(const std::vector<double>& changes, double sigma_mult=3.0);
std::vector<double> compute_volatility(const std::vector<double>& changes, size_t window);

#endif // ANALYSIS_HPP
