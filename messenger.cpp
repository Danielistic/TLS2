#include "messenger.hpp"

int send_msg(int fd, const char *msg)
{
    int len = strlen(msg);
    int sentlen = 0;

    while (sentlen < len)
    {
        int sent = send(fd, msg + sentlen, len - sentlen, 0);
        if (sent <= 0)
            return 0;
        sentlen += sent;
    }
    return sentlen;
}
