#ifndef WTIIINTERPRETER_LIBRARY_H
#define WTIIINTERPRETER_LIBRARY_H

#include <string>

#if defined (_MSC_VER) // Microsoft compiler
    #define EXPORT __declspec(dllexport)
#elif defined (__GNUC__) // GCC
    #define EXPORT __attribute__((visibility("default")))
#else
    #define EXPORT
#endif

extern "C" {
    EXPORT int interpret(std::string filename);
}

#endif //WTIIINTERPRETER_LIBRARY_H
