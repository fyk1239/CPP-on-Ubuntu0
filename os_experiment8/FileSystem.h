#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <math.h>
#include "BitMap.h"
#include "Directory.h"

const short g_BlockSize = 64;
const short g_NumBlocks = 128;
const short g_NumInodes = 32;	// Inode表中Inode的个数
const short g_MaxNumBlock = 13; // 一个文件最多可分配的块数
const short g_MaxFileSize = g_MaxNumBlock * g_BlockSize;

const int g_InodeBitMapSize = ((g_NumInodes + g_NumBitsInWord - 1) / g_NumBitsInWord) * sizeof(int);
const int g_BlockBitMapSize = ((g_NumBlocks + g_NumBitsInWord - 1) / g_NumBitsInWord) * sizeof(int);

struct SInode
{
	char FileType;                  // 'f'表示一般文件，'d'表示目录文件
	short FileSize;
	short NumBlocks;				// 分配给文件的磁盘块数
	short NumLinks;					// 硬链接到该Inode的数目
	short BlockNums[g_MaxNumBlock]; // 存储分配给文件的磁盘块号
};

struct SOpenFile // 增加新的数据结构，管理打开的文件
{
	short InodeNum;
	int CurSeekPos; // 存储当前读写的位置
	SInode Inode;
};

const int g_DiskSize = g_BlockBitMapSize + g_InodeBitMapSize + sizeof(SInode) * g_NumInodes + g_BlockSize * g_NumBlocks;
extern char g_Disk[g_DiskSize];

SDirectory loadDirectoryFromDisk(int vDirInodeNum);
void saveDirectory2Disk(int vDirInodeNum, const SDirectory &vDirectory);
void formatDisk();

bool createFile(const char *vFileName, int vDirInodeNum, char FileType = 'd', int vFileSize = sizeof(SDirectory));
bool removeFile(const char *vFileName, int vDirInodeNum);
SOpenFile *openFile(const char *vFileName);

bool allocateDisk(SInode &voInode, int fileSize, SBitMap &vioBlockBitMap);   // 根据文件大小和BitMap分配磁盘
void deallocateDisk(const SInode &vInode, SBitMap &vioBlockBitMap);		     // 根据文件INode回收磁盘
void saveInode2Disk(const SInode &vInode, int vInodeNum);
SInode loadInodeFromDisk(int vInodeNum);

int readDataFromFile(void *vDestination, int vNumBytes, SOpenFile &vioFile); // 读文件，返回实际读入的字符数
int writeData2File(const void *vSrc, int vNumBytes, SOpenFile &vioFile);	 // 写文件，返回实际写入的字符数

#endif