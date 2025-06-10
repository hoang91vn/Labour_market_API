#include "analysis.hpp"
#include "cleaner.hpp"
#include "data_fetcher.hpp"
#include "forecast.hpp"
#include "plot.hpp"
#include "utils.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

int main() {
    namespace fs = std::filesystem;
    fs::create_directories("output");

    const std::string api_key = "1868e18596ed3625c987d2b148accbed";

    std::map<std::string, double> claims;
    std::map<std::string, double> openings;

    {
        ScopedTimer t("Fetch ICSA");
        claims = fetch_fred_series("ICSA", api_key);
    }
    {
        ScopedTimer t("Fetch JTSJOL");
        openings = fetch_fred_series("JTSJOL", api_key);
    }

    std::map<std::string, Row> table;
    {
        ScopedTimer t("Clean & Merge");
        if (!claims.empty()) {
            std::string start = claims.begin()->first;
            std::string end = claims.rbegin()->first;
            table = clean_and_merge(claims, openings, start, end);
        }
    }

    std::vector<std::string> dates;
    std::vector<double> unemp_raw;
    for (const auto& [d, r] : table) {
        dates.push_back(d);
        unemp_raw.push_back(r.unemployment);
    }

    std::vector<double> unemp_ma;
    std::vector<double> changes;
    std::vector<double> vol;
    std::vector<bool> outliers;

    {
        ScopedTimer t("Analysis");
        unemp_ma = moving_average(unemp_raw, 4);
        changes = percent_change(unemp_raw);
        vol = compute_volatility(changes, 4);
        outliers = flag_outliers(changes, 3.0);
    }

    std::vector<double> forecast_vals;
    std::vector<std::string> forecast_dates;
    {
        ScopedTimer t("Forecast");
        forecast_vals = forecast_sma(unemp_raw, 4, 3);
        // generate forecast dates
        if (!dates.empty()) {
            std::string last = dates.back();
            std::tm tm = {};
            std::istringstream ss(last);
            ss >> std::get_time(&tm, "%Y-%m-%d");
            for (size_t i = 1; i <= 3; ++i) {
                tm.tm_mday += 7;
                std::mktime(&tm);
                char buf[11];
                std::strftime(buf, sizeof(buf), "%Y-%m-%d", &tm);
                forecast_dates.push_back(buf);
            }
        }
    }

    // Rewrite CSV with analysis results
    {
        std::ofstream out("output/analysis.csv");
        out << "Week,UnempRaw,UnempMA,UnempChange,ChangeVolatility,OutlierFlag,Forecast\n";
        for (size_t i = 0; i < dates.size(); ++i) {
            out << dates[i] << ',' << unemp_raw[i] << ',' << unemp_ma[i] << ','
                << changes[i] << ',' << vol[i] << ',' << outliers[i] << ',';
            if (i < forecast_dates.size())
                out << forecast_vals[i];
            out << '\n';
        }
    }

    ascii_sparkline("Unemployment", unemp_raw);
    ascii_sparkline("Trend", unemp_ma);
    ascii_sparkline("Forecast", forecast_vals);

    plot_png("Unemployment", dates, unemp_raw, unemp_ma,
             forecast_dates, forecast_vals, "output/unemployment.png");

    return 0;
}
