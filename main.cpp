#include <iostream>
#include "KVStore.h"
#include "CommandParser.h"

using namespace std;

int main() {
  KVStore kv(3);
    CommandParser parser(kv);

    string line;

    cout << "C++ KV Store started. Type EXIT to quit.\n";

    while (true) {
        cout << "kv> ";
        getline(cin, line);

        if (line == "EXIT") {
            break;
        }

        string response = parser.execute(line);

        if (!response.empty()) {
            cout << response << "\n";
        }
    }

    return 0;
}