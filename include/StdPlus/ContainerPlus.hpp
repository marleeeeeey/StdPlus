#ifndef ContainerPlus_h__
#define ContainerPlus_h__

#include "StdInclude.h"

namespace stdplus
{

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

    template<class T>
    std::vector<T> to_vector(const T * begin, const size_t size)
    {
        std::vector<T> vec;
        vec.assign(begin, begin + size);
        return vec;
    }
    
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

#endif // ContainerPlus_h__
