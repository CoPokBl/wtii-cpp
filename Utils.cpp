#include <iostream>
#include "Utils.h"
#include "ParsedScripts/Values/Constant.h"

bool Utils::EndsWith(std::string str, std::string ending) {
    if (ending.size() > str.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), str.rbegin());
}

std::string Utils::SubString(const std::string& str, int start, int end) {
    return str.substr(start, end - start);
}

std::vector<std::string> Utils::SubVector(const std::vector<std::string> &vec, int start, int end) {
    std::vector<std::string> result;
    for (int i = start; i < end; i++) {
        result.push_back(vec[i]);
    }
    return result;
}

void Utils::DumpErrorInfo(const RuntimeException& e) {
    std::cerr << "An error has occurred" << std::endl;

    // If it's a constant object we can dump more info
    auto val = dynamic_cast<Constant*>(e.Object);

    if (val == nullptr) {
        return;
    }

    std::cerr << "Value: " << val->ToString() << std::endl;
}
