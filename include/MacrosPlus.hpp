#ifndef MacrosPlus_h__
#define MacrosPlus_h__

#include "StdInclude.h"
#include "TimePlus.hpp"


// ********************* PRINT FUNCTION ***********************************

#define AFUNSIG       funSigPrepare(__FUNCSIG__) 
#define AFILENAME     fileNamePrepare(__FILE__) 
#define AFILELINE     to_string(__LINE__)
#define APOSFILE      (AFILENAME + "(" + AFILELINE + ")")
#define APAUSE        std::cout << logCnt() << " PAUSE (press any key for continue)"; std::cin.get()
#define AVAR(var)     std::cout << logCnt() << " VAR " << #var << "=" << to_string(var) << std::endl
#define AMSG(var)     std::cout << logCnt() << " MSG " <<  (var) << std::endl
#define AFUN          std::cout << logCnt() << " FUN " << AFUNSIG << std::endl
#define ASPLIT        std::cout << "-------------------------------------------------------------------------------------\n"
#define ASPACE        std::cout << "\n"
#define ABEEP         std::cout << '\a';



// ********************* EASY EXCEPTION AND ERROR *************************

#define ADEBUGMSG(var)   "[ " + to_string(#var) + " ] in file " + APOSFILE + AFUNSIG
#define AEXCEPT_IF(var)  if(var) throwExcept(ADEBUGMSG(var))
#define AERR_IF(var)     if(var) throwError (ADEBUGMSG(var))


namespace stdplus
{

    inline unsigned int logCnt()
    {
        enum
        {
            e_begin = 100,
            e_end = 999,
        };

        static unsigned counter = e_begin;
        if (++counter >= e_end)
        {
            counter = e_begin;
        }

        return counter;
    }

    inline void throwExcept(const std::string & errormessage = "unknown")
    {
        const std::string prefix = " EXCEPT: ";
        AMSG(prefix + errormessage);
        std::cerr << dateTimeStr() << prefix << errormessage << "\n" << std::endl;
        throw std::logic_error(errormessage);
    }

    inline void throwError(const std::string & errormessage = "unknown")
    {
        const std::string prefix = " RT_ERROR: ";
        AMSG(prefix + errormessage);
        std::cerr << dateTimeStr() << prefix << errormessage << "\n" << std::endl;
    }

}




#endif // MacrosPlus_h__
