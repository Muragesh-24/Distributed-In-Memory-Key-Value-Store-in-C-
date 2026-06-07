#pragma once

#include <string>
#include <vector>
#include "KVStore.h"

using namespace std;

class CommandParser {
private:
    KVStore& kv;

    vector<string> split(const string& input);

public:
    CommandParser(KVStore& store);

    string execute(const string& input);
};