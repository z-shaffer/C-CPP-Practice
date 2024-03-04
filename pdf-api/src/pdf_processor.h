#ifndef PDF_PROCESSOR_H
#define PDF_PROCESSOR_H

#include <stack>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>
#include <chrono>
#include "pdftools_api.h"

using json = nlohmann::json;

/* PDF Processor: After a PDF file has been stored in the API server,
the file ID is tretrieved and passed to the PDF processor with the stack of commands given by the user.
This class will handle interactions with the API to process the PDF according to the commands given by the user.
As well, this class will update the fileId with the fileId of processed files, so commands can be chained together by the user. */

class PDFProcessor {
public:
    static void processPDF(std::string& fileId, std::stack<std::string>& cmdStack);
private:
    static bool getResult(std::string& fileId, const std::string& operationId);
    static void pdfCompressOptimize(std::string& fileId);
    static void pdfValidate(std::string& fileId);
    static size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *responseData);
};

#endif //PDF_PROCESSOR_H
