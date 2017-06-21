/*!
* \author Tyulenev Sergey
* Contact: marleeeeeey@gmail.com
* last version https://github.com/marleeeeeey/StdPlus
*/

#pragma once

#include "StdInclude.h"



namespace stdplus
{
    class SimpleCmdParser
    {
    public:
        SimpleCmdParser()
        {

        }

        inline void parseData(int argc, char** argv)
        {
            for (int i = 0; i < argc; ++i)
            {
                processSeparateData(argv[i]);
            }
        }

        template<typename T>
        T getValue(const std::string & key)
        {
            std::string strValue = m_keyValues.at(key);
            return stdplus::to<T>(strValue);
        }

        template<>
        bool getValue<bool>(const std::string & key)
        {
            std::string strValue = m_keyValues.at(key);
            strValue = stdplus::trim(strValue);
            strValue = stdplus::tolower(strValue);

            if (strValue == "true")
                return true;

            try
            {
                int intValue = stdplus::to<int>(strValue);
                if (intValue != 0)
                    return true;
            }
            catch (std::logic_error & e)
            {
                return false;
            }

            return false;
        }

        template<typename T>
        T getValue(const std::string & key, const T & defaultValue)
        {
            try
            {
                return getValue<T>(key);
            }
            catch (std::out_of_range &)
            {
                return defaultValue;
            }
        }

        inline bool isExistKey(const std::string & key)
        {
            auto itKeyValue = m_keyValues.find(key);

            if (itKeyValue != m_keyValues.end())
                return true;

            return false;
        }

        inline bool isExistIdx(const std::string & indexedValue)
        {
            auto itIndexedValue = std::find(m_indexedValue.begin(), m_indexedValue.end(), indexedValue);

            if (itIndexedValue != m_indexedValue.end())
                return true;

            return false;
        }

        inline const std::vector<std::string> & indexedValues()
        {
            return m_indexedValue;
        }

        inline void print(std::ostream & os) const
        {
            if (!m_indexedValue.empty())
            {
                os << "indexes: \n";
                int i = 0;
                for (auto & v : m_indexedValue)
                    os << i++ << ": " << v << "\n";
            }
            
            if (!m_keyValues.empty())
            {
                os << "keys: \n";
                for (auto & kv : m_keyValues)
                    os << kv.first << ": " << kv.second << "\n";
            }
        }

    private:
        
        inline void processSeparateData(const std::string separateData)
        {
            std::vector<std::string> splits = stdplus::split(separateData, SPLITTER);

            if (splits.size() > 2)
            {
                throw std::logic_error("Too much assing in " + separateData);
            }
            else if (splits.size() == 1 && splits[0] == separateData)
            {
                m_indexedValue.push_back(separateData);
            }
            else if (splits.size() == 2)
            {
                if (isExistKey(splits[0]))
                    throw std::logic_error("Try insert existing element " + splits[0]);

                m_keyValues[splits[0]] = splits[1];
            }
        }

        std::map<std::string, std::string> m_keyValues;
        std::vector<std::string>           m_indexedValue;
        
        const char SPLITTER = '=';
    };
}