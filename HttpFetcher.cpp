#include "HttpFetcher.hpp"
#include <curl/curl.h>
#include <string>

// Callback function for curl to write data into a std::string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string fetchTLE(const std::string& url) {
    CURL* curlHandle = curl_easy_init();
    std::string response;

    if (curlHandle) {
        curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curlHandle, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curlHandle);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curlHandle);
    }

    return response;
}