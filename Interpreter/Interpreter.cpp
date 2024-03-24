#include <iostream>
#include <utility>
#include <fstream>
#include "Interpreter.h"
#include "stack"
#include "../ParsedScripts/Values/RealReference.h"
#include "../ParsedScripts/Values/MethodCall.h"
#include "../ParsedScripts/Values/Constant.h"
#include "../Builtins/BuiltIns.h"
#include "../ParsedScripts/Statements/ThrowStatement.h"
#include "../ProgramExitException.h"
#include "../Utils.h"
#include "../ParsedScripts/Statements/TryCatchStatement.h"
#include "../ParsedScripts/Statements/ReturnStatement.h"
#include "../ParsedScripts/Statements/VariableSetStatement.h"
#include "../ParsedScripts/Statements/IfStatement.h"
#include "../ParsedScripts/Statements/WhileStatement.h"
#include "../ParsedScripts/Statements/EnterScopeStatement.h"
#include "../ParsedScripts/Statements/ExitScopeStatement.h"
#include "../ParsedScripts/Statements/LoadLibStatement.h"
#include "../LIbraries/WtiiLibraryFactory.h"
#include "../Parser/Parser.h"


static std::stack<Scope*> scopes;

void Interpreter::NewScope() {
    scopes.push(new Scope(*scopes.top()));
}

void Interpreter::EndScope() {
    Scope* old = scopes.top();
    scopes.pop();

    for (auto& kvp : old->Variables) {
        if (!scopes.top()->Variables.count(kvp.first)) {  // If the variable is not already defined in the parent scope
            continue;
        }

        // If it is then we need to update it
        scopes.top()->Variables[kvp.first] = old->Variables[kvp.first];
    }

    delete old;
}

RuntimeException Interpreter::Error(std::string msg) {
    return RuntimeException(new Constant(std::move(msg), "string"));
}

std::string Interpreter::ToLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

std::string Interpreter::CapitaliseFirstLetter(std::string str) {
    str[0] = std::toupper(str[0]);
    return str;
}

bool Interpreter::IsCorrectConversionName(std::string name, std::string fromType, std::string toType) {
    if (name == ToLower(fromType) + "_to_" + ToLower(toType)) return true;
    if (name == "to_" + ToLower(toType)) return true;
    if (name == CapitaliseFirstLetter(fromType) + "To" + CapitaliseFirstLetter(toType)) return true;
    if (name == ToLower(toType)) return true;
    if (name == CapitaliseFirstLetter(toType)) return true;
    return false;
}

MethodDefinitionStatement* Interpreter::SearchDefsForConversion(const std::map<std::string, MethodDefinitionStatement*>& definitions, std::string fromType, std::string toType) {
    for (auto const& pair : definitions) {
        std::string name = pair.first;
        MethodDefinitionStatement* value = pair.second;
        if (!IsCorrectConversionName(name, fromType, toType)) continue;
        if (value->ReturnType != toType) continue;
        if (value->Arguments.size() != 1) continue;
        if (value->ArgumentTypes[0] != fromType && value->ArgumentTypes[0] != "any") continue;
        return value;
    }
    return nullptr;
}

MethodDefinitionStatement* Interpreter::FindConversionMethod(const std::string& fromType, const std::string& expectedType) {
    // Check if the type is a currently declared class
    if (scopes.top()->Classes.count(expectedType) != 0) {
        ClassDefinition* classDef = scopes.top()->Classes[expectedType];
        MethodDefinitionStatement* classMethod = SearchDefsForConversion(classDef->GetMethodsAsMap(), fromType, expectedType);
        if (classMethod != nullptr) {
            return classMethod;
        }
    }

    // Check for user defined conversion methods in current scope
    MethodDefinitionStatement* method = SearchDefsForConversion(scopes.top()->Functions, fromType, expectedType);
    if (method != nullptr) {
        return method;
    }

    // Check for valid built-in conversion methods
    MethodDefinitionStatement* builtInMethod = SearchDefsForConversion(BuiltIns::MethodDefinitions, fromType, expectedType);
    if (builtInMethod != nullptr) {
        builtInMethod->CsFunc = BuiltIns::Functions.at(builtInMethod->Name);
    }

    return builtInMethod;  // If no method is found, return nullptr
}

