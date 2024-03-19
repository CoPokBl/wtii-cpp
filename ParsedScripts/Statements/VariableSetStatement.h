#include "../Values/Value.h"
#include <string>
#include <utility>
#include <vector>

class VariableSetStatement : public Statement {
public:
    StatementType Type = StatementType::VariableSet;
    std::vector<std::string> Path;
    Value* VarValue = nullptr;
    std::string ObjectType;

    VariableSetStatement() = default;

    VariableSetStatement(std::vector<std::string> path, class Value* value, std::string objectType) {
        Path = std::move(path);
        VarValue = value;
        ObjectType = std::move(objectType);
    }
};