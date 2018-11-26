/*!
 * \author Tyulenev Sergey
 * Contact: marleeeeeey@gmail.com
 * last version https://github.com/marleeeeeey/StdPlus
*/


#pragma once

#include "StdInclude.h"
#include "MacrosPlus.hpp"
#include "./../persistence1d.hpp"

namespace stdplus
{

    struct Point
    {
        double x;
        double y;
    };


    inline double deg2rad(double angle_deg)
    {
        return angle_deg * (M_PI) / 180;
    }

    inline double rad2deg(double angle_deg)
    {
        return angle_deg * 180 / (M_PI);
    }

    inline double normal_rad(double var)
    {
        return var - ((2 * M_PI)*floor((var / (2 * M_PI))));
    }


    inline double normal_deg(double deg, double min_deg = -180, double max_deg = 180)
    {
        if (min_deg > max_deg)
            throw std::logic_error("ERROR: stdplus::normal_deg: min_deg > max_deg");

        double diff_deg = max_deg - min_deg;
        
        while (deg < min_deg)
            deg += diff_deg;

        while (deg > max_deg)
            deg -= diff_deg;

        return deg;
    }


    inline double sin_deg(double d_deg)
    {
        return sin(deg2rad(d_deg));
    }


    inline double cos_deg(double d_deg)
    {
        return cos(deg2rad(d_deg));
    }

	inline std::vector<int> isolatedIndex(std::vector<double> d, double val)
	{
		std::sort(d.begin(), d.end());
		std::vector<int> result;

		if (d.empty()) return result;

		result.push_back(0);

		for (size_t i = 1; i < d.size() - 1; i++)
		{
			double prev = d.at(i - 1);
			double cur = d.at(i + 0);
			double next = d.at(i + 1);

			double dPrev = abs(cur - prev);
			double dNext = abs(next - cur);

			if (dPrev >= val && dNext >= val)
				result.push_back(i);
		}

		result.push_back(d.size() - 1);

		return result;
	}

	inline std::vector<double> getBoolGraphic(std::vector<int> &indexes, double val = 1)
	{
		std::sort(indexes.begin(), indexes.end());

		std::vector<double> result;

		for (const auto & minIndex : indexes)
		{
			while (static_cast<int>(result.size()) < minIndex)
			{
				result.push_back(0);
			}
			result.push_back(val);
		}

		return result;
	}


        
	inline std::vector<double> minExtremsGraphic(const std::vector<p1d::TPairedExtrema> & extrems)
	{
		std::vector<int> indexes;

		for (auto & extrem : extrems)
			indexes.push_back(extrem.MinIndex);

		return getBoolGraphic(indexes, -1000);
	}

	inline std::vector<double> maxExtremsGraphic(const std::vector<p1d::TPairedExtrema> & extrems)
	{
		std::vector<int> indexes;

		for (auto & extrem : extrems)
			indexes.push_back(extrem.MaxIndex);

		return getBoolGraphic(indexes, +1000);
	}

	template<typename T>
	double mean(const std::vector<T> & v)
	{
		double sum = std::accumulate(v.begin(), v.end(), 0.0);
		return sum / v.size();
	}

	template<typename T>
	double meanAbs(const std::vector<T> & v)
	{
		double sum = std::accumulate(v.begin(), v.end(), 0.0,
			[](double & acc, const double & val)
		{
			return acc += abs(val);			 
		});
		return sum / v.size();
	}


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


	inline std::vector<double> randomGraphic(size_t s, double step = 0.5)
	{
		std::vector<double> graphic(s);

		std::generate(graphic.begin(), graphic.end(),
			[&step]()
		{
			static double prevVal = 0;
			prevVal += stdplus::getRandom<double>(-step, step);
			return prevVal;
		});

		return graphic;
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


        double front = 0;
        double back = 0;
        if (inVec.size() == 1)
        {
            front = inVec.front();
            back = inVec.back();
        }
        else
        {
            auto itFront = inVec.begin();
            front = *(itFront + 0) - (*(itFront + 1) - *(itFront + 0));

            auto itBack = inVec.rbegin();
            back = *(itBack + 0) - (*(itBack + 1) - *(itBack + 0));
        }
		
        extVec.push_back(front);    
        extVec.insert(extVec.end(), inVec.begin(), inVec.end());
        extVec.push_back(back);     

        int index = 0;
        for (auto it = extVec.begin(); it < extVec.end() - 2; it++)
        {
            stdplus::Point p1{ (double)index + 0, *(it + 0) };
            stdplus::Point p2{ (double)index + 1, *(it + 1) };
            stdplus::Point p3{ (double)index + 2, *(it + 2) };
            outVec.push_back(derivative(p1, p2, p3));
        }

        return outVec;
    }

}
