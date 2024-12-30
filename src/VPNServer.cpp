#include "VPNServer.hpp"
#include "TunnelPacket.hpp"
#include <iostream>
#include <unistd.h>   // close()

VPNServer::VPNServer(int port, const std::string &certFile, const std::string &keyFile)
    : serverSock_(-1), port_(port)
{
    // Initialize server
    if (!encHandler_.initServerContext(certFile, keyFile)) {
        std::cerr << "Failed to initialize server SSL context\n";
    }

    // Create server socket
    serverSock_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock_ < 0) {
        perror("Socket creation failed");
        return;
    }

    // Setup address
    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_addr.s_addr = INADDR_ANY;
    serverAddr_.sin_port = htons(port_);
}

VPNServer::~VPNServer()
{
    if (serverSock_ >= 0) {
        close(serverSock_);
    }
}

bool VPNServer::start()
{
    // Bind the socket
    if (bind(serverSock_, (struct sockaddr *)&serverAddr_, sizeof(serverAddr_)) < 0) {
        perror("Bind failed");
        return false;
    }

    // Start listening
    if (listen(serverSock_, 5) < 0) {
        perror("Listen failed");
        return false;
    }

    std::cout << "[Server] Listening on port " << port_ << "...\n";

    while (true) {
        int clientSock = accept(serverSock_, nullptr, nullptr);
        if (clientSock < 0) {
            perror("Accept failed");
            continue;
        }
        std::cout << "[Server] Client connected!\n";

        // For a real server, you'd likely create a thread here:
        // std::thread(&VPNServer::handleClient, this, clientSock).detach();
        // But for simplicity, just handle in-line:
        handleClient(clientSock);
    }
    return true;
}

void VPNServer::handleClient(int clientSock)
{
    SSL* ssl = encHandler_.createSSL(clientSock);
    if (!ssl) {
        std::cerr << "[Server] Failed to create SSL.\n";
        close(clientSock);
        return;
    }

    if (SSL_accept(ssl) <= 0) {
        std::cerr << "[Server] SSL accept failed.\n";
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(clientSock);
        return;
    }
    std::cout << "[Server] TLS handshake successful.\n";

    // Read a TunnelPacket
    TunnelPacket packet;
    int received = SSL_read(ssl, &packet, sizeof(packet));
    if (received <= 0) {
        std::cerr << "[Server] Error reading packet.\n";
    } else {
        std::cout << "[Server] Received packet of size " << received << "\n";
        std::cout << "[Server] Payload: " << packet.payload << "\n";
    }
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(clientSock);
}
