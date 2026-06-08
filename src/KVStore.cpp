#include "KVStore.h"

KVStore::KVStore(int cap) {
    capacity = cap;
}

long long KVStore::currentTimestamp() {
    return chrono::duration_cast<chrono::seconds>(
        chrono::system_clock::now().time_since_epoch()
    ).count();
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

    return currentTimestamp() >= expiryMap[key];
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
        if (isExpired(key)) {
            removeKey(key);
        } else {
            auto it = cacheMap[key];
            it->second = value;

            moveToFront(key);

            return "OK";
        }
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

    long long expiryTimestamp = currentTimestamp() + seconds;
    expiryMap[key] = expiryTimestamp;

    return "OK";
}

string KVStore::expireAt(const string& key, long long expiryTimestamp) {
    if (cacheMap.find(key) == cacheMap.end()) {
        return "NOT_FOUND";
    }

    expiryMap[key] = expiryTimestamp;

    if (isExpired(key)) {
        removeKey(key);
        return "NOT_FOUND";
    }

    return "OK";
}

string KVStore::ttl(const string& key) {
    if (cacheMap.find(key) == cacheMap.end()) {
        return "NOT_FOUND";
    }

    if (isExpired(key)) {
        removeKey(key);
        return "NOT_FOUND";
    }

    if (expiryMap.find(key) == expiryMap.end()) {
        return "-1";
    }

    long long remaining = expiryMap[key] - currentTimestamp();

    return to_string(remaining);
}