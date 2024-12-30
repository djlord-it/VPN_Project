project(AdvancedVPNTests)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(test_vpn test_vpn.cpp)

target_link_libraries(test_vpn PRIVATE
        GTest::gtest_main
        vpn_lib
        OpenSSL::SSL
        OpenSSL::Crypto
        nlohmann_json::nlohmann_json
)

add_test(NAME VPNTests COMMAND test_vpn)