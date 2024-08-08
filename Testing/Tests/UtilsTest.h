#ifndef WTIIINTERPRETER_UTILSTEST_H
#define WTIIINTERPRETER_UTILSTEST_H

#include "../TestMethods.h"
#include "../../Utils.h"

void Trim() {
    AssertEqual(Utils::TrimString("     Hello World      "), "Hello World", "Trim start and end string");
    AssertEqual(Utils::TrimString("      Hello there"), "Hello there", "Trim start");
    AssertEqual(Utils::TrimString("Hello person                  "), "Hello person", "Trim end");
    AssertEqual(Utils::TrimString(" Hello "), "Hello", "Short trim");
}

#endif //WTIIINTERPRETER_UTILSTEST_H
