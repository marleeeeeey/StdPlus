#ifndef TimePlus_h__
#define TimePlus_h__


#include "StdInclude.h"


namespace stdplus
{
    inline tm curTimeInfo()
    {
        time_t t = ::time(0);   // get time now
        tm timeInfo;
        localtime_s(&timeInfo, &t);

        return timeInfo;
    }

    inline std::string dateStr()
    {
        enum { stringSize = 12 }; // TODO Why 12? (10+\0)=11
        char cString[stringSize];
        strftime(cString, stringSize, "%Y-%m-%d", &curTimeInfo());
        std::string retStr(cString);
        return retStr;
    }

#ifndef WindowsPlus_h__

    inline std::string timeStr()
    {
        enum { stringSize = 12 }; // TODO Why 12? 
        char cString[stringSize];
        strftime(cString, stringSize, "%T", &curTimeInfo());
        std::string retStr(cString);
        return retStr;
    }

#endif

    inline std::string dateTimeStr()
    {
        return dateStr() + " " + timeStr();
    }

}

#endif // TimePlus_h__
