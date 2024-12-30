#include <iostream>
#include <fstream>
#include "VPNServer.hpp"
#include "Logger.hpp"
// For JSON parsing:
#include <nlohmann/json.hpp>

int main() {
    try {
        // Load config.json
        std::ifstream configFile("config.json");
        if (!configFile.is_open()) {
            Log("Could not open config.json", LogLevel::ERROR);
            return -1;
        }

        nlohmann::json config;
        configFile >> config;

        std::string ip   = config["server"]["ip"];
        int port         = config["server"]["port"];
        std::string cert = config["certificates"]["cert"];
        std::string key  = config["certificates"]["key"];

        VPNServer server(ip, port, cert, key);
        if (!server.start()) {
            Log("Server failed to start", LogLevel::ERROR);
            return -1;
        }

    } catch (const std::exception &ex) {
        Log(std::string("Exception: ") + ex.what(), LogLevel::ERROR);
        return -1;
    }

    return 0;
}
