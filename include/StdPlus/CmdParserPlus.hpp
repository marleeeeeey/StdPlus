/*!
 * \author Tyulenev Sergey
 * Contact: marleeeeeey@gmail.com
 * last version https://github.com/marleeeeeey/StdPlus
*/

#pragma once

#include "StdInclude.h"


namespace stdplus
{
    class CmdParser
    {
        friend std::ostream & operator<<(std::ostream & os, const stdplus::CmdParser & d);

    public:
        CmdParser()
        {

        }

        inline void parseData(int argc, char** argv)
        {
            m_argc = argc;
            m_argv = argv;

            fillRawData();
            fillNamedData();
            splitAssignData(m_fullNamed, false);
            splitAssignData(m_shortNamed, true);

//             AVAR(m_keys);
//             AVAR(m_keyValues);
//             AVAR(m_indexedValue);
        }
        
        template<typename T>
        T getValue(const std::string & key)
        {
            if (key.size() == 0)
                throw std::logic_error("Empty key");

            auto itKey = m_keys.find(key);
            if (itKey != m_keys.end())
                return T();

            auto itKeyValue = m_keyValues.find(key);
            if (itKeyValue != m_keyValues.end())
            {
                const std::string & strValue = itKeyValue->second;
                return stdplus::to<T>(strValue);
            }

            throw std::logic_error("Not found key " + key);
        }

        template<>
        bool getValue<bool>(const std::string & key)
        {
            if (key.size() == 0)
                throw std::logic_error("Empty key");

            auto itKey = m_keys.find(key);
            if (itKey != m_keys.end())
                return true;

            auto itKeyValue = m_keyValues.find(key);
            if (itKeyValue != m_keyValues.end())
            {
                const std::string & strValue = itKeyValue->second;
                return stdplus::to<bool>(strValue);
            }

            throw std::logic_error("Not found key " + key);
        }

        inline const std::vector<std::string> & indexedValues()
        {
            return m_indexedValue;
        }


        template<typename T>
        T getValue(const std::string & key, const T & defaultValue)
        {
            try
            {
                return getValue<T>(key);
            }
            catch (std::logic_error &)
            {
                return defaultValue;
            }
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

            if (!m_keys.empty())
            {
                os << "bool_keys: ";
                for (auto & key : m_keys)
                    os << key << "; ";
            }

            if (!m_keyValues.empty())
            {
                os << "keys: \n";
                for (auto & kv : m_keyValues)
                    os << kv.first << ": " << kv.second << "\n";
            }
        }

    private:

        inline void fillRawData()
        {
            for (int i = 0; i < m_argc; ++i)
            {
                m_rawItems.push_back(m_argv[i]);
            }

            //AVAR(m_rawItems);
        }

        inline void fillNamedData()
        {
            for (const auto & raw : m_rawItems)
            {
                if (raw.size() > 2 && raw[0] == '-' && raw[1] == '-')
                {
                    std::string crop(raw.begin() + 2, raw.end());
                    m_fullNamed.push_back(crop);
                    continue;
                }

                if (raw.size() > 1 && raw[0] == '-')
                {
                    std::string crop(raw.begin() + 1, raw.end());
                    m_shortNamed.push_back(crop);
                    continue;
                }

                m_indexedValue.push_back(raw);
            }

//             AVAR(m_fullNamed);
//             AVAR(m_shortNamed);
//             AVAR(m_indexedValue);
        }

        inline void splitAssignData(const std::vector<std::string> strings, bool isShort = false)
        {
            for (const auto & namedData : strings)
            {
                std::vector<std::string> splits = stdplus::split(namedData, '=');

                if (splits.size() > 2)
                    throw std::logic_error("Too much assing in " + namedData);

                if (splits.size() == 1 && splits[0] == namedData)
                {
                    if (isShort)
                    {
                        if (splits[0].size() != 1)
                            throw std::logic_error("Not short key " + splits[0]);
                    }

                    if (isExist(splits[0]))
                        throw std::logic_error("Try insert existing element " + splits[0]);

                    m_keys.insert(splits[0]);
                }

                if (splits.size() == 2)
                {
                    if (isShort)
                    {
                        if (splits[0].size() != 1)
                            throw std::logic_error("Not short key " + splits[0]);
                    }

                    if (isExist(splits[0]))
                        throw std::logic_error("Try insert existing element " + splits[0]);

                    m_keyValues[splits[0]] = splits[1];
                }
            }
        }

        inline bool isExist(const std::string & value)
        {
            auto itKeyValue = m_keyValues.find(value);

            auto itKey = m_keys.find(value);

            if (itKeyValue != m_keyValues.end() || itKey != m_keys.end())
                return true;

            return false;
        }

        int                                m_argc;
        char**                             m_argv;
        std::vector<std::string>           m_rawItems;
        std::vector<std::string>           m_fullNamed;
        std::vector<std::string>           m_shortNamed;

        std::set<std::string>              m_keys;
        std::map<std::string, std::string> m_keyValues;
        std::vector<std::string>           m_indexedValue;
    };
}

inline std::ostream & operator<<(std::ostream & os, const stdplus::CmdParser & d)
{
    d.print(os);
    return os;
}