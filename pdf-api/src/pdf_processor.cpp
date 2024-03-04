#include "pdf_processor.h"

void PDFProcessor::processPDF(std::string& fileId, std::stack<std::string>& cmdStack) {
    while (!cmdStack.empty()) {
        std::string cmd = cmdStack.top();
        cmdStack.pop();
        if (cmd == "compress") {
            pdfCompressOptimize(fileId);
        } else if (cmd == "validate") {
            pdfValidate(fileId);
        } else {
            // Clear out the command stack before throwing an error
            while (!cmdStack.empty()) {
                cmdStack.pop();
            }
            throw std::invalid_argument("Invalid argument " + cmd);
        }
    }
}

// Send an operation
void PDFProcessor::pdfCompressOptimize(std::string& fileId) {
    const std::string apiUrl = PDFToolsAPI::getApiUrl() + "operations/optimize";
    const std::string apiKey = PDFToolsAPI::getApiKey();

    CURL* curl = curl_easy_init();
    if (curl) {
        // Prepare JSON data
        std::string jsonString = "{\"options\":{\"profile\":\"archive\"},\"input\":{\"fileId\":\"" + fileId + "\"}}";

        // Configure headers
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Accept: application/json");
        std::string apiKeyHeader = "X-Api-Key: " + apiKey;
        headers = curl_slist_append(headers, apiKeyHeader.c_str());

        // Build http request
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());

        // Set write callback function
        std::string responseData;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Curl error: " << curl_easy_strerror(res) << std::endl;
            throw std::runtime_error("Curl failure");
        } else {
            // Success
            std::cout << "Success: Compressing... " << std::endl;
        }

        // Operation request is being processed by the server. Store the provided operation ID to later retrieve the output
        json jsonResponse = json::parse(responseData);
        std::string operationId = jsonResponse["operationId"];
        if (operationId.empty()) {
            throw std::runtime_error("Operation ID came back empty");
        }

        // Call the server once every second, until it shows that the operation is complete
        while (!getResult(fileId, operationId)) {
            std::this_thread::sleep_for (std::chrono::seconds(1));
        }

        // Everything succeeded, clean up
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    } else {
        std::cerr << "Failed to initialize curl" << std::endl;
        throw std::runtime_error("Curl failure");
    }
}

// Send an operation request to the API server to validate the PDF file
void PDFProcessor::pdfValidate(std::string& fileId) {
    const std::string apiUrl = PDFToolsAPI::getApiUrl() + "operations/validate";
    const std::string apiKey = PDFToolsAPI::getApiKey();

    json jsonData = {
        {"options", {{"conformance", "pdf13"}}},
        {"input", {{"fileId", fileId}}}
    };
    std::string jsonString = jsonData.dump();

    // CURL setup: Most of this could be abstracted into a method
    CURL* curl = curl_easy_init();
    if (curl) {
        // Configure headers
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Accept: application/json");
        std::string apiKeyHeader = "X-Api-Key: " + apiKey;
        headers = curl_slist_append(headers, apiKeyHeader.c_str());

        // Build http request
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());

        // Set write callback function
        std::string responseData;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Curl error: " << curl_easy_strerror(res) << std::endl;
            throw std::runtime_error("Curl failure");
        } else {
            std::cout << "Success: Validating... " << std::endl;
        }

        // Operation request is being processed by the server. Store the provided operation ID to later retrieve the output
        json jsonResponse = json::parse(responseData);
        std::string operationId = jsonResponse["operationId"];
        if (operationId.empty()) {
            throw std::runtime_error("Operation ID came back empty");
        }

        // Call the server once every second, until it shows that the operation is complete
        while (!getResult(fileId, operationId)) {
            std::this_thread::sleep_for (std::chrono::seconds(1));
        }

        // Everything succeeded, clean up
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    } else {
        std::cerr << "Failed to initialize curl" << std::endl;
        throw std::runtime_error("Curl failure");
    }
}

// Method called on loop when an operation request has been sent to the API server, until the server is ready
bool PDFProcessor::getResult(std::string& fileId, const std::string& operationId) {
    // API endpoint and credentials
    const std::string apiUrl = PDFToolsAPI::getApiUrl() + "operations/" + operationId + "/result";
    const std::string apiKey = PDFToolsAPI::getApiKey();

    // Initialize cURL session
    CURL* curl = curl_easy_init();
    if (curl) {
        // Configure headers
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Accept: application/json");
        std::string apiKeyHeader = "X-Api-Key: " + apiKey;
        headers = curl_slist_append(headers, apiKeyHeader.c_str());

        // Build request
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());

        // Execute the GET request
        std::string responseData;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Curl error: " << curl_easy_strerror(res) << std::endl;
            throw std::runtime_error("Curl failure");
        } else {
            try {
                std::cout << responseData;
                json jsonResponse = json::parse(responseData);
                // If the API server is still processing the PDF, try again
                if (jsonResponse["operationStatus"] == "inProgress") {
                    return false;
                }
                // If the server is done, grab the fileID of the completed PDF
                fileId = jsonResponse["fileId"];
                if (fileId.empty()) {
                    throw std::runtime_error("File ID came back empty");
                } else {
                    // Everything succeeded, clean up and indicate this in the return
                    curl_easy_cleanup(curl);
                    return true;
                }
            } catch (const std::exception& e) {
                std::cerr << "Exception caught: " << e.what() << std::endl;
                throw std::runtime_error("Failed to parse JSON");
            }
        }
    } else {
        std::cerr << "Failed to initialize curl" << std::endl;
        throw std::runtime_error("Curl failure");
    }
    return false;
}

// Method for supressing the API response and storing it for parsing
size_t PDFProcessor::writeCallback(void *contents, size_t size, size_t nmemb, std::string *responseData) {
    size_t totalSize = size * nmemb;
    responseData->append((char *)contents, totalSize);
    return totalSize;
}