#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <thread>
#include <string>
#include "messenger.hpp"

#define PORT "8080"
#define LOCHOST "127.0.0.1"

void reciever(int connectfd)
{
    while (1)
    {
        char msg[1000];
        int n = recv(connectfd, msg, 999, 0);
        if (n > 0)
        {
            msg[n] = '\0';
            std::cout << "message recieved by client:" << msg << "\n";
        }
        else
        {
            if (n == -1)
                perror("recv");
            std::cout << "Disconnected from server... \n";
            break;
        }
    }
    exit(0);
}

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

    if (connect(connectfd, result->ai_addr, result->ai_addrlen) == -1)
    {
        perror("connect");
        return 1;
    }

    freeaddrinfo(result);
    std::cout << "we are in the server... i think...\n";

    std::thread rsv(reciever, connectfd);
    rsv.detach();
    while (1)
    {
        std::string msg;
        std::getline(std::cin, msg);

        send_msg(connectfd, msg.c_str());
        std::cout << "the msg sent by us: " << msg << "\n";
    }
}