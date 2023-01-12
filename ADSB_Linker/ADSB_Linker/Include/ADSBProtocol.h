#pragma once
#ifndef ADS_B_COMMON_PROTOCOL
#define ADS_B_COMMON_PROTOCOL

#include <string>
#include <map>
using namespace std;

#pragma pack( push, 1 )

#define SEND_PKT_SIZE 256
#define MAX_BUFFERSIZE 40960

typedef enum eMSGID
{
	MSGID_FLIGHT_DATA = 0x0010,
};

typedef enum SITEID
{
	SITE_KOTI = 1,
	SITE_KAU,
};

typedef enum SOURCE_TYPE
{
	SRC_ASTERIX_CAT021 = 1,
	SRC_JSON_PINGSTATION,
};

typedef struct stADSBCommon 
{
	unsigned char ucSiteID;
	unsigned char ucSource;
	unsigned long ulICAOAddress;
	int nSQUAWK;
	char archCallSign[8];
	double dLatitude;//위도
	double dLongitude;//경도
	double dAltitude;//고도
	double dVelocity;//속도
	double dHeading;//방향
	DWORD dwRecvTime;//수신시간

	stADSBCommon()
	{
		memset(this, 0, sizeof(stADSBCommon));
	}

	stADSBCommon(stADSBCommon &copyConstructor)
	{
		ucSiteID = copyConstructor.ucSiteID;
		ucSource = copyConstructor.ucSource;
		ulICAOAddress = copyConstructor.ulICAOAddress;
		nSQUAWK = copyConstructor.nSQUAWK;
		memcpy_s(archCallSign, 8, copyConstructor.archCallSign, 8);
		dLatitude = copyConstructor.dLatitude;//위도
		dLongitude = copyConstructor.dLongitude;//경도
		dAltitude = copyConstructor.dAltitude;//고도
		dVelocity = copyConstructor.dVelocity;//속도
		dHeading = copyConstructor.dHeading;//방향
		dwRecvTime = copyConstructor.dwRecvTime;//수신시간
	}

	stADSBCommon& operator=(const stADSBCommon &copyConstructor)
	{
		ucSiteID = copyConstructor.ucSiteID;
		ucSource = copyConstructor.ucSource;
		ulICAOAddress = copyConstructor.ulICAOAddress;
		nSQUAWK = copyConstructor.nSQUAWK;
		memcpy_s(archCallSign, 8, copyConstructor.archCallSign, 8);
		dLatitude = copyConstructor.dLatitude;//위도
		dLongitude = copyConstructor.dLongitude;//경도
		dAltitude = copyConstructor.dAltitude;//고도
		dVelocity = copyConstructor.dVelocity;//속도
		dHeading = copyConstructor.dHeading;//방향
		dwRecvTime = copyConstructor.dwRecvTime;//수신시간
		return *this;
	}

}sADSBCommon;

typedef struct sMsgHeader
{
	unsigned short uiMsgID;
	unsigned short uiMsgSRC;
	unsigned short uiMsgDES;
	unsigned short uiMsgOrder;
	unsigned short uiMsgNum;
	unsigned short uiMsgLength;
	unsigned short uiDelimiter;

	sMsgHeader()
	{
		uiMsgID = MSGID_FLIGHT_DATA;
		uiMsgSRC = 0;
		uiMsgDES = SITE_KOTI;
		uiMsgOrder = 1;
		uiMsgNum = 1;
		uiMsgLength = 0;
		uiDelimiter = 0xFFFF;
	}
};

