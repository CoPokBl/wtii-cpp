#ifndef WTIIINTERPRETER_STANDARD_H
#define WTIIINTERPRETER_STANDARD_H


#include <vector>
#include "../../ParsedScripts/Values/Value.h"
#include "../../RuntimeException.h"
#include "../../ParsedScripts/Values/Constant.h"
#include "../../Utils.h"

class Standard {
public:
    static Value* Exit(std::vector<Value*> args);
    static Value* Str(std::vector<Value*> args);
    static Value* Add(std::vector<Value*> args);
    static Value* Subtract(std::vector<Value*> args);
    static Value* Multiply(std::vector<Value*> args);
    static Value* Divide(std::vector<Value*> args);
    static Value* Concat(std::vector<Value*> args);
    static Value* Equals(std::vector<Value*> args);
    static Value* Not(std::vector<Value*> args);
    static Value* GetArrayObject(std::vector<Value*> args);
    static Value* TypeOf(std::vector<Value*> args);
    static Value* Assert(std::vector<Value*> args);
    static Value* Split(std::vector<Value*> args);
    static Value* LoadLib(std::vector<Value*> args);
    static Value* GetVar(std::vector<Value*> args);
    static Value* GetRuntime(const std::vector<Value*>& args);
    static Value* MoreThan(std::vector<Value*> args);
    static Value* LessThan(std::vector<Value*> args);
    static Value* MoreThanOrEquals(std::vector<Value*> args);
    static Value* LessThanOrEquals(std::vector<Value*> args);
    static Value* And(std::vector<Value*> args);
    static Value* Or(std::vector<Value*> args);
};


#endif //WTIIINTERPRETER_STANDARD_H
