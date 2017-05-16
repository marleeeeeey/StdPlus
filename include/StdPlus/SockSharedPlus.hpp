#pragma once

#include <map>
#include <ostream>
#include <string>
#define SHUT_RDWR 2
typedef int socklen_t;

namespace stdplus
{
    enum SocketStateEnum
    {
        SL_SUCCESS                = 0,
        SL_ERROR_INIT             = -1,
        SL_ERROR_SOCKET           = -2,
        SL_ERROR_ADDR             = -3,
        SL_ERROR_BIND             = -4,
        SL_ERROR_LISTEN           = -5,
        SL_ERROR_RESOLVE          = -6,
        SL_ERROR_CONNECT          = -7,
        SL_ERROR_ACCEPT           = -8,
        SL_ERROR_POOL             = -9,
        SL_ERROR_SELECT           = -10,
        SL_ERROR_READ             = -11,
        SL_ERROR_WRITE            = -12,
        SL_ERROR_DISCONNECT       = -13,
        SL_ERROR_ALREADY_INIT     = -14,
        SL_ERROR_NOTINIT          = -15,
        SL_ERROR_NOT_BLOKING_INIT = -16,
        SL_TIMEOUT                = -17,
        SL_NUM_ERRORS             = 18,
    };
    
}


inline std::ostream & operator<<(std::ostream & os, const stdplus::SocketStateEnum & sockState)
{
    using stdplus::SocketStateEnum;

    static std::map<SocketStateEnum, std::string> strings = 
    {
        { SocketStateEnum::SL_SUCCESS                , "no error"                        },
        { SocketStateEnum::SL_ERROR_INIT             , "initialize error"                },
        { SocketStateEnum::SL_ERROR_SOCKET           , "socket() error"                  },
        { SocketStateEnum::SL_ERROR_ADDR             , "inet_aton() error"               },
        { SocketStateEnum::SL_ERROR_BIND             , "bind() error"                    },
        { SocketStateEnum::SL_ERROR_LISTEN           , "listen() error"                  },
        { SocketStateEnum::SL_ERROR_RESOLVE          , "gethostbyname() error"           },
        { SocketStateEnum::SL_ERROR_CONNECT          , "connect() error"                 },
        { SocketStateEnum::SL_ERROR_ACCEPT           , "accept() error"                  },
        { SocketStateEnum::SL_ERROR_POOL             , "pool() error"                    },
        { SocketStateEnum::SL_ERROR_SELECT           , "select() error"                  },
        { SocketStateEnum::SL_ERROR_READ             , "read error"                      },
        { SocketStateEnum::SL_ERROR_WRITE            , "write error"                     },
        { SocketStateEnum::SL_ERROR_DISCONNECT       , "disconnect error"                },
        { SocketStateEnum::SL_ERROR_ALREADY_INIT     , "socklib already initialized"     },
        { SocketStateEnum::SL_ERROR_NOTINIT          , "socklib not initialized"         },
        { SocketStateEnum::SL_ERROR_NOT_BLOKING_INIT , "socklib not blocking init error" },
        { SocketStateEnum::SL_TIMEOUT                , "timeout"                         },
        { SocketStateEnum::SL_NUM_ERRORS             , "max number errors"               },
    };

    os << strings.at(sockState);

    return os;
}
