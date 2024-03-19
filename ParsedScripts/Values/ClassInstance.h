#ifndef CLASSINSTANCE_H
#define CLASSINSTANCE_H

#include "RealReference.h"
#include "../ClassDefinition.h"
#include <string>
#include <map>

class ClassInstance : public RealReference {
public:
    ValueType Type = ValueType::ClassInstance;
    std::map<std::string, std::pair<std::string, Value*>> Properties;
    std::map<std::string, MethodDefinitionStatement*> Methods;

    ClassInstance() = default;

    explicit ClassInstance(ClassDefinition* def, std::map<std::string, std::pair<std::string, Value*>>* props = nullptr) {
        ObjectType = def->Name;
        for (VariableInitStatement* variable : def->Variables) {
            Properties.insert({variable->Name, {variable->VariableType, variable->ToValue()}});
        }
        for (MethodDefinitionStatement* method : def->Methods) {
            Methods.insert({method->Name, method});
        }

        if (props == nullptr) return;

        for (auto& prop : *props) {
            Properties[prop.first] = prop.second;
        }
    }

    std::string GetHash() override {
        std::string hash = ObjectType + "{";
        for (auto& prop : Properties) {
            hash += prop.second.second->GetHash();
        }
        hash += "}";
        return hash;
    }

};

#endif // CLASSINSTANCE_H