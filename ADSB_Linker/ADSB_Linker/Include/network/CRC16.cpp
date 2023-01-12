#include "stdafx.h"
#include "crc16.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

/**
* ������
*/
CCRC16::CCRC16(void)
{

}

/**
* �Ҹ���
*/
CCRC16::~CCRC16(void)
{
}

/**
* CRC16 �ڵ� ����
* @param pucDataBlock ������ �� ������
* @param usSizeOfDataBlock ������ �� ũ��
* @return CRC16 �ڵ�
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
* CRC16 �ڵ� ����
* @param pucDataBlock ������ �� ������
* @param usSizeOfDataBlock ������ �� ũ��
* @return ��ġ����
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
* CRC16 �ڵ� ����
* @param pucBuffer ���� ������
* @param usPosition ��ġ
* @param usTarget ���
*/
void CCRC16::AppendCRCCheck(unsigned char *pucBuffer, unsigned short usPosition, unsigned short usTarget)
{
	pucBuffer[usPosition + 0] = ((unsigned char*)&usTarget)[0];
	pucBuffer[usPosition + 1] = ((unsigned char*)&usTarget)[1];
}

/**
* CRC16 �ڵ� Endian Ÿ�� ��ȯ
* @param pucDataBlock ������ �� ������
* @param usSizeOfDataBlock ������ �� ũ��
*/
void CCRC16::ChangeCRCBytesLittleEndian(unsigned char* pucDataBlock, unsigned short usSizeOfDataBlock)
{
	unsigned char   ucTemp;
	ucTemp = pucDataBlock[usSizeOfDataBlock - 2];
	pucDataBlock[usSizeOfDataBlock - 2] = pucDataBlock[usSizeOfDataBlock - 1];
	pucDataBlock[usSizeOfDataBlock - 1] = ucTemp;
}