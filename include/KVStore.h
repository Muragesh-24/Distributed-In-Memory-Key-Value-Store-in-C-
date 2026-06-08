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

    unordered_map<string, long long> expiryMap;

    void moveToFront(const string& key);
    bool isExpired(const string& key);
    void removeKey(const string& key);
    long long currentTimestamp();

public:
    KVStore(int cap = 3);

    string set(const string& key, const string& value);
    string get(const string& key);
    string del(const string& key);

    string expire(const string& key, int seconds);
    string expireAt(const string& key, long long expiryTimestamp);
    string ttl(const string& key);
};