#include "analysis.hpp"

#include <cmath>
#include <numeric>

std::vector<double> percent_change(const std::vector<double>& series) {
    std::vector<double> result(series.size(), 0.0);
    for (size_t i = 1; i < series.size(); ++i) {
        if (std::isfinite(series[i-1])) {
            result[i] = (series[i] - series[i-1]) / series[i-1] * 100.0;
        } else {
            result[i] = 0.0;
        }
    }
    return result;
}

std::vector<double> moving_average(const std::vector<double>& series, size_t window) {
    std::vector<double> ma(series.size(), 0.0);
    for (size_t i = 0; i < series.size(); ++i) {
        size_t start = (i >= window - 1) ? i - (window - 1) : 0;
        double sum = 0.0;
        size_t count = 0;
        for (size_t j = start; j <= i; ++j) {
            if (std::isfinite(series[j])) {
                sum += series[j];
                ++count;
            }
        }
        ma[i] = count ? sum / count : std::numeric_limits<double>::quiet_NaN();
    }
    return ma;
}

std::vector<double> compute_volatility(const std::vector<double>& changes, size_t window) {
    std::vector<double> vol(changes.size(), 0.0);
    for (size_t i = 0; i < changes.size(); ++i) {
        size_t start = (i >= window - 1) ? i - (window - 1) : 0;
        double sum = 0.0;
        double sum_sq = 0.0;
        size_t count = 0;
        for (size_t j = start; j <= i; ++j) {
            double val = changes[j];
            if (std::isfinite(val)) {
                sum += val;
                sum_sq += val * val;
                ++count;
            }
        }
        if (count > 1) {
            double mean = sum / count;
            double var = (sum_sq / count) - (mean * mean);
            vol[i] = std::sqrt(var);
        } else {
            vol[i] = 0.0;
        }
    }
    return vol;
}

std::vector<bool> flag_outliers(const std::vector<double>& changes, double sigma_mult) {
    std::vector<bool> flags(changes.size(), false);
    double sum = 0.0;
    double sum_sq = 0.0;
    size_t count = 0;
    for (double c : changes) {
        if (std::isfinite(c)) {
            sum += c;
            sum_sq += c * c;
            ++count;
        }
    }
    double mean = count ? sum / count : 0.0;
    double var = count ? (sum_sq / count) - (mean * mean) : 0.0;
    double stddev = std::sqrt(var);

    for (size_t i = 0; i < changes.size(); ++i) {
        if (std::isfinite(changes[i]) && std::fabs(changes[i] - mean) > sigma_mult * stddev) {
            flags[i] = true;
        }
    }
    return flags;
}
