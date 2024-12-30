#pragma once

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string>

class EncryptionHandler {
public:
    EncryptionHandler();
    ~EncryptionHandler();

    // Server side
    bool initServerContext(const std::string &certFile, const std::string &keyFile);

    // Client side
    bool initClientContext();

    SSL* createSSL(int socketFD);

    SSL_CTX* getContext() const { return ctx_; }

private:
    SSL_CTX *ctx_;
};
