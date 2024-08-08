#ifndef WTIIINTERPRETER_COMPILER_H
#define WTIIINTERPRETER_COMPILER_H

#include "../ParsedScripts/ParsedScript.h"

class Compiler {
public:
    static ParsedScript* Parse(std::string code);
    static ParsedScript* Parse(std::vector<std::string>& lines, bool newScope = true);
};


#endif //WTIIINTERPRETER_COMPILER_H
