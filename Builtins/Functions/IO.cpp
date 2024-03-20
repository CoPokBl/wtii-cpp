#include <iostream>
#include "IO.h"
#include "../../ParsedScripts/Values/Constant.h"
#include "../../Interpreter/Interpreter.h"
#include "../../Utils.h"

Value* IO::Print(std::vector<Value*> args) {
    if (args.size() != 1) {
        throw_err("print() takes exactly 1 argument");
    }
    RealReference* reference = Interpreter::ResolveValue(args[0]);
    auto constant = dynamic_cast<Constant*>(reference);
    if (!constant) {
        throw_err("print() argument must be a constant.");
    }
    std::cout << constant->Value;
    return Null;
}

Value* IO::PrintLine(std::vector<Value*> args) {
    if (args.size() != 1) {
        throw_err("println() takes exactly 1 argument");
    }
    RealReference* reference = Interpreter::ResolveValue(args[0]);
    auto constant = dynamic_cast<Constant*>(reference);
    if (!constant) {
        throw_err("println() argument must be a constant.");
    }
    std::cout << constant->Value << std::endl;
    return Null;
}

Value* IO::ReadKey(const std::vector<Value*>& args) {  // Functions the same as dotnet's Console.ReadKey(), doesn't need enter to be pressed
    if (!args.empty()) {
        throw_err("read_key() takes exactly 0 arguments");
    }
    char key = std::cin.get();
    return new Constant(std::string(1, key), "string");
}

Value* IO::ReadLine(const std::vector<Value*>& args) {
    if (!args.empty()) {
        throw_err("read_line() takes exactly 0 arguments");
    }
    std::string line;
    std::cin >> line;
    return new Constant(line, "string");
}