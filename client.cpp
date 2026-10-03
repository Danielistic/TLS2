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
    {
        std::cerr << "getaddrinfo - " << gai_strerror(status);
        return 1;
    }

    int connectfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    // error handling

    connect(connectfd, result->ai_addr, result->ai_addrlen);
    // error handling
}