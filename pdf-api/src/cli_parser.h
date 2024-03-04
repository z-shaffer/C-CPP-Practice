#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#include <string>
#include <algorithm>
#include <filesystem>
#include <sstream>
#include <stack>

namespace fs = std::filesystem;

/* CLI Parser: Parses the CLI input to format it, build a stack of commands,
and ensure a valid input was given, or if 'exit' was entered, the program will terminate. */

class CommandLineParser {
public:
    static void parseInput(std::string& input, std::string& pdfFileName, std::stack<std::string>& cmdStack);

private:
    static bool fileExists(const std::string* fileNamePtr);
    static void buildCmdStack(std::string cmds, std::stack<std::string>& cmdStack);
};

#endif // CLI_PARSER_H
