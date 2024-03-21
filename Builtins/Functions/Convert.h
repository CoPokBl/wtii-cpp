#ifndef WTIIINTERPRETER_CONVERT_H
#define WTIIINTERPRETER_CONVERT_H

#include <vector>
#include "../../ParsedScripts/Values/Value.h"

class Convert {
public:
    static Value* ToInt(std::vector<Value*> args);
    static Value* ToFloat(std::vector<Value*> args);
};


#endif //WTIIINTERPRETER_CONVERT_H
