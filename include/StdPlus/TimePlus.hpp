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

#ifndef WindowsPlus_h__

	inline std::string timeStr()
	{
		return "00-00-00-000";
	}

#endif

    inline std::string dateTimeStr()
    {
        return dateStr() + " " + timeStr();
    }

}

#endif // TimePlus_h__
