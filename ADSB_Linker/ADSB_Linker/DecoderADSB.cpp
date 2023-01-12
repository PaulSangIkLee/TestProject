#include "stdafx.h"
#include "DecoderADSB.h"

// IMPLEMENT_DYNCREATE(CDecoderAsterix, CWnd)
// 
// BEGIN_MESSAGE_MAP(CDecoderAsterix, CWnd)
// 	//{{AFX_MSG_MAP(Cxxxx)
// 	ON_MESSAGE(WM_SEND_ASTERIXMSG, ReceivedMessage)
// 	//}}AFX_MSG_MAP
// END_MESSAGE_MAP()

CDecoderADSB::CDecoderADSB()
{
// 	CString strName = AfxRegisterWndClass(NULL);
// 	CWnd::CreateEx(0, strName, NULL, 0, CRect(0, 0, 0, 0), NULL, NULL);
	m_fpDecodeResult = NULL;
}


CDecoderADSB::~CDecoderADSB()
{
	
}

sADSBCommon CDecoderADSB::GetDecodeCompleteAsterix()
{
	return m_stADSBCommon;
}

//LRESULT CDecoderAsterix::ReceivedMessage(WPARAM wParam, LPARAM lParam)
BOOL CDecoderADSB::DecodeAsterix021(CAsterixBuffer AsterixBuffer)
{
// 	CAsterixBuffer AsterixBuffer;
// 	AsterixBuffer = *((CAsterixBuffer*)wParam);
// 
// 	int nAsterixCategory = 0;
// 	nAsterixCategory = *((int*)lParam);
	BOOL bRetval = FALSE;

	CAsterixDecode021 decodeCat021;
	if (decodeCat021.DecodeMessage(AsterixBuffer) == TRUE)
	{
		ADSBTargetReportsMessage sDecodeResult;
		sDecodeResult = decodeCat021.GetDecodeResult();

		sADSBCommon sCommonProtocol;

		//SITE ID
		sCommonProtocol.ucSiteID = SITE_KAU;

		//SOURCE_TYpe
		sCommonProtocol.ucSource = SRC_ASTERIX_CAT021;

		//ICAO
		sCommonProtocol.ulICAOAddress = sDecodeResult.m_080u32TargetAddress;
		
		//SQAWK
		stMode3A_Code sQUAWK;
		memcpy(&sQUAWK, &sDecodeResult.m_070u16Mode3ACode, sizeof(stMode3A_Code));
		sCommonProtocol.nSQUAWK = sQUAWK.GetOctalValue();
		
		//CallSign
		memcpy(sCommonProtocol.archCallSign, sDecodeResult.m_170s8TargetIdentification, sizeof(char) * 8);
		
		//Lat - 위도
		sCommonProtocol.dLatitude = sDecodeResult.m_131sHighResolutionPositionInWGS84Coord.f64X;
		
		//Lon - 경도
		sCommonProtocol.dLongitude = sDecodeResult.m_131sHighResolutionPositionInWGS84Coord.f64Y;
		
		//Altitude
		sCommonProtocol.dAltitude = sDecodeResult.m_140s32GeometricHeight;

		//Speed
		sCommonProtocol.dVelocity = sDecodeResult.m_160sAirborneGroundVector.f64X * 1852.0; 
		//Heading
		sCommonProtocol.dHeading = sDecodeResult.m_160sAirborneGroundVector.f64Y;
	
		CTime timeHHMMSS;
		timeHHMMSS = (sDecodeResult.m_073u32TimeOfMessageReceptionForPosition / 1000.0);
		CTimeSpan timeDiffHour(0,9,0,0);

		CTime tiCurTime = CTime::GetCurrentTime();
		CTime tDate(tiCurTime.GetYear(), tiCurTime.GetMonth(), tiCurTime.GetDay(), timeHHMMSS.GetHour(), timeHHMMSS.GetMinute(), timeHHMMSS.GetSecond());
		tDate = tDate - timeDiffHour;

		sCommonProtocol.dwRecvTime = tDate.GetTime();
		m_stADSBCommon = sCommonProtocol;

		bRetval = TRUE;
	}

	return bRetval;
//	return 0;
}

// sADSBCommon CDecoderADSB::GetDecodeCompleteJSON()
// {
// 	return m_stADSBCommon;
// }

BOOL CDecoderADSB::SetCallBackFuncAnnounce(LPDECODERESULTPROC fp, LPVOID pParam)
{
	BOOL bRetVal = FALSE;
	if (m_fpDecodeResult == NULL)
	{
		m_fpDecodeResult = fp;
		m_pParam = pParam;

		bRetVal = TRUE;
	}
	return bRetVal;
}

BOOL CDecoderADSB::DecodeJSON(const char* pRecvPkt, int nPktSize)
{
	BOOL bRetVal = FALSE;

	CJSONParserPingStation decodeJSON;
	if(decodeJSON.SetCallBackFuncForRcvResult(RcvCallBackDecodeResult, this))
	{
		if (decodeJSON.DecodeJSON(pRecvPkt, nPktSize) == TRUE)
		{
			bRetVal = TRUE;
		}
	}

	return bRetVal;
}

void CDecoderADSB::RcvCallBackDecodeResult(sADSBCommon stResult, LPVOID pParam)
{
	CDecoderADSB* pThis = (CDecoderADSB*)pParam;
	if(pThis->m_fpDecodeResult)
	{
		(*pThis->m_fpDecodeResult)(stResult, pThis->m_pParam);
	}
}
