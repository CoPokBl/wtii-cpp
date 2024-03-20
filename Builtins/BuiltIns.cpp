#include "BuiltIns.h"
#include "Functions/Standard.h"
#include "Functions/IO.h"

// Initialise all the fields
const std::map<std::string, MethodDefinitionStatement*> BuiltIns::MethodDefinitions = {
        // IO
        { "print", new MethodDefinitionStatement("print", "null", { std::make_pair("s", "string") }) },
        { "println", new MethodDefinitionStatement("println", "null", { std::make_pair("s", "string") }) },
        { "read_key", new MethodDefinitionStatement("read_key", "string", {}) },
        { "read_line", new MethodDefinitionStatement("read_line", "string", {}) },

        // Standard
        { "exit", new MethodDefinitionStatement("exit", "null", { std::make_pair("code", "int") }) },
        { "str", new MethodDefinitionStatement("str", "string", { std::make_pair("value", "any") }) },
        { "add", new MethodDefinitionStatement("add", "int", { std::make_pair("a", "int"), std::make_pair("b", "int") }) },
        { "subtract", new MethodDefinitionStatement("subtract", "int", { std::make_pair("a", "int"), std::make_pair("b", "int") }) },
        { "multiply", new MethodDefinitionStatement("multiply", "int", { std::make_pair("a", "int"), std::make_pair("b", "int") }) },
        { "divide", new MethodDefinitionStatement("divide", "int", { std::make_pair("a", "int"), std::make_pair("b", "int") }) },
        { "concat", new MethodDefinitionStatement("concat", "string", { std::make_pair("a", "string"), std::make_pair("b", "string") }) },
        { "equals" , new MethodDefinitionStatement("equals", "bool", { std::make_pair("a", "any"), std::make_pair("b", "any") }) },
        { "not", new MethodDefinitionStatement("not", "bool", { std::make_pair("a", "bool") }) },
        { "get_array_object", new MethodDefinitionStatement("get_array_object", "any", { std::make_pair("a", "int") }) },
        { "type_of", new MethodDefinitionStatement("type_of", "string", { std::make_pair("a", "null") })},
        { "assert", new MethodDefinitionStatement("assert", "null", { std::make_pair("a", "bool") }) },
        { "split", new MethodDefinitionStatement("split", "string[]", { std::make_pair("a", "string"), std::make_pair("b", "string") }) },
        { "get_var", new MethodDefinitionStatement("get_var", "any", { std::make_pair("a", "string") }) },
        { "load_lib", new MethodDefinitionStatement("load_lib", "null", { std::make_pair("a", "string") }) },
        { "get_runtime", new MethodDefinitionStatement("get_runtime", "string", { })}
};

const std::map<std::string, std::function<Value*(std::vector<Value*>)>> BuiltIns::Functions = {
        // IO
        { "print", IO::Print },
        { "println", IO::PrintLine },
        { "read_key", IO::ReadKey },
        { "read_line", IO::ReadLine },

        // Standard
        { "exit", Standard::Exit },
        { "str", Standard::Str },
        { "add", Standard::Add },
        { "subtract", Standard::Subtract },
        { "multiply", Standard::Multiply },
        { "divide", Standard::Divide },
        { "concat", Standard::Concat },
        { "equals", Standard::Equals },
        { "not", Standard::Not },
        { "get_array_object", Standard::GetArrayObject },
        { "type_of", Standard::TypeOf },
        { "assert", Standard::Assert },
        { "split", Standard::Split },
        { "load_lib", Standard::LoadLib },
        { "get_var", Standard::GetVar },
        { "get_runtime", Standard::GetRuntime }
};

const std::map<std::string, int> BuiltIns::Libraries = {
};