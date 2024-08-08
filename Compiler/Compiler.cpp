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
                            std::vector<std::string> parts =
                        }
                    }
                }
            }
        }
    }
}
