#ifndef MathPlus_h__
#define MathPlus_h__

#include "StdInclude.h"
#include "MacrosPlus.hpp"

namespace stdplus
{

    struct Point
    {
        double x;
        double y;
    };

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
        
        T diff = max - min;

        if (diff < 0)
            throw(std::logic_error("Error random diff"));

        double k = (double)diff / RAND_MAX;
        double ret = k * rand() + min;

        return (T)ret;
    }
    
}


inline stdplus::Point operator-(const stdplus::Point & p)
{
    stdplus::Point ret;
    ret.x = -p.x;
    ret.y = -p.y;
    return ret;
}

inline stdplus::Point operator+(const stdplus::Point & p1, const stdplus::Point & p2)
{
    stdplus::Point ret;
    ret.x = p1.x + p2.x;
    ret.y = p1.y + p2.y;
    return ret;
}

inline stdplus::Point operator-(const stdplus::Point & p1, const stdplus::Point & p2)
{
    stdplus::Point ret = p1 + (-p2);
    return ret;
}

inline stdplus::Point operator/(const stdplus::Point & p1, const double & div)
{
    stdplus::Point ret;
    ret.x = p1.x / div;
    ret.y = p1.y / div;
    return ret;
}

inline std::ostream & operator<<(std::ostream & os, const stdplus::Point & p)
{
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}


namespace stdplus
{
    inline double derivative(stdplus::Point p1, stdplus::Point p2)
    {
        stdplus::Point delta = p2 - p1;        
        AEXCEPT_IF(delta.x == 0);
        return delta.y / delta.x;
    }

    inline double derivative(stdplus::Point p1, stdplus::Point p2, stdplus::Point p3)
    {
        stdplus::Point p23 = (p2 + p3) / 2;
        return derivative(p1, p23);
    }

    inline std::vector<double> derivative(const std::vector<double> & inVec)
    {
        std::vector<double> outVec;

        if (inVec.empty()) return outVec;

        std::vector<double> extVec;
        extVec.push_back(inVec.front());    // + 1
        extVec.insert(extVec.end(), inVec.begin(), inVec.end());
        extVec.push_back(inVec.back());     // + 1 --> 1+1 = 2

        int index = 0;
        for (auto it = extVec.begin(); it < extVec.end() - 2; it++)
        {
            stdplus::Point p1{ index + 0, *(it + 0) };
            stdplus::Point p2{ index + 1, *(it + 1) };
            stdplus::Point p3{ index + 2, *(it + 2) };
            outVec.push_back(derivative(p1, p2, p3));
        }

        return outVec;
    }

}

#endif // MathPlus_h__
