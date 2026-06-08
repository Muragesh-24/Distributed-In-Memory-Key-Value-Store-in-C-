#include "WAL.h"
#include <fstream>
#include <iostream>

using namespace std;

WAL::WAL(const string& path) {
    filePath = path;
}

void WAL::append(const string& command) {
    ofstream file(filePath, ios::app);

    if (!file.is_open()) {
        cerr << "ERROR: Could not open WAL file\n";
        return;
    }

    file << command << "\n";
    file.close();
}

vector<string> WAL::readAll() {
    vector<string> commands;
    ifstream file(filePath);

    if (!file.is_open()) {
        return commands;
    }

    string line;

    while (getline(file, line)) {
        if (!line.empty()) {
            commands.push_back(line);
        }
    }

    file.close();
    return commands;
}