#include <string>
#include "../ParsedScripts/ParsedScript.h"
#include "../nlohmann/json.hpp"

class Parser {

public:
    Parser() = default;
    static ParsedScript Parse(const std::string& script);
    static std::vector<Statement*> ParseStatements(const nlohmann::json& jsonObject);
    static Statement* ParseStatement(const nlohmann::json &jsonObject);
    static std::vector<ClassDefinition*> ParseClasses(const nlohmann::json& jsonObject);
    static Value* ParseValue(const nlohmann::json& jsonObject);
};
