#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <algorithm>

#include "KVStore.h"
#include "CommandParser.h"
#include "WAL.h"

using namespace std;

vector<string> splitLine(const string& input) {
    stringstream ss(input);
    string word;
    vector<string> tokens;

    while (ss >> word) {
        tokens.push_back(word);
    }

    return tokens;
}

long long getCurrentTimestamp() {
    return chrono::duration_cast<chrono::seconds>(
        chrono::system_clock::now().time_since_epoch()
    ).count();
}

string toUpperCase(string text) {
    transform(text.begin(), text.end(), text.begin(), ::toupper);
    return text;
}

bool isWriteCommand(const string& line) {
    vector<string> tokens = splitLine(line);

    if (tokens.empty()) {
        return false;
    }

    string command = toUpperCase(tokens[0]);

    return command == "SET" ||
           command == "DELETE" ||
           command == "EXPIRE" ||
           command == "EXPIREAT";
}

string convertToWalCommand(const string& line) {
    vector<string> tokens = splitLine(line);

    if (tokens.empty()) {
        return "";
    }

    string command = toUpperCase(tokens[0]);

    if (command == "EXPIRE") {
        if (tokens.size() != 3) {
            return "";
        }

        long long seconds = stoll(tokens[2]);
        long long expiryTimestamp = getCurrentTimestamp() + seconds;

        return "EXPIREAT " + tokens[1] + " " + to_string(expiryTimestamp);
    }

    if (command == "SET") {
        if (tokens.size() < 3) {
            return "";
        }

        return line;
    }

    if (command == "DELETE") {
        if (tokens.size() != 2) {
            return "";
        }

        return line;
    }

    if (command == "EXPIREAT") {
        if (tokens.size() != 3) {
            return "";
        }

        return line;
    }

    return "";
}

int main() {
    KVStore kv(3);
    CommandParser parser(kv);
    WAL wal("data/wal.log");

    vector<string> oldCommands = wal.readAll();

    for (string command : oldCommands) {
        parser.execute(command);
    }

    string line;

    cout << "C++ KV Store started. Type EXIT to quit.\n";

    while (true) {
        cout << "kv> ";
        getline(cin, line);

        if (toUpperCase(line) == "EXIT") {
            break;
        }

        string response = parser.execute(line);

        if (!response.empty()) {
            cout << response << "\n";
        }

        if (response == "OK" && isWriteCommand(line)) {
            string walCommand = convertToWalCommand(line);

            if (!walCommand.empty()) {
                wal.append(walCommand);
            }
        }
    }

    return 0;
}