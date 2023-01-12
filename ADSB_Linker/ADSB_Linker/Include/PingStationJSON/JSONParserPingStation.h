#pragma once
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/encodedstream.h"    // EncodedInputStream
#include "rapidjson/reader.h"

#include "ADSBProtocol.h"
#include <algorithm>

using namespace rapidjson;
using namespace std;

class CJSONParserPingStation : public CWnd
{
//	DECLARE_DYNCREATE(CJSONParserPingStation)
public:
	CJSONParserPingStation();
	~CJSONParserPingStation();

// 	afx_msg LRESULT ReceivedMessage(WPARAM wParam, LPARAM lParam);
// 	DECLARE_MESSAGE_MAP()

	BOOL DecodeJSON(const char* pJSON, int nSentenceSize);
//	sADSBCommon GetDecodeResult();

	BOOL SetCallBackFuncForRcvResult(LPDECODERESULTPROC fp, LPVOID pParam);

	LPDECODERESULTPROC	m_fpDecodeResult;
	LPVOID m_pParam;
};

