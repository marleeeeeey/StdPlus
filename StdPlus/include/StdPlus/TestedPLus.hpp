/*!
 * \author Tyulenev Sergey
 * Contact: marleeeeeey@gmail.com
 * last version https://github.com/marleeeeeey/StdPlus
*/


#pragma once

#include "StdInclude.h"



namespace stdplus
{
	inline std::string readBlock(const char & chOpen, const char & chClose, std::istringstream & is)
	{		
		
		char ch;
		while (is.get(ch))
		{
			if (ch == chOpen)
			{
				std::string block;
				int openDelimiter = 0;
				int closeDelimiter = 0;
				openDelimiter++;
				block += ch;
				while (openDelimiter != closeDelimiter && is.get(ch))
				{
					if (ch == chOpen)
						openDelimiter++;
					else if (ch == chClose)
						closeDelimiter++;
					block += ch;
				}
				return block;
			}		
		}
		return "Error, the specified delimiter was not found";
	}

/*    // MARLEEEEEEY check reference return!!!
    inline std::ifstream openTextFile(const std::string & fName)
    {
        std::ifstream  f;
        f.open(fName);
        if (!f.is_open())
        {
            std::cout << "Could not open the file for read" << std::endl;
            std::cin.get();
        }
        return f;
    }
*/
    inline std::string readText(std::ifstream & ifs)
    {
        std::string retString;

        char ch;
        while (ifs.get(ch))
            retString += ch;

        return retString;
    }

    inline std::string readText(const std::string & fileName)
    {
        std::ifstream ifs(fileName);

        std::string retString;

        char ch;
        while (ifs.get(ch))
            retString += ch;

        return retString;
    }

    inline std::string tolower(const std::string & orig)
    {
        std::string ret;
        ret.reserve(orig.size());
        for_each(orig.begin(), orig.end(),
            [&ret](const char & ch)
        {
            ret.push_back(::tolower(ch));
        });

        return ret;
    }

	inline std::vector<std::string> fillingVectorFromCMD(int mainArgc, char *mainArgv[])
	{
		std::vector<std::string> allNames;
		for (int i = 1; i < mainArgc; i++)
		{
			allNames.push_back(mainArgv[i]);
		}
		return allNames;
	}
	
}
