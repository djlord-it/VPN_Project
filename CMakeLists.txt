# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(VPNProject)

# Use C++17
set(CMAKE_CXX_STANDARD 17)

# Find OpenSSL (on Mac, ensure you have it installed, e.g., via brew install openssl)
find_package(OpenSSL REQUIRED)

# Where to look for header files
include_directories(${PROJECT_SOURCE_DIR}/include)

# -- Server executable --
add_executable(server
        ${PROJECT_SOURCE_DIR}/src/main_server.cpp
        ${PROJECT_SOURCE_DIR}/src/VPNServer.cpp
        ${PROJECT_SOURCE_DIR}/src/EncryptionHandler.cpp
)

# Link against OpenSSL libraries (ssl and crypto)
target_link_libraries(server PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# -- Client executable --
add_executable(client
        ${PROJECT_SOURCE_DIR}/src/main_client.cpp
        ${PROJECT_SOURCE_DIR}/src/VPNClient.cpp
        ${PROJECT_SOURCE_DIR}/src/EncryptionHandler.cpp
)

target_link_libraries(client PRIVATE OpenSSL::SSL OpenSSL::Crypto)
