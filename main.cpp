#include "Parser/Parser.h"
#include "library.h"
#include "Testing/UnitTester.h"

int main(int argc, char** argv) {
    if (argc > 1 && strcmp(argv[1], "--test") == 0) {
        return UnitTester::ConductTests();
    }

    std::string filename = "script_parsed.json";
    if (argc > 1) {
        filename = argv[1];
    }

    return Library::InterpretFile(filename);
}
