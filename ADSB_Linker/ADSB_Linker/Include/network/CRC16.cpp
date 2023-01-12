#include "stdafx.h"
#include "crc16.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

/**
* 생성자
*/
CCRC16::CCRC16(void)
{

}

/**
* 소멸자
*/
CCRC16::~CCRC16(void)
{
}

/**
* CRC16 코드 생성
* @param pucDataBlock 데이터 블럭 포인터
* @param usSizeOfDataBlock 데이터 블럭 크기
* @return CRC16 코드
*/
unsigned short CCRC16::GenerateCRC16(unsigned char* pucDataBlock, unsigned short usSizeOfDataBlock)
{
	register unsigned short usTableIndex, usIndex;
	unsigned short usAccumulation = 0;

	for (usIndex = 0; usIndex < usSizeOfDataBlock; usIndex++)
	{
		usTableIndex = ((unsigned short)(usAccumulation >> 8) ^ *pucDataBlock++) & 0x00ff;
		usAccumulation = (usAccumulation << 8) ^ usTableOfCRC16[usTableIndex];
	}

	return usAccumulation;
}

/**
* CRC16 코드 검증
* @param pucDataBlock 데이터 블럭 포인터
* @param usSizeOfDataBlock 데이터 블럭 크기
* @return 일치여부
*/
bool CCRC16::VerifyCRC16(unsigned char* pucDataBlock, unsigned short usSizeOfDataBlock)
{
	USHORT usReceivedCrc16Code;
	memcpy(&usReceivedCrc16Code, &pucDataBlock[usSizeOfDataBlock - 2], sizeof(USHORT));

	USHORT usCrc16Code = GenerateCRC16(pucDataBlock, usSizeOfDataBlock - 2);

	if (usCrc16Code == usReceivedCrc16Code)
		return true;
	else
		return false;
}

/**
* CRC16 코드 삽입
* @param pucBuffer 버퍼 포인터
* @param usPosition 위치
* @param usTarget 대상
*/
void CCRC16::AppendCRCCheck(unsigned char *pucBuffer, unsigned short usPosition, unsigned short usTarget)
{
	pucBuffer[usPosition + 0] = ((unsigned char*)&usTarget)[0];
	pucBuffer[usPosition + 1] = ((unsigned char*)&usTarget)[1];
}

/**
* CRC16 코드 Endian 타입 변환
* @param pucDataBlock 데이터 블럭 포인터
* @param usSizeOfDataBlock 데이터 블럭 크기
*/
void CCRC16::ChangeCRCBytesLittleEndian(unsigned char* pucDataBlock, unsigned short usSizeOfDataBlock)
{
	unsigned char   ucTemp;
	ucTemp = pucDataBlock[usSizeOfDataBlock - 2];
	pucDataBlock[usSizeOfDataBlock - 2] = pucDataBlock[usSizeOfDataBlock - 1];
	pucDataBlock[usSizeOfDataBlock - 1] = ucTemp;
}