#include <utility>
#include <vector>
#include <string>
#include "Value.h"
#include "../Statements/FunctionCallStatement.h"

class MethodCall : public Value {
public:
    ValueType Type = ValueType::MethodCall;
    std::vector<std::string> Path;
    std::vector<Value*> Arguments;

    MethodCall(std::vector<std::string> path, std::vector<Value*> args, std::string objectType = "") {
        Path = std::move(path);
        Arguments = std::move(args);
        ObjectType = std::move(objectType);
    }

    MethodCall() = default;

    static MethodCall* Not(MethodCall* call) {
        return new MethodCall({"not"}, {call}, "bool");
    }

    FunctionCallStatement* ToFunctionCall() {
        return new FunctionCallStatement(Path, Arguments);
    }

    int GetHashCode() {
        std::hash<std::string> hash_fn;
        int hash = 0;
        for (const auto& p : Path) {
            hash ^= hash_fn(p);
        }
        return hash;
    }

    std::string GetHash() override {
        std::string hash = ObjectType + "{";
        for (auto& arg : Arguments) {
            hash += arg->GetHash();
        }
        hash += "}";
        return hash;
    }
};