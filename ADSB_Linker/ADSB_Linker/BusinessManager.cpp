#include "stdafx.h"
#include "BusinessManager.h"
#include "CRC16.h"

IMPLEMENT_DYNCREATE(CBusinessManager, CWnd)

BEGIN_MESSAGE_MAP(CBusinessManager, CWnd)
	//{{AFX_MSG_MAP(Cxxxx)
	ON_MESSAGE(WM_SEND_ASTERIXMSG, RcvAndDecodeAsterixProcess)
	ON_MESSAGE(WM_SEND_JSONMSG, RcvAndDecodeJSONPingStationProcess)
	ON_MESSAGE(WM_SEND_DEBUG, RcvAndDeBug)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CBusinessManager::CBusinessManager()
{
	CString strName = AfxRegisterWndClass(NULL);
	CWnd::CreateEx(0, strName, NULL, 0, CRect(0, 0, 0, 0), NULL, NULL);

	m_nSiteID = SITE_KOTI;
	m_pTcpOriginalDataSend = NULL;
	m_pTcpDisplayDataSend = NULL;
}


CBusinessManager::~CBusinessManager()
{
	if(m_pTcpOriginalDataSend)
	{
		delete m_pTcpOriginalDataSend;
		m_pTcpOriginalDataSend = NULL;
	}

	if (m_pTcpDisplayDataSend)
	{
		delete m_pTcpDisplayDataSend;
		m_pTcpDisplayDataSend = NULL;
	}
}

void CBusinessManager::ReceivedData(const char* cReceivedData, const size_t uiDataLen)
{
	
}

LRESULT CBusinessManager::RcvAndDecodeAsterixProcess(WPARAM wParam, LPARAM lParam)
{
	CAsterixBuffer AsterixBuffer;
 	AsterixBuffer = *((CAsterixBuffer*)wParam);
	
	stADSBCommon sCommonProtocol;

	CDecoderADSB DecoderAsterix;
	if (DecoderAsterix.DecodeAsterix021(AsterixBuffer) == TRUE)
	{
// 		char arADSB[1024];
// 		char arPayload[1024];
// 
// 		memset(arADSB, 0, 1024);
// 		memset(arPayload, 0, 1024);
// 
// 		int nSize = AsterixBuffer.GetLength();
// 		memcpy(arADSB, (char*)wParam, nSize);
// 		memcpy(arPayload, (char*)lParam, nSize);

		//사이트 ID 추가
		sCommonProtocol = DecoderAsterix.GetDecodeCompleteAsterix();
		sCommonProtocol.ucSiteID = m_nSiteID;
		
		//화면에 정보 전달
		::AfxGetMainWnd()->SendMessage(WM_DISP_DECODED, (WPARAM)(&sCommonProtocol), NULL);
	
		//TCP 송신
		//stADSB_StoreDB sSendPkt;
		//
		//sSendPkt.Common = sCommonProtocol;
		//memset(sSendPkt.arOriginalData, 0, SEND_PKT_SIZE);
		//int nSize = AsterixBuffer.GetLength();
		//memcpy(sSendPkt.arOriginalData, (char*)lParam, nSize);
		
		int nSize = AsterixBuffer.GetLength();
		if (m_pTcpDisplayDataSend)
		{
#ifdef TCP_Intergrated
			m_pTcpDisplayDataSend->SendData((char*)(&sCommonProtocol), sizeof(stADSBCommon));
#else
			m_pTcpDisplayDataSend->Send((char*)(&sCommonProtocol), sizeof(stADSBCommon));
#endif
		}

		SendRcvInfoOfADSB(sCommonProtocol);

// 		if (m_pTcpOriginalDataSend)
// 		{
// 			char* arSendPkt = new char[nSize + sizeof(stTCPHeader)];
// 			stTCPHeader stHeader;
// 			stHeader.ucSiteID = m_nSiteID;
// 			memcpy(arSendPkt, &stHeader, sizeof(stTCPHeader));
// 			memcpy(arSendPkt + sizeof(stTCPHeader), (char*)wParam, nSize);
// #ifdef TCP_Intergrated
// 			m_pTcpOriginalDataSend->SendData(arSendPkt, nSize + sizeof(stTCPHeader));
// #else
// 			m_pTcpOriginalDataSend->Send(arSendPkt, nSize + sizeof(stTCPHeader));
// #endif
// 			delete arSendPkt;
// 		}
	}

	return 0;
}

