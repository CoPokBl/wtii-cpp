#ifndef WTIIINTERPRETER_UTILS_H
#define WTIIINTERPRETER_UTILS_H


#include <string>
#include <vector>
#include "RuntimeException.h"
#include "ParsedScripts/Values/Constant.h"

// Helpful macros
#define throw_err(msg) throw RuntimeException(new Constant(msg, "string"))
#define Null new Constant("NULL", "NULL")

class Utils {
public:
    static bool EndsWith(std::string str, std::string ending);
    static std::string SubString(const std::string& str, int start, int end);
    static std::vector<std::string> SubVector(const std::vector<std::string>& vec, int start, int end);
    static void DumpErrorInfo(const RuntimeException& e);
    static std::string GetAbsolutePath(const std::string& path);
    static std::string TrimString(const std::string& val);
};


#endif //WTIIINTERPRETER_UTILS_H
