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
        friend OutputSystem & globalOS();

    public:
        
        inline void addOStream(std::ostream & os)
        {
            m_ostreams.push_back(&os);
        }

        inline void removeOstream(std::ostream & os)
        {
            auto it = std::find(m_ostreams.begin(), m_ostreams.end(), &os);
            m_ostreams.erase(it);
        }

        inline void printString(const std::string & str)
        {
            for (std::ostream * pOStream : m_ostreams)
            {
                std::ostream & os = *pOStream;
                os << str;
            }
        }

    private:
        inline OutputSystem()
        {
            addOStream(std::cout);
        }

        inline ~OutputSystem()
        {
            std::cout << "Count stream = " << m_ostreams.size();
            std::cin.get();
        }

        OutputSystem(const OutputSystem &) = delete;
        void operator=(const OutputSystem &) = delete;

        std::vector<std::ostream *> m_ostreams;
    };


    inline OutputSystem & globalOS()
    {
        static OutputSystem os;
        return os;
    }

    inline std::ostream & stdConsoleOut()
    {
        return std::cout;
    }

}