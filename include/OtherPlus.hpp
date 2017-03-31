#ifndef OtherPlus_h__
#define OtherPlus_h__

#include "StdInclude.h"

namespace stdplus
{
    inline void thread_sleep(unsigned time_ms)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(time_ms));
    }

    inline char waitKeyboardKey()
    {
        char val = _getch();
        val = tolower(val);

        return val;
    }
}

#endif // OtherPlus_h__
