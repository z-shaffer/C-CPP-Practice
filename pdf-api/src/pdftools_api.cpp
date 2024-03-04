#include "pdftools_api.h"

std::string PDFToolsAPI::apiKey;
std::string PDFToolsAPI::apiUrl;

// Called on program startup, to store the api key and api url
void PDFToolsAPI::initialize(const std::string& apiKeyValue, const std::string& apiUrlValue) {
    apiKey = apiKeyValue;
    apiUrl = apiUrlValue;
}

// Method to generate an AWS URL to send the PDF file to and store it on the API server
void PDFToolsAPI::requestUploadUrl(std::string& pdfFileName, std::string& uploadUrl, std::string& fileId) {
    CURL *curl;
    CURLcode res;

    const std::string apiUrl = getApiUrl() + "files/upload/fromLocal";
    const std::string apiKey = getApiKey();
    const std::string payload = "{\"fileName\":\"" + pdfFileName + "\"}";

    // CURL setup: Most of this could be abstracted into a method
    curl = curl_easy_init();
    if (curl) {
        // Configure headers
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, ("X-Api-Key: " + apiKey).c_str());

        // Build request
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());

        // Set write callback function
        std::string responseData;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << curl_easy_strerror(res) << std::endl;
            throw std::runtime_error("Failed to upload file");
        }

        // Parse JSON response
        json jsonResponse;
        try {
            jsonResponse = json::parse(responseData);
        } catch(const json::parse_error& e) {
            throw std::runtime_error("Failed to parse JSON response: " + std::string(e.what()));
        }

        // Extract uploadUrl and fileId
        try {
            uploadUrl = jsonResponse["uploadUrl"];
            if (uploadUrl.empty()) {
                throw std::runtime_error("Upload URL came back empty");
            }
            fileId = jsonResponse["fileId"];
            if (fileId.empty()) {
                throw std::runtime_error("File ID came back empty");
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception caught: " << e.what() << std::endl;
        }

        // Completed
        curl_easy_cleanup(curl);

    } else {
        throw std::runtime_error("CURL config issue");
    }
}

// After a connection to the API server has been created, the URL will be forwarded to this method to send the PDF on
void PDFToolsAPI::uploadLocalFile(const std::string& pdfFileName, const std::string& uploadUrl) {
    FILE *fd;
    CURL *curl;
    CURLcode res;
    long file_size;

    // Open the pdf file
    fd = fopen(pdfFileName.c_str(), "rb");
    file_size = ftell(fd);

    // CURL setup: Most of this could be abstracted into a method
    curl = curl_easy_init();
    if (curl) {
        // Configure headers
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/octet-stream");

        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, uploadUrl.c_str());

        // Set the HTTP PUT method
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the data to be uploaded
        curl_easy_setopt(curl, CURLOPT_READDATA, fd);
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_size);

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << curl_easy_strerror(res) << std::endl;
            throw std::runtime_error("Failed to upload file");
        } else {
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            if (http_code == 200) {
                std::cout << "Success: File uploaded" << std::endl;
            } else {
                std::cerr << http_code << std::endl;
                throw std::runtime_error("Failed to upload file");
            }
        }

        // Everything succeeded, clean up
        curl_easy_cleanup(curl);
    }
}

// Method for supressing the API response and storing it for parsing
size_t PDFToolsAPI::writeCallback(void *contents, size_t size, size_t nmemb, std::string *responseData) {
    size_t totalSize = size * nmemb;
    responseData->append((char *)contents, totalSize);
    return totalSize;
}
