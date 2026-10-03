#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT "8080"
#define LOCHOST "127.0.0.1"

int main()
{
    addrinfo hints{}, *result;

    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;

    int status;
    if (status = getaddrinfo(LOCHOST, PORT, &hints, &result))
        ;

    int sockfd;
}