#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <thread>

int send_all(int fd, const char *msg); // remove if not used directly
int send_msg(int fd, const char *msg, uint8_t mtype);

// recv_all and recv_msg
// return should be same as return

int recv_exact(int fd, char *msg, unsigned int len); // remove if not used directly
std::pair<int, uint8_t> recv_msg(int fd, char *msg, unsigned int maxlen);
