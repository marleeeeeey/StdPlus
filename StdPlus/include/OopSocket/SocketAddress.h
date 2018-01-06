#pragma once

#include "OopSocketShared.h" 

class SocketAddress
{
public:
    SocketAddress(std::string ipStr, uint16_t port);

    SocketAddress(uint32_t inAddress, uint16_t inPort);

    SocketAddress(const sockaddr& inSockAddr);

    SocketAddress();

    bool operator==(const SocketAddress& inOther) const;

    size_t GetHash() const;

    uint32_t GetSize() const;

    std::string ToString() const;

private:
    friend class UDPSocket;
    friend class TCPSocket;

    sockaddr mSockAddr;
#if _WIN32
    uint32_t& GetIP4Ref() { return *reinterpret_cast<uint32_t*>(&GetAsSockAddrIn()->sin_addr.S_un.S_addr); }
    const uint32_t& GetIP4Ref() const { return *reinterpret_cast<const uint32_t*>(&GetAsSockAddrIn()->sin_addr.S_un.S_addr); }
#else
    uint32_t& GetIP4Ref() { return GetAsSockAddrIn()->sin_addr.s_addr; }
    const uint32_t& GetIP4Ref() const { return GetAsSockAddrIn()->sin_addr.s_addr; }
#endif

    sockaddr_in* GetAsSockAddrIn();
    const sockaddr_in* GetAsSockAddrIn() const;

};

typedef std::shared_ptr< SocketAddress > SocketAddressPtr;

std::ostream & operator<<(std::ostream & os, const SocketAddress & addr);

namespace std
{
    template<> struct hash< SocketAddress >
    {
        size_t operator()(const SocketAddress& inAddress) const
        {
            return inAddress.GetHash();
        }
    };
}