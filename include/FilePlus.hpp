#ifndef FilePlus_h__
#define FilePlus_h__

#include "StdInclude.h"
#include "StringPlus.hpp"

namespace stdplus
{
    inline bool file_exist(const std::string& name)
    {
        std::ifstream f(name.c_str());
        return f.good();
    }


    inline void createFolder(const std::string & path)
    {
        _mkdir(path.c_str());
    }

    template<typename T>
    inline void writeVecToStream(std::ostream & os, const std::vector<T> vec, 
        bool isReplaceDotToComma = true, const std::string sep = "\t")
    {
        for_each(vec.begin(), vec.end()-1,
            [&](const T & d)
        {
            std::string str = to_string(d);
            if (isReplaceDotToComma)
                std::replace(str.begin(), str.end(), '.', ',');            
            os << str << sep;
        });
        
        std::string str = to_string(vec.back());
        if (isReplaceDotToComma)
            std::replace(str.begin(), str.end(), '.', ',');

        os << str;
    }

    template<typename T>
    inline void appendVecToFile(const std::string & fileName, const std::vector<T> vec, const std::string & lineName = "NoName",
        bool isReplaceDotToComma = true, const std::string sep = "\t")
    {
        std::ofstream ost(fileName, std::ios_base::app);
        ost << lineName << sep;
        writeVecToStream(ost, vec, isReplaceDotToComma, sep);
        ost << std::endl;
    }


}

#endif // FilePlus_h__
