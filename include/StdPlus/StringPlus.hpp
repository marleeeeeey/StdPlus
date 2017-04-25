#ifndef StringPlus_h__
#define StringPlus_h__

#include "StdInclude.h"

namespace stdplus
{

    inline std::vector<std::string> split(const std::string &s, char delim)
    {
        std::stringstream ss(s);
        std::string item;
        std::vector<std::string> tokens;
        while (getline(ss, item, delim))
        {
            tokens.push_back(item);
        }
        return tokens;
    }


    inline std::vector<std::string> splitByLenght(std::string str, size_t line_sz)
    {
        if (str.size() <= line_sz)
            return{ str };

        std::vector<std::string> result;

        for (size_t i = 0; i < str.size() - line_sz; i += line_sz)
        {
            result.push_back(str.substr(i, line_sz));
        }

        return result;
    }


    // trim from left
    inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
    {
        s.erase(0, s.find_first_not_of(t));
        return s;
    }

    // trim from right
    inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
    {
        s.erase(s.find_last_not_of(t) + 1);
        return s;
    }

    // trim from left & right
    inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v")
    {
        return ltrim(rtrim(s, t), t);
    }


    template<class T>
    std::string to_string(const T& t)
    {
        std::ostringstream os;
        os << t;
        return os.str();
    }

    inline int to_int(const std::string & str)
    {
        int val;
        std::istringstream ss(str);
        ss >> val;

        return val;
    }

    template<>
    inline std::string to_string(const bool& t)
    {
        if (t == true)
            return "true";

        return "false";
    }


    inline std::string removeFromString(
        const std::string & source, const std::string remString)
    {
        std::size_t foundPos = source.find(remString);

        if (foundPos == std::string::npos)
            return source;

        std::string ret = source;
        ret.erase(foundPos, remString.length());
        return ret;
    }


    inline std::string removeFromString(const std::string source,
        const std::initializer_list<const char *> & remStrings)
    {
        std::string ret = source;

        for (auto & remStr : remStrings)
            ret = removeFromString(ret, to_string(remStr));

        return ret;
    }


    inline std::string removeFromString(const std::string source,
        const std::vector<std::string> & remStrings)
    {
        std::string ret = source;

        for (auto & remStr : remStrings)
            ret = removeFromString(ret, to_string(remStr));

        return ret;
    }

    // ****************** Special Functions ******************

    inline std::string funSigPrepare(std::string fs)
    {
        return removeFromString(fs,
        { "__thiscall ", "__cdecl " });
    }

    inline std::string fileNamePrepare(std::string fileName)
    {
        return split(fileName, '\\').back();
    }

}


#endif // StringPlus_h__
