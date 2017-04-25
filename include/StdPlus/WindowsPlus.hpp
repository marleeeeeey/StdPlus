#ifndef WindowsPlus_h__
#define WindowsPlus_h__


#include "StdInclude.h"
#include <Windows.h>    // SYSTEMTIME
#include <direct.h>     // mkdir


namespace stdplus
{
	inline void createFolder(const std::string & path)
	{
		_mkdir(path.c_str());
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

}

#endif // WindowsPlus_h__
