#include "Compiler.h"

#include <utility>
#include <stack>
#include <cstring>
#include <iostream>
#include "../Utils.h"
#include "../Scope.h"
#include "../ParsedScripts/Statements/FunctionCallStatement.h"
#include "../ParsedScripts/Values/Variable.h"
#include "../ParsedScripts/Statements/VariableSetStatement.h"
#include "../ParsedScripts/Statements/ReturnStatement.h"
#include "../ParsedScripts/Statements/LoadLibStatement.h"
#include "../ParsedScripts/Statements/IfStatement.h"

std::stack<Scope> scopes;
Scope defaultScope = Scope();
std::string currentLine;

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

ParsedScript* Compiler::Parse(std::string code) {
    return Compiler::Parse(Split(std::move(code)));
}

ParsedScript *Compiler::Parse(std::vector<std::string>& lines, bool newScope) {
    if (newScope) NewScope();
    std::vector<Statement*> statements;
    std::vector<ClassDefinition*> classes;

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
                            
                            auto* def = new MethodDefinitionStatement(name, returnType, argPairs);
                            EndScope();
                            
                            statements.push_back(def);
                            scopes.top().Functions[name] = def;
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
                            std::vector<std::string> rawArgs = Utils::SplitString(argsString, ',');
                            
                            // Trim all args
                            for (std::string &arg : rawArgs) {
                                std::string trimmed = Utils::TrimString(arg);
                                if (trimmed != "") args.push_back(trimmed);
                            }
                        }
                        
                        std::vector<std::string> parts;
                        std::string type;
                        Compiler::ParseDotNotationMethod(token, parts, type);
                        
                        std::vector<Value*> argsValues;
                        for (std::string &arg : args) {
                            argsValues.push_back(EvalValue(arg));
                        }
                        auto* call = new FunctionCallStatement(parts, argsValues);
                        
                        statements.push_back(call);
                        handled = true;
                        break;
                    }
                    
                    case '=':
                    // Variable init
                    // Example: int x = 5;
                    {
                        std::vector<std::string> parts = Utils::SplitString(l, '=');
                        std::string declaration = Utils::TrimString(parts[0]);
                        
                        std::vector<std::string> declarationParts = Utils::SplitString(declaration, ' ');
                        if (declarationParts.size() == 2) {  // Otherwise it isn't a declaration
                            std::string type = declarationParts[0];
                            std::string name = declarationParts[1];
                            std::string value = Utils::TrimString(parts[1]);
                            
                            auto* init = new VariableInitStatement(name, EvalValue(value), type);
                            
                            statements.push_back(init);
                            scopes.top().SetVariable(name, init->VarValue);
                            handled = true;
                            break;
                        }
                    }
                    
                    // Variable set
                    {
                        std::vector<std::string> parts = Utils::SplitString(l, '=', 2);
                        std::string name = Utils::TrimString(parts[0]);
                        std::string value = Utils::TrimString(parts[1]);
                        std::string type;
                        Variable* var = ParseDotNotationVariable(name, type);
                        
                        auto* set = new VariableSetStatement(var->Path, EvalValue(value), var->ObjectType);
                        statements.push_back(set);
                        handled = true;
                        break;
                    }
                    
                    
                    // All space based keyword statements
                    case ' ': {
                        if (token == "return") {
                            std::string value = l.substr(7);
                            auto* ret = new ReturnStatement(EvalValue(value));
                            statements.push_back(ret);
                            handled = true;
                            break;
                        }

                        else if (token == "use") {
                            std::string lib = l.substr(4);
                            
                            Value* fileName = EvalValue(lib);
                            if (fileName->ObjectType != "string") {
                                throw_err("Library name must be a string.");
                            }
                            
                            // Omitting the constant lib name requirement for added flexibility
                            
                            // Check if it is constant to see if we can load it now
                            Constant* fileNameConst = dynamic_cast<Constant*>(fileName);
                            if (fileNameConst != nullptr) {
                                // TODO: Load the library
                            }
                            
                            auto* loadLib = new LoadLibStatement(lib);
                            statements.push_back(loadLib);
                            handled = true;
                            break;
                        }
                        
                        else if (token == "if") {
                            std::string condition = l.substr(4, l.size() - 5);
                            Value* conditionValue = EvalValue(condition);
                            if (conditionValue->ObjectType != "bool") {
                                throw_err("If condition must be a boolean.");
                            }
                            
                            // TODO: Finish
                            break;
                        }
                    }
                }
            }
        }
    } catch (RuntimeException& e) {
        std::cerr << "An error has occurred on line: " << currentLine << std::endl;
        throw e;
    }
    
    auto* script = new ParsedScript();
    script->Statements = statements;
    script->Classes = classes;
    return script;
}

MethodDefinitionStatement* Compiler::ParseDotNotationMethod(std::string value, std::vector<std::string> &parts, std::string &type) {
    return nullptr;  // TODO
}

Value *Compiler::EvalValue(const std::string &value) {
    return nullptr;  // TODO
}

Variable *Compiler::ParseDotNotationVariable(std::string value, std::string &type) {
    return nullptr;  // TODO
}
