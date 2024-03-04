#ifndef PDFTOOLS_API_H
#define PDFTOOLS_API_H

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/* API Handler: This class handles all interactions with preparing the PDFTools' API.
If a valid command was given, and a valid PDF has been provided, the PDF will now be
run through the PDFTools' API according to the given command. An upload URL will be generated,
and a file will be sent to the provided upload URL. */

class PDFToolsAPI {
public:
    static void initialize(const std::string& apiKey, const std::string& apiUrl);
    static void requestUploadUrl(std::string& pdfFileName, std::string& uploadUrl, std::string& fileId);
    static void uploadLocalFile(const std::string& pdfFileName, const std::string& uploadUrl);

    static const std::string& getApiKey() {
        return apiKey;
    }

    static const std::string& getApiUrl() {
        return apiUrl;
    }

private:
    static size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *responseData);
    static std::string apiKey;
    static std::string apiUrl;
};

#endif //PDFTOOLS_API_H
