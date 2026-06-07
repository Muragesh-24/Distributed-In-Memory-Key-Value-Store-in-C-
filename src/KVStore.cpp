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
    }

    cacheList.push_front({key, value});
    cacheMap[key] = cacheList.begin();

    return "OK";
}

string KVStore::get(const string& key) {
    if (cacheMap.find(key) == cacheMap.end()) {
        return "NOT_FOUND";
    }

    moveToFront(key);

    return cacheMap[key]->second;
}

string KVStore::del(const string& key) {
    if (cacheMap.find(key) == cacheMap.end()) {
        return "NOT_FOUND";
    }

    auto it = cacheMap[key];

    cacheList.erase(it);
    cacheMap.erase(key);

    return "OK";
}