std::pair<MethodDefinitionStatement*, ClassInstance*> Interpreter::GetMethodDefinition(FunctionCallStatement* call) {
    if (call->Path.size() == 1) {
        return {scopes.top()->Functions[call->Path[0]], nullptr};
    }

    ClassInstance* currentHop = nullptr;
    for (int i = 0; i < call->Path.size() - 1; i++) {
        std::string currentToken = call->Path[i];
        std::map<std::string, std::pair<std::string, Value*>> properties = currentHop == nullptr ? scopes.top()->Variables : currentHop->Properties;
        Value* val = properties[currentToken].second;
        if (!dynamic_cast<ClassInstance*>(val)) {
            throw_err("Variable " + currentToken + " is not a class instance.");
        }
        currentHop = dynamic_cast<ClassInstance*>(val);
    }

    if (currentHop == nullptr) {
        throw_err("currentHop is null. IMPOSSIBLE");
    }

    return {currentHop->Methods[call->Path.back()], currentHop};
}

void SetVariable(std::vector<std::string> path, Value* newValue) {
    if (path.size() == 1) {
        if (scopes.top()->Variables.find(path[0]) == scopes.top()->Variables.end()) {
            throw_err("Variable '" + path[0] + "' does not exist in this scope.");
        }
        // Assign the variable a value while preserving the type
        scopes.top()->Variables[path[0]] = std::make_pair(scopes.top()->Variables[path[0]].first, newValue);
        return;
    }

    ClassInstance* currentHop = nullptr;
    for (int i = 0; i < path.size() - 1; i++) {
        std::string currentToken = path[i];
        std::map<std::string, std::pair<std::string, Value*>> properties = currentHop == nullptr ? scopes.top()->Variables : currentHop->Properties;
        Value* val = properties.count(currentToken) ? properties[currentToken].second : Null;
        if (dynamic_cast<ClassInstance*>(val) == nullptr) {
            throw_err("Variable " + currentToken + " is not a class instance.");
        }
        currentHop = dynamic_cast<ClassInstance*>(val);
    }
    if (currentHop == nullptr) {
        throw_err("currentHop is null. IMPOSSIBLE");
    }
    // Assign the variable a value while preserving the type
    currentHop->Properties[path.back()] = std::make_pair(currentHop->Properties[path.back()].first, newValue);
}

Value* Interpreter::EvalFunction(MethodDefinitionStatement* method, std::vector<Value*> args) {
    return Interpreter::EvalFunction(method, std::move(args), "Anonymous");
}

Value* Interpreter::EvalFunction(DirectMethod* call) {
    return Interpreter::EvalFunction(call->Method, call->Arguments, "DIRECT METHOD");
}

