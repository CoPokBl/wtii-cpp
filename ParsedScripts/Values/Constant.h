#ifndef CONSTANT_H
#define CONSTANT_H


#include <string>
#include <utility>
#include "RealReference.h"

class Constant : public RealReference {
public:
    ValueType Type = ValueType::Constant;
    std::string Value;

    Constant(std::string s, std::string type) {
        Value = std::move(s);
        ObjectType = std::move(type);
    }

    static Constant FromString(std::string s) {
        return {std::move(s), "string"};
    }

    std::string ToString() {
        return Value;
    }

    std::string GetHash() override {
        return ObjectType + "{" + Value + "}";
    }

    Constant() = default;
};

#endif // CONSTANT_H