#ifndef WTIIINTERPRETER_COMPILEREXCEPTION_H
#define WTIIINTERPRETER_COMPILEREXCEPTION_H

#include <exception>
#include <utility>
#include "ParsedScripts/Values/Value.h"

class CompilerException : public std::exception {
public:
    std::string Msg;

    [[nodiscard]] const char* what() const noexcept override {
        return Msg.c_str();
    }

    explicit CompilerException(std::string object) {
        Msg = std::move(object);
    }
};


#endif //WTIIINTERPRETER_COMPILEREXCEPTION_H
