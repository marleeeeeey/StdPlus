/*!
* \author Tyulenev Sergey
* Contact: marleeeeeey@gmail.com
* last version https://github.com/marleeeeeey/StdPlus
*/

#pragma once

#include "StdInclude.h"
#include "StringPlus.hpp"
#include "TestedPlus.hpp"

namespace stdplus
{
    class CmdParser
    {
        friend class Settings;

    public:

        CmdParser()
        {
            
        }
        
        inline char getSplitter() const { return m_splitter; }
        inline void setSplitter(char ch) { m_splitter = ch; }

        inline void clearData()
        {
            m_keyValues.clear();
            m_indexedValue.clear();
        }

        inline void parseData(const std::vector<std::string> & lines)
        {
            clearData();

            for (auto & line : lines)
            {
                processSeparateData(line);
            }
        }

        inline void parseData(int argc, char** argv)
        {
            clearData();

            for (int i = 0; i < argc; ++i)
            {
                processSeparateData(argv[i]);
            }
        }

        template<typename T>
        T getValue(const std::string & key)
        {
            std::string strValue = getStrValueByKey(key);
            return stdplus::to<T>(strValue);
        }

        template<typename T>
        T getValue(const std::string & key, const T & defaultValue)
        {
            try
            {
                return getValue<T>(key);
            }
            catch (std::exception &)
            {
                return defaultValue;
            }
        }
        
        template<typename T>
        inline void setValue(const std::string & key, const T & value)
        {
            m_keyValues[key] = stdplus::to_string(value);
        }

        template<typename T>
        inline void updateValue(const std::string & key, const T & value)
        {
            m_keyValues.at(key) = stdplus::to_string(value);
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
            m_requests.push_back(indexedValue);

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

        std::vector<std::string> getRequests() const { return m_requests; }

    private:
        
        // ******************* FOR FRIENDS *******************

        using Store = std::map<std::string, std::string>;

        inline Store & getRawKeyValues() { return m_keyValues; }

        inline const char getSplitter() { return m_splitter; }        

        // ***************************************************


        inline std::string getStrValueByKey(const std::string & key)
        {
            m_requests.push_back(key + m_splitter + "...");

            try
            {
                std::string strValue = m_keyValues.at(key);
                return strValue;
            }
            catch (std::out_of_range & e)  
            {
                throw std::logic_error("Not found key \'" + key + "\'. " + e.what());
            }
        }

        inline void processSeparateData(const std::string separateData)
        {
            std::vector<std::string> splits = stdplus::split(separateData, m_splitter);

            if (splits.size() > 2)
            {
                throw std::logic_error("Too much assign in " + separateData);
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

        Store                    m_keyValues;
        std::vector<std::string> m_indexedValue;        
        char                     m_splitter = '=';
        std::vector<std::string> m_requests;
    };
	

    template<>
        inline bool CmdParser::getValue<bool>(const std::string & key, const bool & defaultValue)
        {
            if (isExistKey(key))
            {
                std::string strValue = getStrValueByKey(key);
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
                catch (std::logic_error &)
                {
                    return defaultValue;
                }
        
                return defaultValue;
            }
            else
            {
                return isExistIdx(key);
            }
        
        }
	
	template<>
	inline bool CmdParser::getValue<bool>(const std::string & key)
	{
		std::string strValue = getStrValueByKey(key);
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
		catch (std::logic_error & )
		{
			return false;
		}

		return false;
	}

}
