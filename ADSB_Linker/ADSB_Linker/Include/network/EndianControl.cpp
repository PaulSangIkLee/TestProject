// Version : 1.0.2015.03.17

#define NULL 0x00

#include "stdafx.h"
#include "EndianControl.h"


CEndianControl::CEndianControl(void)
{
}

CEndianControl::~CEndianControl(void)
{
}


bool CEndianControl::SwitchEndian(void *pvData, int nByteSize )
{
	char *pchData = NULL;
	char chTemp = 0;
	int nLoop = 0;
	bool bReturnValue = 0;

	if( nByteSize < 1 )
	{
		bReturnValue = 0;
	}
	else
	{
		pchData = (char *)pvData;
		for( nLoop = 0; nLoop < (int)(nByteSize / 2); nLoop++ )
		{
			chTemp = pchData[nLoop];
			pchData[nLoop] = pchData[nByteSize - nLoop - 1];
			pchData[nByteSize - nLoop - 1] = chTemp;
		}

		bReturnValue = 1;
	}

	return bReturnValue;

}

bool CEndianControl::IsLittleEndian()
{
	int nEndianCheck = 1;
	bool bReturnValue = 0;

	if( *(char *)&nEndianCheck == 1 )
		bReturnValue = DEFINE_LITTLE_ENDIAN;
	else
		bReturnValue = DEFINE_BIG_ENDIAN;

	return bReturnValue;		


}

bool CEndianControl::IsAscendingBitOrder()
{
	TBitOrderCheckStruct tTestData;
	bool bReturnValue = 0;

	tTestData.unchData = 0;
	tTestData.BIT.unchBit1 = 1;

	if( tTestData.unchData == 1 )
		bReturnValue = DEFINE_ASCENDING_BIT_ORDER;
	else
		bReturnValue = DEFINE_DESCENDING_BIT_ORDER;

	return bReturnValue;
}