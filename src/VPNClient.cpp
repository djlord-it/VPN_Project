#include "VPNClient.hpp"
#include "TunnelPacket.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

VPNClient::VPNClient(const std::string &serverIP, int serverPort)
    : serverIP_(serverIP), serverPort_(serverPort), sock_(-1), ssl_(nullptr)
{
    encHandler_.initClientContext();

    // Create socket
    sock_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_ < 0) {
        perror("[Client] Socket creation failed");
    }

    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_port = htons(serverPort_);
    inet_pton(AF_INET, serverIP_.c_str(), &serverAddr_.sin_addr);
}

VPNClient::~VPNClient()
{
    if (ssl_) {
        SSL_shutdown(ssl_);
        SSL_free(ssl_);
    }
    if (sock_ >= 0) {
        close(sock_);
    }
}

bool VPNClient::connectToServer()
{
    if (connect(sock_, (struct sockaddr *)&serverAddr_, sizeof(serverAddr_)) < 0) {
        perror("[Client] Connection failed");
        return false;
    }

    ssl_ = encHandler_.createSSL(sock_);
    if (!ssl_) {
        std::cerr << "[Client] Failed to create SSL.\n";
        return false;
    }

    if (SSL_connect(ssl_) <= 0) {
        std::cerr << "[Client] SSL connect failed.\n";
        return false;
    }

    std::cout << "[Client] Connected and TLS handshake successful.\n";
    return true;
}

bool VPNClient::sendTunnelPacket(const char* data)
{
    if (!ssl_) {
        std::cerr << "[Client] SSL is not established.\n";
        return false;
    }

    TunnelPacket packet{};
    packet.srcPort  = 55555;  // Example
    packet.destPort = 80;     // Example
    packet.srcIP    = 0x7F000001; // 127.0.0.1 in hex
    packet.destIP   = 0x7F000001; // 127.0.0.1 in hex
    // Copy data into payload
    strncpy(packet.payload, data, TunnelPacket::MAX_PAYLOAD_SIZE - 1);

    int sent = SSL_write(ssl_, &packet, sizeof(packet));
    return (sent == sizeof(packet));
}
