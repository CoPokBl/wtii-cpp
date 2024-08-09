#include "Compiler.h"

#include <utility>
#include <stack>
#include "../Utils.h"
#include "../Scope.h"
#include "../ParsedScripts/Values/ClassInstance.h"
#include "../CompilerException.h"
#include "../ParsedScripts/Values/MethodCall.h"
#include "../ParsedScripts/Statements/FunctionCallStatement.h"
#include "../ParsedScripts/Values/Constant.h"

#define fail(val) throw CompilerException(val)
#define null Constant("NULL", "NULL")            // null constant value
#define nullp new Constant("NULL", "NULL")       // null constant value as pointer

// Scope data
std::stack<Scope> scopes;
Scope defaultScope = Scope();

// Used to debug errors
std::string currentLine;

// Constants
const static std::string ReservedCharacters = "+=-/* (){};[]!\"";
const static std::string PrimitiveTypes[] = {
        "int", "float", "string", "bool", "NULL"
};

bool IsReservedCharacter(const char& c) {
    return std::any_of(ReservedCharacters.begin(), ReservedCharacters.end(), [&c](const char& cc) {
        return cc == c;
    });
}

void NewScope() {
    scopes.emplace(scopes.empty() ? defaultScope : scopes.top());
}

void EndScope() {
    scopes.pop();
}

std::vector<std::string>& Split(std::string code) {
    std::vector<std::string> lines;
    std::string currentLine;
    bool escape = false;
    bool inString = false;

    for (int i = 0; i < code.size(); ++i) {
        char c = code[i];
        if (c == '"') {
            inString = !inString;
        }
        bool longEnough = i + 1 < code.size();
        if (!inString && c == '/' && longEnough && code[i + 1] == '/') {
            // Comment
            while (code[i] != '\n') {
                i++;
            }
            lines.push_back(currentLine);
            continue;
        }

        if (!inString && c == '/' && longEnough && code[i + 1] == '*') {
            // Multiline comment
            i += 2;
            while (code[i] != '*' && code[i + 1] != '/') {
                i++;
            }
            i += 2;
            continue;
        }

        if ((c == '}' || c == '{') && !escape) {
            if (!Utils::TrimString(currentLine).empty()) lines.push_back(currentLine);
            currentLine.clear();
            currentLine += c;
            lines.push_back(currentLine);
            currentLine.clear();
        } else if (c == '\n' && !escape) {
            if (!Utils::TrimString(currentLine).empty()) lines.push_back(currentLine);
            currentLine.clear();
        } else if (c == ';' && !escape) {
            if (!Utils::TrimString(currentLine).empty()) lines.push_back(currentLine);
            currentLine.clear();
        } else if (c == '\\' && !escape) {
            escape = true;
        } else {
            currentLine += c;
            escape = false;
        }
    }

    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }
    lines.emplace_back("");
    return lines;
}

static std::vector<std::string> Split(const std::vector<std::string>& lines, int startIndex, int& finishPoint) {
    int closingBracket = -1;
    int bracketCount = 0;
    for (int i = 0; i < lines.size(); ++i) {
        std::string line2 = lines[i];
        for (int j = 0; j < line2.size(); ++j) {
            char c2 = line2[j];
            if (c2 == '{') {
                bracketCount++;
            } else if (c2 == '}') {
                bracketCount--;
                if (bracketCount == 0) {
                    closingBracket = i;
                    break;
                }
            }
        }
        
        if (closingBracket != -1) {
            break;
        }
    }
    
    std::vector<std::string> bodyLines = Utils::SubVector(lines, startIndex + 2, closingBracket);
    finishPoint = closingBracket;
    return bodyLines;
}

