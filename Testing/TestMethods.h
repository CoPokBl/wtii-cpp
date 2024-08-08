#ifndef WTIIINTERPRETER_TESTMETHODS_H
#define WTIIINTERPRETER_TESTMETHODS_H

#include <string>
#include <utility>
#include <cstring>
#include "TestFailedException.h"

void Throw(std::string reason) {
    throw TestFailedException(std::move(reason));
}

void AssertEqual(int& a, int& b, std::string msg) {
    if (a != b) {
        Throw(std::move(msg));
    }
}

void AssertEqual(const std::string& a, const std::string& b, const std::string& msg) {
    if (a != b) {
        std::string fullMsg = msg + " Expected: " + b + ". Got: " + a;
        Throw(std::move(fullMsg));
    }
}

void AssertTrue(bool val, std::string msg) {
    if (!val) {
        Throw(std::move(msg));
    }
}

#endif //WTIIINTERPRETER_TESTMETHODS_H
