#include "Parser.h"
#include "../ParsedScripts/Statements/EnterScopeStatement.h"
#include "../ParsedScripts/Statements/ExitScopeStatement.h"
#include "../ParsedScripts/Statements/FunctionCallStatement.h"
#include "../ParsedScripts/Statements/IfStatement.h"
#include "../ParsedScripts/Statements/VariableSetStatement.h"
#include "../ParsedScripts/Statements/ReturnStatement.h"
#include "../ParsedScripts/Statements/WhileStatement.h"
#include "../ParsedScripts/Statements/ThrowStatement.h"
#include "../ParsedScripts/Statements/TryCatchStatement.h"
#include "../ParsedScripts/Statements/LoadLibStatement.h"
#include "../ParsedScripts/Values/Constant.h"
#include "../ParsedScripts/Values/Variable.h"
#include "../ParsedScripts/Values/MethodCall.h"
#include "../ParsedScripts/Values/ArrayValue.h"
#include "../ParsedScripts/Values/ClassInstance.h"
#include "../ParsedScripts/Values/DirectMethod.h"

ParsedScript Parser::Parse(const std::string &script) {
    // We are going to put the entire script together manually
    nlohmann::json jsonObject = nlohmann::json::parse(script);

    // Statements
    std::vector<Statement*> statements = ParseStatements(jsonObject["Statements"]);

    // Classes
    std::vector<ClassDefinition*> classes = ParseClasses(jsonObject["Classes"]);

    // Return the parsed script
    ParsedScript parsedScript = ParsedScript();
    parsedScript.Statements = statements;
    parsedScript.Classes = classes;
    return parsedScript;
}

std::vector<Statement*> Parser::ParseStatements(const nlohmann::json &jsonObject) {
    std::vector<Statement*> statements;
    for (auto& statement : jsonObject) {
        statements.push_back(ParseStatement(statement));
    }
    return statements;
}

Statement* Parser::ParseStatement(const nlohmann::json &statement) {
    auto type = (StatementType)statement["Type"];

    switch (type) {

        case StatementType::EnterScope:
            return new EnterScopeStatement();

        case StatementType::ExitScope:
            return new ExitScopeStatement();

        case StatementType::FunctionCall: {
            auto* functionCall = new FunctionCallStatement();

            // The path is a string array
            functionCall->Path = std::vector<std::string>();
            for (auto& path : statement["Path"]) {
                functionCall->Path.push_back(path);
            }

            // The arguments are a value array
            functionCall->Arguments = std::vector<Value*>();
            for (auto& argument : statement["Arguments"]) {
                // Parse the value
                auto* value = ParseValue(argument);
                functionCall->Arguments.push_back(value);
            }

            return functionCall;
        }

        case StatementType::If: {
            auto* ifStatement = new IfStatement();

            // Condition is a VarValue
            ifStatement->Condition = ParseValue(statement["Condition"]);

            // Statements is a Statement array
            ifStatement->Statements = ParseStatements(statement["Statements"]);

            // ElseStatements is a Statement array
            ifStatement->ElseStatements = ParseStatements(statement["ElseStatements"]);

            return ifStatement;
        }

        case StatementType::VariableSet: {
            auto* variableSet = new VariableSetStatement();

            // The path is a string array
            variableSet->Path = std::vector<std::string>();
            for (auto& path : statement["Path"]) {
                variableSet->Path.push_back(path);
            }

            // The value is a VarValue
            variableSet->VarValue = ParseValue(statement["Value"]);

            // The object type is a string
            variableSet->ObjectType = statement["ObjectType"];

            return variableSet;
        }

        case StatementType::VariableInit: {
            auto* variableInit = new VariableInitStatement();

            // Name is a string
            variableInit->Name = statement["Name"];

            // VarValue is a VarValue
            variableInit->VarValue = ParseValue(statement["Value"]);

            // ObjectType is a string
            variableInit->VariableType = statement["VariableType"];

            return variableInit;
        }

        case StatementType::MethodDefinition: {
            auto* methodDefinition = new MethodDefinitionStatement();

            // Name is a string
            methodDefinition->Name = statement["Name"];

            // Return type is a string
            methodDefinition->ReturnType = statement["ReturnType"];

            // Arguments is a string array
            methodDefinition->Arguments = std::vector<std::string>();
            for (auto& argument : statement["Arguments"]) {
                methodDefinition->Arguments.push_back(argument);
            }

            // ArgumentTypes is a string array
            methodDefinition->ArgumentTypes = std::vector<std::string>();
            for (auto& argumentType : statement["ArgumentTypes"]) {
                methodDefinition->ArgumentTypes.push_back(argumentType);
            }

            // Statements is a Statement array
            methodDefinition->Statements = ParseStatements(statement["Statements"]);

            // CsFunc is a function which we will ignore since we are not using C# and also
            // it will never be parsed to JSON.

            return methodDefinition;
        }

        case StatementType::Return: {
            auto* returnStatement = new ReturnStatement();

            // VarValue is a VarValue
            returnStatement->RetValue = ParseValue(statement["Value"]);

            return returnStatement;
        }

        case StatementType::While: {
            auto* whileStatement = new WhileStatement();

            // Condition is a VarValue
            whileStatement->Condition = ParseValue(statement["Condition"]);

            // Statements is a Statement array
            whileStatement->Statements = ParseStatements(statement["Statements"]);

            return whileStatement;
        }

        case StatementType::Throw: {
            auto* throwStatement = new ThrowStatement();

            // Exception is a VarValue
            throwStatement->Exception = ParseValue(statement["Exception"]);

            return throwStatement;
        }

        case StatementType::TryCatch: {
            auto* tryCatchStatement = new TryCatchStatement();

            // Statements is a Statement array
            tryCatchStatement->Statements = ParseStatements(statement["Statements"]);

            // CatchStatements is a Statement array
            tryCatchStatement->CatchStatements = ParseStatements(statement["CatchStatements"]);

            // ExceptionName is a string
            tryCatchStatement->ExceptionName = statement["ExceptionName"];

            return tryCatchStatement;
        }

        case StatementType::LoadLib: {
            auto* loadLibStatement = new LoadLibStatement();

            // Lib is a string
            loadLibStatement->Lib = statement["Lib"];

            return loadLibStatement;
        }

        default:
            return nullptr;

    }
}

