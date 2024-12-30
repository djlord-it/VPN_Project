#include "VPNServer.hpp"

int main()
{
    // Use your actual certificate and key files
    VPNServer server(8080, "server.crt", "server.key");
    if (!server.start()) {
        return -1;
    }
    return 0;
}
