#include "../Values/Value.h"
#include <string>
#include <utility>

#ifndef VARIABLEINITSTATEMENT_H
#define VARIABLEINITSTATEMENT_H

class VariableInitStatement : public Statement {
public:
    StatementType Type = StatementType::VariableInit;
    std::string Name;
    Value* VarValue = nullptr;
    std::string VariableType;

    VariableInitStatement() = default;

    VariableInitStatement(std::string name, class Value* value, std::string variableType) {
        Name = std::move(name);
        VarValue = value;
        VariableType = std::move(variableType);
    }

    class Value* ToValue() {
        return VarValue;
    }
};

#endif // VARIABLEINITSTATEMENT_H