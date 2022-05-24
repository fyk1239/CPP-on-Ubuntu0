#ifndef BITMAP_H_
#define BITMAP_H_

const short g_NumBitsInWord = sizeof(int) * 8;

struct SBitMap
{
	int NumBits;   //位示图大小
	int *pMapData;
};

int countClearBits(const SBitMap& vBitMap);                        //统计vBitMap没有被占用的位数（为1表示占用，0表示没占用）
int findAndSetAvailableBit(SBitMap& voBitMap);                     //返回第一个没有被占用的位，并将其置1;没有则返回-1
bool isAvailableBitAt(int vBitPosition, const SBitMap& vBitMap);   //测试第vBitPosition位是否为0
void createEmptyBitMap(SBitMap& voBitMap, int vNumTotalBits);      //创建一个大小为vNumTotalBits的空位示图
void markBitAt(int vBitPosition, SBitMap& vioBitMap);              //将位示图中第vBitPosition位置为1
void clearBitAt(int vBitPosition, SBitMap& voBitMap);              //将位示图中第vBitPosition位置为0
void printfBitMap(const SBitMap& vBitMap);                         //打印位示图

#endif