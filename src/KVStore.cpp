#include "KVStore.h"

string KVStore::set(const string& key, const string& value) {
    store[key] = value;
    return "OK";
}

string KVStore::get(const string& key) {
    if (store.find(key) == store.end()) {
        return "NOT_FOUND";
    }

    return store[key];
}

string KVStore::del(const string& key) {
    if (store.erase(key)) {
        return "OK";
    }

    return "NOT_FOUND";
}