Value* Interpreter::EvalFunction(FunctionCallStatement* call) {
    std::string callName = call->Path[0];

    if (BuiltIns::Functions.count(callName)) {
        auto function = BuiltIns::Functions.find(callName)->second;
        MethodDefinitionStatement* def = BuiltIns::MethodDefinitions.find(callName)->second;
        if (def->Arguments.size() != call->Arguments.size()) {
            throw Error("Argument count mismatch in function call: " + callName + ". Expected " + std::to_string(def->Arguments.size()) + " but got " + std::to_string(call->Arguments.size()));
        }
        std::vector<Value*> evaledArgs(call->Arguments.size());
        for (int i = 0; i < call->Arguments.size(); i++) {
            evaledArgs[i] = ResolveValue(call->Arguments[i], def->ArgumentTypes[i]);
        }
        for (int i = 0; i < def->Arguments.size(); i++) {
            if (def->ArgumentTypes[i] == "any" || (def->ArgumentTypes[i] == "any[]" && Utils::EndsWith(evaledArgs[i]->ObjectType, "[]"))) {
                continue;
            }
            if (def->ArgumentTypes[i] != evaledArgs[i]->ObjectType) {
                throw Error("Argument type mismatch in function call: " + callName + ". Expected " + def->ArgumentTypes[i] + " but got " + call->Arguments[i]->ObjectType + ".");
            }
        }
        return function(evaledArgs);
    }

    std::pair<MethodDefinitionStatement*, ClassInstance*> methodAndClass = GetMethodDefinition(call);
    MethodDefinitionStatement* method = methodAndClass.first;
    ClassInstance* parentClass = methodAndClass.second;
    if (method == nullptr) {
        throw_err("Unknown function call: " + callName);
    }

    NewScope();
    if (parentClass != nullptr) {
        for (auto& kvp : parentClass->Properties) {
            scopes.top()->Variables[kvp.first] = kvp.second;
        }
        for (auto& kvp : parentClass->Methods) {
            scopes.top()->Functions[kvp.first] = kvp.second;
        }
    }

    Value* result = EvalFunction(method, call->Arguments, callName);

    if (parentClass != nullptr) {
        //Debug("Checking for changed variables in class...");
        for (auto& kvp : parentClass->Properties) {
            if (scopes.top()->Variables[kvp.first] == parentClass->Properties[kvp.first]) continue;
            parentClass->Properties[kvp.first] = scopes.top()->Variables[kvp.first];
        }
    }

    EndScope();
    if (parentClass != nullptr) {
        std::vector<std::string> pathMinusOne = Utils::SubVector(call->Path, 0, call->Path.size() - 1);
        SetVariable(pathMinusOne, parentClass);
    }
    return result;
}

Value* Interpreter::EvalFunction(MethodDefinitionStatement* method, std::vector<Value*> args, const std::string& callName) {
    if (method->Arguments.size() != args.size()) {
        throw Error("Argument count mismatch in function call: " + callName + ". Expected " + std::to_string(method->Arguments.size()) + " but got " + std::to_string(args.size()));
    }
    NewScope();
    for (int i = 0; i < method->Arguments.size(); i++) {
        std::string argumentType = method->ArgumentTypes[i];
        bool isAny = argumentType == "any" || (argumentType == "any[]" && Utils::EndsWith(args[i]->ObjectType, "[]"));
        if (argumentType != args[i]->ObjectType && !isAny) {
            throw_err("Argument type mismatch in function call: " + callName + ". Expected " + argumentType + " but got " + args[i]->ObjectType + ".");
        }
        std::string arg = method->Arguments[i];
        scopes.top()->SetVariable(arg, ResolveValue(args[i], argumentType));
    }

    Value* result = method->CsFunc != nullptr ? method->CsFunc(args) : ExecuteFunction(method);
    result = ResolveValue(result, method->ReturnType);
    EndScope();
    return result;
}

RealReference* Interpreter::ResolveValue(Value* value, std::string exceptedType) {
    auto* constant = dynamic_cast<RealReference*>(value);
    if (constant) {
        if (constant->ObjectType != exceptedType) {  // Try to convert implicitly
            MethodDefinitionStatement* method = exceptedType.empty() ? nullptr : FindConversionMethod(constant->ObjectType, exceptedType);
            if (method != nullptr) {
                Value* result = Interpreter::EvalFunction(new DirectMethod(method, {constant}));
                return ResolveValue(result, exceptedType);
            }
        }
        return constant;
    }

    auto* variable = dynamic_cast<Variable*>(value);
    if (variable) {
        return ResolveValue(EvalVariable(variable), exceptedType);
    }

    auto* call = dynamic_cast<MethodCall*>(value);
    if (call) {
        Value* eval = EvalFunction(call->ToFunctionCall());
        return ResolveValue(eval, exceptedType);
    }

    throw_err("Unknown value type: " + std::to_string(static_cast<int>(value->Type)));
}

