#include "Directory.h"
#include <memory.h>
#include <string.h>

SDirectory createEmptyDirectory()
{
	SDirectory Directory;
	memset(Directory.FileName, 0, g_MaxFileNameLen+1);
	for (short i=0; i<g_MaxNumFiles; ++i)
	{
		Directory.FileSet[i].InodeNum = -1;
		Directory.FileSet[i].IsInUse = false;
		memset(Directory.FileSet[i].FileName, 0, g_MaxFileNameLen+1);
	}

	return Directory;
}

int findFileInodeNum(const char* vFileName, const SDirectory& vDirectory)
{
	int FileIndex = findFileIndex(vFileName, vDirectory);

	return FileIndex != -1 ? vDirectory.FileSet[FileIndex].InodeNum : -1;
}

int findFileIndex(const char* vFileName, const SDirectory& vDirectory)
{

}

bool addFile2Directory(const char* vFileName, short vInodeNum, SDirectory& voDirectory)
{
	//遍历目录项数组，寻找可用的目录项
}

bool removeFileFromDirectory(const char *vFileName, SDirectory& vioDirectory)
{ 
	int FileIndex = findFileIndex(vFileName, vioDirectory);

	if (FileIndex == -1) return false;

	vioDirectory.FileSet[FileIndex].InodeNum = -1;
	vioDirectory.FileSet[FileIndex].IsInUse = false;
	memset(vioDirectory.FileSet[FileIndex].FileName, 0, g_MaxFileNameLen+1);
	return true;
}