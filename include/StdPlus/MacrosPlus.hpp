/*!
 * \author Tyulenev Sergey
 * Contact: marleeeeeey@gmail.com
 * last version https://github.com/marleeeeeey/StdPlus
*/

#pragma once

#include "StdInclude.h"
#include "TimePlus.hpp"
#include "OtherPlus.hpp"
#include "StringPlus.hpp"
#include "Ones.h"

// *********************** PREPARE FUNCTIONS ******************************

#if defined(__GNUC__) || defined(__GNUG__)
#  define AFUNSIG           stdplus::funSigPrepare(__PRETTY_FUNCTION__)
#elif defined(_MSC_VER)
#  define AFUNSIG           stdplus::funSigPrepare(__FUNCSIG__) 
#endif

#define AFILENAME         stdplus::fileNamePrepare(__FILE__) 
#define AFILELINE         stdplus::to_string(__LINE__)
#define APOSFILE          (AFILENAME + "(" + AFILELINE + ")")
#define AFUNPOS           AFUNSIG + " " + APOSFILE

#define LOG_STD_FUNSIG    AFUNSIG    
#define LOG_STD_FILENAME  AFILENAME  
#define LOG_STD_FILELINE  AFILELINE  
#define LOG_STD_POSFILE   APOSFILE   
#define LOG_STD_FUNPOS    AFUNPOS    


// ********************** DEBUG PRINT FUNCTIONs ***************************

//#    define ATHREAD       std::string()
#    define ATHREAD       stdplus::thisThreadIdString() + " "

//#    define ALOG_INDEX  std::string()
#    define ALOG_INDEX  stdplus::logCnt()
//#    define ALOG_INDEX  stdplus::timeStr()

#define LOG_STD_REGISTER_STREAM(var)   stdplus::oneOutputSystem().addOStream(var)
#define LOG_STD_UNREGISTER_STREAM(var) stdplus::oneOutputSystem().removeOstream(var)
#define LOG_STD_CLEAR_STREAM           stdplus::oneOutputSystem().clear()

#define SP_BEGIN { std::ostringstream os___; os___
#define SP_END   stdplus::oneOutputSystem().printString(os___.str()); } stdplus::oneOutputSystem() 

#ifndef STD_PLUS_DEBUG_PRINT_OFF
#    define AVAR(var)      SP_BEGIN << ALOG_INDEX << " VAR " << ATHREAD << #var << "=" << stdplus::to_string(var) << std::endl; SP_END
#    define AHEX(var)      SP_BEGIN << ALOG_INDEX << " HEX " << ATHREAD << #var << "=" << stdplus::to_string_hex(var) << std::endl; SP_END
#    define ANMD(var, str) SP_BEGIN << ALOG_INDEX << " NMD " << ATHREAD << str  << "=" << stdplus::to_string(var) << std::endl; SP_END
#    define AMSG(var)      SP_BEGIN << ALOG_INDEX << " MSG " << ATHREAD <<  (var) << std::endl; SP_END
#    define AFUN           SP_BEGIN << ALOG_INDEX << " FUN " << ATHREAD << AFUNSIG << std::endl; SP_END
#    define AFUN_COUNTER   static unsigned funCounter___ = 0; funCounter___++; SP_BEGIN << ALOG_INDEX << " AFUN_COUNTER <" << ATHREAD << std::setw(5) << funCounter___ << "> " << AFUNSIG << std::endl; SP_END
#    define ASPLIT         SP_BEGIN << "-------------------------------------------------------------------------------------\n"; SP_END
#    define ASPACE         SP_BEGIN << "\n"; SP_END
#    define ABEEP          stdplus::stdConsoleOut() << '\a'    //TODO
#else  // STD_PLUS_DEBUG_PRINT_OFF
#    define AVAR(var)     
#    define AHEX(var)     
#    define ANMD(var, str)
#    define AMSG(var)     
#    define AFUN          
#    define AFUN_COUNTER  
#    define ASPLIT        
#    define ASPACE        
#    define ABEEP         
#endif // STD_PLUS_DEBUG_PRINT_OFF

#define LOG_STD_VAR(var)        AVAR(var)      
#define LOG_STD_NMD(var, str)   ANMD(var, str) 
#define LOG_STD_MSG(var)        AMSG(var)      
#define LOG_STD_FUN             AFUN           
#define LOG_STD_FUN_COUNTER     AFUN_COUNTER   
#define LOG_STD_SPLIT           ASPLIT         
#define LOG_STD_SPACE           ASPACE         
#define LOG_STD_BEEP            ABEEP          


// ************************* LOGIC FUNCTIONs ******************************

#define APAUSE_MSG(var) SP_BEGIN << ALOG_INDEX << " PSE " << ATHREAD << var << std::endl; SP_END; std::cin.get()
#define APAUSE          APAUSE_MSG("(press any key for continue)")
#define ARED(val)       stdplus::consoleRead(val, #val)

#define LOG_STD_PAUSE_MSG(var)  APAUSE_MSG(var) 
#define LOG_STD_PAUSE           APAUSE          
#define LOG_STD_RED(val)        ARED(val)       


// ********************* EASY EXCEPTION AND ERROR *************************

#define ADEBUGMSG(var)   "[ " + stdplus::to_string(#var) + " ] in file " + APOSFILE + AFUNSIG
#define AEXCEPT          stdplus::throwExcept(AFUNPOS)
#define AEXCEPT_IF(var)  if(var) stdplus::throwExcept(ADEBUGMSG(var))
#define AERR_IF(var)     if(var) stdplus::throwError (ADEBUGMSG(var))

#define LOG_STD_DEBUGMSG(var)    ADEBUGMSG(var)  
#define LOG_STD_EXCEPT           AEXCEPT         
#define LOG_STD_EXCEPT_IF(var)   AEXCEPT_IF(var) 
#define LOG_STD_ERR_IF(var)      AERR_IF(var)    


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
        //std::cerr << dateTimeStr() << prefix << errormessage << "\n" << std::endl;
        throw std::logic_error(errormessage);
    }

    inline void throwError(const std::string & errormessage = "unknown")
    {
        const std::string prefix = " RT_ERROR: ";
        AMSG(prefix + errormessage);
        //std::cerr << dateTimeStr() << prefix << errormessage << "\n" << std::endl;
    }

    template<typename T>
    inline void consoleRead(T & val, const std::string & msg)
    {

        stdplus::stdConsoleOut()
            << ALOG_INDEX << " INP "
            << msg << "[" << val << "]" << ":";

        char ch = std::cin.peek();
        if (ch != '\n')
            std::cin >> val;
        
        std::cin.get();

    }

}