LRESULT CBusinessManager::RcvAndDecodeJSONPingStationProcess(WPARAM wParam, LPARAM lParam)
{
	stADSBCommon sCommonProtocol;
	int nPktSize = *((int*)lParam);

	CDecoderADSB DecoderJSON;
	DecoderJSON.SetCallBackFuncAnnounce(RcvCallBackJSON_DecodeResult, this);

	if (DecoderJSON.DecodeJSON((const char*)wParam, nPktSize) == TRUE)
	{
// 		if(m_pTcpOriginalDataSend)
// 		{
// 			char* arSendPkt = new char[nPktSize + sizeof(sMsgHeader) + sizeof(sFlightInfo)];
// 			sMsgHeader stHeader;
// 			stHeader.ucSiteID = m_nSiteID;
// 			memcpy(arSendPkt, &stHeader, sizeof(stTCPHeader));
// 			memcpy(arSendPkt + sizeof(stTCPHeader), (char*)wParam, nPktSize);
// 			
// #ifdef TCP_Intergrated
// 			if (m_pTcpOriginalDataSend->IsConnected())
// 				m_pTcpOriginalDataSend->SendData(arSendPkt, nPktSize + sizeof(stTCPHeader));
// 			else
// 				m_pTcpOriginalDataSend->StartConnectThread(m_pTcpOriginalDataSend);
// #else
// 			if (m_pTcpOriginalDataSend->IsConnected())
// 				m_pTcpOriginalDataSend->Send(arSendPkt, nPktSize + sizeof(stTCPHeader));
// 			else
// 				m_pTcpOriginalDataSend->RetrySocketConnection();
// #endif
// 				delete arSendPkt;
// 		}
	}
	
	return 0;
}

BOOL CBusinessManager::SendRcvInfoOfADSB(stADSBCommon stADSB_Info)
{
	BOOL bRetVal = FALSE;
// 	if (m_pTcpOriginalDataSend)
// 	{
// 		int nMsgSize = sizeof(sMsgHeader) + sizeof(sFlightInfo);
// 		char* arSendPkt = new char[nMsgSize];
// 		sMsgHeader stHeader;
// 		stHeader.uiMsgSRC = m_nSiteID;
// 		memcpy(arSendPkt, &stHeader, sizeof(sMsgHeader));
// 		sFlightInfo stFlightInfo(stADSB_Info);
// 		memcpy(arSendPkt + sizeof(sMsgHeader), (char*)&stFlightInfo, sizeof(sFlightInfo));
// #ifdef TCP_Intergrated
// 		m_pTcpOriginalDataSend->SendData(arSendPkt, nMsgSize);
// #else
// 		m_pTcpOriginalDataSend->Send(arSendPkt, nSize + sizeof(stTCPHeader));
// #endif
// 		delete arSendPkt;
// 	}
	if (m_pTcpOriginalDataSend)
	{
		int nMsgSize = sizeof(sADSBPacket);
		char* arSendPkt = new char[nMsgSize];
		sMsgHeader stHeader;
		stHeader.uiMsgSRC = m_nSiteID;
		memcpy(arSendPkt, &stHeader, sizeof(sMsgHeader));
		sFlightInfo stFlightInfo(stADSB_Info);
		stFlightInfo.usSiteID = m_nSiteID;
		memcpy(arSendPkt + sizeof(sMsgHeader), (char*)&stFlightInfo, sizeof(sFlightInfo));
		CCRC16::AppendCRCCheck((UCHAR*)arSendPkt, nMsgSize - sizeof(uint16_t), CCRC16::GenerateCRC16((UCHAR*)arSendPkt, nMsgSize - sizeof(uint16_t)));
#ifdef TCP_Intergrated
		m_pTcpOriginalDataSend->SendData(arSendPkt, nMsgSize);
#else
		m_pTcpOriginalDataSend->Send(arSendPkt, nSize + sizeof(stTCPHeader));
#endif
		delete arSendPkt;
	}
	return bRetVal;
}


