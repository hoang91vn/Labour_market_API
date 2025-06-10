#ifndef PLOT_HPP
#define PLOT_HPP

#include <string>
#include <vector>

void ascii_sparkline(const std::string& title, const std::vector<double>& data);

void plot_png(const std::string& title,
              const std::vector<std::string>& dates,
              const std::vector<double>& raw,
              const std::vector<double>& trend,
              const std::vector<std::string>& forecast_dates,
              const std::vector<double>& forecast_vals,
              const std::string& outpath);

#endif // PLOT_HPP
