#include <utility>
#include <vector>
#include <string>
#include "RealReference.h"

class ArrayValue : public RealReference {
public:
    ValueType Type = ValueType::ArrayValue;
    std::vector<Value*> Values;
    std::string BaseObjectType;

    ArrayValue(const std::string& baseObjectType, std::vector<Value*> values) {
        Values = std::move(values);
        ObjectType = baseObjectType + "[]";
        BaseObjectType = baseObjectType;
    }

    std::string GetHash() override {
        std::string hash = ObjectType + "{";
        for (auto& value : Values) {
            hash += value->GetHash();
        }
        hash += "}";
        return hash;
    }

    ArrayValue() = default;
};