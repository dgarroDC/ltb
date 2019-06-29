#include <iostream>
#include "../thirdparty/json.hpp"

using namespace std;
using json = nlohmann::json;

string LTB_JSON_INPUT;
string LTB_JSON_OUTPUT;

// Check if the request was successful from its response,
// returning its 'result' field if OK, throwing error if not oK
void LTB_JSON_GETRESULT() {
    json j = json::parse(LTB_JSON_INPUT);
    bool ok = j["ok"];
    if (!ok) {
        cerr << "LTB TG error #" << j["error_code"] << ": " << j["description"] << endl;
        exit(1);
    }
    LTB_JSON_OUTPUT = j["result"].dump();
}
