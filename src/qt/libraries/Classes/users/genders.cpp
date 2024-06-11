#include "genders.h"
#include <map>


std::string genderToString(Gender gender) {
    auto it = genderMap.find(gender);
    return (it != genderMap.end()) ? it->second : "Unknown";
}

std::vector<std::string> getAllGenders() {
    std::vector<std::string> genders;

    for (const auto &pair : genderMap) {
        genders.push_back(pair.second);
    }

    return genders;
}
