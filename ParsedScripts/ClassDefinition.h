#include <string>
#include <utility>
#include <vector>
#include <map>
#include "Statements/MethodDefinitiionStatement.h"
#include "Statements/VariableInitStatement.h"

#ifndef CLASSDEFINITION_H
#define CLASSDEFINITION_H

class ClassDefinition {
public:
    std::string Name;
    std::vector<MethodDefinitionStatement*> Methods;
    std::vector<VariableInitStatement*> Variables;

    std::map<std::string, MethodDefinitionStatement*> GetMethodsAsMap() {
        std::map<std::string, MethodDefinitionStatement*> map;
        for (auto method : Methods) {
            map[method->Name] = method;
        }
        return map;
    }

    ClassDefinition() = default;

    ClassDefinition(std::string name, std::vector<MethodDefinitionStatement*> methods, std::vector<VariableInitStatement*> variables) {
        Name = std::move(name);
        Methods = std::move(methods);
        Variables = std::move(variables);
    }
};

#endif // CLASSDEFINITION_H