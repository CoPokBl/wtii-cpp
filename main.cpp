#include <iostream>
#include <fstream>
#include "Parser/Parser.h"
#include "Interpreter/Interpreter.h"

int main(int argc, char** argv) {
    std::string filename = "script_parsed.json";
    if (argc > 1) {
        filename = argv[1];
    }

#ifdef DEBUG
    std::cout << "Using file: " << filename << std::endl;
#endif

    std::ifstream file("script_parsed.json");

    if (!file) {
        std::cerr << "Unable to open file";
        return 1; // return with error code
    }

    std::string fileStr;
    std::string line;
    while (std::getline(file, line)) {
        fileStr += line;
    }
    file.close();

    ParsedScript script = Parser::Parse(fileStr);

#ifdef DEBUG
    std::cout << "Script parsed successfully" << std::endl;
#endif

    int result = Interpreter::Interpret(&script);
    return result;
}
