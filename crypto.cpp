#include "crypto.hpp"

crypto::crypto()
{
    flag = 0;
}

std::string crypto::ByteToHex(unsigned char *b, uint32_t blen)
{
    std::string s = "";
    char hexref[] = "0123456789ABCDEF";
    for (int i = 0; i < blen; i++)
    {
        uint8_t a1 = int(b[i]);
        uint8_t a2 = a1 >> 4;
        uint8_t a3 = a1 - (a2 << 4);

        s += hexref[a2];
        s += hexref[a3];
        s += ' ';
    }
    return s;
}

uint8_t ChartoInt(char c)
{
    if (c >= '0' && c <= '9')
        return uint8_t(c - '0');
    if (c >= 'A' && c < 'G')
        return uint8_t(c - 'A' + 10);
    return -1; // error is not handled may god bless this function
}

void *crypto::HexToByte(const std::string &h, uint32_t hlen, unsigned char *out)
{
    int n = 0;
    for (int i = 0; i < hlen; i += 3)
    {
        uint8_t a1 = ChartoInt(h[i]);
        uint8_t a2 = ChartoInt(h[i + 1]);
        uint8_t a3 = (a1 << 4) | a2;
        out[n] = (unsigned char)a3;
        n++;
    }
    return 0; // never heard of error handling
}

int crypto::KeyGen()
{
    if (sodium_init() < 0)
    {
        std::cout << "How do we lack Sodium this close to the sea";
        return -1;
    }
    if (crypto_kx_keypair(publickey, secretkey) != 0)
        return -1;
    flag = 1;
    return 0;
}

int crypto::CreateSharedKey(unsigned char *recieved)
{
    bool flag = false;
    for (int i = 0; i < 32; i++)
    {
        if (publickey[i] < recieved[i])
        {
            flag = true;
        }
    }

    if (flag)
    {
        if (crypto_kx_client_session_keys(recievekey, sendkey, publickey, secretkey, recieved) < 0)
        {
            return -1;
        }
    }
    else
    {
        if (crypto_kx_server_session_keys(recievekey, sendkey, publickey, secretkey, recieved) < 0)
            return -1;
    }
    return 0;
}

// int crypto::CreateSharedKey_server(unsigned char *recieved)
// {
//     if (crypto_kx_server_session_keys(recievekey, sendkey, publickey, secretkey, recieved) < 0)
//     {
//         return -1;
//     }
//     return 0;
// }

int crypto::SendPublicKey(int fd)
{
    if (send_msg(fd, (char *)publickey, 32, 1) < 0)
        return -1;
    return 0;
}