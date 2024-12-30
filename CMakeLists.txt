cmake_minimum_required(VERSION 3.14)
project(AdvancedVPN CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

find_package(OpenSSL REQUIRED)

find_package(nlohmann_json 3.2.0 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(vpn_lib STATIC
        src/EncryptionHandler.cpp
        src/VPNClient.cpp
        src/VPNServer.cpp
)

target_link_libraries(vpn_lib PUBLIC OpenSSL::SSL OpenSSL::Crypto nlohmann_json::nlohmann_json)

target_include_directories(vpn_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(server
        src/main_server.cpp
)

target_link_libraries(server PRIVATE vpn_lib)

add_executable(client
        src/main_client.cpp
)

target_link_libraries(client PRIVATE vpn_lib)

enable_testing()

add_subdirectory(tests)