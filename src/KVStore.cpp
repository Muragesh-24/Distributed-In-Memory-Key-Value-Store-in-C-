#include "KVStore.h"

KVStore::KVStore(int cap) {
    capacity = cap;
}

void KVStore::moveToFront(const string& key) {
    auto it = cacheMap[key];

    pair<string, string> keyValue = *it;

    cacheList.erase(it);

    cacheList.push_front(keyValue);

    cacheMap[key] = cacheList.begin();
}

bool KVStore::isExpired(const string& key) {
    if (expiryMap.find(key) == expiryMap.end()) {
        return false;
    }

    auto now = chrono::steady_clock::now();

    return now >= expiryMap[key];
}

void KVStore::removeKey(const string& key) {
    if (cacheMap.find(key) == cacheMap.end()) {
        return;
    }

    auto it = cacheMap[key];

    cacheList.erase(it);
    cacheMap.erase(key);
    expiryMap.erase(key);
}

string KVStore::set(const string& key, const string& value) {
    if (cacheMap.find(key) != cacheMap.end()) {
        auto it = cacheMap[key];
        it->second = value;

        moveToFront(key);

        return "OK";
    }

    if ((int)cacheList.size() == capacity) {
        auto last = cacheList.back();
        string leastUsedKey = last.first;

        cacheList.pop_back();
        cacheMap.erase(leastUsedKey);
        expiryMap.erase(leastUsedKey);
    }

    cacheList.push_front({key, value});
    cacheMap[key] = cacheList.begin();

    return "OK";
}

string KVStore::get(const string& key) {
    if (cacheMap.find(key) == cacheMap.end()) {
        return "NOT_FOUND";
    }

    if (isExpired(key)) {
        removeKey(key);
        return "NOT_FOUND";
    }

    moveToFront(key);

    return cacheMap[key]->second;
}

string KVStore::del(const string& key) {
    if (cacheMap.find(key) == cacheMap.end()) {
        return "NOT_FOUND";
    }

    removeKey(key);

    return "OK";
}

string KVStore::expire(const string& key, int seconds) {
    if (cacheMap.find(key) == cacheMap.end()) {
        return "NOT_FOUND";
    }

    if (isExpired(key)) {
        removeKey(key);
        return "NOT_FOUND";
    }

    auto expiryTime = chrono::steady_clock::now() + chrono::seconds(seconds);

    expiryMap[key] = expiryTime;

    return "OK";
}

string KVStore::ttl(const string& key) {
    if (cacheMap.find(key) == cacheMap.end()) {
        return "NOT_FOUND";
    }

    if (expiryMap.find(key) == expiryMap.end()) {
        return "-1";
    }

    if (isExpired(key)) {
        removeKey(key);
        return "NOT_FOUND";
    }

    auto now = chrono::steady_clock::now();
    auto remaining = chrono::duration_cast<chrono::seconds>(expiryMap[key] - now).count();

    return to_string(remaining);
}