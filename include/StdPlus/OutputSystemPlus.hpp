/*!
* \author Tyulenev Sergey
* Contact: marleeeeeey@gmail.com
* last version https://github.com/marleeeeeey/StdPlus
*/

#pragma once

#include "StdInclude.h"

namespace stdplus
{
    class OutputSystem
    {
    public:
        inline OutputSystem()
        {
            addOStream(std::cout);
        }

        inline ~OutputSystem()
        {

        }
        
        inline void addOStream(std::ostream & os)
        {
            auto it = std::find(m_ostreams.begin(), m_ostreams.end(), &os);

            if (it == m_ostreams.end())
                m_ostreams.push_back(&os);            
        }

        inline void removeOstream(std::ostream & os)
        {
            auto it = std::find(m_ostreams.begin(), m_ostreams.end(), &os);
            m_ostreams.erase(it);
        }

        inline void clear()
        {
            m_ostreams.clear();
        }

        inline void printString(const std::string & str)
        {
            for (std::ostream * pOStream : m_ostreams)
            {
                std::ostream & os = *pOStream;
                os << str;
                os.flush();
            }
        }

        inline size_t size()
        {
            return m_ostreams.size();
        }

    private:

        OutputSystem(const OutputSystem &) = delete;
        void operator=(const OutputSystem &) = delete;

        std::vector<std::ostream *> m_ostreams;
    };


    inline std::ostream & stdConsoleOut()
    {
        return std::cout;
    }

}