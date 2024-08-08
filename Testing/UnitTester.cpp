#include <functional>
#include <iostream>
#include "UnitTester.h"
#include "Tests/UtilsTest.h"
#include "TestFailedException.h"

void RunTest(int& passed, int& failed, const std::function<void()>& method) {
    try {
        method();
        passed++;
    } catch (TestFailedException& e) {
        std::cout << "Test failed: " << e.reason << "." << std::endl;
        failed++;
    }
}

int UnitTester::ConductTests() {
    int passed = 0;
    int failed = 0;

    std::cout << "Running unit tests..." << std::endl;

    RunTest(passed, failed, Trim);

    std::cout << "Test complete!" << std::endl
        << "Tests Success: " << passed << "." << std::endl
        << "Tests Failed: " << failed << "." << std::endl
        << "Tests Run: " << passed + failed << "." << std::endl;

    return failed;
}
