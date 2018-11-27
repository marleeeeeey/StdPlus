/*!
 * \author Tyulenev Sergey
 * Contact: marleeeeeey@gmail.com
 * last version https://github.com/marleeeeeey/StdPlus
*/

#pragma once
#include "StdInclude.h"

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
	#include <Windows.h>
#endif

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
		#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
			SYSTEMTIME st;
			GetSystemTime(&st); // WINDOWS ONLY

			Time t = { 0 };
			t.year = st.wYear;
			t.month = st.wMonth;
			t.day = st.wDay;
			t.hour = st.wHour;
			t.minute = st.wMinute;
			t.second = st.wSecond;
			t.millisecond = st.wMilliseconds;
	
		#elif (defined(__linux__))	
			std::time_t seconds = time(NULL);
			std::tm timeInfo{};
			#if defined(__unix__)
				localtime_r(&seconds, &timeInfo);
			#elif defined(_MSC_VER)
				localtime_s(&timeInfo, &seconds);
			#else
				static std::mutex mtx;
				std::lock_guard<std::mutex> lock(mtx);
				timeInfo = *std::localtime(&seconds);
			#endif

			Time t = { 0 };
			t.year = timeInfo.tm_year + 1900;
		t.month = timeInfo.tm_mon;
		t.day = timeInfo.tm_mday;
		t.hour = timeInfo.tm_hour;
		t.minute = timeInfo.tm_min;
		t.second = timeInfo.tm_sec;
		t.millisecond = timeInfo.tm_sec;
		#endif		
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

