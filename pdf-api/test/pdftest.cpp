#include <iostream>
#include <stdexcept>
#include <string>
#include <stack>
#include "cli_parser.h"

CommandLineParser parser;
std::string fileName;
std::stack<std::string> stack;
std::string input;
std::string output;

void test_invalid_input1() {
    input = "test";
    output = "Invalid input";
    try {
        parser.parseInput(input, fileName, stack);
        std::cerr << "Test failed: Expected invalid_argument exception not thrown\n";
    } catch (const std::invalid_argument& e) {
        if (e.what() != output) {
            std::cerr << "Test failed: Unexpected error message: " << e.what() << "\n";
        } else {
            std::cout << "Test 1 passed" << std::endl;
        }
    } catch (...) {
        std::cerr << "Test failed: Unexpected exception thrown\n";
    }
}

void test_invalid_input2() {
    input = "test test";
    output = "PDF file not found";
    try {
        parser.parseInput(input, fileName, stack);
        std::cerr << "Test failed: Expected invalid_argument exception not thrown\n";
    } catch (const std::invalid_argument& e) {
        if (e.what() != output) {
            std::cerr << "Test failed: Unexpected error message: " << e.what() << "\n";
        } else {
            std::cout << "Test 2 passed" << std::endl;
        }
    } catch (...) {
        std::cerr << "Test failed: Unexpected exception thrown\n";
    }
}

void test_invalid_input3() {
    input = "test.pdf compress validate";
    output = "PDF file not found";
    try {
        parser.parseInput(input, fileName, stack);
        std::cerr << "Test failed: Expected invalid_argument exception not thrown\n";
    } catch (const std::invalid_argument& e) {
        if (e.what() != output) {
            std::cerr << "Test failed: Unexpected error message: " << e.what() << "\n";
        } else {
            std::cout << "Test 3 passed" << std::endl;
        }
    } catch (...) {
        std::cerr << "Test failed: Unexpected exception thrown\n";
    }
}

int main() {
    test_invalid_input1();
    test_invalid_input2();
    test_invalid_input3();
    return 0;
}