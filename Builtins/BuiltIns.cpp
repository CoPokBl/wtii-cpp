#include "BuiltIns.h"
#include "Functions/Standard.h"
#include "Functions/IO.h"
#include "Functions/Convert.h"
#include "Functions/Files.h"
#include "Functions/WtiiJson.h"

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
        { "string", new MethodDefinitionStatement("str", "string", { std::make_pair("value", "any") }) },
        { "add", new MethodDefinitionStatement("add", "int", { std::make_pair("a", "int"), std::make_pair("b", "int") }) },
        { "subtract", new MethodDefinitionStatement("subtract", "int", { std::make_pair("a", "int"), std::make_pair("b", "int") }) },
        { "multiply", new MethodDefinitionStatement("multiply", "int", { std::make_pair("a", "int"), std::make_pair("b", "int") }) },
        { "divide", new MethodDefinitionStatement("divide", "int", { std::make_pair("a", "int"), std::make_pair("b", "int") }) },
        { "concat", new MethodDefinitionStatement("concat", "string", { std::make_pair("a", "string"), std::make_pair("b", "string") }) },
        { "equals" , new MethodDefinitionStatement("equals", "bool", { std::make_pair("a", "any"), std::make_pair("b", "any") }) },
        { "not", new MethodDefinitionStatement("not", "bool", { std::make_pair("a", "bool") }) },
        { "get_array_object", new MethodDefinitionStatement("get_array_object", "any", { std::make_pair("arr", "any[]"), std::make_pair("a", "int") }) },
        { "type_of", new MethodDefinitionStatement("type_of", "string", { std::make_pair("a", "null") })},
        { "assert", new MethodDefinitionStatement("assert", "null", { std::make_pair("a", "bool") }) },
        { "split", new MethodDefinitionStatement("split", "string[]", { std::make_pair("a", "string"), std::make_pair("b", "string") }) },
        { "get_var", new MethodDefinitionStatement("get_var", "any", { std::make_pair("a", "string") }) },
        { "load_lib", new MethodDefinitionStatement("load_lib", "null", { std::make_pair("a", "string") }) },
        { "get_runtime", new MethodDefinitionStatement("get_runtime", "string", { }) },
        { "more_than", new MethodDefinitionStatement("more_than", "bool", { std::make_pair("a", "float"), std::make_pair("b", "float") }) },
        { "less_than", new MethodDefinitionStatement("less_than", "bool", { std::make_pair("a", "float"), std::make_pair("b", "float") }) },
        { "more_than_or_equals", new MethodDefinitionStatement("more_than_or_equals", "bool", { std::make_pair("a", "float"), std::make_pair("b", "float") }) },
        { "less_than_or_equals", new MethodDefinitionStatement("less_than_or_equals", "bool", { std::make_pair("a", "float"), std::make_pair("b", "float") }) },
        { "and", new MethodDefinitionStatement("and", "bool", { std::make_pair("a", "bool"), std::make_pair("b", "bool") }) },
        { "or", new MethodDefinitionStatement("or", "bool", { std::make_pair("a", "bool"), std::make_pair("b", "bool") }) },

        // Convert
        { "to_int", new MethodDefinitionStatement("to_int", "int", { std::make_pair("a", "float") }) },
        { "to_float", new MethodDefinitionStatement("to_float", "float", { std::make_pair("a", "int") }) },

        // Files
        { "read_file_as_text", new MethodDefinitionStatement("read_file_as_text", "string", { std::make_pair("path", "string") }) },
        { "write_text_to_file", new MethodDefinitionStatement("write_text_to_file", "null", { std::make_pair("path", "string"), std::make_pair("text", "string") }) },
        { "file_exists", new MethodDefinitionStatement("file_exists", "bool", { std::make_pair("path", "string") }) },
        { "delete_file", new MethodDefinitionStatement("delete_file", "null", { std::make_pair("path", "string") }) },
        { "get_files", new MethodDefinitionStatement("get_files", "string[]", { std::make_pair("path", "string") }) },
        { "get_directories", new MethodDefinitionStatement("get_directories", "string[]", { std::make_pair("path", "string") }) },
        { "create_directory", new MethodDefinitionStatement("create_directory", "null", { std::make_pair("path", "string") }) },
        { "delete_directory", new MethodDefinitionStatement("delete_directory", "null", { std::make_pair("path", "string") }) },
        { "directory_exists", new MethodDefinitionStatement("directory_exists", "bool", { std::make_pair("path", "string") }) },
        { "get_current_directory", new MethodDefinitionStatement("get_current_directory", "string", { }) },
        { "set_current_directory", new MethodDefinitionStatement("set_current_directory", "null", { std::make_pair("path", "string") }) },

        // JSON
        { "to_json", new MethodDefinitionStatement("to_json", "string", { std::make_pair("a", "any") }) },
        { "from_json", new MethodDefinitionStatement("from_json", "any", { std::make_pair("a", "string") }) }
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
        { "string", Standard::Str },
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
        { "get_runtime", Standard::GetRuntime },
        { "more_than", Standard::MoreThan },
        { "less_than", Standard::LessThan },
        { "more_than_or_equals", Standard::MoreThanOrEquals },
        { "less_than_or_equals", Standard::LessThanOrEquals },
        { "and", Standard::And },
        { "or", Standard::Or },

        // Convert
        { "to_int", Convert::ToInt },
        { "to_float", Convert::ToFloat },

        // Files
        { "read_file_as_text", Files::ReadFileAsText },
        { "write_text_to_file", Files::WriteTextToFile },
        { "file_exists", Files::FileExists },
        { "delete_file", Files::DeleteFile },
        { "get_files", Files::GetFiles },
        { "get_directories", Files::GetDirectories },
        { "create_directory", Files::CreateDirectory },
        { "delete_directory", Files::DeleteDirectory },
        { "directory_exists", Files::DirectoryExists },
        { "get_current_directory", Files::GetCurrentDirectory },
        { "set_current_directory", Files::SetCurrentDirectory },
        { "get_full_path", Files::GetFullPath },

        // JSON
        { "to_json", WtiiJson::ToJson },
        { "from_json", WtiiJson::FromJson }
};

const std::map<std::string, int> BuiltIns::Libraries = {
};