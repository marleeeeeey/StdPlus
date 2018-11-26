#include "StdPlus/Ones.h"


#if defined(STD_PLUS_DLL) 

namespace stdplus
{
    OutputSystem & oneOutputSystem()
    {
        static OutputSystem os;
        return os;
    }
}

#endif
