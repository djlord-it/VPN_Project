#include "VPNServer.hpp"
#include "Logger.hpp"
#include "TunnelPacket.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

VPNServer::VPNServer(const std::string &ip, int port,
                     const std::string &certFile,
                     const std::string &keyFile)
    : serverSock_(-1), running_(false)
{
    if (!encHandler_.initServerContext(certFile, keyFile)) {
        Log("Failed to initialize server SSL context", LogLevel::ERROR);
    }

    serverSock_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock_ < 0) {
        Log("Socket creation failed", LogLevel::ERROR);
        return;
    }

    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr_.sin_addr);
}

VPNServer::~VPNServer()
{
    running_ = false;
    if (serverSock_ >= 0) {
        close(serverSock_);
    }
    // Join all client threads if needed
    for (auto &t : clientThreads_) {
        if (t.joinable()) {
            t.join();
        }
    }
}

bool VPNServer::start()
{
    if (bind(serverSock_, (struct sockaddr*)&serverAddr_, sizeof(serverAddr_)) < 0) {
        Log("Bind failed", LogLevel::ERROR);
        return false;
    }
    if (listen(serverSock_, 10) < 0) {
        Log("Listen failed", LogLevel::ERROR);
        return false;
    }
    running_ = true;

    Log("Server listening on port...", LogLevel::INFO);

    while (running_) {
        int clientSock = accept(serverSock_, nullptr, nullptr);
        if (clientSock < 0) {
            Log("Accept failed", LogLevel::WARN);
            continue;
        }
        Log("Client connected, spawning thread...", LogLevel::INFO);
        // Launch a thread to handle the client
        clientThreads_.emplace_back(&VPNServer::handleClient, this, clientSock);
    }

    return true;
}

void VPNServer::handleClient(int clientSock)
{
    SSL* ssl = encHandler_.createSSL(clientSock);
    if (!ssl) {
        Log("Failed to create SSL object for client", LogLevel::ERROR);
        close(clientSock);
        return;
    }

    if (SSL_accept(ssl) <= 0) {
        Log("SSL accept failed for client", LogLevel::ERROR);
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(clientSock);
        return;
    }
    Log("TLS handshake successful with client", LogLevel::INFO);

    // Example: read a single TunnelPacket
    TunnelPacket packet;
    int received = SSL_read(ssl, &packet, sizeof(packet));
    if (received > 0) {
        Log("Received packet from client", LogLevel::INFO);
        Log("Payload: " + std::string(packet.payload), LogLevel::DEBUG);
        // In real code, forward traffic or handle it...
    } else {
        Log("Failed to read packet from client", LogLevel::ERROR);
    }

    // Clean up
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(clientSock);
    Log("Client disconnected", LogLevel::INFO);
}
