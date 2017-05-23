#include "TCPSocket.h"
#include "SocketUtil.h"


int TCPSocket::Connect(const SocketAddress& inAddress)
{
    sockaddr addr = inAddress.mSockAddr;
    sockaddr_in * addrin = (sockaddr_in *)&addr;

    int err = connect(mSocket, &inAddress.mSockAddr, inAddress.GetSize());
    if (err < 0)
    {
        throw std::logic_error("TCPSocket::Connect");
//         SocketUtil::ReportError("TCPSocket::Connect");
//         return -SocketUtil::GetLastError();
    }
    return NO_ERROR;
}

int TCPSocket::Listen(int inBackLog)
{
    int err = listen(mSocket, inBackLog);
    if (err < 0)
    {
        throw std::logic_error("TCPSocket::Listen");
//         SocketUtil::ReportError("TCPSocket::Listen");
//         return -SocketUtil::GetLastError();
    }
    return NO_ERROR;
}

TCPSocketPtr TCPSocket::Accept(SocketAddress& inFromAddress)
{
    socklen_t length = inFromAddress.GetSize();
    SOCKET newSocket = accept(mSocket, &inFromAddress.mSockAddr, &length);

    if (newSocket != INVALID_SOCKET)
    {
        return TCPSocketPtr(new TCPSocket(newSocket));
    }
    else
    {
        throw std::logic_error("TCPSocket::Accept");
//         SocketUtil::ReportError("TCPSocket::Accept");
//         return nullptr;
    }
}

int32_t TCPSocket::Send(const void* inData, size_t inLen)
{
    int bytesSentCount = send(mSocket, static_cast<const char*>(inData), inLen, 0);
    if (bytesSentCount < 0)
    {
        throw std::logic_error("TCPSocket::Send");
//         SocketUtil::ReportError("TCPSocket::Send");
//         return -SocketUtil::GetLastError();
    }
    return bytesSentCount;
}

int32_t TCPSocket::Receive(void* inData, size_t inLen)
{
    int bytesReceivedCount = recv(mSocket, static_cast<char*>(inData), inLen, 0);
    if (bytesReceivedCount < 0)
    {
        throw std::logic_error("TCPSocket::Receive");
//         SocketUtil::ReportError("TCPSocket::Receive");
//         return -SocketUtil::GetLastError();
    }
    return bytesReceivedCount;
}

int32_t TCPSocket::ReceiveFullSize(void* inBuffer, size_t inLen)
{
    size_t bufferDiff = 0;
    char * buffer = static_cast<char*>(inBuffer);
    int recvBytes = 0;

    while (inLen > 0)
    {
        size_t curRecv = Receive(buffer + bufferDiff, inLen);
        recvBytes += curRecv;
        inLen -= curRecv;
        bufferDiff += curRecv;
    }

    return recvBytes;
}

int TCPSocket::Bind(const SocketAddress& inBindAddress)
{
    int error = bind(mSocket, &inBindAddress.mSockAddr, inBindAddress.GetSize());
    if (error != 0)
    {
        throw std::logic_error("TCPSocket::Bind");
//         SocketUtil::ReportError("TCPSocket::Bind");
//         return SocketUtil::GetLastError();
    }

    return NO_ERROR;
}

TCPSocket::~TCPSocket()
{
#if _WIN32
    closesocket(mSocket);
#else
    close(mSocket);
#endif
}
