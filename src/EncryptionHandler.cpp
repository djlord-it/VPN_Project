#include "EncryptionHandler.hpp"
#include "Logger.hpp"

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
}

bool EncryptionHandler::initServerContext(const std::string &certFile, const std::string &keyFile)
{
    const SSL_METHOD* method = TLS_method();
    ctx_ = SSL_CTX_new(method);
    if (!ctx_) {
        Log("Failed to create SSL_CTX for server", LogLevel::ERROR);
        ERR_print_errors_fp(stderr);
        return false;
    }

    // Force TLS 1.2 or higher
    SSL_CTX_set_min_proto_version(ctx_, TLS1_2_VERSION);

    // Load certificate
    if (SSL_CTX_use_certificate_file(ctx_, certFile.c_str(), SSL_FILETYPE_PEM) <= 0) {
        Log("Failed to load server certificate: " + certFile, LogLevel::ERROR);
        ERR_print_errors_fp(stderr);
        return false;
    }

    // Load private key
    if (SSL_CTX_use_PrivateKey_file(ctx_, keyFile.c_str(), SSL_FILETYPE_PEM) <= 0) {
        Log("Failed to load server key: " + keyFile, LogLevel::ERROR);
        ERR_print_errors_fp(stderr);
        return false;
    }

    if (!SSL_CTX_check_private_key(ctx_)) {
        Log("Private key does not match public certificate", LogLevel::ERROR);
        return false;
    }

    // Optionally disable old protocols, ciphers here for more security
    // e.g., SSL_CTX_set_options(ctx_, SSL_OP_NO_TLSv1 | SSL_OP_NO_TLSv1_1);

    return true;
}

bool EncryptionHandler::initClientContext()
{
    const SSL_METHOD* method = TLS_method();
    ctx_ = SSL_CTX_new(method);
    if (!ctx_) {
        Log("Failed to create SSL_CTX for client", LogLevel::ERROR);
        ERR_print_errors_fp(stderr);
        return false;
    }
    SSL_CTX_set_min_proto_version(ctx_, TLS1_2_VERSION);

    // Optionally set up CA chain for verifying server certificate
    // SSL_CTX_load_verify_locations(ctx_, "ca.crt", nullptr);
    // SSL_CTX_set_verify(ctx_, SSL_VERIFY_PEER, nullptr);

    return true;
}

SSL* EncryptionHandler::createSSL(int socketFD)
{
    if (!ctx_) {
        Log("SSL_CTX is not initialized", LogLevel::ERROR);
        return nullptr;
    }
    SSL* ssl = SSL_new(ctx_);
    if (!ssl) {
        Log("SSL_new failed", LogLevel::ERROR);
        return nullptr;
    }
    SSL_set_fd(ssl, socketFD);
    return ssl;
}
