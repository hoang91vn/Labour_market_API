#include "plot.hpp"

#include <iostream>
#include <matplotlibcpp.h>
#include <cmath>

namespace plt = matplotlibcpp;

void ascii_sparkline(const std::string& title, const std::vector<double>& data) {
    const char* ticks = " ▁▂▃▄▅▆▇█";
    double minv = *std::min_element(data.begin(), data.end());
    double maxv = *std::max_element(data.begin(), data.end());
    double range = maxv - minv;
    std::cout << title << " ";
    for (double v : data) {
        int idx = 0;
        if (range > 0) {
            idx = static_cast<int>(std::round((v - minv) / range * 8));
            if (idx < 0) idx = 0;
            if (idx > 8) idx = 8;
        }
        std::cout << ticks[idx];
    }
    std::cout << std::endl;
}

void plot_png(const std::string& title,
              const std::vector<std::string>& dates,
              const std::vector<double>& raw,
              const std::vector<double>& trend,
              const std::vector<std::string>& forecast_dates,
              const std::vector<double>& forecast_vals,
              const std::string& outpath) {
    plt::figure_size(1200, 600);
    plt::named_plot("Raw", dates, raw, "b-");
    plt::named_plot("Trend", dates, trend, "r-");
    plt::named_plot("Forecast", forecast_dates, forecast_vals, "g--");
    plt::legend();
    plt::title(title);
    plt::save(outpath);
    plt::close();
}
