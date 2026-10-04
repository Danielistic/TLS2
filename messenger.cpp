#include "messenger.hpp"

int send_all(int fd, const char *msg, unsigned int len)
{
    unsigned int sentlen = 0;

    while (sentlen < len)
    {
        int sent = send(fd, msg + sentlen, len - sentlen, MSG_NOSIGNAL);
        if (sent <= 0)
            return 0;
        sentlen += sent;
    }
    return sentlen;
}

int send_msg(int fd, const char *msg, uint8_t mtype) // -1 iseither 0 len string or error, otherwise len
{
    unsigned int len = strlen(msg);
    if (len == 0)
    {
        return -1;
    }
    unsigned int temp = htonl(len);

    if (
        send_all(fd, (char *)&mtype, 1) &&
        send_all(fd, (char *)&temp, sizeof temp) &&
        send_all(fd, msg, len))
    {
        return strlen(msg);
    }
    else
        return -1;
}

int recv_exact(int fd, char *msg, unsigned int len)
{
    unsigned int recieved = 0;
    long n;
    while (recieved < len)
    {
        n = recv(fd, msg + recieved, len - recieved, 0);
        if (n == 0)
            return 0;
        if (n == -1)
            return -1;
        recieved += n;
    }
    return recieved;
}

std::pair<int, uint8_t> recv_msg(int fd, char *msg, unsigned int maxlen) // -2 for len 0 and -3 for
{
    uint8_t mtype;
    int f;
    if ((f = recv_exact(fd, (char *)&mtype, 1)) <= 0)
    {
        return {f, 0};
    }
    unsigned int len;
    if ((f = recv_exact(fd, (char *)&len, sizeof len)) <= 0)
    {
        return {f, 0};
    }
    len = ntohl(len);

    // if (len > maxlen) // im albert einstein
    // {
    //     char *shit = new char[len];
    //     unsigned int n = recv_exact(fd, shit, len);
    //     delete[] shit;
    //     return {-2, 0};
    // }
    if (len == 0)
        return {-2, 0};
    if (len > maxlen)
        return {-3, 0};
    return {recv_exact(fd, msg, len), mtype};
}
