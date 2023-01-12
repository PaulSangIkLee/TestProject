#include "stdafx.h"
#include "ListnerAstrerix.h"
#include "AsterixDecode021.h"

CListnerAstrerix::CListnerAstrerix(int port, CString strIP, CString strNICCardIP)
: CUDPSocket(port, strIP.GetBuffer(), strNICCardIP.GetBuffer())
//: CUDPSocket(30003, "192.168.0.17", NULL)
, m_nPktCnt(0)
{
	m_pBusinessManager = NULL;
}

CListnerAstrerix::~CListnerAstrerix()
{
	printf("What?\n");
}

void CListnerAstrerix::ReceivedPayload(int nRecvSize, const char * cRecvPayload)
{
	stAsterixHeader sAsterixHeader;
	CAsterixBuffer AsterixBuffer;
	int nMaxPacketSize = 0;
	BOOL bCatChk = TRUE;

	memcpy(&sAsterixHeader, cRecvPayload, sizeof(stAsterixHeader));
	sAsterixHeader.SwitchEndian();

	if (sAsterixHeader.CAT != 21)
	{
		CString strMsg;
		strMsg.Format(L"Asterix Cat No Fail : Cat No is - %d", sAsterixHeader.CAT);
		//AfxMessageBox(strMsg);
		return;
	}

	if (nRecvSize != sAsterixHeader.LEN)
	{
		CString strMsg;
		strMsg.Format(L"Asterix Length Fail : Length is - %d, RecvSize is - %d", sAsterixHeader.LEN, nRecvSize);
		//AfxMessageBox(strMsg);
		return;
	}

	S8 sTmpValue;
	for (int i = 0; i < sAsterixHeader.LEN; i++)
	{
		sTmpValue = *(cRecvPayload + i);
		AsterixBuffer.WriteBufferS8(sTmpValue);
	}

	// 	CAsterixDecode021 DecodeCat021;
	// 	DecodeCat021.DecodeMessage(AsterixBuffer);

		//m_DecoderAsterix.SendMessage(WM_SEND_ASTERIXMSG, (WPARAM)&AsterixBuffer, (LPARAM)&sAsterixHeader.CAT);
	//	m_Asterix062Parser.SendMessage(WM_SEND_ASTERIXMSG, (WPARAM)&sRecvMessage, (LPARAM)&m_nPktCnt);
	if (m_pBusinessManager != NULL)
	{
// 		char arADSB[1024];
// 		char arPayload[1024];
// 
// 		memset(arADSB, 0, 1024);
// 		memset(arPayload, 0, 1024);
// 
// 		int nSize = sAsterixHeader.LEN;
// 		memcpy(arADSB, (char*)&AsterixBuffer, nSize);
// 		memcpy(arPayload, cRecvPayload, nSize);

		m_pBusinessManager->SendMessage(WM_SEND_ASTERIXMSG, (WPARAM)&AsterixBuffer, (LPARAM)cRecvPayload);

	}
}

void CListnerAstrerix::SetBusinessManager(CBusinessManager* pBusinessMng)
{
	if(m_pBusinessManager == NULL)
		m_pBusinessManager = pBusinessMng;
}
