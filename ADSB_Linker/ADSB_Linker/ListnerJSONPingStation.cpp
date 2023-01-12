#include "stdafx.h"
#include "ListnerJSONPingStation.h"
#include "AsterixDecode021.h"

CListnerJSONPingStation::CListnerJSONPingStation(int port, CString strIP, CString strNICCardIP)
: CUDPSocket(port, strIP.GetBuffer(), strNICCardIP.GetBuffer())
//: CUDPSocket(30003, "192.168.0.17", NULL)
, m_nPktCnt(0)
{
	m_pBusinessManager = NULL;
}

CListnerJSONPingStation::~CListnerJSONPingStation()
{
	printf("What?\n");
}

void CListnerJSONPingStation::ReceivedPayload(int nRecvSize, const char * cRecvPayload)
{
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

		m_pBusinessManager->SendMessage(WM_SEND_JSONMSG, (WPARAM)cRecvPayload, (LPARAM)&nRecvSize);

	}
}

void CListnerJSONPingStation::SetBusinessManager(CBusinessManager* pBusinessMng)
{
	if(m_pBusinessManager == NULL)
		m_pBusinessManager = pBusinessMng;
}
