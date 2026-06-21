#include <iostream>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include "KVStore.h"
#include "CommandParser.h"
#include "WAL.h"

using namespace std;
vector<string> splitCommand(string input) {

    stringstream ss(input);
    string word;

    vector<string> tokens;

    while(ss >> word) {
        tokens.push_back(word);
    }

    return tokens;
}


string upper(string s) {

    transform(
        s.begin(),
        s.end(),
        s.begin(),
        ::toupper
    );

    return s;
}


long long currentTime() {

    return chrono::duration_cast<
        chrono::seconds
    >(
        chrono::system_clock::now().time_since_epoch()
    ).count();
}
int main() {

    KVStore kv(3);
    CommandParser parser(kv);
    WAL wal("data/wal.log");

    vector<string> commands = wal.readAll();

    for (const string& cmd : commands) {
        parser.execute(cmd);
    }

    cout << "Loaded "
         << commands.size()
         << " WAL commands\n";

    int serverFd = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );

    if (serverFd < 0) {
        cerr << "Socket creation failed\n";
        return 1;
    }

    sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (
        bind(
            serverFd,
            (sockaddr*)&address,
            sizeof(address)
        ) < 0
    ) {
        cerr << "Bind failed\n";
        return 1;
    }

    if (listen(serverFd, 5) < 0) {
        cerr << "Listen failed\n";
        return 1;
    }

    cout << "KV Server listening on port 8080\n";

    sockaddr_in clientAddress;
    socklen_t clientLen = sizeof(clientAddress);

    int clientFd = accept(
        serverFd,
        (sockaddr*)&clientAddress,
        &clientLen
    );

    if (clientFd < 0) {
        cerr << "Accept failed\n";
        return 1;
    }

    cout << "Client connected\n";

    char buffer[1024];

    while (true) {

        int bytesReceived =
            recv(
                clientFd,
                buffer,
                sizeof(buffer) - 1,
                0
            );

        if (bytesReceived <= 0) {
            break;
        }

        buffer[bytesReceived] = '\0';

        string command(buffer);

      string response = parser.execute(command);
vector<string> tokens = splitCommand(command);


if(!tokens.empty()) {

    string cmd = upper(tokens[0]);


    if(response == "OK") {


        if(cmd == "SET") {

            wal.append(command);

        }


        else if(cmd == "DELETE") {

            wal.append(command);

        }


        else if(cmd == "EXPIREAT") {

            wal.append(command);

        }


        else if(cmd == "EXPIRE") {

            if(tokens.size() == 3) {

                long long expiry =
                    currentTime()
                    + stoll(tokens[2]);


                string converted =
                    "EXPIREAT "
                    + tokens[1]
                    + " "
                    + to_string(expiry);


                wal.append(converted);
            }
        }
    }
}
send(
    clientFd,
    response.c_str(),
    response.size(),
    0
);
    }

    close(clientFd);
    close(serverFd);

    return 0;
}