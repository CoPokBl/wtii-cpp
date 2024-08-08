#include "Compiler.h"
#include "../Utils.h"

std::vector<std::string>* Split(std::string code) {
    std::vector<std::string> lines;
    std::string currentLine = "";
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
    return &lines;
}

ParsedScript* Compiler::Parse(std::string code) {
    return nullptr;
}
