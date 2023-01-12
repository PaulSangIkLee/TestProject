#pragma once
#include "UDPSocket.h"
#include "EndianControl.h"
#include "BusinessManager.h"

#pragma pack( push, 1 )

#pragma pack( pop )

class CListnerJSONPingStation :
	public CUDPSocket

{
public:
	CListnerJSONPingStation(int port, CString strIP, CString strNICCardIP);
	~CListnerJSONPingStation();

	void ReceivedPayload(int nRecvSize, const char* cRecvPayload);
	unsigned int m_nPktCnt;

	//CAsterixBuffer m_AsterixBuffer;
	//CDecoderAsterix m_DecoderAsterix;

	void SetBusinessManager(CBusinessManager* pBusinessMng);

	CBusinessManager* m_pBusinessManager;
};

