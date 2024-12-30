#include "VPNClient.hpp"
#include "Logger.hpp"
#include "TunnelPacket.hpp"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

VPNClient::VPNClient(const std::string &serverIP, int serverPort)
    : serverIP_(serverIP), serverPort_(serverPort),
      sock_(-1), ssl_(nullptr)
{
    encHandler_.initClientContext();
    sock_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_ < 0) {
        Log("Socket creation failed", LogLevel::ERROR);
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
    if (connect(sock_, (struct sockaddr*)&serverAddr_, sizeof(serverAddr_)) < 0) {
        Log("Connection failed", LogLevel::ERROR);
        return false;
    }

    ssl_ = encHandler_.createSSL(sock_);
    if (!ssl_) {
        Log("Failed to create SSL for client", LogLevel::ERROR);
        return false;
    }

    if (SSL_connect(ssl_) <= 0) {
        Log("SSL connect failed", LogLevel::ERROR);
        ERR_print_errors_fp(stderr);
        return false;
    }
    Log("TLS handshake successful with server", LogLevel::INFO);

    return true;
}

bool VPNClient::sendTunnelPacket(const char* data)
{
    if (!ssl_) {
        Log("SSL not established, cannot send packet", LogLevel::ERROR);
        return false;
    }

    TunnelPacket packet;
    packet.srcPort  = 12345;  // example
    packet.destPort = 80;     // example
    packet.srcIP    = 0x7F000001; // 127.0.0.1
    packet.destIP   = 0x7F000001;
    strncpy(packet.payload, data, sizeof(packet.payload) - 1);

    int sent = SSL_write(ssl_, &packet, sizeof(packet));
    return (sent == sizeof(packet));
}
