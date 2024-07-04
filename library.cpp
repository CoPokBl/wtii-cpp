#include <iostream>
#include <fstream>
#include "library.h"
#include "ParsedScripts/ParsedScript.h"
#include "Parser/Parser.h"
#include "Interpreter/Interpreter.h"
//#define DEBUG

extern "C" {

    int interpret(const char* filename) {
#ifdef DEBUG
        std::cout << "Using file: " << filename << std::endl;
#endif

        std::ifstream file;
        try {
            file = std::ifstream(filename);

            if (!file) {
                std::cerr << "Unable to open file: " << filename << std::endl;
                return 1; // return with error code
            }
        } catch (const std::exception& e) {
            std::cerr << "Error with opening file (" << filename << "): " << e.what() << std::endl;
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

}

int Library::InterpretFile(const std::string& filename) {
    return interpret(filename.c_str());
}
