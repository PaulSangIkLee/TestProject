#pragma once

#include "AsterixDecode021.h"
#include "JSONParserPingStation.h"
#include "ADSBProtocol.h"
#include "BusinessManager.h"

class CDecoderADSB/* : public CWnd*/
{
	//DECLARE_DYNCREATE(CDecoderAsterix)
public:
	CDecoderADSB();
	~CDecoderADSB();

	//afx_msg LRESULT ReceivedMessage(WPARAM wParam, LPARAM lParam);
	//DECLARE_MESSAGE_MAP()
	sADSBCommon GetDecodeCompleteAsterix();
	BOOL DecodeAsterix021(CAsterixBuffer AsterixBuffer);

	//sADSBCommon GetDecodeCompleteJSON();
	BOOL SetCallBackFuncAnnounce(LPDECODERESULTPROC fp, LPVOID pParam);
	BOOL DecodeJSON(const char* pRecvPkt , int nPktSize);

	static void RcvCallBackDecodeResult(sADSBCommon stResult, LPVOID pParam);

	LPDECODERESULTPROC	m_fpDecodeResult;
	LPVOID m_pParam;
private:
	sADSBCommon m_stADSBCommon;
};

