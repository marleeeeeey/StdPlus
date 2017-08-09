#pragma once

#include "StdPlus/StdPlus.h"

namespace stdplus
{

    class Settings
    {
    public:
        inline Settings()
        {

        }

        template<typename T>
        inline T getValue(const std::string & key)
        {
            return m_cmdParser.getValue<T>(key);
        }

        template<typename T>
        inline T getValue(const std::string & key, const T & defaultValue)
        {
            return m_cmdParser.getValue<T>(key, defaultValue);
        }

        template<typename T>
        inline void setValue(const std::string & key, const T & value)
        {
            m_cmdParser.setValue(key, value);
        }

        template<typename T>
        inline void updateValue(const std::string & key, const T & value)
        {
            m_cmdParser.updateValue(key, value);
        }

        inline void open(const std::string & fileName)
        {
            m_fileName = fileName;

            std::ifstream fin(m_fileName);

            if (!fin.is_open())
                throw std::logic_error("Cannot open file \'" + m_fileName + "\'");

            std::string line;
            std::vector<std::string> lines;

            while (getline(fin, line))
            {
                lines.push_back(line);
            }

            m_cmdParser.parseData(lines);
        }

        inline void save(std::string fileName = "")
        {
            if (fileName.empty())
                fileName = m_fileName;

            std::ofstream fout(fileName);

            if (!fout.is_open())
                throw std::logic_error("Cannot open file \'" + fileName + "\' for save settings");

            // TODO insert create backup copy

            const char splitter = m_cmdParser.getSplitter();

            for (auto & keyValue : m_cmdParser.getRawKeyValues())
            {
                fout << keyValue.first << splitter << keyValue.second << std::endl;
            }
        }

    private:
        std::string        m_fileName;
        stdplus::CmdParser m_cmdParser;
    };


}
