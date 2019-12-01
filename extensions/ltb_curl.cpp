// +---------------------------------+
// |     ltb - LDPL Telegram Bot     |
// +---------------------------------+
// | by Damián Garro                 |
// | https://github.com/dgarroDC/ltb |
// +---------------------------------+

#include <iostream>
#include <vector>
#include <curl/curl.h>
#include "ldpl-types.h"

using namespace std;


ldpl_text LTB_CURL_IN_URL;
ldpl_list<ldpl_text> LTB_CURL_IN_ARGNAMES;
ldpl_list<ldpl_text> LTB_CURL_IN_ARGVALUES;
ldpl_text LTB_CURL_OUT_RESPONSE;

extern ldpl_number LTB_EC; // defined in ltb_json.cpp
extern ldpl_text LTB_ET; // defined in ltb_json.cpp

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
    string url = LTB_CURL_IN_URL.str_rep();
    string name, value;
    char * e_name, * e_value;
    for (int i = 0; i < LTB_CURL_IN_ARGNAMES.inner_collection.size(); i++) {
        name = LTB_CURL_IN_ARGNAMES.inner_collection[i].str_rep();
        value = LTB_CURL_IN_ARGVALUES.inner_collection[i].str_rep();
        e_name = curl_easy_escape(curl, name.c_str(), name.size());
        e_value = curl_easy_escape(curl, value.c_str(), value.size());
        url += (i == 0 ? "?" : "&") + string(e_name) + "=" + string(e_value);
        curl_free(e_name);
        curl_free(e_value);
    }
    string out_response = "";
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out_response);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ltb_curl_write_callback);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60L);
    CURLcode res = curl_easy_perform(curl);
    LTB_CURL_OUT_RESPONSE = out_response;
    curl_easy_cleanup(curl);
    if (res != CURLE_OK) {
        LTB_ET = "LTB curl error: " + string(curl_easy_strerror(res))
               + "\nduring request " + LTB_CURL_IN_URL;
        LTB_EC = 1;
        cerr << LTB_ET << endl;
        SETERRORCODE();
        return;
    }
    LTB_EC = 0;
    LTB_ET = "";
    SETERRORCODE();
}
