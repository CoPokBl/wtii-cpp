#ifndef WTIIINTERPRETER_RUNTIMEEXCEPTION_H
#define WTIIINTERPRETER_RUNTIMEEXCEPTION_H

#include <exception>
#include "ParsedScripts/Values/Value.h"

class RuntimeException : public std::exception {
public:
    Value* Object;

    [[nodiscard]] const char* what() const noexcept override {
        return "Runtime exception";
    }

    explicit RuntimeException(Value* object) {
        Object = object;
    }
};


#endif //WTIIINTERPRETER_RUNTIMEEXCEPTION_H
