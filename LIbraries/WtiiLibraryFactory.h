#ifndef WTIIINTERPRETER_WTIILIBRARYFACTORY_H
#define WTIIINTERPRETER_WTIILIBRARYFACTORY_H


#include "IWtiiLibrary.h"

class WtiiLibraryFactory {
public:
    typedef IWtiiLibrary* (*CreateLibraryFn)();

    void* lib_handle;
    CreateLibraryFn createLibrary;

    void LoadLibraries(const std::string& path);
    IWtiiLibrary* createInstance();
};


#endif //WTIIINTERPRETER_WTIILIBRARYFACTORY_H
