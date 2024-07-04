#include "Parser/Parser.h"
#include "library.h"

int main(int argc, char** argv) {
    std::string filename = "script_parsed.json";
    if (argc > 1) {
        filename = argv[1];
    }

    return Library::InterpretFile(filename);
}
