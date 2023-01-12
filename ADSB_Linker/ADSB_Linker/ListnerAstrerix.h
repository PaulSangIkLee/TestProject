#pragma once
#include "UDPSocket.h"
#include "EndianControl.h"
#include "AsterixBuffer.h"
//#include "DecoderAsterix.h"
#include "BusinessManager.h"

#pragma pack( push, 1 )
typedef struct stAsterixHeader
{
	char CAT;
	short LEN;

	stAsterixHeader()
	{
		memset(this, 0, sizeof(stAsterixHeader));
	}

	CString GetRecvString()
	{
		CString strReturn;
		CString strTmp;

		strTmp.Format(_T("*** Header ***\r\n"));
		strReturn += strTmp;
		strTmp.Format(_T("Category : %03d\r\n"), CAT);
		strReturn += strTmp;
		strTmp.Format(_T("Length : %d\r\n"), LEN);
		strReturn += strTmp;

		return strReturn;
	}

	void SwitchEndian()
	{
		CEndianControl::SwitchEndian(&LEN, sizeof(LEN));
	}
};
#pragma pack( pop )
class CListnerAstrerix :
	public CUDPSocket

{
public:
	CListnerAstrerix(int port, CString strIP, CString strNICCardIP);
	~CListnerAstrerix();

	void ReceivedPayload(int nRecvSize, const char* cRecvPayload);
	unsigned int m_nPktCnt;

	//CAsterixBuffer m_AsterixBuffer;
	//CDecoderAsterix m_DecoderAsterix;

	void SetBusinessManager(CBusinessManager* pBusinessMng);

	CBusinessManager* m_pBusinessManager;
};

