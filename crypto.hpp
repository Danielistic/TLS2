#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <thread>
#include <vector>
#include <iomanip>
#include <unordered_map>
#include "messenger.hpp"
#include <sodium.h>

class crypto
{

    unsigned char publickey[crypto_kx_PUBLICKEYBYTES];
    unsigned char secretkey[crypto_kx_SECRETKEYBYTES];
    unsigned char r_publickey[crypto_kx_PUBLICKEYBYTES];
    unsigned char recievekey[crypto_kx_SESSIONKEYBYTES];
    unsigned char sendkey[crypto_kx_SESSIONKEYBYTES];
    int flag;

public:
    crypto();
    std::string ByteToHex(unsigned char *b, uint32_t blen);
    void *HexToByte(const std::string &h, uint32_t hlen, unsigned char *out);

    int KeyGen();
    int CreateSharedKey(unsigned char *recieved);
    // int CreateSharedKey_server(unsigned char *recieved);
    int SendPublicKey(int fd);
};