static ClassInstance* GetFinalDotNotationValue(const std::string& value, std::vector<std::string>& parts, std::string& lastSymbol) {
    parts = Utils::SplitString(value, '.');
    std::string varName = parts[parts.size()-2];  // ^1

    if (parts.size() == 1) {
        lastSymbol = value;
        return nullptr;
    }

    // For each part except the last part
    std::pair<std::string, Value*>* lastVar = nullptr;
    for (int i = 0; i < parts.size(); ++i) {
        std::string part = parts[i];
        std::string requiredNextVariableName = parts[i + 1];

        auto* classInstance = lastVar == nullptr ? nullptr : dynamic_cast<ClassInstance*>(lastVar->second);
        if (lastVar != nullptr && classInstance == nullptr) {
            fail("lastVar is not a ClassInstance");
        }

        std::map<std::string, std::pair<std::string, Value*>> vars = lastVar == nullptr ? scopes.top().Variables : classInstance->Properties;

        if (!std::any_of(vars.begin(), vars.end(), [&part](
                const std::pair<const std::basic_string<char>, std::pair<std::string, Value*>>& c) {
            return c.first == part;
        })) {  // Check to ensure that the function is in the current scope
            fail("Unknown variable: " + part + ".");
        }

        std::pair<std::string, Value*> var = vars[part];

        // Realloc space for var to stop it going out of scope and free existing memory from
        // var if it already exists. Create a new pointer for it before copying value.
        delete lastVar;
        lastVar = new std::pair<std::string, Value *>;
        *lastVar = var;

        // Check class
        if (!std::any_of(scopes.top().Classes.begin(), scopes.top().Classes.end(), [&var](
                const std::pair<const std::basic_string<char>, ClassDefinition*>& c) {
            return c.first == var.first;
        })) {
            fail("Unknown class: " + part + ".");
        }
        ClassDefinition* classDef = scopes.top().Classes[var.first];

        // Check variable
        if (std::all_of(classDef->Variables.begin(), classDef->Variables.end(), [&requiredNextVariableName](
                VariableInitStatement* v) {
            return v->Name != requiredNextVariableName;
        }) &&
        std::all_of(classDef->Methods.begin(), classDef->Methods.end(), [&requiredNextVariableName](
                MethodDefinitionStatement* v) {
            return v->Name != requiredNextVariableName;
        })) {
            fail("Unknown variable: " + part + ".");
        }
    }

    lastSymbol = parts[parts.size() - 2];

    auto* mc = lastVar == nullptr ? nullptr : dynamic_cast<MethodCall*>(lastVar->second);
    if (mc != nullptr) {
        std::string classType = lastVar->first;

        if (!std::any_of(scopes.top().Classes.begin(), scopes.top().Classes.end(), [&classType](
                const std::pair<const std::basic_string<char>, ClassDefinition*>& c) {
            return c.first == classType;
        })) {
            fail("Unknown class: " + classType + ".");
        }
        ClassDefinition* classDef = scopes.top().Classes[classType];
        return new ClassInstance(classDef);
    }
    delete mc;  // Free it since we never even use it

    auto* cI = lastVar == nullptr ? nullptr : dynamic_cast<ClassInstance*>(lastVar->second);
    if (cI == nullptr) {
        fail("lastVar is null or not a class");
    }

    return cI;
}

static MethodDefinitionStatement* ParseDotNotationMethod(const std::string& val, std::vector<std::string>& parts, std::string& type) {
    std::string finalPart;
    ClassInstance* finalInstance = GetFinalDotNotationValue(val, parts, finalPart);

    if (finalInstance == nullptr) {  // It's from the current scope
        if (!std::any_of(scopes.top().Functions.begin(), scopes.top().Functions.end(), [&val](
                const std::pair<const std::basic_string<char>, MethodDefinitionStatement*>& c) {
            return c.first == val;
        })) {  // Check to ensure that the function is in the current scope
            fail("Unknown function: " + val + ".");
        }

        type = scopes.top().Functions[val]->ReturnType;
        return scopes.top().Functions[val];
    }

    if (!std::any_of(finalInstance->Methods.begin(), finalInstance->Methods.end(), [&finalPart](
            const std::pair<const std::basic_string<char>, MethodDefinitionStatement*>& c) {
        return c.first == finalPart;
    })) {
        fail("Unknown method: " + finalPart + ".");
    }

    MethodDefinitionStatement* finalMethod = finalInstance->Methods[finalPart];

    type = finalMethod->ReturnType;
    return finalMethod;
}

Value* Compiler::EvalValue(std::string value, std::string& type) {
    value = Utils::TrimString(value);

    if (value == "NULL") {
        type = "NULL";
        return nullp;
    }

    if (Utils::ContainsChar(value, '(') && Utils::ContainsChar(value, ')')) {
        // To get the method name we go back from ( until we hit a reserved character
        std::string methodName = "";
        for (int i = value.find('(') - 1; i >= 0; --i) {
            char c = value[i];
            if (IsReservedCharacter(c)) {
                break;
            }

            // This is `c + methodName`
            methodName.insert(methodName.begin(), c);
        }

        std::vector<std::string> parts;
        ParseDotNotationMethod(methodName, parts, type);

        // Split into before bracket and after bracket and ignore the last character of the second part
        std::string argsString = Utils::SplitString(value, '(')[1].substr(0, value.size() - 2);
        std::vector<std::string> args = Utils::SplitString(argsString, ',');
        std::vector<Value*> valueArgs;   // Init empty and populate if argsString != ""

        if (!argsString.empty()) {
            valueArgs = Compiler::EvalValues(args);
        }

        return new MethodCall(parts, valueArgs);
    }

    // Check for operators by going through each character checking if it's an operator and ignoring if it's part of a string
    bool inString = false;
    for (int i = 0; i < value.size(); ++i) {
        char c = value[i];
        if (c == '"') {
            inString = !inString;
        }

        if (inString) {
            continue;
        }

        char nc = value.size()-1 <= i ? ' ' : value[i + 1];
        std::string op;  // Empty means no operator

        // Checks for operators, with c (current character) and nc (next character)
        if (c == '!' && nc == '=') op = "!=";
        else if (c == '=' && nc == '=') op = "==";
        else if (c == '<' && nc == '=') op = "<=";
        else if (c == '>' && nc == '=') op = ">=";
        else if (c == '|' && nc == '|') op = "||";
        else if (c == '&' && nc == '&') op = "&&";
        else if (c == '<') op = "<";
        else if (c == '>') op = ">";

        // Logical boolean operators
        if (!inString && !op.empty()) {
            std::vector<std::string> parts = Utils::SplitString(value, op, )
        }
    }
}

