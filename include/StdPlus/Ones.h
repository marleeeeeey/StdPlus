/*!
* \author Tyulenev Sergey
* Contact: marleeeeeey@gmail.com
* last version https://github.com/marleeeeeey/StdPlus
*/

#pragma once
#include "OutputSystemPlus.hpp"


#if defined(STD_PLUS_DLL)    
#  if defined(STD_PLUS_EXPORT)
#    define STD_PLUS_API __declspec(dllexport)
#  else
#    define STD_PLUS_API __declspec(dllimport)  
#  endif

namespace stdplus
{
    STD_PLUS_API OutputSystem & oneOutputSystem();
}

#else    

namespace stdplus
{
    inline OutputSystem & oneOutputSystem()
    {
        static OutputSystem os;
        return os;
    }
}


#  define STD_PLUS_API_EXPORT
#  define STD_PLUS_API_IMPORT
#endif


