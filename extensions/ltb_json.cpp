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

ldpl_number LTB_EC;
string LTB_ET;

void SETERRORCODE();

// Check if the request was successful from its response,
// returning its 'result' field if OK, throwing error if not oK
void LTB_JSON_GETRESULT() {
    json j;
    try {
        j = json::parse(LTB_JSON_IN_JSON);
    } catch (json::parse_error& e) {
        LTB_ET = "LTB JSON parse error #" + to_string(e.id) + ": " + string(e.what())
               + "\ninput JSON:\n" + LTB_JSON_IN_JSON;
        LTB_EC = 1;
        cerr << LTB_ET << endl;
        SETERRORCODE();
        return;
    }
    bool ok = j["ok"];
    if (!ok) {
        LTB_ET = "LTB TG error #" + j["error_code"].dump() + ": " + j["description"].dump();
        LTB_EC = 1;
        cerr << LTB_ET << endl;
        SETERRORCODE();
        return;
    }
    LTB_EC = 0;
    LTB_ET = "";
    SETERRORCODE();
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
