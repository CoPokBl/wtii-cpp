#include <map>
#include <string>
#include "ParsedScripts/ClassDefinition.h"
#include "ParsedScripts/Statements/MethodDefinitiionStatement.h"

class Scope {
public:
    std::map<std::string, std::pair<std::string, Value*>> Variables;
    std::map<std::string, MethodDefinitionStatement*> Functions;
    std::map<std::string, ClassDefinition*> Classes;

    Scope() = default;

    Scope(const Scope& parent) {
        Variables = parent.Variables;
        Functions = parent.Functions;
        Classes = parent.Classes;
    }

    void AppendScope(const Scope& scope) {
        Variables.insert(scope.Variables.begin(), scope.Variables.end());
        Functions.insert(scope.Functions.begin(), scope.Functions.end());
        Classes.insert(scope.Classes.begin(), scope.Classes.end());
    }

    void SetVariable(const std::string& name, Value* value) {
        if (Variables.find(name) != Variables.end()) Variables[name] = {Variables[name].first, value};
        else Variables.insert({name, {value->ObjectType, value}});
    }
};