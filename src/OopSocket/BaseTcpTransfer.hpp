#pragma once

#include "TCPSocket.h"
#include <string>

namespace Transfer
{
    template<typename T>
    size_t send(TCPSocketPtr sock, const T & d)
    {
        size_t numberBytes = 0;
        numberBytes += sock->Send(&d, sizeof(d));
        return numberBytes;
    }

    template<typename T>
    size_t recv(TCPSocketPtr sock, T * pData)
    {
        size_t numberBytes = 0;
        numberBytes += sock->ReceiveFullSize(pData, sizeof(T));
        return numberBytes;
    }


    inline size_t send(TCPSocketPtr sock, const std::string & str)
    {
        size_t numberBytes = 0;

        size_t stringSize = str.size();
        Transfer::send(sock, stringSize);
        numberBytes += sock->Send(str.c_str(), str.size());

        return numberBytes;
    }

    inline size_t recv(TCPSocketPtr sock, std::string * pData)
    {
        size_t numberBytes = 0;

        size_t stringSize;
        Transfer::recv(sock, &stringSize);

        char * strBuf = new char[stringSize + 1];
        numberBytes += sock->Receive(strBuf, stringSize);

        strBuf[stringSize] = '\0';
        *pData = strBuf;
        delete[] strBuf;

        return numberBytes;
    }

}

