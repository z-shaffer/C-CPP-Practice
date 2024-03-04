#include "cli_parser.h"

void CommandLineParser::parseInput(std::string& input, std::string& pdfFileName, std::stack<std::string>& cmdStack) {

    // Normalize input to lowercase
    transform(input.begin(), input.end(), input.begin(), ::tolower);

    // Find where to split the input
    size_t spacePos = input.find(' ');

    // Check for an exit request
    if (input == "exit") {
        throw std::runtime_error("Exiting");
    }
    // Check if the space was not found
    else if (spacePos == std::string::npos) {
        throw std::invalid_argument("Invalid input");
    }
    else {
        // Split the input
        pdfFileName = input.substr(0, spacePos);
        buildCmdStack(input.substr(spacePos + 1), cmdStack);
        std::string ext = pdfFileName.substr(pdfFileName.length() - 4);
        transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (!fileExists(&pdfFileName) || ext != ".pdf") {
            throw std::invalid_argument("PDF file not found");
        }
    }
}

bool CommandLineParser::fileExists(const std::string* fileNamePtr) {
    if (fileNamePtr == nullptr)
        return false;
    return fs::exists(*fileNamePtr);
}

void CommandLineParser::buildCmdStack(std::string cmds, std::stack<std::string>& cmdStack) {
    std::istringstream iss(cmds);
    std::string word;
    while (iss >> word) {
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        cmdStack.push(word);
    }
}
