#ifndef WTIIINTERPRETER_COMPILER_H
#define WTIIINTERPRETER_COMPILER_H

#include "../ParsedScripts/ParsedScript.h"
#include "../ParsedScripts/Values/Variable.h"

class Compiler {
    static MethodDefinitionStatement* ParseDotNotationMethod(std::string value, std::vector<std::string>& parts, std::string& type);
    static Value* EvalValue(const std::string& value);
    static Variable* ParseDotNotationVariable(std::string value, std::string& type);
public:
    static ParsedScript* Parse(std::string code);
    static ParsedScript* Parse(std::vector<std::string>& lines, bool newScope = true);
};


#endif //WTIIINTERPRETER_COMPILER_H
