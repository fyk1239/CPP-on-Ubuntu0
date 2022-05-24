#ifndef DATASTRUCTURE_H_
#define DATASTRUCTURE_H_

const short g_MaxNumFiles = 10;
const short g_MaxFileNameLen = 10;

struct SDirectoryEntry // 目录项
{
	bool IsInUse;
	short InodeNum;
	char FileName[g_MaxFileNameLen + 1];
};

struct SDirectory
{
	char FileName[g_MaxFileNameLen + 1];
	SDirectoryEntry FileSet[g_MaxNumFiles]; // 假定一个目录只能存储g_MaxNumFiles个文件
};

bool addFile2Directory(const char *vFileName, short vInodeNum, SDirectory &voDirectory); // 在目录中添加vFileName文件,文件对应的INode为vInodeNum
bool removeFileFromDirectory(const char *vFileName, SDirectory &vioDirectory);			 // 从目录中移除vFileName文件
int findFileInodeNum(const char *vFileName, const SDirectory &vDirectory);				 // 查找vFileName文件的INode编号
int findFileIndex(const char *vFileName, const SDirectory &vDirectory);					 // 查找文件名为vFileName的下标，不存在则返回-1
SDirectory createEmptyDirectory();

#endif