#include "Compiler.h"

#include <utility>
#include <stack>
#include "../Utils.h"
#include "../Scope.h"

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
                            std::vector<std::string> rawArgs = argsString.substr() Utils::SplitString(argsString, ',');
                            
                            // Remove empty args
                            for (std::string &arg : rawArgs) {
                                std::string trimmed = Utils::TrimString(arg);
                                if (trimmed != "") args.push_back(trimmed);
                            }
                        }
                        
                        
                    }
                }
            }
        }
    }
}
