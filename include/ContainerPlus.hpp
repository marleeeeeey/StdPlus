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

}

#endif // ContainerPlus_h__