Value* Interpreter::EvalVariable(Variable* variable) {
    if (variable->Path.size() == 1) {  // If single variable then look for it in current scope.
        auto it = scopes.top()->Variables.find(variable->Path[0]);
        if (it != scopes.top()->Variables.end()) {
            return ResolveValue(it->second.second);
        } else {
            throw_err("Variable does not exist: " + variable->Path[0]);
        }
    }

    if (variable->Path.empty()) throw_err("Variable path is empty.");

    ClassInstance* currentHop = nullptr;
    for (size_t i = 0; i < variable->Path.size() - 1; i++) {
        std::string currentToken = variable->Path[i];
        std::map<std::string, std::pair<std::string, Value*>>& properties = currentHop == nullptr ? scopes.top()->Variables : currentHop->Properties;
        auto it = properties.find(currentToken);
        if (it == properties.end() || dynamic_cast<ClassInstance*>(it->second.second) == nullptr) {
            throw_err("Variable " + currentToken + " is not a class instance.");
        }
        currentHop = dynamic_cast<ClassInstance*>(it->second.second);
    }

    if (currentHop == nullptr) throw_err("currentHop is null. IMPOSSIBLE");

    std::string finalToken = variable->Path.back();
    auto it = currentHop->Properties.find(finalToken);
    if (it == currentHop->Properties.end() || dynamic_cast<Variable*>(it->second.second) != nullptr) {
        throw_err("Variable " + finalToken + " is a variable. INFINITE LOOP DETECTED.");
    }
    return Interpreter::ResolveValue(it->second.second);
}

Value* Interpreter::ExecuteFunction(MethodDefinitionStatement *function) {
    bool didReturn;
    Value* result = ExecuteFunction(function, didReturn);
    return result;
}

