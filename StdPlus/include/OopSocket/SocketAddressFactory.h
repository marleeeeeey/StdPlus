#pragma once
#pragma message("DON'T USE OBSOLETE SocketAddressFactory")

#include "OopSocketShared.h" 
#include "SocketAddress.h"

class SocketAddressFactory
{
public:
    static SocketAddressPtr CreateIPv4FromString(const std::string & ipAndPort);
    static SocketAddressPtr CreateIPv4FromString(const std::string & ip, unsigned port);
};