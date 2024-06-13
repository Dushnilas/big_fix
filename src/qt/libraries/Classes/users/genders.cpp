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

std::map<std::string, Gender> ReverseMap() {
    std::map<std::string, Gender> reverseGenderMap;
    for (const auto &pair : genderMap) {
        reverseGenderMap[pair.second] = pair.first;
    }
    return reverseGenderMap;
}

Gender strToGender(const std::string &genderStr) {
    static const std::map<std::string, Gender> reverseGenderMap = ReverseMap();
    auto it = reverseGenderMap.find(genderStr);
    if (it != reverseGenderMap.end()) {
        return it->second;
    } else {
        return Gender::Undefined;
    }
}
