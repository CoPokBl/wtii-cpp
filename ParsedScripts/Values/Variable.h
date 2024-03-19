#include <vector>
#include <string>
#include "Value.h"

class Variable : public Value {
public:
    ValueType Type = ValueType::Variable;
    std::vector<std::string> Path;

    Variable(std::vector<std::string> path) {
        Path = path;
    }

    Variable(std::vector<std::string> path, std::string ot) {
        Path = path;
        ObjectType = ot;
    }

    Variable() = default;

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
        for (const auto& p : Path) {
            hash += p;
        }
        hash += "}";
        return hash;
    }
};