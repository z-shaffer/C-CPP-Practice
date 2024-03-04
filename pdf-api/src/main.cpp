#include <iostream>
#include "pdftools_api.h"
#include "cli_parser.h"
#include "pdf_processor.h"

/* Main: Entry point of the program. Main runs the input loop, calls the CLI parser to parse input,
passes the parsed input to the API handler to initiate the connection with the PDFTools API server,
and then passes this connection with the server to the PDF processor. */

int main() {
    std::string input;

    // Parse data //
    std::string pdfFileName;
    std::stack<std::string> cmdStack;

    // API response data //
    std::string uploadUrl;
    std::string fileId;

    // Instances //
    std::unique_ptr<PDFToolsAPI> apiHandler = std::make_unique<PDFToolsAPI>();
    std::unique_ptr<CommandLineParser> parser = std::make_unique<CommandLineParser>();
    std::unique_ptr<PDFProcessor> processor = std::make_unique<PDFProcessor>();

    apiHandler->initialize("ZHbjV9T0Wz8v0x3vGuP233Lb0we57N6f55waeySk", "https://api.pdf-tools.com/v1-beta/"); // Key in source code for demonstration purposes

    // Start of input loop
    while (true) {
        std::cout << "** Enter your PDF file name and command(s) or type 'exit' to exit. " << std::endl << "** Example: resume.pdf compress validate " << std::endl;
        std::getline(std::cin, input);
        // Parse input using CLI parser. Update pdfFileName and and cmd
        try {
            parser->parseInput(input, pdfFileName, cmdStack);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid Argument Error: " << e.what() << std::endl;
            continue;
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl; // Exit request recieved
            return 0;
        }

        // Generate a PDF file upload URL using the PDFTools API and upload to it
        try {
            apiHandler->requestUploadUrl(pdfFileName, uploadUrl, fileId);
            if (!uploadUrl.empty()) {
                std::cout << "Success: Upload URL retrieved" << std::endl;
            } else {
                throw std::runtime_error("No upload URL");
            }
            apiHandler->uploadLocalFile(pdfFileName, uploadUrl);
        } catch (const std::runtime_error& e) {
            std::cerr << "API Error: " << e.what() << std::endl;
            continue;
        }

        // Use the previously created connection with the PDFTools API to execute oprations on the PDF file
        try {
            processor->processPDF(fileId, cmdStack);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid Argument Error: " << e.what() << std::endl;
            continue;
        } catch (const std::runtime_error& e) {
            std::cerr << "Runtime Error: " << e.what() << std::endl;
            continue;
        }
    }
    return 0;
}

