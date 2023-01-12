#include "stdafx.h"
#include "JSONParserPingStation.h"


CJSONParserPingStation::CJSONParserPingStation()
{
	m_fpDecodeResult = NULL;
}


CJSONParserPingStation::~CJSONParserPingStation()
{
}

BOOL CJSONParserPingStation::DecodeJSON(const char * pJSON, int nSentenceSize)
{
	BOOL bRetVal = FALSE;
	Document document;  // Default template parameter uses UTF8 and MemoryPoolAllocator.

	// In-situ parsing, decode strings directly in the source string. Source must be string.
	if(nSentenceSize > 0)
	{
		char* arJSONBuffer;
		arJSONBuffer = new char[nSentenceSize];
		memcpy(arJSONBuffer, pJSON, nSentenceSize);
//		if (!(document.ParseInsitu(arJSONBuffer).HasParseError()))

		std::string strParse = arJSONBuffer;
		strParse.erase(std::remove(strParse.begin(), strParse.end(), '\r'), strParse.end());
		strParse.erase(std::remove(strParse.begin(), strParse.end(), '\n'), strParse.end());
		strParse.erase(std::remove(strParse.begin(), strParse.end(), '\t'), strParse.end());
		strParse.erase(std::remove(strParse.begin(), strParse.end(), ' '), strParse.end());
		int nIndex = strParse.rfind('}');

		std::string strResult;
		strResult = strParse.substr(0, nIndex+1);

		if (!(document.Parse<0>(strResult.c_str()).HasParseError()))
		{
			Value::MemberIterator beginItr;
			Value::ConstMemberIterator memberItr;

			if(document.IsObject())
			{
				beginItr = document.FindMember("aircraft");
				if (beginItr != document.MemberEnd())
				{
					Document singleDoc;
					const Value& valAircraft = document["aircraft"];
					for (SizeType index = 0; valAircraft.Size() != index; index++)
					{
						sADSBCommon stADSBResult;
						//SOURCE_TYpe
						stADSBResult.ucSource = SRC_JSON_PINGSTATION;
						memberItr = valAircraft[index].FindMember("icaoAddress");
						if (memberItr != valAircraft[index].MemberEnd())
						{
							if (memberItr->value.IsString())
							{
								CString strTmp;
								strTmp = memberItr->value.GetString();
								strTmp.TrimLeft(_T("0x"));
								stADSBResult.ulICAOAddress = _tcstol(strTmp, NULL, 16);
								strTmp = _T("Hello");
							}
						}

						memberItr = valAircraft[index].FindMember("squawk");
						if (memberItr != valAircraft[index].MemberEnd())
						{
							if (memberItr->value.IsInt())
							{
								stADSBResult.nSQUAWK = memberItr->value.GetInt();
							}
						}

						memberItr = valAircraft[index].FindMember("callsign");
						if (memberItr != valAircraft[index].MemberEnd())
						{
							const Value& arChar = valAircraft[index]["callsign"];
							memcpy(stADSBResult.archCallSign, arChar.GetString(), arChar.GetStringLength());
						}

						memberItr = valAircraft[index].FindMember("latDD");
						if (memberItr != valAircraft[index].MemberEnd())
						{
							if (memberItr->value.IsDouble())
							{
								stADSBResult.dLatitude = memberItr->value.GetDouble();
							}
						}

						memberItr = valAircraft[index].FindMember("lonDD");
						if (memberItr != valAircraft[index].MemberEnd())
						{
							if (memberItr->value.IsDouble())
							{
								stADSBResult.dLongitude = memberItr->value.GetDouble();
							}
						}

						memberItr = valAircraft[index].FindMember("altitudeMM");
						if (memberItr != valAircraft[index].MemberEnd())
						{
							if (memberItr->value.IsInt64())
							{
								//m_stADSBCommon.nAltitude = (int)(memberItr->value.GetInt64() * 0.000001);//kilometer로 치환
								//stADSBResult.dAltitude = memberItr->value.GetInt64() * 0.001;//meter로 치환
								stADSBResult.dAltitude = memberItr->value.GetInt64() * 0.00328084;//millimeters to feet로 치환
							}
						}

						memberItr = valAircraft[index].FindMember("horVelocityCMS");
						if (memberItr != valAircraft[index].MemberEnd())
						{
							if (memberItr->value.IsInt())
							{
								//stADSBResult.nVelocity = (int)(memberItr->value.GetInt() * 0.01);//meter per second로 치환
								//stADSBResult.dVelocity = memberItr->value.GetDouble() * 0.036;//meter per second로 치환
								stADSBResult.dVelocity = memberItr->value.GetDouble() * 0.01;//meter per second로 치환
							}
						}

 						memberItr = valAircraft[index].FindMember("headingDE2");
						if (memberItr != valAircraft[index].MemberEnd())
						{
							if (memberItr->value.IsInt())
							{
								stADSBResult.dHeading = memberItr->value.GetInt() * 0.01;//degree로 치환
							}
						}

						memberItr = valAircraft[index].FindMember("timeStamp");
						if (memberItr != valAircraft[index].MemberEnd())
						{
							if (memberItr->value.IsString())
							{
								CString strTime(memberItr->value.GetString());
								strTime.Replace(L" ", NULL);
								strTime.Replace(L"T", L" ");
								strTime.Replace(L"Z", L" ");

								int nYear = _wtoi(strTime.Left(4));
								int nMon = _wtoi(strTime.Mid(5, 2));
								int nDay = _wtoi(strTime.Mid(8, 2));
								int nHour = _wtoi(strTime.Mid(11, 2));
								int nMin = _wtoi(strTime.Mid(14, 2));
								int nSec = _wtoi(strTime.Mid(17, 2));


								CTime timeHHMMSS(nYear, nMon, nDay, nHour, nMin, nSec);

								//CTime tiCurTime = CTime::GetCurrentTime();
								//CTime tDate(tiCurTime.GetYear(), tiCurTime.GetMonth(), tiCurTime.GetDay(), timeHHMMSS.GetHour() - 9, timeHHMMSS.GetMinute(), timeHHMMSS.GetSecond());

								CString str;
								str = timeHHMMSS.Format("%Y.%m.%d  %H:%M:%S");

								stADSBResult.dwRecvTime = timeHHMMSS.GetTime();
							}
						}
						else
						{
							CTime tiCurTime = CTime::GetCurrentTime();
							stADSBResult.dwRecvTime = tiCurTime.GetTime();
						}

						//수신시간과 ADSB시간이 맞지 않는 부분이 확인되어 시스템 시간을 수신시간에 추가하기로 결정(20200313)
						SYSTEMTIME curTime;
						::GetSystemTime(&curTime);
						CTime tiCurTime = CTime(curTime);
						//stADSBResult.dwRecvTime = tiCurTime.GetTime();
						

						if (m_fpDecodeResult)
						{
							(*m_fpDecodeResult)(stADSBResult, m_pParam);
						}
					}
					bRetVal = TRUE;
				}
			}
		}
		delete arJSONBuffer;
	}
	return bRetVal;
}

// sADSBCommon CJSONParserPingStation::GetDecodeResult()
// {
// 	return m_stADSBCommon;
// }

BOOL CJSONParserPingStation::SetCallBackFuncForRcvResult(LPDECODERESULTPROC fp, LPVOID pParam)
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