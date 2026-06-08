#pragma once

#include <string>
#include <vector>

using namespace std;

class WAL {
private:
    string filePath;

public:
    WAL(const string& path = "data/wal.log");

    void append(const string& command);
    vector<string> readAll();
};