#ifndef CLEANER_HPP
#define CLEANER_HPP

#include <map>
#include <string>

struct Row {
    double iclaims;
    double unemployment;
    double openings;
};

std::map<std::string, Row> clean_and_merge(
    const std::map<std::string,double>& claims,
    const std::map<std::string,double>& openings,
    const std::string& start, const std::string& end);

#endif // CLEANER_HPP
