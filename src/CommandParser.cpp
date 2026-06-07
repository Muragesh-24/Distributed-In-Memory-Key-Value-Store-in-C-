#include "CommandParser.h"
#include <sstream>

using namespace std;

CommandParser::CommandParser(KVStore& store) : kv(store) {}

vector<string> CommandParser::split(const string& input) {
    stringstream ss(input);
    string word;
    vector<string> tokens;

    while (ss >> word) {
        tokens.push_back(word);
    }

    return tokens;
}

    string CommandParser::execute(const string& input) {
    vector<string> tokens = split(input);

    if (tokens.empty()) {
        return "";
    }

    string command = tokens[0];

    if (command == "SET") {
        if (tokens.size() < 3) {
            return "ERROR: Usage SET key value";
        }

        return kv.set(tokens[1], tokens[2]);
    }

    if (command == "GET") {
        if (tokens.size() != 2) {
            return "ERROR: Usage GET key";
        }

        return kv.get(tokens[1]);
    }

    if (command == "DELETE") {
        if (tokens.size() != 2) {
            return "ERROR: Usage DELETE key";
        }

        return kv.del(tokens[1]);
    }

    if (command == "EXPIRE") {
        if (tokens.size() != 3) {
            return "ERROR: Usage EXPIRE key seconds";
        }

        int seconds = stoi(tokens[2]);

        return kv.expire(tokens[1], seconds);
    }

    if (command == "TTL") {
        if (tokens.size() != 2) {
            return "ERROR: Usage TTL key";
        }

        return kv.ttl(tokens[1]);
    }

    if (command == "PING") {
        return "PONG";
    }

    return "ERROR: Unknown command";
}