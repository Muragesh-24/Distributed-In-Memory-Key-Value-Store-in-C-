#pragma once
#include <string>
#include <unordered_map>
#include <list>

using namespace std;

class KVStore {
private:
    int capacity;

    list<pair<string, string>> cacheList;

    unordered_map<string, list<pair<string, string>>::iterator> cacheMap;

    void moveToFront(const string& key);

public:
    KVStore(int cap = 3);

    string set(const string& key, const string& value);
    string get(const string& key);
    string del(const string& key);
};