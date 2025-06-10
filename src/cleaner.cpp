#include "cleaner.hpp"

#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>

std::map<std::string, Row> clean_and_merge(
    const std::map<std::string,double>& claims,
    const std::map<std::string,double>& openings,
    const std::string& start, const std::string& end) {
    std::map<std::string, Row> table;

    auto it_claim = claims.lower_bound(start);
    for (; it_claim != claims.end() && it_claim->first <= end; ++it_claim) {
        const std::string& date = it_claim->first;
        Row row{};
        row.iclaims = it_claim->second;

        // In this simplified example we treat weekly initial claims as the
        // unemployment series as well.
        row.unemployment = it_claim->second;
        // openings: align with most recent monthly data
        auto open_it = openings.upper_bound(date);
        if (open_it != openings.begin()) {
            --open_it;
            row.openings = open_it->second;
        } else {
            row.openings = std::numeric_limits<double>::quiet_NaN();
        }
        table[date] = row;
    }

    std::ofstream out("output/analysis.csv");
    out << "Week,UnempRaw,UnempMA,UnempChange,ChangeVolatility,OutlierFlag,Forecast\n";
    for (const auto& [d, r] : table) {
        out << d << ',' << r.iclaims << ',' << r.unemployment << ',' << 0 << ','
            << 0 << ',' << 0 << ',' << 0 << '\n';
    }
    return table;
}