std::vector<ClassDefinition *> Parser::ParseClasses(const nlohmann::json &jsonObject) {
    std::vector<ClassDefinition*> classes = std::vector<ClassDefinition*>();

    for (auto& classDef : jsonObject) {
        auto* classDefinition = new ClassDefinition();

        // Name is a string
        classDefinition->Name = classDef["Name"];

        // Methods is a MethodDefinitionStatement array, because MethodDefinitionStatement is a Statement
        // we can use ParseStatements to parse the methods and just cast them to MethodDefinitionStatement.
        std::vector<Statement*> tempMethods = ParseStatements(classDef["Methods"]);
        std::vector<MethodDefinitionStatement*> methods;
        for(auto* statement : tempMethods) {
            if(auto* method = dynamic_cast<MethodDefinitionStatement*>(statement)) {
                methods.push_back(method);
            }
        }
        classDefinition->Methods = methods;

        // Variables is a VariableInitStatement array, we can do the same as with methods.
        std::vector<Statement*> tempVariables = ParseStatements(classDef["Variables"]);
        std::vector<VariableInitStatement*> variables;
        for(auto* statement : tempVariables) {
            if(auto* variable = dynamic_cast<VariableInitStatement*>(statement)) {
                variables.push_back(variable);
            }
        }
        classDefinition->Variables = variables;

        classes.push_back(classDefinition);
    }

    return classes;
}

Value* Parser::ParseValue(const nlohmann::json &jsonObject) {
    auto type = (ValueType) jsonObject["Type"];

    // ObjectType is a string
    const auto& objectType = jsonObject["ObjectType"];

    switch (type) {

        case ValueType::Constant: {
            auto* constant = new Constant();

            // VarValue is a string
            constant->Value = jsonObject["Value"];

            constant->ObjectType = objectType;
            return constant;
        }

        case ValueType::Variable: {
            auto* variable = new Variable();

            // Path is a string array
            variable->Path = std::vector<std::string>();
            for (auto& path : jsonObject["Path"]) {
                variable->Path.push_back(path);
            }

            variable->ObjectType = objectType;
            return variable;
        }

        case ValueType::MethodCall: {
            auto* methodCall = new MethodCall();

            // Path is a string array
            methodCall->Path = std::vector<std::string>();
            for (auto& path : jsonObject["Path"]) {
                methodCall->Path.push_back(path);
            }

            // Arguments is a VarValue array
            methodCall->Arguments = std::vector<Value*>();
            for (auto& argument : jsonObject["Arguments"]) {
                // Parse the value
                auto* value = ParseValue(argument);
                methodCall->Arguments.push_back(value);
            }

            methodCall->ObjectType = objectType;
            return methodCall;
        }

        case ValueType::ArrayValue: {
            // Values is a VarValue array
            std::vector<Value*> values = std::vector<Value*>();
            for (auto& value : jsonObject["Values"]) {
                // Parse the value
                Value* parsedValue = ParseValue(value);
                values.push_back(parsedValue);
            }

            std::string baseObjectType = jsonObject["BaseObjectType"];
            return new ArrayValue(baseObjectType, values);
        }

        case ValueType::ClassInstance: {
            auto* classInstance = new ClassInstance();

            // Properties is a json object
            auto propsJson = jsonObject["Properties"];

            // Go through each key in that object, and save the key, Item1 (string) and Item2 (VarValue)
            auto vars = std::map<std::string, std::pair<std::string, Value*>>();
            for (auto& [key, value] : propsJson.items()) {
                std::string name = key;
                auto varType = value["Item1"];
                Value* val = ParseValue(value["Item2"]);
                vars[name] = std::make_pair(varType, val);
            }

            // Methods is a json object
            auto methodsJson = jsonObject["Methods"];

            auto methods = std::map<std::string, MethodDefinitionStatement*>();
            for (auto& [key, value] : methodsJson.items()) {
                std::string name = key;
                auto method = dynamic_cast<MethodDefinitionStatement*>(ParseStatement(value));
                methods[name] = method;
            }

            classInstance->Properties = vars;
            classInstance->Methods = methods;
            classInstance->ObjectType = objectType;
            return classInstance;
        }

        case ValueType::DirectMethod: {
            auto* directMethod = new DirectMethod();

            // Method is a MethodDefinitionStatement
            auto method = dynamic_cast<MethodDefinitionStatement*>(ParseStatement(jsonObject["Method"]));

            // Arguments is a VarValue array
            std::vector<Value*> args = std::vector<Value*>();
            for (auto& argument : jsonObject["Arguments"]) {
                // Parse the value
                auto* value = ParseValue(argument);
                args.push_back(value);
            }

            directMethod->Method = method;
            directMethod->Arguments = args;
            directMethod->ObjectType = objectType;
            return directMethod;
        }

    }

    return nullptr;
}