Value* Interpreter::ExecuteFunction(MethodDefinitionStatement* function, bool& didReturn) {
    try {
//        if (function->Statements.empty()) {
//            std::cout << "Function has no statements: " << function->Name << std::endl;
//        }
        for (auto& statement : function->Statements) {

            // Function Call
            if (auto call = dynamic_cast<FunctionCallStatement*>(statement)) {
                EvalFunction(call);
            }

            else if (auto throwStatement = dynamic_cast<ThrowStatement*>(statement)) {
                Value* result = ResolveValue(throwStatement->Exception);
                throw RuntimeException(result);
            }

            else if (auto tryCatchStatement = dynamic_cast<TryCatchStatement*>(statement)) {
                NewScope();
                Value* retValue = nullptr;
                MethodDefinitionStatement tryMethod = MethodDefinitionStatement();
                tryMethod.Name = "try";
                tryMethod.Arguments = std::vector<std::string>();
                tryMethod.Statements = tryCatchStatement->Statements;
                tryMethod.ReturnType = "any";
                try {
                    bool returned = false;
                    retValue = ExecuteFunction(&tryMethod, returned);
                    if (returned) {
                        didReturn = true;
                        return retValue;
                    }
                } catch (RuntimeException& e) {
                    NewScope();
                    scopes.top()->SetVariable(tryCatchStatement->ExceptionName, e.Object);

                    MethodDefinitionStatement catchMethod = MethodDefinitionStatement();
                    catchMethod.Name = "catch";
                    catchMethod.Arguments = std::vector<std::string>{tryCatchStatement->ExceptionName};
                    catchMethod.Statements = tryCatchStatement->CatchStatements;
                    catchMethod.ReturnType = "any";
                    bool returned = false;
                    retValue = ExecuteFunction(&catchMethod, returned);
                    EndScope();

                    if (returned) {
                        didReturn = true;
                        return retValue;
                    }
                }
            }

            else if (auto variableInit = dynamic_cast<VariableInitStatement*>(statement)) {
                if (scopes.top()->Variables.count(variableInit->Name)) {
                    throw_err("Variable '" + variableInit->Name + "' is already defined in this scope.");
                }

                std::string expectedType = variableInit->VariableType;
                Value* value = ResolveValue(variableInit->VarValue, expectedType);

                if (value->ObjectType != expectedType) {
                    throw_err("Variable type mismatch. Expected " + expectedType + " but got " + value->ObjectType);
                }

                scopes.top()->SetVariable(variableInit->Name, value);
            }

            else if (auto methodDef = dynamic_cast<MethodDefinitionStatement*>(statement)) {
                scopes.top()->Functions[methodDef->Name] = methodDef;
            }

            else if (auto ret = dynamic_cast<ReturnStatement*>(statement)) {
                if (ret->RetValue->ObjectType != function->ReturnType && function->ReturnType != "any") {
                    throw_err("Return type mismatch. Expected " + function->ReturnType + " but got " + ret->RetValue->ObjectType);
                }

                didReturn = true;
                return ResolveValue(ret->RetValue, function->ReturnType);
            }

            else if (auto varSet = dynamic_cast<VariableSetStatement*>(statement)) {
                Value* value = ResolveValue(varSet->VarValue);

                if (value->ObjectType != varSet->ObjectType) {
                    throw_err("Variable type mismatch. Expected " + varSet->ObjectType + " but got " + value->ObjectType);
                }

                SetVariable(varSet->Path, value);
            }

            else if (auto ifStatement = dynamic_cast<IfStatement*>(statement)) {
                RealReference* conditionRef = ResolveValue(ifStatement->Condition, "bool");
                if (conditionRef->ObjectType != "bool") {
                    throw_err("Condition must be of type bool, but got " + conditionRef->ObjectType);
                }

                auto condition = dynamic_cast<Constant*>(conditionRef);

                if (condition == nullptr) {  // Condition isn't a constant
                    throw_err("Condition must be a constant, but got " + conditionRef->ObjectType);
                }

                if (condition->Value == "true") {
                    NewScope();
                    bool returned = false;

                    MethodDefinitionStatement ifMethod = MethodDefinitionStatement();
                    ifMethod.Name = "if";
                    ifMethod.Arguments = std::vector<std::string>();
                    ifMethod.Statements = ifStatement->Statements;
                    ifMethod.ReturnType = "any";

                    Value* retValue = ExecuteFunction(&ifMethod, returned);
                    EndScope();
                    if (returned) {
                        didReturn = true;
                        return retValue;
                    }
                } else if (condition->Value == "false" && !ifStatement->Statements.empty()) {
                    NewScope();
                    bool returned = false;

                    MethodDefinitionStatement elseMethod = MethodDefinitionStatement();
                    elseMethod.Name = "else";
                    elseMethod.Arguments = std::vector<std::string>();
                    elseMethod.Statements = ifStatement->ElseStatements;
                    elseMethod.ReturnType = "any";

                    Value* retValue = ExecuteFunction(&elseMethod, returned);
                    EndScope();
                    if (returned) {
                        didReturn = true;
                        return retValue;
                    }
                }
                // break
            }

            else if (auto whileStatement = dynamic_cast<WhileStatement*>(statement)) {
                RealReference* conditionRef = ResolveValue(whileStatement->Condition, "bool");
                if (conditionRef->ObjectType != "bool") {
                    throw_err("Condition must be of type bool, but got " + conditionRef->ObjectType);
                }

                auto condition = dynamic_cast<Constant*>(conditionRef);
                if (condition == nullptr) {  // Condition isn't a constant
                    throw_err("Condition must be a constant, but got " + conditionRef->ObjectType);
                }

                MethodDefinitionStatement whileMethod = MethodDefinitionStatement();
                whileMethod.Name = "while";
                whileMethod.Arguments = std::vector<std::string>();
                whileMethod.Statements = whileStatement->Statements;
                whileMethod.ReturnType = "any";

                while (condition->Value == "true") {
                    NewScope();
                    bool returned = false;
                    Value* retValue = ExecuteFunction(&whileMethod, returned);
                    EndScope();
                    if (returned) {
                        didReturn = true;
                        return retValue;
                    }

                    conditionRef = ResolveValue(whileStatement->Condition, "bool");
                    if (conditionRef->ObjectType != "bool") {
                        throw_err("Condition must be of type bool, but got " + conditionRef->ObjectType);
                    }
                    condition = dynamic_cast<Constant*>(conditionRef);
                    if (condition == nullptr) {  // Condition isn't a constant
                        throw_err("Condition must be a constant, but got " + conditionRef->ObjectType);
                    }
                }
            }

            else if (auto _ = dynamic_cast<EnterScopeStatement*>(statement)) {
                NewScope();
            }

            else if (auto _ = dynamic_cast<ExitScopeStatement*>(statement)) {
                EndScope();
            }

            else if (auto loadLib = dynamic_cast<LoadLibStatement*>(statement)) {
                LoadLibrary(loadLib->Lib);
            }

            else {
                throw std::runtime_error("Unknown statement type: " + std::to_string(static_cast<int>(statement->Type)));
            }
        }
    }
    catch (ProgramExitException& e) {
        exit(e.ExitCode);
    }

    didReturn = false;
    return Null;
}