typedef struct sFlightInfo
{
	//위도
	double dLatitude;
	//경도
	double dLongitude;
	//고도
	double dAltitude;
	//방향
	double dHeading;
	//ICAO Address
	unsigned long ulICAOAddress;
	//콜사인
	char chCallSign[8];
	//속도
	double dVelocity;
	//site ID
	unsigned short usSiteID;
	//수신시간
	DWORD dwRecvTime;
	//SSR Code
	char chSSRCode[4];
	//ORG Code
	char chORGCode[4];
	//DES Code
	char chDESCode[4];
	//항공기 종류
	char chACType[6];
	//항공사 명
	char chALName[24];
	//등록번호
	char chRegNo[6];
	//STD
	char chSTD[14];
	//STA
	char chSTA[14];
	//Reserved
	char chReserved[4];
	sFlightInfo()
	{
		dLatitude = 0.0;
		dLongitude = 0.0;
		dAltitude = 0;
		dHeading = 0.0;
		ulICAOAddress = 0;
		memset(chCallSign, 0, sizeof(chCallSign));
		dVelocity = 0.0;
		usSiteID = 0;
		dwRecvTime = 0;
		memset(chSSRCode, 0, sizeof(chSSRCode));
		memset(chORGCode, 0, sizeof(chORGCode));
		memset(chDESCode, 0, sizeof(chDESCode));
		memset(chACType, 0, sizeof(chACType));
		memset(chALName, 0, sizeof(chALName));
		memset(chRegNo, 0, sizeof(chRegNo));
		memset(chSTD, 0, sizeof(chSTD));
		memset(chSTA, 0, sizeof(chSTA));
		memset(chReserved, 0, sizeof(chReserved));
	}

	CString GetIcaoAddress()
	{
		CString strICAOAddress;
		strICAOAddress.Format(_T("%06X"), ulICAOAddress);
		return strICAOAddress;
	}

	sFlightInfo(sFlightInfo &copyInstructor)
	{
		dLatitude = copyInstructor.dLatitude;
		dLongitude = copyInstructor.dLongitude;
		dAltitude = copyInstructor.dAltitude;
		dHeading = copyInstructor.dHeading;
		ulICAOAddress = copyInstructor.ulICAOAddress;
		memcpy_s(chCallSign, sizeof(chCallSign), copyInstructor.chCallSign, sizeof(chCallSign));
		dVelocity = copyInstructor.dVelocity;
		usSiteID = copyInstructor.usSiteID;
		dwRecvTime = copyInstructor.dwRecvTime;
		memcpy_s(chSSRCode, sizeof(chSSRCode), copyInstructor.chSSRCode, sizeof(chSSRCode));
		memcpy_s(chORGCode, sizeof(chORGCode), copyInstructor.chORGCode, sizeof(chORGCode));
		memcpy_s(chDESCode, sizeof(chDESCode), copyInstructor.chDESCode, sizeof(chDESCode));
		memcpy_s(chACType, sizeof(chACType), copyInstructor.chACType, sizeof(chACType));
		memcpy_s(chALName, sizeof(chALName), copyInstructor.chALName, sizeof(chALName));
		memcpy_s(chRegNo, sizeof(chRegNo), copyInstructor.chRegNo, sizeof(chRegNo));
		memcpy_s(chSTD, sizeof(chSTD), copyInstructor.chSTD, sizeof(chSTD));
		memcpy_s(chSTA, sizeof(chSTA), copyInstructor.chSTA, sizeof(chSTA));
		memcpy_s(chReserved, sizeof(chReserved), copyInstructor.chReserved, sizeof(chReserved));
	}

	sFlightInfo& operator=(const sFlightInfo &copyInstructor)
	{
		dLatitude = copyInstructor.dLatitude;
		dLongitude = copyInstructor.dLongitude;
		dAltitude = copyInstructor.dAltitude;
		dHeading = copyInstructor.dHeading;
		ulICAOAddress = copyInstructor.ulICAOAddress;
		memcpy_s(chCallSign, sizeof(chCallSign), copyInstructor.chCallSign, sizeof(chCallSign));
		dVelocity = copyInstructor.dVelocity;
		usSiteID = copyInstructor.usSiteID;
		dwRecvTime = copyInstructor.dwRecvTime;
		memcpy_s(chSSRCode, sizeof(chSSRCode), copyInstructor.chSSRCode, sizeof(chSSRCode));
		memcpy_s(chORGCode, sizeof(chORGCode), copyInstructor.chORGCode, sizeof(chORGCode));
		memcpy_s(chDESCode, sizeof(chDESCode), copyInstructor.chDESCode, sizeof(chDESCode));
		memcpy_s(chACType, sizeof(chACType), copyInstructor.chACType, sizeof(chACType));
		memcpy_s(chALName, sizeof(chALName), copyInstructor.chALName, sizeof(chALName));
		memcpy_s(chRegNo, sizeof(chRegNo), copyInstructor.chRegNo, sizeof(chRegNo));
		memcpy_s(chSTD, sizeof(chSTD), copyInstructor.chSTD, sizeof(chSTD));
		memcpy_s(chSTA, sizeof(chSTA), copyInstructor.chSTA, sizeof(chSTA));
		memcpy_s(chReserved, sizeof(chReserved), copyInstructor.chReserved, sizeof(chReserved));
		return *this;
	}
	
	sFlightInfo(stADSBCommon &copyInstructor)
	{
		dLatitude = copyInstructor.dLatitude;
		dLongitude = copyInstructor.dLongitude;
		dAltitude = (int)copyInstructor.dAltitude;
		dHeading = copyInstructor.dHeading;
		ulICAOAddress = copyInstructor.ulICAOAddress;
		memcpy_s(chCallSign, sizeof(chCallSign), copyInstructor.archCallSign, sizeof(copyInstructor.archCallSign));
		dVelocity = copyInstructor.dVelocity;
		dwRecvTime = copyInstructor.dwRecvTime;
		memset(chSSRCode, 0, sizeof(chSSRCode));
		memset(chORGCode, 0, sizeof(chORGCode));
		memset(chDESCode, 0, sizeof(chDESCode));
		memset(chACType, 0, sizeof(chACType));
		memset(chALName, 0, sizeof(chALName));
		memset(chRegNo, 0, sizeof(chRegNo));
		memset(chSTD, 0, sizeof(chSTD));
		memset(chSTA, 0, sizeof(chSTA));
		memset(chReserved, 0, sizeof(chReserved));
	}

	sFlightInfo& operator=(const stADSBCommon  &copyInstructor)
	{
		dLatitude = copyInstructor.dLatitude;
		dLongitude = copyInstructor.dLongitude;
		dAltitude = (int)copyInstructor.dAltitude;
		dHeading = copyInstructor.dHeading;
		ulICAOAddress = copyInstructor.ulICAOAddress;
		memcpy_s(chCallSign, sizeof(chCallSign), copyInstructor.archCallSign, sizeof(copyInstructor.archCallSign));
		dVelocity = copyInstructor.dVelocity;
		dwRecvTime = copyInstructor.dwRecvTime;
		memset(chSSRCode, 0, sizeof(chSSRCode));
		memset(chORGCode, 0, sizeof(chORGCode));
		memset(chDESCode, 0, sizeof(chDESCode));
		memset(chACType, 0, sizeof(chACType));
		memset(chALName, 0, sizeof(chALName));
		memset(chRegNo, 0, sizeof(chRegNo));
		memset(chSTD, 0, sizeof(chSTD));
		memset(chSTA, 0, sizeof(chSTA));
		memset(chReserved, 0, sizeof(chReserved));
		return *this;
	}
};

