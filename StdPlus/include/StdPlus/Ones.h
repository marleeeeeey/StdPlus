/*!
* \author Tyulenev Sergey
* Contact: marleeeeeey@gmail.com
* last version https://github.com/marleeeeeey/StdPlus
*/

#pragma once
#include "OutputSystemPlus.hpp"

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
	#define STD_PLUS_WIN32
#elif (defined(__linux__))
	#define STD_PLUS_LINUX
#endif

#if defined(STD_PLUS_DLL)    
	#if (defined(STD_PLUS_WIN32))
		#ifdef STD_PLUS_EXPORT
			#define STD_PLUS_API __declspec(dllexport)
		#else 
			#pragma comment(lib, "StdPlus.lib")
			#define STD_PLUS_API __declspec(dllimport)  
		#endif 
	#elif (defined(STD_PLUS_LINUX))
		#define STD_PLUS_API
	#endif 

	namespace stdplus
	{
		STD_PLUS_API OutputSystem & oneOutputSystem();
	}
#else    
	namespace stdplus
	{
		inline OutputSystem & oneOutputSystem()
		{
			static OutputSystem os;
			return os;
		}
	}
#endif


