/*!
 * \author Tyulenev Sergey
 * Contact: marleeeeeey@gmail.com
 * last version https://github.com/marleeeeeey/StdPlus
*/

#pragma once

#include "StdInclude.h"

namespace stdplus
{
    inline std::string thisThreadIdString()
    {
        std::stringstream os;
        os << "[" << std::setw(5) << std::this_thread::get_id() << "]";
        return os.str();
    }

    inline void thread_sleep(unsigned time_ms)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(time_ms));
    }

    inline char waitKeyboardKey()
    {
        char val = std::cin.get(); //_getch();
        val = tolower(val);

        return val;
    }
}
