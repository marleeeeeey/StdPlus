/*!
* \author Tyulenev Sergey
* Contact: marleeeeeey@gmail.com
* last version https://github.com/marleeeeeey/StdPlus
*/

#pragma once
#include "OutputSystemPlus.hpp"


#if defined(STD_PLUS_DLL)    
//#  pragma message("USED STD_PLUS_DLL")
#  if defined(STD_PLUS_EXPORT)
//#    pragma message("USED EXPORT API")
#    define STD_PLUS_API __declspec(dllexport)
#  else
//#    pragma message("USED IMPORT API")
#    pragma comment(lib, "StdPlus.lib")
#    define STD_PLUS_API __declspec(dllimport)  
#  endif


namespace stdplus
{
    STD_PLUS_API OutputSystem & oneOutputSystem();
}

#else    
//#  pragma message("USED STD_PLUS_INCLUDE_MODE")

namespace stdplus
{
    inline OutputSystem & oneOutputSystem()
    {
        static OutputSystem os;
        return os;
    }
}

#endif


