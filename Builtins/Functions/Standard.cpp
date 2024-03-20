#include <iostream>
#include "Standard.h"
#include "../../ProgramExitException.h"
#include "../../Interpreter/Interpreter.h"
#include "../../ParsedScripts/Values/ClassInstance.h"
#include "../../Utils.h"
#include "../../ParsedScripts/Values/ArrayValue.h"

Value* Standard::Exit(std::vector<Value*> args) {
    auto constant = dynamic_cast<Constant*>(args[0]);

    if (constant == nullptr) {
        throw_err("Not constant val");
    }

    if (constant->ObjectType != "int") {
        throw_err("Not int val");
    }

    int val = std::stoi(constant->Value);
    throw ProgramExitException(val);
}

Value* Standard::Str(std::vector<Value*> args) {
    RealReference* reference = Interpreter::ResolveValue(args[0]);
    if (dynamic_cast<Constant*>(reference) != nullptr) {
        return new Constant(dynamic_cast<Constant*>(reference)->Value, "string");
    }
    else if (dynamic_cast<ClassInstance*>(reference) != nullptr) {
        return new Constant("Not implemented", "string");//Json::ToJson(args);
    }
    else {
        throw_err("str() does not support this type.");
    }
}

Value* Standard::Add(std::vector<Value*> args) {
    RealReference* ar = Interpreter::ResolveValue(args[0]);
    RealReference* br = Interpreter::ResolveValue(args[1]);
    if (dynamic_cast<Constant*>(ar) == nullptr || dynamic_cast<Constant*>(br) == nullptr) {
        throw_err("add() arguments must be constants");
    }
    float a = std::stof(dynamic_cast<Constant*>(ar)->Value);
    float b = std::stof(dynamic_cast<Constant*>(br)->Value);
    return new Constant(std::to_string(a + b), "float");
}

Value* Standard::Subtract(std::vector<Value*> args) {
    RealReference* ar = Interpreter::ResolveValue(args[0]);
    RealReference* br = Interpreter::ResolveValue(args[1]);
    if (dynamic_cast<Constant*>(ar) == nullptr || dynamic_cast<Constant*>(br) == nullptr) {
        throw_err("subtract() arguments must be constants");
    }
    float a = std::stof(dynamic_cast<Constant*>(ar)->Value);
    float b = std::stof(dynamic_cast<Constant*>(br)->Value);
    return new Constant(std::to_string(a - b), "float");
}

Value* Standard::Multiply(std::vector<Value*> args) {
    RealReference* ar = Interpreter::ResolveValue(args[0]);
    RealReference* br = Interpreter::ResolveValue(args[1]);
    if (dynamic_cast<Constant*>(ar) == nullptr || dynamic_cast<Constant*>(br) == nullptr) {
        throw_err("multiply() arguments must be constants");
    }
    float a = std::stof(dynamic_cast<Constant*>(ar)->Value);
    float b = std::stof(dynamic_cast<Constant*>(br)->Value);
    return new Constant(std::to_string(a * b), "float");
}

Value* Standard::Divide(std::vector<Value*> args) {
    RealReference* ar = Interpreter::ResolveValue(args[0]);
    RealReference* br = Interpreter::ResolveValue(args[1]);
    if (dynamic_cast<Constant*>(ar) == nullptr || dynamic_cast<Constant*>(br) == nullptr) {
        throw_err("divide() arguments must be constants");
    }
    float a = std::stof(dynamic_cast<Constant*>(ar)->Value);
    float b = std::stof(dynamic_cast<Constant*>(br)->Value);
    return new Constant(std::to_string(a / b), "float");
}

Value* Standard::Concat(std::vector<Value*> args) {
    RealReference* ar = Interpreter::ResolveValue(args[0]);
    RealReference* br = Interpreter::ResolveValue(args[1]);
    if (dynamic_cast<Constant*>(ar) == nullptr || dynamic_cast<Constant*>(br) == nullptr) {
        throw_err("concat() arguments must be constants");
    }
    std::string a = dynamic_cast<Constant*>(ar)->Value;
    std::string b = dynamic_cast<Constant*>(br)->Value;
    return new Constant(a + b, "string");
}

Value *Standard::Equals(std::vector<Value *> args) {
    RealReference* ar = Interpreter::ResolveValue(args[0]);
    RealReference* br = Interpreter::ResolveValue(args[1]);
    std::string a = ar->GetHash();
    std::string b = br->GetHash();
    return new Constant(a == b ? "true" : "false", "bool");
}

Value *Standard::Not(std::vector<Value *> args) {
    RealReference* ar = Interpreter::ResolveValue(args[0]);
    auto constant = dynamic_cast<Constant*>(ar);
    if (constant == nullptr) {
        throw_err("Not constant val");
    }
    if (constant->ObjectType != "bool") {
        throw_err("Not bool val");
    }
    return new Constant(constant->Value == "true" ? "false" : "true", "bool");
}

Value *Standard::GetArrayObject(std::vector<Value *> args) {
    RealReference* ar = Interpreter::ResolveValue(args[0]);
    auto arrVal = dynamic_cast<ArrayValue*>(ar);
    if (arrVal == nullptr) {
        throw_err("Not array val");
    }

    int index = std::stoi(dynamic_cast<Constant*>(args[1])->Value);
    return arrVal->Values[index];
}

Value *Standard::TypeOf(std::vector<Value *> args) {
    RealReference* ar = Interpreter::ResolveValue(args[0]);
    return new Constant(ar->ObjectType, "string");
}

Value *Standard::Assert(std::vector<Value *> args) {
    RealReference* ar = Interpreter::ResolveValue(args[0]);
    auto constant = dynamic_cast<Constant*>(ar);
    if (constant == nullptr) {
        throw_err("Not constant val");
    }
    if (constant->ObjectType != "bool") {
        throw_err("Not bool val");
    }
    if (constant->Value != "true") {
        auto msg = dynamic_cast<Constant*>(args[1])->Value;
        throw_err("Assertion failed: " + msg);
    }
    return new Constant("true", "bool");
}

Value *Standard::Split(std::vector<Value *> args) {
    RealReference* ar = Interpreter::ResolveValue(args[0]);
    auto constant = dynamic_cast<Constant*>(ar);
    if (constant == nullptr) {
        throw_err("Not constant val");
    }
    if (constant->ObjectType != "string") {
        throw_err("Not string val");
    }
    auto delimiter = dynamic_cast<Constant*>(args[1])->Value;
    std::string str = constant->Value;
    std::vector<Value*> result;
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        result.push_back(new Constant(token, "string"));
        str.erase(0, pos + delimiter.length());
    }
    result.push_back(new Constant(str, "string"));
    return new ArrayValue("string", result);
}

Value *Standard::LoadLib(std::vector<Value *> args) {
    throw_err("LoadLib is not implemented");
}

Value *Standard::GetVar(std::vector<Value *> args) {
    RealReference* ar = Interpreter::ResolveValue(args[0]);
    auto constant = dynamic_cast<Constant*>(ar);
    if (constant == nullptr) {
        throw_err("Not constant val");
    }
    if (constant->ObjectType != "string") {
        throw_err("Not string val");
    }
    std::string varName = constant->Value;
    if (!Interpreter::GetScope()->top()->Variables.count(varName)) {
        throw_err("Variable not found: " + varName);
    }

    return Interpreter::GetScope()->top()->Variables[varName].second;
}

Value *Standard::GetRuntime(const std::vector<Value *>& args) {
    return new Constant("WtiiInterpreter-cpp", "string");
}