LRESULT CBusinessManager::RcvAndDeBug(WPARAM wParam, LPARAM lParam)
{
	////////// debug //////////
// 	int nPktSize = *((int*)lParam);
// 	char* arSendPkt = new char[nPktSize + sizeof(stTCPHeader)];
// 	memset(arSendPkt, 0, nPktSize + sizeof(stTCPHeader));
// 	stTCPHeader stHeader;
// 	stHeader.ucSiteID = m_nSiteID;
// 	memcpy(arSendPkt, &stHeader, sizeof(stTCPHeader));
// 	memcpy(arSendPkt + sizeof(stTCPHeader), (char*)wParam, nPktSize);
// 
// 	m_pTcpOriginalDataSend->SendData(arSendPkt, nPktSize + sizeof(stTCPHeader));
// 	delete arSendPkt;
	////////// debug //////////
	return 0;
}

void CBusinessManager::RcvCallBackJSON_DecodeResult(sADSBCommon stResult, LPVOID pParam)
{
	CBusinessManager* pThis = (CBusinessManager*)pParam;
	stResult.ucSiteID = pThis->m_nSiteID;

	//화면에 정보 전달
	::AfxGetMainWnd()->SendMessage(WM_DISP_DECODED, (WPARAM)(&stResult), NULL);

	//화면전시 Data 전달
	if (pThis->m_pTcpDisplayDataSend)
	{
#ifdef TCP_Intergrated
		pThis->m_pTcpDisplayDataSend->SendData((char*)(&stResult), sizeof(stADSBCommon));
#else
		pThis->m_pTcpDisplayDataSend->Send((char*)(&stResult), sizeof(stADSBCommon));
#endif
	}
	pThis->SendRcvInfoOfADSB(stResult);
}

BOOL CBusinessManager::MakeConnection()
{
	BOOL bRetVal = FALSE;
	int nCnt = 0;
	if (!m_pTcpOriginalDataSend)
	{
#ifdef TCP_Intergrated
		m_pTcpOriginalDataSend = new CTcpSocket;
		//m_pTcpOriginalDataSend->Create(10000);
		
		m_pTcpOriginalDataSend->SetServerInfo(m_strDB_IP, m_nPortOriginData);
		m_pTcpOriginalDataSend->Create();
		m_pTcpOriginalDataSend->Connect(m_nPortOriginData, m_strDB_IP);
		//m_pTcpOriginalDataSend->StartConnectThread(m_pTcpOriginalDataSend);

#else
		m_pTcpOriginalDataSend = new CTCPSocket;
		m_pTcpOriginalDataSend->Connect(m_nPortOriginData, m_strDB_IP);
#endif
		
		nCnt++;
	}
	if (!m_pTcpDisplayDataSend)
	{
		//m_pTcpDisplayDataSend = new CTCPSocket;
// 		m_pTcpDisplayDataSend = new CTcpSocket;
// 		m_pTcpDisplayDataSend->Connect(m_nPortDispData, m_strDB_IP);
// 		nCnt++;
	}
//	if (nCnt > 1)
	if (nCnt > 0)
	{
		bRetVal = TRUE;
	}
	
	return bRetVal;
}

void CBusinessManager::SetSendIPandPortForTCP(CString strIP, int nPortOriginData, int nPortDispData)
{
	m_strDB_IP = strIP;
	m_nPortOriginData = nPortOriginData;
	m_nPortDispData = nPortDispData;
}

BOOL CBusinessManager::IsTCPConnect()
{
	BOOL bRetVal = FALSE;
	if (m_pTcpOriginalDataSend != NULL)
	{
		bRetVal = m_pTcpOriginalDataSend->IsConnected();
	}
	return bRetVal;
}

BOOL CBusinessManager::SetSiteID(int nSiteID)
{
	BOOL bRetVal = FALSE;
	if (nSiteID > 0)
	{
		m_nSiteID = nSiteID;
		bRetVal = TRUE;
	}

	return bRetVal;
}

int CBusinessManager::GetSiteID()
{
	return m_nSiteID;
}
