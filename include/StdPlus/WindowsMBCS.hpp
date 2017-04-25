#pragma once
#include "StdInclude.h"
#include <Windows.h>    

namespace stdplus
{
	inline std::vector<std::string> getSubFolders(const std::string & mainPath, const std::string & mask)
	{
		std::vector<std::string> folders;

		std::string maskFolder = mainPath + "\\" + mask;

		WIN32_FIND_DATA inFold;
		HANDLE folderHandle;
		folderHandle = FindFirstFile(maskFolder.c_str(), &inFold);

		if (folderHandle != INVALID_HANDLE_VALUE)
		{
			do {
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

	inline std::vector<std::string> getSubFiles(const std::string & dir, const std::string & mask)
	{
		std::vector<std::string> files;

		std::string maskFile = dir + "\\" + mask;

		WIN32_FIND_DATA inFile;
		HANDLE fileHandle;
		fileHandle = FindFirstFile(maskFile.c_str(), &inFile);

		if (fileHandle != INVALID_HANDLE_VALUE)
		{
			do {
				if (!(inFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					files.push_back(dir + "\\" + inFile.cFileName);
				}
			} while (FindNextFile(fileHandle, &inFile));
		}
		FindClose(fileHandle);

		return files;
	}
}