typedef struct sADSBPacket
{
	sMsgHeader stHeader;
	sFlightInfo stPayload;
	uint16_t uiCRC16;
	sADSBPacket()
	{
		uiCRC16 = 0;
	}
};

typedef struct sFlightDisp
{
	//위도
	double dLatitude;
	//경도
	double dLongitude;
	//고도
	double dAltitude;
	//방향
	double dHeading;
	//ICAO Address
	unsigned long ulICAOAddress;
	//콜사인
	CString strCallSign;
	//속도
	double dVelocity;
	//site id
	unsigned short usSiteID;
	//수신시간
	DWORD dwRecvTime;
	//SSR Code
	CString strSSRCode;
	//ORG Code
	CString strORGCode;
	//DES Code
	CString strDESCode;
	//항공기 종류
	CString strACType;
	//항공사 명
	CString strALName;
	//등록번호
	CString strRegNo;
	//STD
	CString strSTD;
	//STA
	CString strSTA;

	sFlightDisp()
	{
		dLatitude = 0.0;
		dLongitude = 0.0;
		dAltitude = 0;
		dHeading = 0.0;
		ulICAOAddress = 0;
		strCallSign = _T("");
		dVelocity = 0.0;
		usSiteID = 0;
		dwRecvTime = 0;
		strSSRCode = _T("");
		strORGCode = _T("");
		strDESCode = _T("");
		strACType = _T("");
		strALName = _T("");
		strRegNo = _T("");
		strSTD = _T("");
		strSTA = _T("");
	}

	CString GetIcaoAddress()
	{
		CString strICAOAddress;
		strICAOAddress.Format(_T("%06X"), ulICAOAddress);
		return strICAOAddress;
	}

	CString GetInsertStrSQL()
	{
		CString strSqlInsert = _T("");
		CString strTmp;
		CTime timeUTC = dwRecvTime;
		strTmp = timeUTC.Format(_T("%04Y-%02m-%02d-%02H:%02M:%02S"));
		strSqlInsert = _T(" (") + strTmp + _T(", ");
		strSqlInsert = strSqlInsert + strCallSign + _T(", ");
		strSqlInsert = strSqlInsert + GetIcaoAddress() + _T(", ");
		strTmp.Format(_T("%f"), dLatitude);
		strSqlInsert = strSqlInsert + strTmp + _T(", ");
		strTmp.Format(_T("%f"), dLongitude);
		strSqlInsert = strSqlInsert + strTmp + _T(", ");
		strTmp.Format(_T("%f"), dAltitude);
		strSqlInsert = strSqlInsert + strTmp + _T(", ");
		strTmp.Format(_T("%f"), dHeading);
		strSqlInsert = strSqlInsert + strTmp + _T(", ");
		strTmp.Format(_T("%f"), dVelocity);
		strSqlInsert = strSqlInsert + strTmp + _T(", ");
		strTmp.Format(_T("%d"), usSiteID);
		strSqlInsert = strSqlInsert + strTmp + _T(" )");
		return strSqlInsert;
	}

// 	sFlightDisp(sFlightDisp &copyInstructor)
// 	{
// 		dLatitude = copyInstructor.dLatitude;
// 		dLongitude = copyInstructor.dLongitude;
// 		nAltitude = copyInstructor.nAltitude;
// 		dHeading = copyInstructor.dHeading;
// 		ulICAOAddress = copyInstructor.ulICAOAddress;
// 		strCallSign = copyInstructor.strCallSign;
// 		dVelocity = copyInstructor.dVelocity;
// 		dwRecvTime = copyInstructor.dwRecvTime;
// 		strSSRCode = copyInstructor.strSSRCode;
// 		strORGCode = copyInstructor.strORGCode;
// 		strDESCode = copyInstructor.strDESCode;
// 		strACType = copyInstructor.strACType;
// 		strALName = copyInstructor.strALName;
// 		strRegNo = copyInstructor.strRegNo;
// 		strSTD = copyInstructor.strSTD;
// 		strSTA = copyInstructor.strSTA;
// 	}
// 
	sFlightDisp& operator=(const sFlightDisp &copyInstructor)
	{
		dLatitude = copyInstructor.dLatitude;
		dLongitude = copyInstructor.dLongitude;
		dAltitude = copyInstructor.dAltitude;
		dHeading = copyInstructor.dHeading;
		ulICAOAddress = copyInstructor.ulICAOAddress;
		strCallSign = copyInstructor.strCallSign;
		dVelocity = copyInstructor.dVelocity;
		usSiteID = copyInstructor.usSiteID;
		dwRecvTime = copyInstructor.dwRecvTime;
		strSSRCode = copyInstructor.strSSRCode;
		strORGCode = copyInstructor.strORGCode;
		strDESCode = copyInstructor.strDESCode;
		strACType = copyInstructor.strACType;
		strALName = copyInstructor.strALName;
		strRegNo = copyInstructor.strRegNo;
		strSTD = copyInstructor.strSTD;
		strSTA = copyInstructor.strSTA;
		return *this;
	}
// 
// 	sFlightDisp(sFlightInfo &copyInstructor)
// 	{
// 		dLatitude = copyInstructor.dLatitude;
// 		dLongitude = copyInstructor.dLongitude;
// 		nAltitude = copyInstructor.nAltitude;
// 		dHeading = copyInstructor.dHeading;
// 		ulICAOAddress = copyInstructor.ulICAOAddress;
// 		strCallSign.Format(_T("%s"), copyInstructor.chCallSign);
// 		dVelocity = copyInstructor.dVelocity;
// 		dwRecvTime = copyInstructor.dwRecvTime;
// 		strSSRCode.Format(_T("%s"), copyInstructor.chSSRCode);
// 		strORGCode.Format(_T("%s"), copyInstructor.chORGCode);
// 		strDESCode.Format(_T("%s"), copyInstructor.chDESCode);
// 		strACType.Format(_T("%s"), copyInstructor.chACType);
// 		strALName.Format(_T("%s"), copyInstructor.chALName);
// 		strRegNo.Format(_T("%s"), copyInstructor.chRegNo);
// 		strSTD.Format(_T("%s"), copyInstructor.chSTD);
// 		strSTA.Format(_T("%s"), copyInstructor.chSTA);
// 	}
// 
	sFlightDisp& operator=(const sFlightInfo &copyInstructor)
	{
		CString strTmp;
		dLatitude = copyInstructor.dLatitude;
		dLongitude = copyInstructor.dLongitude;
		dAltitude = copyInstructor.dAltitude;
		dHeading = copyInstructor.dHeading;
		ulICAOAddress = copyInstructor.ulICAOAddress;
		//strCallSign.Format(_T("%s"), copyInstructor.chCallSign);
		strTmp = copyInstructor.chCallSign;
		strCallSign = strTmp.Left(8);
		dVelocity = copyInstructor.dVelocity;
		usSiteID = copyInstructor.usSiteID;
		dwRecvTime = copyInstructor.dwRecvTime;
		//strSSRCode.Format(_T("%s"), copyInstructor.chSSRCode);
		strTmp = copyInstructor.chSSRCode;
		strSSRCode = strTmp.Left(4);
		//strORGCode.Format(_T("%s"), copyInstructor.chORGCode);
		strTmp = copyInstructor.chORGCode;
		strORGCode = strTmp.Left(4);
		//strDESCode.Format(_T("%s"), copyInstructor.chDESCode);
		strTmp = copyInstructor.chDESCode;
		strDESCode = strTmp.Left(4);
		//strACType.Format(_T("%s"), copyInstructor.chACType);
		strTmp = copyInstructor.chACType;
		strACType = strTmp.Left(4);
		//strALName.Format(_T("%s"), copyInstructor.chALName);
		strTmp = copyInstructor.chALName;
		strALName = strTmp.Left(4);
		//strRegNo.Format(_T("%s"), copyInstructor.chRegNo);
		strTmp = copyInstructor.chRegNo;
		strRegNo = strTmp.Left(4);
		//strSTD.Format(_T("%s"), copyInstructor.chSTD);
		strTmp = copyInstructor.chSTD;
		strSTD = strTmp.Left(4);
		//strSTA.Format(_T("%s"), copyInstructor.chSTA);
		strTmp = copyInstructor.chSTA;
		strSTA = strTmp.Left(4);
		return *this;
	}
// 
// 	sFlightDisp(stADSBCommon &copyInstructor)
// 	{
// 		dLatitude = copyInstructor.dLatitude;
// 		dLongitude = copyInstructor.dLongitude;
// 		nAltitude = (int)copyInstructor.dAltitude;
// 		dHeading = copyInstructor.dHeading;
// 		ulICAOAddress = copyInstructor.ulICAOAddress;
// 		strCallSign.Format(_T("%s"), copyInstructor.archCallSign);
// 		dVelocity = copyInstructor.dVelocity;
// 		dwRecvTime = copyInstructor.dwRecvTime;
// 		strSSRCode = _T("");
// 		strORGCode = _T("");
// 		strDESCode = _T("");
// 		strACType = _T("");
// 		strALName = _T("");
// 		strRegNo = _T("");
// 		strSTD = _T("");
// 		strSTA = _T("");
// 	}
// 
	sFlightDisp& operator=(const stADSBCommon  &copyInstructor)
	{
		CString strTmp;
		dLatitude = copyInstructor.dLatitude;
		dLongitude = copyInstructor.dLongitude;
		dAltitude = (int)copyInstructor.dAltitude;
		dHeading = copyInstructor.dHeading;
		ulICAOAddress = copyInstructor.ulICAOAddress;
		//strCallSign.Format(_T("%s"), copyInstructor.archCallSign);
		strTmp = copyInstructor.archCallSign;
		strCallSign = strTmp.Left(8);
		dVelocity = copyInstructor.dVelocity;
		dwRecvTime = copyInstructor.dwRecvTime;
		strSSRCode = _T("");
		strORGCode = _T("");
		strDESCode = _T("");
		strACType = _T("");
		strALName = _T("");
		strRegNo = _T("");
		strSTD = _T("");
		strSTA = _T("");
		return *this;
	}
};

