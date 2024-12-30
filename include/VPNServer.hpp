#pragma once

#include <string>
#include <netinet/in.h>
#include "EncryptionHandler.hpp"

class VPNServer {
public:
    VPNServer(int port, const std::string &certFile, const std::string &keyFile);
    ~VPNServer();
    bool start();

private:
    void handleClient(int clientSock);

    int serverSock_;
    int port_;
    sockaddr_in serverAddr_;
    EncryptionHandler encHandler_;
};
