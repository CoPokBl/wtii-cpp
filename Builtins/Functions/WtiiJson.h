#ifndef WTIIINTERPRETER_WTIIJSON_H
#define WTIIINTERPRETER_WTIIJSON_H

#include <variant>
#include "../../nlohmann/json.hpp"
#include "../../ParsedScripts/Values/ClassInstance.h"

using json = nlohmann::json;

using ValueMap = std::map<std::string, Value>;


class WtiiJson {
public:
    static Value* ToJson(std::vector<Value*> args);
    static Value* FromJson(std::vector<Value*> args);
};


#endif //WTIIINTERPRETER_WTIIJSON_H
