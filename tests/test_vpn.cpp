#include <gtest/gtest.h>
#include "EncryptionHandler.hpp"
#include "VPNClient.hpp"
#include "VPNServer.hpp"

TEST(EncryptionHandlerTest, InitServerContext) {
    EncryptionHandler handler;
    bool result = handler.initServerContext("server.crt", "server.key");
    EXPECT_TRUE(result);
}

TEST(ClientTest, BasicInstantiation) {
    VPNClient client("127.0.0.1", 8080);
    EXPECT_TRUE(true);
}
