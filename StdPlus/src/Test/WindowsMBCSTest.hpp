#pragma once

#include "gtest/gtest.h"
#include "StdPlus/StdPlus.h"

TEST(WindowsMBCS, GetSubFolders)
{
    std::vector<std::string> files = stdplus::getSubFiles();
    AVAR(files);

    std::vector<std::string> folders = stdplus::getSubFolders();
    AVAR(folders);

    std::vector<std::string> subFiles = stdplus::getSubFilesIncludeSubfolders(".", "*rit*");
    AVAR(subFiles);
}

