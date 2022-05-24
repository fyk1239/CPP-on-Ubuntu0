#include "FileSystem.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

char g_Disk[g_DiskSize] = {'\0'};
SDirectory g_RootDir = createEmptyDirectory();

bool createFile(const char *vFileName, int vDirInodeNum, char FileType, int vFileSize)
{
	if (vDirInodeNum >= g_NumInodes)
		return false;
	if (vFileSize > g_MaxFileSize)
		return false;

	SDirectory TempDirectory = loadDirectoryFromDisk(vDirInodeNum);

	bool IsSuccess = true;
	if (findFileInodeNum(vFileName, TempDirectory) != -1)
		IsSuccess = false;
	else
	{
		SBitMap InodeBitMap;
		createEmptyBitMap(InodeBitMap, g_NumInodes);
		memcpy(InodeBitMap.pMapData, g_Disk + g_BlockBitMapSize, g_InodeBitMapSize); // 从虚拟硬盘中读取Inode位示图

		int InodeIndex = findAndSetAvailableBit(InodeBitMap);
		if (InodeIndex == -1)
			IsSuccess = false;
		else if (!addFile2Directory(vFileName, InodeIndex, TempDirectory))
			IsSuccess = false;
		else
		{

			SBitMap DataBlockBitMap;
			createEmptyBitMap(DataBlockBitMap, g_NumBlocks);
			memcpy(DataBlockBitMap.pMapData, g_Disk, g_BlockBitMapSize); // 从虚拟硬盘读取数据块位示图

			SInode TempInode;
			if (!allocateDisk(TempInode, vFileSize, DataBlockBitMap))
				IsSuccess = false;
			else
			{
				printf("Creating file %s(type:%c) size %d at %s\n", vFileName, FileType, vFileSize, TempDirectory.FileName);
				TempInode.FileType = FileType;
				saveInode2Disk(TempInode, InodeIndex);										 // 将Inode信息写回虚拟硬盘
				memcpy(g_Disk, DataBlockBitMap.pMapData, g_BlockBitMapSize);				 // 将数据块位示图写回虚拟硬盘
				memcpy(g_Disk + g_BlockBitMapSize, InodeBitMap.pMapData, g_InodeBitMapSize); // 将Inode位示图写回虚拟硬盘
				saveDirectory2Disk(vDirInodeNum, TempDirectory);
				if (FileType == 'd')
				{
					SDirectory SubDir = createEmptyDirectory();
					strncpy(SubDir.FileName, vFileName, g_MaxFileNameLen);
					saveDirectory2Disk(InodeIndex, SubDir);
				}
			}
			delete DataBlockBitMap.pMapData;
		}
		delete InodeBitMap.pMapData;
	}
	return IsSuccess;
}

bool removeFile(const char *vFileName, int vDirInodeNum)
{
	SDirectory TempDirectory = loadDirectoryFromDisk(vDirInodeNum);

	//...       从目录中找到文件对应的Inode编号，根据Inode编号读出Inode数据，完成文件的删除

	removeFileFromDirectory(vFileName, TempDirectory);
	saveDirectory2Disk(vDirInodeNum, TempDirectory); // 将目录内容重新写回虚拟硬盘
	return true;
}

SDirectory loadDirectoryFromDisk(int vDirInodeNum)
{
	return g_RootDir; // 这里先用全局变量保存Dir，等实现了文件读写再来完成
}

void saveDirectory2Disk(int vDirInodeNum, const SDirectory &vDirectory)
{
	g_RootDir = vDirectory; // 这里也是用全局的变量保存目录，等实现了文件读写再来完成
}

void formatDisk()
{
	memset(g_Disk, 0, g_DiskSize);

	SBitMap InodeDiskBitMap;
	createEmptyBitMap(InodeDiskBitMap, g_NumInodes);								 // 创建Inode位示图
	markBitAt(0, InodeDiskBitMap);													 // 为目录分配Inode，默认为第0个Inode
	memcpy(g_Disk + g_BlockBitMapSize, InodeDiskBitMap.pMapData, g_InodeBitMapSize); // 将Inode位示图写回虚拟硬盘
	delete InodeDiskBitMap.pMapData;

	SBitMap DataBlockBitMap;
	createEmptyBitMap(DataBlockBitMap, g_NumBlocks); // 创建数据块位示图

	SDirectory Directory = createEmptyDirectory(); // 创建根目录
	strcpy(Directory.FileName, "/");
	SInode DirInode;
	DirInode.FileType = 'd';
	allocateDisk(DirInode, sizeof(SDirectory), DataBlockBitMap); // 分配根目录数据块

	memcpy(g_Disk + g_BlockBitMapSize + g_InodeBitMapSize, &DirInode, sizeof(SInode)); // 将目录Inode信息写回                               //将Inode信息写回虚拟硬盘
	memcpy(g_Disk, DataBlockBitMap.pMapData, g_BlockBitMapSize);					   // 将数据块位示图写回虚拟硬盘
	delete DataBlockBitMap.pMapData;

	saveDirectory2Disk(0, Directory);
}

bool allocateDisk(SInode &voInode, int vFileSize, SBitMap &vioBlockBitMap)
{
	voInode.FileSize = vFileSize;
	voInode.NumLinks = 1;
	voInode.NumBlocks = (int)ceil(vFileSize / (double)g_BlockSize);

	if (countClearBits(vioBlockBitMap) < voInode.NumBlocks)
		return false;

	for (int i = 0; i < voInode.NumBlocks; ++i)
		voInode.BlockNums[i] = findAndSetAvailableBit(vioBlockBitMap);
	for (int i = voInode.NumBlocks; i < g_MaxNumBlock; ++i)
		voInode.BlockNums[i] = -1;

	return true;
}

void deallocateDisk(const SInode &vInode, SBitMap &vioCBitMap)
{
	// 回收磁盘块
}

SInode loadInodeFromDisk(int vInodeNum)
{
	SInode DirInode;
	memcpy(&DirInode, g_Disk + g_BlockBitMapSize + g_InodeBitMapSize + sizeof(SInode) * vInodeNum, sizeof(SInode));

	return DirInode;
}

void saveInode2Disk(const SInode &vInode, int vInodeNum)
{
	memcpy(g_Disk + g_BlockBitMapSize + g_InodeBitMapSize + sizeof(SInode) * vInodeNum, &vInode, sizeof(SInode));
}