std::vector<Value*> Compiler::EvalValues(const std::vector<std::string>& args) {
    std::vector<Value*> out;
    for (const std::string& arg : args) {
        std::string type;
        out.push_back(Compiler::EvalValue(arg, type));
    }
    return out;
}

ParsedScript* Compiler::Parse(std::string code) {
    return Compiler::Parse(Split(std::move(code)));
}

ParsedScript *Compiler::Parse(std::vector<std::string>& lines, bool newScope) {
    if (newScope) NewScope();
    std::vector<Statement> statements;
    std::vector<ClassDefinition> classes;

    try {
        for (int statement = 0; statement < lines.size(); ++statement) {
            std::string l = Utils::TrimString(lines[statement]);
            currentLine = l;

            if (l == "") {
                continue;
            }

            std::string token;
            bool handled = false;
            for (char c : l) {
                switch (c) {
                    case '(':
                    // Function definition
                    {
                        if (Utils::ContainsChar(token, ' ')) {  // It is a definition
                            std::vector<std::string> parts = Utils::SplitString(token, ' ');
                            std::string returnType = parts[0];
                            std::string name = parts[1];
                            std::string argsString = l.substr(token.size() + 1, l.size() - token.size() - 2);
                            std::vector<std::string> rawArgs = Utils::SplitString(argsString, ',');
                            
                            // Remove empty args
                            std::vector<std::string> args;
                            for (std::string &arg : rawArgs) {
                                std::string trimmed = Utils::TrimString(arg);
                                if (trimmed != "") args.push_back(trimmed);
                            }
                            rawArgs.clear();

                            // Split args into arg types
                            std::map<std::string, std::string> argTypes;
                            for (std::string arg : args) {
                                std::vector<std::string> argParts = Utils::SplitString(arg, ' ');
                                argTypes[argParts[0]] = argParts[1];
                            }
                            
                            // Get body
                            int closingBracket = -1;
                            std::vector<std::string> body = Split(lines, statement, closingBracket);
                            
                            NewScope();
                            
                            // Add args to scope
                            for (std::pair<std::string, std::string> thing : argTypes) {
                                scopes.top().SetVariable(thing.first, new Constant("NULL", thing.second));
                            }
                            
                            // Convert the map to a vector of pairs of <string, string> for name and types
                            std::vector<std::pair<std::string, std::string>> argPairs;
                            for (std::pair<std::string, std::string> pair : argTypes) {
                                argPairs.push_back(pair);
                            }
                            
                            MethodDefinitionStatement def = MethodDefinitionStatement(name, returnType, argPairs);
                            EndScope();
                            
                            statements.push_back(def);
                            scopes.top().Functions[name] = &def;
                            handled = true;
                            
                            // Move statement index to closing bracket
                            statement = closingBracket + 1;
                            break;
                        }
                    }
                    
                    // Function call
                    {
                        std::string argsString = l.substr(token.size(), l.size() - token.size());
                        std::vector<std::string> args;

                        if (argsString != "()") {
                            argsString = argsString.substr(1, argsString.size() - 2);
                            std::vector<std::string> rawArgs = Utils::SafeSplit(argsString.substr(1, argsString.size()-1), ',');
                            
                            // Remove empty args
                            for (std::string &arg : rawArgs) {
                                std::string trimmed = Utils::TrimString(arg);
                                if (trimmed != "") args.push_back(trimmed);
                            }
                        }  // Otherwise leave the vector empty

                        std::vector<std::string> parts;
                        std::string type;  // Unused
                        ParseDotNotationMethod(token, parts, type);


                        auto call = FunctionCallStatement(parts, );
                    }
                }
            }
        }
    }
}


// Walter was here.