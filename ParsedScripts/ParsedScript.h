#include "ClassDefinition.h"
#include <vector>

#ifndef WTIIINTERPRETER_PARSEDSCRIPT_H
#define WTIIINTERPRETER_PARSEDSCRIPT_H

class ParsedScript {
public:
    std::vector<Statement*> Statements;
    std::vector<ClassDefinition*> Classes;

    ParsedScript() = default;
};

#endif //WTIIINTERPRETER_PARSEDSCRIPT_H