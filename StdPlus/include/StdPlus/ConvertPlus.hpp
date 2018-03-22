/*!
* \author Tyulenev Sergey
* Contact: marleeeeeey@gmail.com
* last version https://github.com/marleeeeeey/StdPlus
*/


#pragma once

#include "StdInclude.h"

namespace stdplus
{
    template<class T>
    std::string to_string_hex(const T& t)
    {
        std::ostringstream os;
        os << "0x" << std::setw(sizeof(T) * 2) << std::setfill('0') << std::hex << t;
        return os.str();
    }

    template<class T>
    std::string to_string(const T& t)
    {
        std::ostringstream os;
        os << t;
        return os.str();
    }

    template<>
    inline std::string to_string(const bool& t)
    {
        if (t == true)
            return "true";

        return "false";
    }
    
    /*!
    \brief Convert std::string to template type T std::istringstream 
    */
    template<typename T>
    inline T to(const std::string & str)
    {
        T val;
        std::istringstream ss(str);

        if (ss.eof())
            throw std::logic_error("Probably empty value \'" 
                + str + "\' in to<...> (ConvertPlus.hpp)"); 

        ss >> val;

        if (ss.fail())
            throw std::logic_error("Error convert " + str 
                + " to special type in to<...> (ConvertPlus.hpp)");	
        
        return val;
    }


    /*!
    \brief Template specialization: Convert std::string to type std::string
    */
    template<>
    inline std::string to(const std::string & str)
    {
        return str;
    }


}

