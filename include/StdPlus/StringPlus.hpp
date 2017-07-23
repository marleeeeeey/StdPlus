/*!
 * \author Tyulenev Sergey
 * Contact: marleeeeeey@gmail.com
 * last version https://github.com/marleeeeeey/StdPlus
*/


#pragma once

#include "StdInclude.h"
#include "ConvertPlus.hpp"


namespace stdplus
{
    inline bool isWordSplitter(const char ch)
    {
        if (isalpha(ch) || ch == '\'')
            return false;

        return true;
    }


    inline void replaceWordAtAll(std::string & source, const std::string & oldWord, const std::string & newValue)
    {
        size_t index = 0;

        while ((index = source.find(oldWord, index)) != std::string::npos)
        {
            size_t charAfterWordIndex = index + oldWord.size();
            if (charAfterWordIndex >= source.size())
                return;

            char charAfterWord = source.at(charAfterWordIndex);
            if (!isWordSplitter(charAfterWord))
            {
                index = charAfterWordIndex;
                continue;
            }


            size_t charBeforeWordIndex = index - 1;
            if (charBeforeWordIndex > 0)
            {
                char charBeforeWord = source.at(charBeforeWordIndex);
                if (!isWordSplitter(charBeforeWord))
                {
                    index = charAfterWordIndex;
                    continue;
                }
            }


            source.replace(index, oldWord.size(), newValue);
            index += newValue.size();
        };
    }


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

    inline std::string fileNameFromFullPath(std::string fileName)
    {
        return split(fileName, '\\').back();
    }

    // TODO - delete this
    inline std::string fileNamePrepare(std::string fileName)
    {
        return split(fileName, '\\').back();
    }


    // TODO
//     inline std::string onlyPathFromFullPath(std::string fileName)
//     {
//         return split(fileName, '\\').back();
//     }


}

