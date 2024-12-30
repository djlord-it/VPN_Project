# VPN Project: A Secure Communication System 🌐🔒

Welcome to the **VPN Project**! This project provides a robust, secure, and efficient implementation of a Virtual Private Network (VPN). Designed with modern **C++** and powered by **OpenSSL**, it facilitates encrypted communication between clients and servers. This VPN is ideal for exploring secure communication, understanding VPN internals, or even building customized network solutions.

---

## Key Features 

### **1. Secure Communication**
Leverages OpenSSL to establish TLS-encrypted channels, ensuring:
- **Confidentiality:** Data remains unreadable to unauthorized parties.
- **Integrity:** Protects data from being altered during transmission.
- **Authentication:** Ensures communication occurs between verified entities.

### **2. Cross-Platform Compatibility**
The VPN works seamlessly on **Linux**, **macOS**, and within **Docker containers**, providing a versatile environment for deployment and testing.

### **3. Easy Configuration**
All server and client settings are managed via a simple JSON file (`config.json`), making it straightforward to customize and deploy.

### **4. Multi-Client Support**
Built with threading to efficiently manage multiple client connections simultaneously, ensuring high scalability.

### **5. Custom Data Encapsulation**
Implements a `TunnelPacket` structure to encapsulate and manage transmitted data, ensuring flexibility and efficiency in handling packetized information.

### **6. Logging Utility**
A lightweight logging system (`Logger.hpp`) is included for real-time tracking of server and client activities, helping with debugging and monitoring.

### **7. Docker Support**
Ready-to-use **Dockerfile** and setup instructions make deployment easy in containerized environments, reducing dependencies and configuration overhead.

### **8. Educational Exploration**
This project serves as an educational sandbox to explore foundational VPN concepts, including:
- Setting up **TCP sockets** for server-client communication.
- Implementing **TLS/SSL encryption** using OpenSSL.
- Structuring **C++ classes** for modular server, client, and encryption handling.
- Exploring the basics of **packet forwarding** (TUN/TAP interfaces).

---

## Project Structure 📂

The project is organized to ensure clarity and modularity:
```
VPN_Project/
├── CMakeLists.txt           # Build system configuration
├── config.json              # Configuration file for server and client
├── Dockerfile               # Optional Docker setup
├── include/                 # Header files
│   ├── EncryptionHandler.hpp   # Handles encryption and decryption
│   ├── VPNServer.hpp           # Server functionality
│   ├── VPNClient.hpp           # Client functionality
│   └── TunnelPacket.hpp        # Custom packet structure
├── src/                     # Source files
│   ├── EncryptionHandler.cpp  # Encryption implementation
│   ├── VPNServer.cpp          # Server implementation
│   ├── VPNClient.cpp          # Client implementation
│   ├── main_server.cpp        # Entry point for the server
│   ├── main_client.cpp        # Entry point for the client
│   └── Logger.hpp             # Logging utility
└── build/                    # Build directory (ignored in version control)
└── tests/                   # Unit tests
    ├── CMakeLists.txt         # Build configuration for tests
    └── test_vpn.cpp           # Test cases for VPN functionality
```

---

## Getting Started 🚀

### Prerequisites
To build and run this project, ensure the following tools are installed:

#### **1. C++ Compiler**
- Requires a compiler with **C++17** support, such as:
   - `g++` (Linux)
   - `clang++` (macOS)

#### **2. OpenSSL Library**
- Necessary for encryption. Install it using:
  ```bash
  # Linux
  sudo apt install libssl-dev

  # macOS
  brew install openssl
  ```

#### **3. CMake**
- For managing the build process. Install it using:
  ```bash
  # Linux
  sudo apt install cmake

  # macOS
  brew install cmake
  ```

#### **4. Docker** (Optional)
- For containerized deployment and testing. Install Docker from [Docker's official website](https://www.docker.com/).

#### **5. OpenSSL Certificates**
Generate a self-signed certificate and key for testing:
```bash
openssl req -x509 -newkey rsa:2048 -nodes \
    -keyout server.key -out server.crt -days 365
```

---

### Building the Project

#### **Step 1: Clone the Repository**
```bash
git clone https://github.com/djlord-it/VPN_Project.git
cd VPN_Project
```

#### **Step 2: Create a Build Directory**
```bash
mkdir build
cd build
```

#### **Step 3: Build with CMake**
```bash
cmake ..
make
```

If OpenSSL is in a non-standard location, specify its path:
```bash
cmake -DOPENSSL_ROOT_DIR=/path/to/openssl ..
```

This produces two executables: `server` and `client`.

---

### Running the Server and Client

#### **1. Start the Server**
```bash
./server
```
- The server reads its configuration from `config.json` and begins listening for client connections.

#### **2. Run the Client**
```bash
./client
```
- The client connects to the server and transmits a test packet for verification.

#### Expected Output
- **Server Logs:**
```plaintext
[Server] Listening on port 8080...
[Server] Client connected!
[Server] TLS handshake successful.
[Server] Received packet of size ...
[Server] Payload: Hello from VPNClient!
```

- **Client Logs:**
```plaintext
[Client] Connected and TLS handshake successful.
[Client] Packet sent successfully.
```

---

## Docker Setup 🐳

Deploy the VPN using Docker for simplified setup and isolated testing.

### **Building the Docker Image**
```bash
docker build -t vpn_server .
```

### **Running the Server in a Container**
```bash
docker run -d -p 8080:8080 --name vpn_server_instance vpn_server
```

### **Running the Client in a Container**
```bash
docker run --network host --name vpn_client_instance vpn_client
```

---

## Testing 🧪

### Local Testing
- Use **Wireshark** to observe TLS-encrypted packets. Monitor traffic on the `lo` or `eth0` interface.

### Cross-Machine Testing
- Run the server on one machine and the client on another.
- Update `VPNClient.cpp` to connect to the server's IP.
- Ensure the server's firewall allows incoming traffic on the configured port (e.g., 8080).

---

## Configuration ⚙️

The VPN's behavior is controlled via `config.json`:
```json
{
  "server": {
    "ip": "0.0.0.0",
    "port": 8080
  },
  "certificates": {
    "cert": "server.crt",
    "key": "server.key"
  }
}
```

### Configuration Options
- **`server.ip`**: The IP address the server binds to. Use `0.0.0.0` for all interfaces.
- **`server.port`**: Port number for the server to listen on.
- **`certificates.cert`**: Path to the server's SSL certificate.
- **`certificates.key`**: Path to the server's private key.

---

## Contributing 🤝

We welcome contributions to improve the project! You can contribute by:
- Fixing bugs.
- Adding new features.
- Enhancing documentation.

Feel free to open an issue or submit a pull request on the [GitHub repository](https://github.com/djlord-it/VPN_Project).

---
