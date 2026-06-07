#pragma once

#include <string>
#include <unordered_map>
#include <list>
#include <chrono>

using namespace std;

class KVStore {
private:
    int capacity;

    list<pair<string, string>> cacheList;
    unordered_map<string, list<pair<string, string>>::iterator> cacheMap;

    unordered_map<string, chrono::steady_clock::time_point> expiryMap;

    void moveToFront(const string& key);
    bool isExpired(const string& key);
    void removeKey(const string& key);

public:
    KVStore(int cap = 3);

    string set(const string& key, const string& value);
    string get(const string& key);
    string del(const string& key);

    string expire(const string& key, int seconds);
    string ttl(const string& key);
};