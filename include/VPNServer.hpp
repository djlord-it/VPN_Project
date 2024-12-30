#pragma once

#include <string>
#include <netinet/in.h>
#include <vector>
#include <thread>
#include "EncryptionHandler.hpp"

class VPNServer {
public:
    VPNServer(const std::string &ip, int port,
              const std::string &certFile,
              const std::string &keyFile);
    ~VPNServer();

    bool start();  // Start listening for clients

private:
    void handleClient(int clientSock);

    int serverSock_;
    bool running_;
    sockaddr_in serverAddr_;

    EncryptionHandler encHandler_;

    std::vector<std::thread> clientThreads_;
};
