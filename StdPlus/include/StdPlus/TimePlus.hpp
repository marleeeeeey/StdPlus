/*!
 * \author Tyulenev Sergey
 * Contact: marleeeeeey@gmail.com
 * last version https://github.com/marleeeeeey/StdPlus
*/


#pragma once
#include "StdInclude.h"

namespace stdplus
{
    struct Time
    {
        int year;
        int month;
        int day;
        int hour;
        int minute;
        int second;
        int millisecond;
    };
}

inline std::ostream & operator<<(std::ostream & os, const stdplus::Time & d)
{
    os
        << std::setw(4) << std::setfill('0') << d.year        << "-"
        << std::setw(2) << std::setfill('0') << d.month       << "-"
        << std::setw(2) << std::setfill('0') << d.day         << " "
        << std::setw(2) << std::setfill('0') << d.hour        << "-"
        << std::setw(2) << std::setfill('0') << d.minute      << "-"
        << std::setw(2) << std::setfill('0') << d.second      << "-"
        << std::setw(3) << std::setfill('0') << d.millisecond 
        ;

    return os;
}

namespace stdplus
{
    inline Time getCurrentTime()
    {
		time_t seconds = time(NULL);
		tm* timeInfo = localtime(&seconds);

		Time t = { 0 };
        t.year = timeInfo->tm_year + 1900;
        t.month = timeInfo->tm_mon;
        t.day = timeInfo->tm_mday;
        t.hour = timeInfo->tm_hour;
        t.minute = timeInfo->tm_min;
        t.second = timeInfo->tm_sec;
		
		t.millisecond = timeInfo->tm_sec*1000; // ctime doesnt have milliseconds TODO it Later
		
        return t;
    }

    inline std::string dateStr(const char splitter = '-')
    {
        Time t = getCurrentTime();

        std::ostringstream ss;
        ss 
            << std::setw(4) << std::setfill('0') << t.year  << splitter 
            << std::setw(2) << std::setfill('0') << t.month << splitter 
            << std::setw(2) << std::setfill('0') << t.day
            ;

        return ss.str();
    }
       
    inline std::string timeStr(const char splitter = '-')
    {
        Time t = getCurrentTime();

        std::ostringstream ss;
        ss 
            << std::setw(2) << std::setfill('0') << t.hour   << splitter 
            << std::setw(2) << std::setfill('0') << t.minute << splitter
            << std::setw(2) << std::setfill('0') << t.second << splitter
            << std::setw(3) << std::setfill('0') << t.millisecond
            ;

        return ss.str();
    }

    inline std::string dateTimeStr(const char dateSplitter = '-', const char timeSplitter = '-')
    {
        return dateStr(dateSplitter) + " " + timeStr(timeSplitter);
    }

    inline std::string getLogFileName(const std::string & mainName)
    {
        return stdplus::dateTimeStr() + " " + mainName + " log.txt";
    }
}

