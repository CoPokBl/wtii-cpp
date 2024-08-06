#ifndef WTIIINTERPRETER_BUILTINS_H
#define WTIIINTERPRETER_BUILTINS_H


#include <map>
#include <string>
#include <functional>
#include "../ParsedScripts/Values/Value.h"
#include "../ParsedScripts/Statements/MethodDefinitiionStatement.h"
#include "../Libraries/IWtiiLibrary.h"

class BuiltIns {
public:
    static const std::map<std::string, IWtiiLibrary*> Libraries;
    static const std::map<std::string, std::function<Value*(std::vector<Value*>)>> Functions;
    static const std::map<std::string, MethodDefinitionStatement*> MethodDefinitions;
};


#endif //WTIIINTERPRETER_BUILTINS_H
