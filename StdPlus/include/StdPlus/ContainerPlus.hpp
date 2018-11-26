#pragma once

#include "StdInclude.h"

namespace stdplus
{    

    /*!
    \brief Convert std::vector<Tin> ot std::vector<Tout> 
    */
	template<typename Tin, typename Tout>
	std::vector<Tout> convert(const std::vector<Tin> & dataIn)
	{
		std::vector<Tout> dataOut;

		for_each(dataIn.begin(), dataIn.end(),
			[&dataOut](const Tin & d)
		{
			dataOut.push_back((Tout)d);
		});

		return dataOut;
	}


    /*!
    \brief Convert c-array and c-size to std::vector
    */
    template<class T>
    std::vector<T> to_vector(const T * begin, const size_t size)
    {
        std::vector<T> vec;
        vec.assign(begin, begin + size);
        return vec;
    }
    

    /*!
    \brief Overload for print std::vector<T> in std::ostream
    */
    template<class T>
    std::ostream & operator<<(std::ostream & os, const std::vector<T> & vec)
    {
        os << " VECTOR_SIZE=" << vec.size() << ": ";

        if (vec.empty())
            return os;

        int index = 0;
        for (const auto & el : vec)
        {

#ifdef STDPLUS_MULTILINE_PRINT 
            os 
                << std::endl
                << "[" << std::setw(3) << index << "]: " 
                << el
                ;
#else
            os << el << ", ";
#endif
            index++;
        }

        return os;
    }


    /*!
    \brief Overload for print std::set<T> in std::ostream
    */
    template<class T>
    std::ostream & operator<<(std::ostream & os, const std::set<T> & vec)
    {
        os << " SET_SIZE=" << vec.size() << ": ";

        if (vec.empty())
            return os;

        int index = 0;
        for (const auto & el : vec)
        {

#ifdef STDPLUS_MULTILINE_PRINT 
            os
                << std::endl
                << "[" << std::setw(3) << index << "]: "
                << el
                ;
#else
            os << el << ", ";
#endif
            index++;
        }

        return os;
    }


    /*!
    \brief Overload for print std::map<T, T2> in std::ostream
    */
    template<class T, class T2>
    std::ostream & operator<<(std::ostream & os, const std::map<T, T2> & vec)
    {
        os << " MAP_SIZE=" << vec.size() << ": ";

        if (vec.empty())
            return os;

        int index = 0;
        for (const auto & el : vec)
        {

#ifdef STDPLUS_MULTILINE_PRINT 
            os
                << std::endl
                << "[" << std::setw(10) << el.first << "]: "
                << el.second
                ;
#else
            os << el << ", ";
#endif
            index++;
        }

        return os;
    }


    /*!
    \brief Get a certain number of items (target vector<T>) from the source vector<T>.
    \param [in] vec sourse vector<T>
    \param [in] dev certain number of items
    \return Target vector<T> of size dev where the values ​​are distributed evenly
    */
	template<typename T>
	std::vector<T> evenly(const std::vector<T> vec, int dev)
	{
		size_t size = vec.size();
		size_t step = size / dev;
		std::vector<T> result;

		for (size_t i = 0; i < size; i += step)
		{
			result.push_back(vec.at(i));
			if (result.size() == dev)
				break;
		}

		return result;
	}

    
}


