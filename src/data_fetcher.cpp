#include "data_fetcher.hpp"

#include <curl/curl.h>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <limits>

namespace {
size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total = size * nmemb;
    std::string* s = static_cast<std::string*>(userp);
    s->append(static_cast<char*>(contents), total);
    return total;
}
}

std::map<std::string, double> fetch_fred_series(const std::string& series_id,
                                                const std::string& api_key) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to init curl");
    }

    std::string url = "https://api.stlouisfed.org/fred/series/observations?";
    url += "series_id=" + series_id + "&api_key=" + api_key +
           "&file_type=csv&frequency=w";

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        throw std::runtime_error("curl_easy_perform failed");
    }
    curl_easy_cleanup(curl);

    std::map<std::string, double> result;
    std::regex line_re(R"((\d{4}-\d{2}-\d{2}),([\d\.NA]+))");
    std::stringstream ss(response);
    std::string line;
    // skip header
    std::getline(ss, line);
    while (std::getline(ss, line)) {
        std::smatch m;
        if (std::regex_search(line, m, line_re)) {
            std::string date = m[1];
            std::string value_str = m[2];
            double value = std::numeric_limits<double>::quiet_NaN();
            if (value_str != "NA") {
                value = std::stod(value_str);
            }
            result[date] = value;
        }
    }
    return result;
}
