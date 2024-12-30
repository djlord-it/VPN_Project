# VPN Project Exploration

This repository contains a minimal proof-of-concept VPN server and client written in C++. I started this project to learn how secure connections and packet forwarding can work under the hood—both from a code perspective and from a networking standpoint. The result is not a fully production-ready VPN, but rather an educational sandbox to explore:

- How to set up a **TCP socket** for a server and a client.
- How to **encrypt** communications using **OpenSSL**.
- How to structure **C++ classes** for server, client, and encryption handling.

## Table of Contents

- [Overview](#overview)
- [Project Structure](#project-structure)
- [Installation and Compilation](#installation-and-compilation)
- [How to Run](#how-to-run)
- [Testing](#testing)
- [Future Enhancements](#future-enhancements)
- [License](#license)

---

## Overview

Initially, the idea was to discover what it takes to build a simple VPN from scratch, or at least the building blocks:

1. **TCP Sockets**: Basic networking to accept client connections.
2. **TLS/SSL Encryption**: Secure data exchange using OpenSSL.
3. **Traffic Forwarding**: (In a more advanced setup) forward packets to the actual destination via a TUN/TAP interface.

The code here is intentionally **minimal** and focuses on:

- A **VPNServer** class that listens for connections on a chosen port and performs a TLS handshake.
- A **VPNClient** class that connects to the server, negotiates encryption, and sends a simple “TunnelPacket”.
- An **EncryptionHandler** that initializes the OpenSSL context and handles certificate loading.

**Disclaimer**: This is a learning project. It is **not** meant to replace a production VPN solution like OpenVPN, WireGuard, or IPSec.

---

## Project Structure

```
VPN_Project/
├── CMakeLists.txt
├── include/
│   ├── VPNServer.hpp
│   ├── VPNClient.hpp
│   ├── EncryptionHandler.hpp
│   └── TunnelPacket.hpp
├── src/
│   ├── VPNServer.cpp
│   ├── VPNClient.cpp
│   ├── EncryptionHandler.cpp
│   ├── main_server.cpp
│   └── main_client.cpp
└── build/         // Where we build files (added to .gitignore typically)
```

- **VPNServer**: Creates a listening socket, accepts clients, performs SSL handshake, receives a “tunnel” packet.
- **VPNClient**: Connects to the server, negotiates TLS, sends a test packet.
- **EncryptionHandler**: Wraps OpenSSL context creation (both server and client side).
- **TunnelPacket**: A small struct to represent tunneled data (in practice, you’d read from a TUN interface).

---

## Installation and Compilation

### Prerequisites

- A C++ compiler that supports at least **C++17**.
- **CMake** (version >= 3.10).
- **OpenSSL** development headers (libssl-dev on Debian/Ubuntu/Mint).

On Debian/Ubuntu/Mint, install dependencies:

```bash
sudo apt-get update
sudo apt-get install cmake build-essential libssl-dev
```

### Build Steps

1. Clone or download this repository.

2. Generate a self-signed certificate and key for testing:

    ```bash
    openssl req -x509 -newkey rsa:2048 -nodes \
        -keyout server.key -out server.crt -days 365
    ```

   When prompted, fill out the certificate info (Country, State, etc.). This produces two files: `server.key` and `server.crt`.

3. Create and enter the build directory:

    ```bash
    mkdir build
    cd build
    ```

4. Run CMake:

    ```bash
    cmake ..
    ```

   If OpenSSL is in a non-standard location, you may need to specify its path:

    ```bash
    cmake -DOPENSSL_ROOT_DIR=/path/to/openssl ..
    ```

5. Compile:

    ```bash
    make
    ```

   This produces two executables: `server` and `client`.

---

## How to Run

1. Move or copy `server.crt` and `server.key` into the same folder as the built executables (usually `build/`), or adjust your code to point to their absolute paths.

2. Run the server in one terminal:

    ```bash
    ./server
    ```

   You should see:

    ```
    [Server] Listening on port 8080...
    ```

3. Run the client in another terminal (on the same machine for now):

    ```bash
    ./client
    ```

   You should see:

    ```
    [Client] Connected and TLS handshake successful.
    [Client] Packet sent successfully.
    ```

   Server logs:

    ```
    [Server] Client connected!
    [Server] TLS handshake successful.
    [Server] Received packet of size ...
    [Server] Payload: Hello from VPNClient!
    ```

   That confirms a basic encrypted handshake and a single test message exchange.

---

## Testing

### Local Testing

- **Wireshark**: If you want to see encrypted packets, run Wireshark on `lo` (or `eth0` if you’re bridging between machines). You’ll see TLS traffic, not plain text.

### Cross-Machine Testing

- Put the server on one machine (e.g., Linux Mint) and the client on another (e.g., macOS).
- In the `VPNClient.cpp` code, change `127.0.0.1` to the IP address of the server machine.
- Ensure that:
    - The server’s firewall/iptables allows incoming connections on port 8080.
    - You can reach the server machine over the network.

---

## License

This project is provided as-is, without warranty. You’re free to use it for educational or experimental purposes. For anything production-grade, consider well-established VPN projects.

---