// typedef struct stADSBCommonJSONParser
// {
// 	const char* type;
// 	std::string data;
// 
// 	bool Null() { type = "NULL"; data.clear(); return true; }
// 
// 	BOOL SiteID(
// 
// 	unsigned char ucSiteID;
// 	unsigned char ucSource;
// 	unsigned long ulICAOAddress;
// 	int nSQUAWK;
// 	char archCallSign[8];
// 	double dLatitude;//위도
// 	double dLongitude;//경도
// 	double dAltitude;//고도
// 	int nVelocity;//속도
// 	double dHeading;//방향
// 	DWORD dwRecvTime;//수신시간
// 
// 	stADSBCommonJSONParser() : type(), data()
// 	{
// 	}
// }sADSBCommonJSONParser;

typedef struct stADSBCommonDisp
{
	stADSBCommon Common;
	DWORD dwUpdateTime;//업데이트시간
}sADSBCommonDisp;

// typedef struct stADSB_StoreDB
// {
// 	stADSBCommon Common;
// 	int nPktSize;
// 	char arOriginalData[SEND_PKT_SIZE];
// }sADSB_StoreDB;

//typedef struct stTCPHeader
//{
//	unsigned char ucSiteID;
//	unsigned char ucReserved;
//	stTCPHeader()
//	{
//		ucSiteID = 0;
//		ucReserved = 0xff;
//	}
//}sTCPHeader;