// This is the same as the Execute function from the c# implementation
int Interpreter::Interpret(ParsedScript *script) {
    scopes = std::stack<Scope*>();
    scopes.push(new Scope());

    for (auto &cl : script->Classes) {
        scopes.top()->Classes[cl->Name] = cl;
    }

    MethodDefinitionStatement mainMethod = MethodDefinitionStatement();
    mainMethod.Name = "main";
    mainMethod.Arguments = std::vector<std::string>();
    mainMethod.Statements = script->Statements;
    mainMethod.ReturnType = "int";

    Value* rawReturn;
    try {
        rawReturn = ExecuteFunction(&mainMethod);
    } catch (RuntimeException& e) {
        // Serialise the exception to JSON and print, but not now
        Utils::DumpErrorInfo(e);
        return 1;
    }

    ResolveValue(rawReturn);
    return 0;
}

std::stack<Scope*>* Interpreter::GetScope() {
    return &scopes;
}

void Interpreter::LoadLibrary(const std::string &name) {
    bool fileExists = false;
    for (auto& lib : BuiltIns::Libraries) {
        if (lib.first == name) {
            fileExists = true;
            break;
        }
    }

    // Check file system
    std::ifstream file(name);
    if (file) {
        fileExists = true;
        // Keep the file open because we will need to read from it later
    }

    if (!fileExists) {
        throw_err("Library " + name + " does not exist.");
    }

    bool containsPeriod = name.find('.') != std::string::npos;
    std::string fileExtension = containsPeriod ? name.substr(name.find_last_of('.') + 1) : "";

    std::vector<IWtiiLibrary*> libs;
    if (fileExtension == "wtii") {  // WTII library file
        // TODO: Load the library
        // This will involve reading the file and parsing/interpreting it
        // which is not currently implemented.
    } else if (fileExtension == "dll" || fileExtension == "so") {  // C++ Library
        WtiiLibraryFactory factory;
        factory.LoadLibraries(name);
        IWtiiLibrary* lib = factory.createInstance();
        libs.push_back(lib);
    } else if (fileExtension.empty()) {  // Check for builtin
        if (BuiltIns::Libraries.count(name) == 0) {
            throw_err("Library " + name + " does not exist.");
        }

        IWtiiLibrary* lib = BuiltIns::Libraries.find(name)->second;
        libs.push_back(lib);
    } else if (fileExtension == "json" || fileExtension == "wtiic") {  // Pre parsed library
        ParsedScript lib = Parser::Parse(name);
        for (ClassDefinition* cl : lib.Classes) {
            scopes.top()->Classes[cl->Name] = cl;
        }

        MethodDefinitionStatement mainMethod = MethodDefinitionStatement();
        mainMethod.Name = "main";
        mainMethod.ReturnType = "int";
        mainMethod.Statements = lib.Statements;
        ExecuteFunction(&mainMethod);
    } else {
        throw_err("Library file extension not recognised: " + fileExtension);
    }

    file.close();

    for (auto lib : libs) {
        lib->Init();
        scopes.top()->AppendScope(lib->GetScope());
        lib->Run();
    }

#ifdef DEBUG
    std::cout << "Loaded library: " << name << std::endl;
#endif
}
