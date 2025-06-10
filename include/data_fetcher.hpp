#ifndef DATA_FETCHER_HPP
#define DATA_FETCHER_HPP

#include <map>
#include <string>

std::map<std::string, double> fetch_fred_series(const std::string& series_id,
                                                const std::string& api_key);

#endif // DATA_FETCHER_HPP
