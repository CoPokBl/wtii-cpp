// In VarValue.h
#ifndef VALUE_TYPE_DEFINED
#define VALUE_TYPE_DEFINED

#include <string>

enum class ValueType {
    Constant,       // 0
    Variable,       // 1
    MethodCall,     // 2
    ArrayValue,     // 3
    ClassInstance,  // 4
    DirectMethod    // 5
};

class Value {
public:
    ValueType Type;
    std::string ObjectType;
    virtual ~Value() = default;
    virtual std::string GetHash() {
        return "0";
    }

    Value() = default;
};

#endif // VALUE_TYPE_DEFINED