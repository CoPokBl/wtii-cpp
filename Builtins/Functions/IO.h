#ifndef WTIIINTERPRETER_IO_H
#define WTIIINTERPRETER_IO_H


#include <vector>
#include "../../ParsedScripts/Values/Value.h"

class IO {
public:
    static Value* Print(std::vector<Value*> args);
    static Value* PrintLine(std::vector<Value*> args);
    static Value* ReadKey(const std::vector<Value*>& args);
    static Value* ReadLine(const std::vector<Value*>& args);
};


#endif //WTIIINTERPRETER_IO_H
