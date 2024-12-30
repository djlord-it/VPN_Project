#include "VPNClient.hpp"
#include <iostream>

int main()
{
    VPNClient client("127.0.0.1", 8080);
    if (!client.connectToServer()) {
        return -1;
    }

    // Send some data
    if (client.sendTunnelPacket("Hello from VPNClient!")) {
        std::cout << "[Client] Packet sent successfully.\n";
    } else {
        std::cerr << "[Client] Failed to send packet.\n";
    }

    return 0;
}
