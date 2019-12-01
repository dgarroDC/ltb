// +---------------------------------+
// |     ltb - LDPL Telegram Bot     |
// +---------------------------------+
// | by Damián Garro                 |
// | https://github.com/dgarroDC/ltb |
// +---------------------------------+

#include <iostream>
#include <vector>
#include "../thirdparty/json.hpp"
#include "ldpl-types.h"

using namespace std;
using json = nlohmann::json;

ldpl_text LTB_JSON_IN_JSON;
ldpl_text LTB_JSON_IN_VALUE;
ldpl_text LTB_JSON_OUT_TEXT;
ldpl_list<ldpl_text> LTB_JSON_OUT_TEXTLIST;

ldpl_number LTB_EC;
ldpl_text LTB_ET;

void SETERRORCODE();

// Check if the request was successful from its response,
// returning its 'result' field if OK, throwing error if not oK
void LTB_JSON_GETRESULT() {
    json j;
    try {
        j = json::parse(LTB_JSON_IN_JSON.str_rep());
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
    json j = json::parse(LTB_JSON_IN_JSON.str_rep());
    for (auto & element : j)
        LTB_JSON_OUT_TEXTLIST.inner_collection.push_back(element.dump());
}

// Get a value from a JSON, "" if not foud
void LTB_JSON_GETVALUE() {
    json j = json::parse(LTB_JSON_IN_JSON.str_rep());
    auto f = j.find(LTB_JSON_IN_VALUE.str_rep());
    if (f != j.end()) {
        LTB_JSON_OUT_TEXT = f->dump();
        if (f->type() == json::value_t::string)
            LTB_JSON_OUT_TEXT = LTB_JSON_OUT_TEXT.substr(1, LTB_JSON_OUT_TEXT.size() - 2);
    } else {
        LTB_JSON_OUT_TEXT = "";
    }
}
