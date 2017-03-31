#ifndef FilePlus_h__
#define FilePlus_h__

#include "StdInclude.h"

namespace stdplus
{
    inline bool file_exist(const std::string& name)
    {
        std::ifstream f(name.c_str());
        return f.good();
    }


    inline void createFolder(const std::string & path)
    {
        _mkdir(path.c_str());
    }

}

#endif // FilePlus_h__
