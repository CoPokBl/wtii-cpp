#include "../Statements/MethodDefinitiionStatement.h"
#include <utility>
#include <vector>

class DirectMethod : public Value {
public:
    ValueType Type = ValueType::DirectMethod;
    MethodDefinitionStatement* Method = nullptr;
    std::vector<Value*> Arguments;

    DirectMethod() = default;

    DirectMethod(MethodDefinitionStatement* method, std::vector<Value*> args) {
        Method = method;
        Arguments = std::move(args);
    }

    std::string GetHash() override {
        std::string hash = Method->Name + "{";
        for (auto& arg : Arguments) {
            hash += arg->GetHash();
        }
        hash += "}";
        return hash;
    }

};