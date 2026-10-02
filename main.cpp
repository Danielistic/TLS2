#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> //what even are headers, addrinfo is defined in netdb

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
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // int getaddrinfo(const char *node,   // e.g. "www.example.com" or IP
    //                 const char *service,  // e.g. "http" or port number
    //                 const struct addrinfo *hints,
    //                 struct addrinfo **res);
    if (getaddrinfo(NULL, "8080", &hints, &result))
    {
        std::cout << "\nerror in getaddrinfo\n";
    }

    return 0;
}