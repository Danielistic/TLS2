#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> //what even are headers, addrinfo is defined in netdb
#include <arpa/inet.h>
#include <cstring>
#include <thread>
#include <vector>
#include <mutex>
#include "messenger.hpp"

#define PORT "8080"

std::vector<int> Clist(2);
std::mutex mtx;

void clienthandle(int clientfd)
{
    while (1)
    {
        char msg[1000];
        int n = recv(clientfd, msg, 999, 0);
        if (n > 0)
        {
            msg[n] = '\0';
            std::lock_guard<std::mutex> lock(mtx);
            if (Clist[0] == 0 || Clist[1] == 0)
                send_msg(clientfd, "Nah mate nobody here");
            else if (Clist[0] == clientfd)
            {
                send_msg(Clist[1], msg);
                std::cout << "message recieved at server: " << msg << "\n";
            }
            else
            {
                send_msg(Clist[0], msg);
                std::cout << "message recieved at server: " << msg << "\n";
            }
        }
        else if (n <= 0)
        {
            if (n == -1)
                perror("recv");
            std::cout << "one of the clients disconnected\n";
            {
                std::lock_guard<std::mutex> lock(mtx);
                if (clientfd == Clist[0])
                    Clist[0] = 0;
                else
                    Clist[1] = 0;
            }
            break;
        }
    }
}

//------------------addrinfo------------------------------
// struct addrinfo {
//     int              ai_flags;     // AI_PASSIVE, AI_CANONNAME, etc.
//     int              ai_family;    // AF_INET, AF_INET6, AF_UNSPEC
//     int              ai_socktype;  // SOCK_STREAM, SOCK_DGRAM
//     int              ai_protocol;  // use 0 for "any"
//     size_t           ai_addrlen;   // size of ai_addr in bytes
//     struct sockaddr *ai_addr;      // struct sockaddr_in or _in6
//     char            *ai_canonname; // full canonical hostname

//     struct addrinfo *ai_next;      // linked list, next node
// };
//-------------------------------------------------------

int main()
{
    addrinfo hints{}; // creating a hint to pass into getaddrinfo
    addrinfo *result;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // int getaddrinfo(const char *node,   // e.g. "www.example.com" or IP
    //                 const char *service,  // e.g. "http" or port number
    //                 const struct addrinfo *hints,
    //                 struct addrinfo **res);
    int status;
    if (status = getaddrinfo(NULL, PORT, &hints, &result))
    {
        std::cerr << "getaddrinfo - " << gai_strerror(status);
        return 1;
    }

    // sockaddr_in *addr4 = (sockaddr_in *)result->ai_addr;
    // char ip[INET_ADDRSTRLEN];
    // std::cout << inet_ntop(AF_INET, &addr4->sin_addr, ip, result->ai_addrlen);

    // int socket(int domain, int type, int protocol);
    // int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
    // int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);
    // int listen(int sockfd, int backlog);

    int sockfd;
    sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    // error handling for socket creating is pending

    if (bind(sockfd, result->ai_addr, result->ai_addrlen) == 1)
    {
        perror("bind");
        return -1;
    }

    freeaddrinfo(result);

    if (listen(sockfd, 5) == 1)
    {
        perror("listen");
        return -1;
    };

    std::cout << "listening on PORT:" << PORT << "\n";

    while (1)
    {

        if (Clist[0] == 0 || Clist[1] == 0)
        {
            int clientfd;
            // sockaddr_storage caddr;
            // socklen_t caddrlen = sizeof caddr;
            sockaddr_in caddr;
            socklen_t caddrlen = sizeof caddr;

            clientfd = accept(sockfd, (sockaddr *)&caddr, &caddrlen); // error handling pending
            if (clientfd == -1)
            {
                perror("accept");
                continue;
            }
            // std::cout << inet_ntop(AF_INET, &caddr.sin_addr, ip, caddrlen);

            {
                std::lock_guard<std::mutex> lock(mtx);
                if (Clist[0] == 0)
                    Clist[0] = clientfd;
                else
                    Clist[1] = clientfd; // need to handle errors if connection is lost by the time it comes here
                std::thread handle(clienthandle, clientfd);
                handle.detach();
            }
        }
    }

    return 0;
}