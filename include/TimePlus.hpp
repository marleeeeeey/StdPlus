#ifndef TimePlus_h__
#define TimePlus_h__


#include "StdInclude.h"


namespace stdplus
{
    inline std::string dateStr()
    {
        time_t t = ::time(0);   // get time now
        tm timeInfo;
        localtime_s(&timeInfo, &t);

        enum { stringSize = 12 };
        char cString[stringSize];
        strftime(cString, stringSize, "%Y-%m-%d", &timeInfo);

        std::string retStr(cString);

        return retStr;
    }

    inline std::string timeStr()
    {
        using std::setw;
        using std::setfill;

        SYSTEMTIME st;
        GetSystemTime(&st);

        std::ostringstream os;
        os
            << setw(2) << setfill('0') << st.wHour << "-"
            << setw(2) << setfill('0') << st.wMinute << "-"
            << setw(2) << setfill('0') << st.wSecond << "-"
            << setw(3) << setfill('0') << st.wMilliseconds //<< " "
            ;

        return os.str();
    }

    inline std::string dateTimeStr()
    {
        return dateStr() + " " + timeStr();
    }

}

#endif // TimePlus_h__
