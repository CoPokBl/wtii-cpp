#include <iostream>
#include <filesystem>
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

std::string Utils::GetAbsolutePath(const std::string& path) {
    std::filesystem::path pathInput(path);

    // Add current path if the input path is relative
    std::filesystem::path fullPath;
    if (pathInput.is_absolute()) {
        fullPath = pathInput;
    } else {
        std::filesystem::path currentPath = std::filesystem::current_path();
        fullPath = currentPath / pathInput;
    }
    fullPath = fullPath.lexically_normal();
    return fullPath.string();
}

std::string Utils::TrimString(const std::string &val) {
    int firstNonSpace = -1;
    for (int i = 0; i < val.size(); ++i) {
        if (val[i] == ' ') {
            continue;
        }

        firstNonSpace = i;
        break;
    }

    int lastNonSpace = -1;
    for (int i = val.size(); i > 0; --i) {
        if (val[i] == ' ' || val[i] == '\0') {
            continue;
        }

        lastNonSpace = i+1;
        break;
    }

    return val.substr(firstNonSpace, lastNonSpace - firstNonSpace);
}

bool Utils::ContainsChar(const std::string &str, const char &v) {
    for (char c : str) {
        if (c == v) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> Utils::SplitString(const std::string &str, const char &delimiter) {
    // TODO: Implement
}
