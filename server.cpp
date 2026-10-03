#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> //what even are headers, addrinfo is defined in netdb
#include <arpa/inet.h>

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
    if (status = getaddrinfo(NULL, "8080", &hints, &result))
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

    bind(sockfd, result->ai_addr, result->ai_addrlen);
    // error handling pending

    listen(sockfd, 5);

    return 0;
}