// +---------------------------------+
// |     ltb - LDPL Telegram Bot     |
// +---------------------------------+
// | by Damián Garro                 |
// | https://github.com/dgarroDC/ltb |
// +---------------------------------+

#include <iostream>
#include <vector>
#include "../thirdparty/json.hpp"

using namespace std;
using json = nlohmann::json;

#ifndef ldpl_vector
template<typename T>
struct ldpl_list {
    vector<T> inner_collection;
};
#endif
#define ldpl_number double

string LTB_JSON_IN_JSON;
string LTB_JSON_IN_VALUE;
string LTB_JSON_OUT_TEXT;
ldpl_list<string> LTB_JSON_OUT_TEXTLIST;
void SETERRORCODE();
ldpl_number LTB_EC;
string LTB_ET;


// Check if the request was successful from its response,
// returning its 'result' field if OK, throwing error if not oK
void LTB_JSON_GETRESULT() {
    json j = json::parse(LTB_JSON_IN_JSON);
    bool ok = j["ok"];
    if (!ok) {
        cerr << "LTB TG error #" << j["error_code"] << ": " << j["description"] << endl;
        LTB_EC = 1;
        LTB_ET = j["description"];
        SETERRORCODE();
    }else{
        LTB_EC = 0;
        LTB_ET = "";
        SETERRORCODE();
    }
    LTB_JSON_OUT_TEXT = j["result"].dump();
}

// Parse a JSON array as a TEXT LIST
void LTB_JSON_GETTEXTLIST() {
    LTB_JSON_OUT_TEXTLIST.inner_collection.clear();
    json j = json::parse(LTB_JSON_IN_JSON);
    for (auto & element : j)
        LTB_JSON_OUT_TEXTLIST.inner_collection.push_back(element.dump());
}

// Get a value from a JSON, "" if not foud
void LTB_JSON_GETVALUE() {
    json j = json::parse(LTB_JSON_IN_JSON);
    auto f = j.find(LTB_JSON_IN_VALUE);
    if (f != j.end()) {
        LTB_JSON_OUT_TEXT = f->dump();
        if (f->type() == json::value_t::string)
            LTB_JSON_OUT_TEXT = LTB_JSON_OUT_TEXT.substr(1, LTB_JSON_OUT_TEXT.size() - 2);
    } else {
        LTB_JSON_OUT_TEXT = "";
    }
}
