#ifndef WTIIINTERPRETER_PROGRAMEXITEXCEPTION_H
#define WTIIINTERPRETER_PROGRAMEXITEXCEPTION_H

#include <exception>

class ProgramExitException : public std::exception {
public:
    int ExitCode;

    [[nodiscard]] const char* what() const noexcept override {
        return "Program exit exception";
    }

    explicit ProgramExitException(int exc) {
        ExitCode = exc;
    }
};


#endif //WTIIINTERPRETER_PROGRAMEXITEXCEPTION_H
