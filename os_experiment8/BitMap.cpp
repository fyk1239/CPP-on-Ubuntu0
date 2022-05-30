#include "BitMap.h"
#include <math.h>
#include <stdio.h>

void createEmptyBitMap(SBitMap &voBitMap, int vNumTotalBits)
{
	voBitMap.NumBits = vNumTotalBits;
	int NumWords = (int)ceil(vNumTotalBits / (double)g_NumBitsInWord);
	voBitMap.pMapData = new int[NumWords];

	for (int i = 0; i < NumWords; ++i)
		voBitMap.pMapData[i] = 0;
}

void markBitAt(int vBitPosition, SBitMap &vioBitMap)
{
	vioBitMap.pMapData[vBitPosition / g_NumBitsInWord] |= 1 << (vBitPosition % g_NumBitsInWord);
}

void clearBitAt(int vBitPosition, SBitMap &vioBitMap)
{
	vioBitMap.pMapData[vBitPosition / g_NumBitsInWord] &= ~(1 << (vBitPosition % g_NumBitsInWord));
}

bool isAvailableBitAt(int vBitPosition, const SBitMap &vBitMap)
{
	if (vBitMap.pMapData[vBitPosition / g_NumBitsInWord] & (1 << (vBitPosition % g_NumBitsInWord)) == 0)
		return true;
	return false;
}

int countClearBits(const SBitMap &vBitMap)
{
	int num = 0;
	for (int i = 0; i < vBitMap.NumBits; ++i)
	{
		if (isAvailableBitAt(i, vBitMap))
			++num;
	}
	return num;
}

int findAndSetAvailableBit(SBitMap &voBitMap)
{
	for (int i = 0; i < voBitMap.NumBits; ++i)
	{
		if (isAvailableBitAt(i, voBitMap))
		{
			markBitAt(i, voBitMap);
			return i;
		}
	}
	return -1;
}

void printfBitMap(const SBitMap &vBitMap)
{
	for (int i = 0; i < vBitMap.NumBits; ++i)
	{
		if (isAvailableBitAt(i, vBitMap))
			printf("0 ");
		else
			printf("1 ");
	}
	printf("\n");
}