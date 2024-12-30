#pragma once

#include <string>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include "EncryptionHandler.hpp"

class VPNClient {
public:
    VPNClient(const std::string &serverIP, int serverPort);
    ~VPNClient();

    bool connectToServer();
    bool sendTunnelPacket(const char* data);

private:
    std::string serverIP_;
    int serverPort_;
    int sock_;
    sockaddr_in serverAddr_;
    EncryptionHandler encHandler_;
    SSL* ssl_;
};
