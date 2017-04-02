#ifndef ContainerPlus_h__
#define ContainerPlus_h__

#include "StdInclude.h"

namespace stdplus
{
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
            os << std::endl;
            os << "Vector index [" << std::setw(2) << index << "]" << std::endl;
            os << el;

            index++;
        }

        return os;
    }

    
	template<typename T>
	std::vector<T> evenly(const std::vector<T> vec, int dev)
	{
		int size = vec.size();
		int step = size / dev;
		std::vector<T> result;

		for (int i = 0; i < size; i += step)
		{
			result.push_back(vec.at(i));
			if (result.size() == dev)
				break;
		}

		return result;
	}

    
}

#endif // ContainerPlus_h__
