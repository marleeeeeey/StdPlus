#include "OopSocket/SocketAddress.h"

sockaddr getSockAddrIPv4FromString(const std::string & host, uint16_t port)
{
    std::string service = std::to_string(port);

    addrinfo hint;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;

    addrinfo* result;
    int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);
    if (error != 0 && result != nullptr)
    {
        std::cerr << "Error: getSockAddrIPv4FromString";
        return { 0 };
    }

    while (!result->ai_addr && result->ai_next)
        result = result->ai_next;

    if (!result->ai_addr)
        return { 0 };
    
    sockaddr toRet = *result->ai_addr;

    freeaddrinfo(result);

    return toRet;
}

sockaddr getSockAddrIPv4FromString(const std::string & ipAndPort)
{
    auto pos = ipAndPort.find_last_of(':');
    std::string host;
    uint16_t service;
    if (pos != std::string::npos)
    {
        host = ipAndPort.substr(0, pos);
        std::string strService = ipAndPort.substr(pos + 1);
        std::istringstream iss (strService);
        iss >> service;
    }
    else
    {
        host = ipAndPort;        
        service = 0; //use default port...
    }

    return getSockAddrIPv4FromString(ipAndPort, service);
}


SocketAddress::SocketAddress(uint32_t inAddress, uint16_t inPort)
{
    GetAsSockAddrIn()->sin_family = AF_INET;
    GetIP4Ref() = htonl(inAddress);
    GetAsSockAddrIn()->sin_port = htons(inPort);
}


SocketAddress::SocketAddress(const sockaddr& inSockAddr)
{
    memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
}


SocketAddress::SocketAddress()
{
    GetAsSockAddrIn()->sin_family = AF_INET;
    GetIP4Ref() = INADDR_ANY;
    GetAsSockAddrIn()->sin_port = 0;
}


SocketAddress::SocketAddress(std::string ipStr, uint16_t port)
{
    mSockAddr = getSockAddrIPv4FromString(ipStr, port);
}

size_t SocketAddress::GetHash() const
{
    return (GetIP4Ref()) |
        ((static_cast<uint32_t>(GetAsSockAddrIn()->sin_port)) << 13) |
        mSockAddr.sa_family;
}


uint32_t SocketAddress::GetSize() const
{
    return sizeof(sockaddr);
}

bool SocketAddress::operator==(const SocketAddress& inOther) const
{
    return (mSockAddr.sa_family == AF_INET &&
        GetAsSockAddrIn()->sin_port == inOther.GetAsSockAddrIn()->sin_port) &&
        (GetIP4Ref() == inOther.GetIP4Ref());
}

std::string SocketAddress::ToString() const
{
#if _WIN32
    const sockaddr_in* s = GetAsSockAddrIn();
    char destinationBuffer[128];

    InetNtop(s->sin_family, const_cast<in_addr*>(&s->sin_addr), 
        destinationBuffer, sizeof(destinationBuffer));
    
    std::ostringstream os;
    os
        << destinationBuffer
        << ":"
        << ntohs(s->sin_port)
        ;
    
    return os.str();

#else
    //not implement on mac for now...
    return string("not implemented on mac for now");
#endif
}


sockaddr_in* SocketAddress::GetAsSockAddrIn()
{
    return reinterpret_cast<sockaddr_in*>(&mSockAddr);
}


const sockaddr_in* SocketAddress::GetAsSockAddrIn() const
{
    return reinterpret_cast<const sockaddr_in*>(&mSockAddr);
}

std::ostream & operator<<(std::ostream & os, const SocketAddress & addr)
{
    os << addr.ToString();
    return os;
}
