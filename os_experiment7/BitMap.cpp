#include "BitMap.h"
#include <math.h>
#include <stdio.h>

void createEmptyBitMap(SBitMap& voBitMap, int vNumTotalBits)
{
	voBitMap.NumBits  = vNumTotalBits;
	int NumWords = (int)ceil(vNumTotalBits / (double)g_NumBitsInWord);
	voBitMap.pMapData = new int[NumWords];

	for (int i=0; i<NumWords; ++i) voBitMap.pMapData[i] = 0;
}

void markBitAt(int vBitPosition, SBitMap& vioBitMap) 
{ 
	vioBitMap.pMapData[vBitPosition / g_NumBitsInWord] |= 1 << (vBitPosition % g_NumBitsInWord);
}

void clearBitAt(int vBitPosition, SBitMap& vioBitMap) 
{
	
}

bool isAvailableBitAt(int vBitPosition, const SBitMap& vBitMap)
{

}

int countClearBits(const SBitMap& vBitMap)
{

}

int findAndSetAvailableBit(SBitMap& voBitMap) 
{
	
}

void printfBitMap(const SBitMap& vBitMap)
{
	for (int i=0; i<vBitMap.NumBits; ++i) 
	{
		if (isAvailableBitAt(i, vBitMap)) printf("0 ");
		else printf("1 "); 
	}
	printf("\n"); 
}