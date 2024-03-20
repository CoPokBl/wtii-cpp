#ifndef WTIIINTERPRETER_INTERPRETER_H
#define WTIIINTERPRETER_INTERPRETER_H


#include <stack>
#include "../ParsedScripts/ParsedScript.h"
#include "../ParsedScripts/Values/RealReference.h"
#include "../RuntimeException.h"
#include "../ParsedScripts/Values/Variable.h"
#include "../ParsedScripts/Values/DirectMethod.h"
#include "../ParsedScripts/Statements/FunctionCallStatement.h"
#include "../ParsedScripts/Values/ClassInstance.h"
#include "../Scope.h"

class Interpreter {
public:
    static int Interpret(ParsedScript* script);
    static RealReference* ResolveValue(Value* value, std::string exceptedType = "");
    static std::stack<Scope*>* GetScope();
    static void LoadLibrary(const std::string& name);

private:
    static void NewScope();
    static void EndScope();
    static std::string ToLower(std::string str);
    static std::string CapitaliseFirstLetter(std::string str);
    static bool IsCorrectConversionName(std::string name, std::string fromType, std::string toType);
    static MethodDefinitionStatement* SearchDefsForConversion(const std::map<std::string, MethodDefinitionStatement*>& definitions, std::string fromType, std::string toType);
    static MethodDefinitionStatement* FindConversionMethod(const std::string& fromType, const std::string& expectedType);
    static Value* EvalFunction(MethodDefinitionStatement* method, std::vector<Value*> args, const std::string& callName = "NOT SPECIFIC");

    static RuntimeException Error(std::string msg);
    static Value* EvalVariable(Variable* variable);
    static Value* EvalFunction(MethodDefinitionStatement* method, std::vector<Value*> args);
    static Value* EvalFunction(DirectMethod* call);
    static Value* ExecuteFunction(MethodDefinitionStatement* function, bool& didReturn);
    static Value* EvalFunction(FunctionCallStatement* call);
    static Value* ExecuteFunction(MethodDefinitionStatement* function);
    static std::pair<MethodDefinitionStatement*, ClassInstance*> GetMethodDefinition(FunctionCallStatement* call);
};


#endif //WTIIINTERPRETER_INTERPRETER_H
