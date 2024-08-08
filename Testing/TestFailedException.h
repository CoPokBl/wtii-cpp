#ifndef WTIIINTERPRETER_TESTFAILEDEXCEPTION_H
#define WTIIINTERPRETER_TESTFAILEDEXCEPTION_H

#include <string>
#include <utility>

class TestFailedException : std::exception {

public:
    std::string reason;

    explicit TestFailedException(std::string r) {
        reason = std::move(r);
    }
};

#endif //WTIIINTERPRETER_TESTFAILEDEXCEPTION_H
