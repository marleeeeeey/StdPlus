#pragma once

#include "OopSocket/OopSocket.h"
#include "gtest/gtest.h"
#include "StdPlus/StdPlus.h"


TEST(TCPSocket, NonBlockTest)
{
    SocketUtil::StaticInit();

    TCPSocketPtr sock = SocketUtil::CreateTCPSocket(SocketAddressFamily::INET);
    SocketAddress sa("127.0.0.1", 34567);
    sock->Connect(sa);

    sock->SetNonBlockingMode(true);

    for (int i = 0; i < 30; ++i)
    {
        char ch = 'a' + i;
        LOG_STD_VAR(ch);
        sock->Send(&ch, sizeof(ch));
        APAUSE;
    }

    SocketUtil::CleanUp();
}
