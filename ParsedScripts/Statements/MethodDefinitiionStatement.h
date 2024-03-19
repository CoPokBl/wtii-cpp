#include "Statement.h"
#include "../Values/Value.h"
#include <string>
#include <utility>
#include <vector>
#include <functional>

#ifndef METHODDEFINITIONSTATEMENT_H
#define METHODDEFINITIONSTATEMENT_H

class MethodDefinitionStatement : public Statement {
public:
    StatementType Type = StatementType::MethodDefinition;
    std::string Name;
    std::string ReturnType;
    std::vector<std::string> Arguments;
    std::vector<std::string> ArgumentTypes;
    std::vector<Statement*> Statements;
    std::function<Value*(std::vector<Value*>)> CsFunc;

    MethodDefinitionStatement() = default;

    MethodDefinitionStatement(std::string name, std::string returnType, const std::vector<std::pair<std::string, std::string>>& args) {
        Name = std::move(name);
        ReturnType = std::move(returnType);
        for (auto& arg : args) {
            Arguments.push_back(arg.first);
            ArgumentTypes.push_back(arg.second);
        }
    }
};

#endif // METHODDEFINITIONSTATEMENT_H