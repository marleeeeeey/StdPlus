/*!
 * \author Tyulenev Sergey
 * Contact: marleeeeeey@gmail.com
 * last version https://github.com/marleeeeeey/StdPlus
*/


#pragma once


#include "StdInclude.h"
#include <windows.h>        // GetSystemTime(...)


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


    inline int getCurrentMS()
    {
        SYSTEMTIME st;
        // WINDOWS ONLY
        GetSystemTime(&st);

        return st.wMilliseconds;
    }


    inline std::string timeStr()
    {
        enum { stringSize = 12 }; // TODO Why 12? 
        char cString[stringSize];

        tm timeInfo = curTimeInfo();

        strftime(cString, stringSize, "%T", &timeInfo);
        std::string retStr(cString);

        std::replace(retStr.begin(), retStr.end(), ':', '-');
        
        retStr += "-" + std::to_string(getCurrentMS());

        return retStr;
    }

    inline std::string dateTimeStr()
    {
        return dateStr() + " " + timeStr();
    }

    inline std::string getLogFileName(const std::string & mainName)
    {
        return stdplus::dateTimeStr() + " " + mainName;
    }

}

