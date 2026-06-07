#pragma once
#include <string>
#include <unordered_map>

using namespace std;

class KVStore {
private:
    unordered_map<string, string> store;

public:
    string set(const string& key, const string& value);
    string get(const string& key);
    string del(const string& key);
};