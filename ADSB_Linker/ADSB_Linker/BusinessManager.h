#pragma once
#include <afxwin.h>
#include "TCPSocket.h"
#include "ADSBProtocol.h"
#include "DecoderADSB.h"

//class CBusinessManager : public CWnd, public CTCPSocket
class CBusinessManager : public CWnd//, public CTcpSocket
{
	DECLARE_DYNCREATE(CBusinessManager)
public:
	CBusinessManager();
	~CBusinessManager();

	void ReceivedData(const char* cReceivedData, const size_t uiDataLen);

	BOOL SetSiteID(int nSiteID);
	int GetSiteID();

	afx_msg LRESULT RcvAndDecodeAsterixProcess(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT RcvAndDecodeJSONPingStationProcess(WPARAM wParam, LPARAM lParam);
	BOOL SendRcvInfoOfADSB(stADSBCommon stADSB_Info);
	afx_msg LRESULT RcvAndDeBug(WPARAM wParam, LPARAM lParam);

	static void RcvCallBackJSON_DecodeResult(sADSBCommon stResult, LPVOID pParam);

	DECLARE_MESSAGE_MAP()
public:
	BOOL MakeConnection();
	void SetSendIPandPortForTCP(CString strIP, int nPortOriginData, int nPortDispData);
	BOOL IsTCPConnect();

	int m_nSiteID;
#ifdef TCP_Intergrated
	CTcpSocket* m_pTcpOriginalDataSend;
	CTcpSocket* m_pTcpDisplayDataSend;
#else
	CTCPSocket* m_pTcpOriginalDataSend;
	CTCPSocket* m_pTcpDisplayDataSend;
#endif
	CString m_strDB_IP;
	int m_nPortOriginData;
	int m_nPortDispData; 
};

