#include "OopSocket/TCPSocket.h"
#include "OopSocket/SocketUtil.h"


int TCPSocket::Connect(const SocketAddress& inAddress)
{
    sockaddr addr = inAddress.mSockAddr;
    sockaddr_in * addrin = (sockaddr_in *)&addr;

    int err = connect(mSocket, &inAddress.mSockAddr, inAddress.GetSize());
    if (err < 0)
    {
        std::string err = std::to_string(SocketUtil::GetLastError());
        throw std::logic_error("ERROR: TCPSocket::Connect. ErrorNum=" + err);
    }
    return NO_ERROR;
}

int TCPSocket::Listen(int inBackLog)
{
    int err = listen(mSocket, inBackLog);
    if (err < 0)
    {
        std::string err = std::to_string(SocketUtil::GetLastError());
        throw std::logic_error("ERROR: TCPSocket::Listen. ErrorNum=" + err);
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
        std::string err = std::to_string(SocketUtil::GetLastError());
        throw std::logic_error("ERROR: TCPSocket::Accept. ErrorNum=" + err);
    }
}

int32_t TCPSocket::Send(const void* inData, size_t inLen)
{
    int bytesSentCount = send(mSocket, static_cast<const char*>(inData), inLen, 0);
    if (bytesSentCount < 0)
    {
        std::string err = std::to_string(SocketUtil::GetLastError());
        throw std::logic_error("ERROR: TCPSocket::Send. ErrorNum=" + err);
    }
    return bytesSentCount;
}

int32_t TCPSocket::Receive(void* inData, size_t inLen)
{
    int bytesReceivedCount = recv(mSocket, static_cast<char*>(inData), inLen, 0);
    if (bytesReceivedCount < 0)
    {
        std::string err = std::to_string(SocketUtil::GetLastError());
        throw std::logic_error("ERROR: TCPSocket::Receive. ErrorNum=" + err
            + "; bytesReceivedCount=" + std::to_string(bytesReceivedCount));
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

int TCPSocket::SetNonBlockingMode(bool inShouldBeNonBlocking)
{
#if _WIN32
    u_long arg = inShouldBeNonBlocking ? 1 : 0;
    int result = ioctlsocket(mSocket, FIONBIO, &arg);
#else
    int flags = fcntl(mSocket, F_GETFL, 0);
    flags = inShouldBeNonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
    int result = fcntl(mSocket, F_SETFL, flags);
#endif

    if (result == SOCKET_ERROR)
    {
        std::string err = std::to_string(SocketUtil::GetLastError());
        throw std::logic_error("ERROR: TCPSocket::SetNonBlockingMode. ErrorNum=" + err);
    }
    else
    {
        return NO_ERROR;
    }
}

int TCPSocket::Bind(const SocketAddress& inBindAddress)
{
    int error = bind(mSocket, &inBindAddress.mSockAddr, inBindAddress.GetSize());
    if (error != 0)
    {
        std::string err = std::to_string(SocketUtil::GetLastError());
        throw std::logic_error("ERROR: TCPSocket::Bind. ErrorNum=" + err);
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
