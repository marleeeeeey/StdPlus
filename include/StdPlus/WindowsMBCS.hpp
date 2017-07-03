/*!
 * \author Tyulenev Sergey
 * Contact: marleeeeeey@gmail.com
 * last version https://github.com/marleeeeeey/StdPlus
*/


#pragma once

#include "StdInclude.h"
#include <Windows.h>    

namespace stdplus
{
	inline std::vector<std::string> getSubFolders(const std::string & mainPath = ".", const std::string & mask = "*")
	{
		std::vector<std::string> folders;

		std::string maskFolder = mainPath + "\\" + mask;

		WIN32_FIND_DATA inFold;
		HANDLE folderHandle;
		folderHandle = FindFirstFile(maskFolder.c_str(), &inFold);

		if (folderHandle != INVALID_HANDLE_VALUE)
		{
			do 
            {
				if (inFold.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{

					if (inFold.cFileName[0] == '.')
						continue;
					else
					{	
						std::string subFolder = mainPath + "\\" + inFold.cFileName;
						folders.push_back(subFolder);
						std::vector<std::string> sub = getSubFolders(subFolder, mask);
						folders.insert(folders.end(), sub.begin(), sub.end());
					}
				}
			} while (FindNextFile(folderHandle, &inFold));
		}
		FindClose(folderHandle);
		
		return folders;
	}

    inline std::vector<std::string> getSubFolders(const std::string & mainPath, const std::vector<std::string> & masks)
    {
        std::vector<std::string> allFolders;

        for (const auto & mask : masks)
        {
            auto curFolders = getSubFolders(mainPath, mask);
            allFolders.insert(allFolders.end(), curFolders.begin(), curFolders.end());
        }

        return allFolders;
    }
    
	inline std::vector<std::string> getSubFiles(const std::string & dir = ".", const std::string & mask = "*")
	{
		std::vector<std::string> files;

		std::string maskFile = dir + "\\" + mask;

		WIN32_FIND_DATA inFile;
		HANDLE fileHandle;
		fileHandle = FindFirstFile(maskFile.c_str(), &inFile);

		if (fileHandle != INVALID_HANDLE_VALUE)
		{
			do 
            {
				if (!(inFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					files.push_back(dir + "\\" + inFile.cFileName);
				}
			} while (FindNextFile(fileHandle, &inFile));
		}
		FindClose(fileHandle);

		return files;
	}
    
    inline std::vector<std::string> getSubFiles(const std::string & dir, const std::vector<std::string> & masks)
    {
        std::vector<std::string> allSubFiles;
        for (const auto & mask : masks)
        {
            auto curFiles = getSubFiles(dir, mask);
            allSubFiles.insert(allSubFiles.end(), curFiles.begin(), curFiles.end());
        }

        return allSubFiles;
    }

    inline std::vector<std::string> getSubFilesIncludeSubfolders(
        const std::string & dir = ".", const std::string & maskFile = "*", const std::string & maskDir = "*")
    {
        std::vector<std::string> subFolders = getSubFolders(dir, maskDir);
        subFolders.push_back(dir);
        
        std::vector<std::string> allFiles;

        for (const auto & subFolder : subFolders)
        {
            std::vector<std::string> subFiles = getSubFiles(subFolder, maskFile);
            allFiles.insert(allFiles.end(), subFiles.begin(), subFiles.end());
        }

        return allFiles;
    }    

    inline std::vector<std::string> getSubFilesIncludeSubfolders(
        const std::string & dir, 
        const std::vector<std::string> & fileMasks, 
        const std::vector<std::string> & dirMasks = { "*" })
    {
        std::vector<std::string> allSubFiles;

        for (const auto & dirMask : dirMasks)
        {
            for (const auto & fileMask : fileMasks)
            {
                std::vector<std::string> curSubFiles;
                curSubFiles = getSubFilesIncludeSubfolders(dir, fileMask, dirMask);
                allSubFiles.insert(allSubFiles.end(), curSubFiles.begin(), curSubFiles.end());
            }
        }

        std::sort(allSubFiles.begin(), allSubFiles.end());
        allSubFiles.erase(
            std::unique(allSubFiles.begin(), allSubFiles.end()), allSubFiles.end());

        return allSubFiles;
    }
}
