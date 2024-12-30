#include "EncryptionHandler.hpp"
#include <iostream>

EncryptionHandler::EncryptionHandler()
    : ctx_(nullptr)
{
    // Initialize OpenSSL
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
}

EncryptionHandler::~EncryptionHandler()
{
    if (ctx_) {
        SSL_CTX_free(ctx_);
        ctx_ = nullptr;
    }
    // Note: Additional OpenSSL cleanup could go here if needed
}

bool EncryptionHandler::initServerContext(const std::string &certFile, const std::string &keyFile)
{
    const SSL_METHOD* method = SSLv23_server_method();
    ctx_ = SSL_CTX_new(method);
    if (!ctx_) {
        ERR_print_errors_fp(stderr);
        return false;
    }
    if (SSL_CTX_use_certificate_file(ctx_, certFile.c_str(), SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return false;
    }
    if (SSL_CTX_use_PrivateKey_file(ctx_, keyFile.c_str(), SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return false;
    }
    if (!SSL_CTX_check_private_key(ctx_)) {
        std::cerr << "Private key does not match the public certificate\n";
        return false;
    }
    return true;
}

bool EncryptionHandler::initClientContext()
{
    const SSL_METHOD* method = SSLv23_client_method();
    ctx_ = SSL_CTX_new(method);
    if (!ctx_) {
        ERR_print_errors_fp(stderr);
        return false;
    }
    return true;
}

SSL* EncryptionHandler::createSSL(int socketFD)
{
    if (!ctx_) {
        return nullptr;
    }
    SSL* ssl = SSL_new(ctx_);
    SSL_set_fd(ssl, socketFD);
    return ssl;
}
