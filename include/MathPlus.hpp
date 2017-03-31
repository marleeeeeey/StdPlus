#ifndef MathPlus_h__
#define MathPlus_h__

#include "StdInclude.h"

namespace stdplus
{

    template<class T>
    T getRandom(const T & min, const T & max)
    {
        static bool isFirstRun = true;
        if (isFirstRun)
        {
            srand((unsigned)time(NULL));
            isFirstRun = false;
        }

        if (min == max)
            return min;

        unsigned xMax = RAND_MAX;

        T diff = max - min;

        if (diff < 0)
            throw(std::logic_error("Error random diff"));

        double k = (double)diff / xMax;
        double ret = k * rand() + min;

        return (T)ret;
    }

}

#endif // MathPlus_h__
