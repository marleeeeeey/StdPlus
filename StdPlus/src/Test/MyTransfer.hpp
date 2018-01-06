#pragma once

#include "OopSocket\OopSocket.h"


namespace MyTestTransfer
{

    const int STREAM_BUFF_SIZE = 1024;

    inline long long send(TCPSocketPtr sock, const std::istream & in_ifs)
    {
        long long numberBytes = 0;


        std::istream & ifs = const_cast<std::istream &>(in_ifs);
        ifs.seekg(0, std::ios::end);
        const long long fileSize = ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        Transfer::send(sock, fileSize);

        char * sendDataBuffer = new char[STREAM_BUFF_SIZE];

        while (numberBytes != fileSize)
        {
            long long curBuffSize = 0;
            curBuffSize = fileSize - numberBytes;
            if (curBuffSize > STREAM_BUFF_SIZE)
                curBuffSize = STREAM_BUFF_SIZE;

            ifs.read(sendDataBuffer, curBuffSize);
            long long curSend = sock->Send(sendDataBuffer, curBuffSize);
            numberBytes += curSend;
        }
        delete[] sendDataBuffer;
        return numberBytes;
    }

    inline long long recv(TCPSocketPtr sock, std::ostream & ofs)
    {
        long long numberBytes = 0;

        long long fileSize = 0;
        Transfer::recv(sock, &fileSize);

        char * dataBuff = new char[STREAM_BUFF_SIZE];

        while (numberBytes != fileSize)
        {
            long long curBuffSize = 0;
            curBuffSize = fileSize - numberBytes;
            if (curBuffSize > STREAM_BUFF_SIZE)
                curBuffSize = STREAM_BUFF_SIZE;

            long long curSend = sock->ReceiveFullSize(dataBuff, curBuffSize);
            ofs.write(dataBuff, curBuffSize);
            numberBytes += curSend;
        }
        delete[] dataBuff;
        return numberBytes;
    }



}