cmake_minimum_required(VERSION 3.22)
project(WtiiInterpreter)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(DEBUG)
endif()

add_executable(WtiiInterpreter main.cpp
        ParsedScripts/Values/Value.h
        ParsedScripts/Values/ArrayValue.h
        ParsedScripts/Values/Constant.h
        ParsedScripts/Values/RealReference.h
        ParsedScripts/Values/MethodCall.h
        ParsedScripts/Values/Variable.h
        ParsedScripts/Statements/EnterScopeStatement.h
        ParsedScripts/Statements/Statement.h
        ParsedScripts/Statements/ExitScopeStatement.h
        ParsedScripts/Statements/FunctionCallStatement.h
        ParsedScripts/Statements/IfStatement.h
        ParsedScripts/Statements/LoadLibStatement.h
        ParsedScripts/Statements/MethodDefinitiionStatement.h
        ParsedScripts/Statements/ReturnStatement.h
        ParsedScripts/Statements/ThrowStatement.h
        ParsedScripts/Statements/TryCatchStatement.h
        ParsedScripts/Statements/VariableInitStatement.h
        ParsedScripts/Statements/VariableSetStatement.h
        ParsedScripts/Statements/WhileStatement.h
        ParsedScripts/ClassDefinition.h
        ParsedScripts/ParsedScript.h
        ParsedScripts/Values/ClassInstance.h
        ParsedScripts/Values/DirectMethod.h
        Scope.h
        Parser/Parser.cpp
        Parser/Parser.h
        Interpreter/Interpreter.cpp
        Interpreter/Interpreter.h
        Builtins/BuiltIns.h
        RuntimeException.h
        ProgramExitException.h
        Utils.cpp
        Utils.h
        Builtins/BuiltIns.cpp
        Builtins/Functions/Standard.cpp
        Builtins/Functions/Standard.h
        Builtins/Functions/IO.cpp
        Builtins/Functions/IO.h)
