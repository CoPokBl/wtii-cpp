#ifndef WTIIINTERPRETER_IWTIILIBRARY_H
#define WTIIINTERPRETER_IWTIILIBRARY_H


#include "../Scope.h"

class IWtiiLibrary {
public:
    virtual Scope* GetScope() = 0;
    virtual void Init() = 0;
    virtual void Run() = 0;
};


#endif //WTIIINTERPRETER_IWTIILIBRARY_H
