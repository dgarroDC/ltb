#include <iostream>
#include <curl/curl.h>

using namespace std;

size_t ptb_curl_write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    string& response = *(string *)userdata;
    size_t read_size = size * nmemb;
    response.append(ptr, read_size);
    return read_size;
}

string LTB_CURL_URL;
string LTB_CURL_RESPONSE;
void LTB_CURL_REQUEST(){
    CURL* curl = curl_easy_init();
    if (!curl) {
        cerr << "LTB curl error: could not start a libcurl easy session" << endl;
    }
    curl_easy_setopt(curl, CURLOPT_URL, LTB_CURL_URL.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &LTB_CURL_RESPONSE);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ptb_curl_write_callback);
    LTB_CURL_RESPONSE = "";
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        cerr << "LTB curl error: " << curl_easy_strerror(res) << endl;
        cerr << "during request " << LTB_CURL_URL << endl;
        exit(1);
    }
    curl_easy_cleanup(curl);
}
