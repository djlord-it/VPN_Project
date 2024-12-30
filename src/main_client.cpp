#include <iostream>
#include "VPNClient.hpp"
#include "Logger.hpp"

int main() {
    // For simplicity, hard-code or load from JSON
    std::string serverIP = "127.0.0.1";
    int serverPort = 8080;

    VPNClient client(serverIP, serverPort);
    if (!client.connectToServer()) {
        Log("Failed to connect to the server", LogLevel::ERROR);
        return -1;
    }

    if (client.sendTunnelPacket("Hello client!")) {
        Log("Packet sent successfully", LogLevel::INFO);
    } else {
        Log("Failed to send packet", LogLevel::ERROR);
    }

    return 0;
}
