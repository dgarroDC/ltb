// +---------------------------------+
// |     ltb - LDPL Telegram Bot     |
// +---------------------------------+
// | by Damián Garro                 |
// | https://github.com/dgarroDC/ltb |
// +---------------------------------+

#include <iostream>
#include <vector>
#include <curl/curl.h>

using namespace std;

#ifndef ldpl_vector
template<typename T>
struct ldpl_list {
    vector<T> inner_collection;
};
#endif
#define ldpl_number double

string LTB_CURL_IN_URL;
ldpl_list<string> LTB_CURL_IN_ARGNAMES;
ldpl_list<string> LTB_CURL_IN_ARGVALUES;
string LTB_CURL_OUT_RESPONSE;

extern ldpl_number LTB_EC; // defined in ltb_json.cpp
extern string LTB_ET; // defined in ltb_json.cpp

void SETERRORCODE();

size_t ltb_curl_write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    string& response = *(string *)userdata;
    size_t read_size = size * nmemb;
    response.append(ptr, read_size);
    return read_size;
}

void LTB_CURL_REQUEST(){
    CURL* curl = curl_easy_init();
    if (!curl) {
        LTB_ET = "LTB curl error: could not start a libcurl easy session";
        LTB_EC = 1;
        cerr << LTB_ET << endl;
        SETERRORCODE();
        return;
    }
    // URL query string
    string url = LTB_CURL_IN_URL;
    string name, value;
    for (int i = 0; i < LTB_CURL_IN_ARGNAMES.inner_collection.size(); i++) {
        name = LTB_CURL_IN_ARGNAMES.inner_collection[i];
        value = LTB_CURL_IN_ARGVALUES.inner_collection[i];
        url += (i == 0 ? "?" : "&")
             + string(curl_easy_escape(curl, name.c_str(), name.size())) + "="
             + string(curl_easy_escape(curl, value.c_str(), value.size()));
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &LTB_CURL_OUT_RESPONSE);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ltb_curl_write_callback);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60L);
    LTB_CURL_OUT_RESPONSE = "";
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        LTB_ET = "LTB curl error: " + string(curl_easy_strerror(res))
               + "\nduring request " + LTB_CURL_IN_URL;
        LTB_EC = 1;
        cerr << LTB_ET << endl;
        SETERRORCODE();
        return;
    }
    curl_easy_cleanup(curl);
    LTB_EC = 0;
    LTB_ET = "";
    SETERRORCODE();
}
