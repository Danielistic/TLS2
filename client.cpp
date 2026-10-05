#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <thread>
#include <string>
#include <chrono>
#include "messenger.hpp"
#include "crypto.hpp"

#define PORT "8080"
#define LOCHOST "127.0.0.1"

bool KEYESTABLISHED = false;
crypto C;

void reciever(int connectfd)
{
    while (1)
    {
        char msg[65500];
        auto r = recv_msg(connectfd, msg, 65499);
        int n = r.first;
        uint8_t t = r.second;

        if (n > 0 && t == 2)
        {
            C.SendPublicKey(connectfd);
            continue;
        }

        if (t == 1)
        {
            if (C.CreateSharedKey((unsigned char *)msg) < 0)
            {
                std::cout << "key Recieved and Dropped\n";
                continue;
            }
            std::cout << "key Recieved and Accepted\n";
            KEYESTABLISHED = true;
            continue;
        }

        if (n > 0)
        {
            if (t == 0 && !KEYESTABLISHED)
            {
                std::cout << "message recieved and Dropped due to lack of key\n";
                continue;
            }
            msg[n] = '\0';
            std::cout << "message recieved by client:" << msg << "\n";
            std::cout << "recieved bytes: " << n << "\n";
        }
        else
        {
            if (n == -1)
                perror("recv");
            else if (n == -2 || n == -3)
                std::cout << "error code: " << n << "\n";
            std::cout << "Disconnected from server... \n";
            break;
        }
    }
    exit(0); // add exit(1) for n = -1
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
    std::cout << "we are in the server... \n";

    if (C.KeyGen() < 0)
    {
        std::cout << "error generating Key\n";
        return 0;
    }

    std::thread rsv(reciever, connectfd);
    rsv.detach();
    while (1)
    {

        // if (!KEYESTABLISHED)
        // {
        //     C.SendPublicKey(connectfd);
        //     std::this_thread::sleep_for(std::chrono::seconds(2));
        //     continue;
        // }
        std::string msg;
        std::getline(std::cin, msg);

        send_msg(connectfd, msg.c_str(), msg.size(), 0);
        std::cout << "Bytes sent: " << msg.size() << "\n";
    }
}