typedef struct stMode3A_Code
{
	unsigned short D : 3;
	unsigned short C : 3;
	unsigned short B : 3;
	unsigned short A : 3;
	unsigned short spare : 4;

	stMode3A_Code()
	{
		memset(this, 0, sizeof(stMode3A_Code));
	}

	CString GetRecvString()
	{
		CString strReturn;
		CString strTmp;

		strTmp.Format(L"Mode 3/A Code\r\n");
		strReturn += strTmp;
		strTmp.Format(L"%d = A\r\n", A);
		strReturn += strTmp;
		strTmp.Format(L"%d = B\r\n", B);
		strReturn += strTmp;
		strTmp.Format(L"%d = C\r\n", C);
		strReturn += strTmp;
		strTmp.Format(L"%d = D\r\n", D);
		strReturn += strTmp;

		return strReturn;
	}

	int  GetDecimalValue()
	{
		int nReturn = 0;

		nReturn += A * 512;
		nReturn += B * 64;
		nReturn += C * 8;
		nReturn += D * 1;

		return nReturn;
	}

	int  GetOctalValue()
	{
		int nReturn = 0;

		nReturn += A * 1000;
		nReturn += B * 100;
		nReturn += C * 10;
		nReturn += D * 1;

		return nReturn;
	}
};

typedef map <unsigned long, sADSBCommonDisp>	MAP_ADSB;
typedef MAP_ADSB::iterator		MAP_ADSB_ITR;

// typedef map <CString, sFlightDisp> MAP_ADSB_FLIGHT;
// typedef MAP_ADSB_FLIGHT::iterator MAP_ADSB_FLIGHT_ITR;

typedef map <unsigned long, sFlightDisp> MAP_ADSB_FLIGHT;
typedef MAP_ADSB_FLIGHT::iterator MAP_ADSB_FLIGHT_ITR;

typedef map <unsigned long, CString> MAP_ADSB_CALLSIGN;
typedef MAP_ADSB_CALLSIGN::iterator MAP_ADSB_CALLSIGN_ITR;

//JSON 디코드 결과 수신 콜백 함수
//stResult	: 디코딩 프로토콜(디코딩 결과)
typedef void(*LPDECODERESULTPROC)(sADSBCommon stResult, LPVOID pParam);

//sADSBPacket 수신결과(TCP) 수신 콜백 함수
//stResult	: 디코딩 프로토콜(디코딩 결과)
typedef void(*LPPACKETRECVPROC)(char* pRecvPkt, LPVOID pParam);
#pragma pack( pop )
#endif