#ifndef WTIIINTERPRETER_FILES_H
#define WTIIINTERPRETER_FILES_H


#include <vector>
#include "../../ParsedScripts/Values/Value.h"

class Files {
public:
    static Value* ReadFileAsText(std::vector<Value*> args);
    static Value* WriteTextToFile(std::vector<Value*> args);
    static Value* FileExists(std::vector<Value*> args);
    static Value* DeleteFile(std::vector<Value*> args);
    static Value* GetFiles(std::vector<Value*> args);
    static Value* GetDirectories(std::vector<Value*> args);
    static Value* CreateDirectory(std::vector<Value*> args);
    static Value* DeleteDirectory(std::vector<Value*> args);
    static Value* DirectoryExists(std::vector<Value*> args);
    static Value* GetCurrentDirectory(const std::vector<Value*>& args);
    static Value* SetCurrentDirectory(const std::vector<Value*>& args);
    static Value* GetFullPath(const std::vector<Value*>& args);
};


#endif //WTIIINTERPRETER_FILES_H
