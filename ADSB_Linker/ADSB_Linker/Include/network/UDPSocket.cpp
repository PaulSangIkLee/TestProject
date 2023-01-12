#include "stdafx.h"
#include "UDPSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//char* CUDPSocket::m_pReceivedPayloadBuffer = NULL;

CUDPSocket::CUDPSocket(const bool bIsOnlySend)
	: m_receiveSocket(NULL), m_sendSocket(NULL), m_bIsOnlySend(bIsOnlySend)
{
	/*
	UDP Send 전용
	해당 소켓은 Send 할 때 일회성으로 생성하고, 전송 후 닫음.
	*/
	this->InitSocket();
	
}

#ifndef UNICODE
CUDPSocket::CUDPSocket(const unsigned short usPort, const char* cMulticatIP, const char* cNicCardIP)
	: m_receiveSocket(NULL), m_pushReceivedPayloadThread(nullptr), m_popReceivedPayloadThread(nullptr),
	m_sendSocket(NULL), m_bIsOnlySend(false)
#else
CUDPSocket::CUDPSocket(const unsigned short usPort, const wchar_t* cMulticatIP, const wchar_t* cNicCardIP)
	: m_receiveSocket(NULL), m_pushReceivedPayloadThread(nullptr), m_popReceivedPayloadThread(nullptr),
	m_sendSocket(NULL), m_bIsOnlySend(false)
#endif

{
	this->InitSocket();

	this->CreateSocket(cMulticatIP, usPort, cNicCardIP);
}


CUDPSocket::~CUDPSocket()
{
	TRACE("This : 0x%x\t \n", this);
	m_bIsThreadRunning = false;
	Sleep(100);
	if(m_bIsOnlySend == false)
	{
		unsigned long ulExitCode = NULL;
		
		if(m_pushReceivedPayloadThread != nullptr)
		{
			//WaitForSingleObject(m_pushReceivedPayloadThread->m_hThread, INFINITE);
			//WaitForSingleObject(m_pushReceivedPayloadThread->m_hThread, 100);
			GetExitCodeThread(m_pushReceivedPayloadThread->m_hThread, &ulExitCode);
			TerminateThread(m_pushReceivedPayloadThread->m_hThread, ulExitCode);
			delete m_pushReceivedPayloadThread;
			m_pushReceivedPayloadThread = nullptr;
		}
// 		if (m_pushReceivedPayloadThread != nullptr)
// 		{
// 			unsigned long ulExitCode = NULL;
// 			GetExitCodeThread(m_pushReceivedPayloadThread->m_hThread, &ulExitCode);
// 			TerminateThread(m_pushReceivedPayloadThread->m_hThread, ulExitCode);
// 
// 			delete m_pushReceivedPayloadThread;
// 			m_pushReceivedPayloadThread = nullptr;
// 		}

		if(m_popReceivedPayloadThread != nullptr)
		{
			//WaitForSingleObject(m_popReceivedPayloadThread->m_hThread, INFINITE);
			//WaitForSingleObject(m_popReceivedPayloadThread->m_hThread, 100);
			GetExitCodeThread(m_popReceivedPayloadThread->m_hThread, &ulExitCode);
			TerminateThread(m_popReceivedPayloadThread->m_hThread, ulExitCode);
			delete m_popReceivedPayloadThread;
			m_popReceivedPayloadThread = nullptr;
		}
// 		if (m_popReceivedPayloadThread != nullptr)
// 		{
// 			unsigned long ulExitCode = NULL;
// 			GetExitCodeThread(m_popReceivedPayloadThread->m_hThread, &ulExitCode);
// 			TerminateThread(m_popReceivedPayloadThread->m_hThread, ulExitCode);
// 
// 			delete m_popReceivedPayloadThread;
// 			m_popReceivedPayloadThread = nullptr;
// 		}

// 		if (m_pReceivedPayloadBuffer)
// 		{
// 			delete m_pReceivedPayloadBuffer;
// 			m_pReceivedPayloadBuffer = NULL;
// 		}

		char* cPayload;
		while(m_payloadQueue.size() > 0 )
		{
			cPayload = m_payloadQueue.front();
			delete cPayload;
			m_payloadQueue.pop();

			m_payloadSizeQueue.front();
			m_payloadSizeQueue.pop();
		}

		if (m_sendSocket != NULL)
		{
			closesocket(m_sendSocket);
			m_sendSocket = NULL;
		}

		if(m_receiveSocket != NULL)
		{
			closesocket(m_receiveSocket);
			m_receiveSocket = NULL;
		}
	}
}


void CUDPSocket::InitSocket()
{
	if (AfxSocketInit() == false)
	{
		TRACE(_T("AfxSocketInit() failed.."));
		exit(1);
	}
}


#pragma warning (disable : 4996) // inet_addr -> inet_pton
bool CUDPSocket::SendPayload(const char* cPayload, const char* destinationIP, const unsigned short destinationPort, const int iPayloadLength)
{
	if (m_sendSocket == NULL)
	{
		m_sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (m_sendSocket == SOCKET_ERROR)
		{
			m_sendSocket = NULL;
			return false;
		}
	}

	sockaddr_in destinationAddress;
	memset(&destinationAddress, 0x00, sizeof(destinationAddress));
	destinationAddress.sin_family = AF_INET;
	destinationAddress.sin_port = htons(destinationPort);
	destinationAddress.sin_addr.s_addr = inet_addr(destinationIP);

	int iSendByte = sendto(m_sendSocket, cPayload, iPayloadLength, 0, (sockaddr*)&destinationAddress, sizeof(sockaddr_in));
	if (iSendByte == SOCKET_ERROR)
	{
		closesocket(m_sendSocket);
		m_sendSocket = NULL;
		return false;
	}

	return true;
}

#ifndef  UNICODE
void CUDPSocket::CreateSocket(const char* cMulticastIP, const unsigned short usPort, const char* cNicCardIP)
#else
void CUDPSocket::CreateSocket(const wchar_t* cMulticastIP, const unsigned short usPort, const wchar_t* cNicCardIP)
#endif
{
	//m_receiveSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	m_receiveSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_receiveSocket == SOCKET_ERROR)
	{
		AfxMessageBox(L"소켓생성실패", MB_OK);
		TRACE(_T("socket() failed.."));
		exit(1);
	}

	
	if (cMulticastIP != nullptr) // empty check
	{
		int iReUse = 1; // 이미 사용된 주소를 재사용 - bind
		int iSocketOpt = setsockopt(m_receiveSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&iReUse), sizeof(iReUse));
		if (iSocketOpt == SOCKET_ERROR)
		{
			AfxMessageBox(L"SOCKET_ERROR", MB_OK);
			closesocket(m_receiveSocket);
			exit(1);
		}
		/*
		struct ip_mreq sMuticastGroup;
// #ifndef KILL_DB
 		sMuticastGroup.imr_multiaddr.s_addr = inet_addr(cMulticastIP); // multicast group address
 		sMuticastGroup.imr_interface.s_addr = inet_addr(cNicCardIP); // self interface address
// #else
//		sMuticastGroup.imr_multiaddr.s_addr = inet_addr(cMulticastIP); // multicast group address
//		sMuticastGroup.imr_interface.s_addr = htonl(INADDR_ANY); // self interface address
//#endif // !KILL_DB
		
		iSocketOpt = setsockopt(m_receiveSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, reinterpret_cast<char *>(&sMuticastGroup), sizeof(sMuticastGroup));
		if (iSocketOpt == SOCKET_ERROR)
		{
			AfxMessageBox("ADD_MEMEBERSHIP failed..", MB_OK);
			TRACE(_T("ADD_MEMEBERSHIP failed.."));
			closesocket(m_receiveSocket);
			exit(1);
		}
		*/
	}
	

	struct sockaddr_in receiveAddress;
	memset(&receiveAddress, 0x00, sizeof(sockaddr_in));
	receiveAddress.sin_family = AF_INET; // IPv4
	receiveAddress.sin_port = htons(usPort);
	receiveAddress.sin_addr.s_addr = INADDR_ANY;

#ifdef UNICODE
	if (::bind(m_receiveSocket, (struct sockaddr*)&receiveAddress, sizeof(receiveAddress)) == SOCKET_ERROR)
#else // UNICODE
	if (bind(m_receiveSocket, (struct sockaddr*)&receiveAddress, sizeof(receiveAddress)) == SOCKET_ERROR)
#endif // UNICODE

	{
		AfxMessageBox(L"bind() failed..", MB_OK);
		TRACE(_T("bind() failed.."));
		closesocket(m_receiveSocket);
		exit(1);
	}

	this->ReceivePayloadFromSocket();
}

void CUDPSocket::ReceivePayloadFromSocket()
{
	m_bIsThreadRunning = true;
	m_pushReceivedPayloadThread = AfxBeginThread(PushReceivedPayloadQueueThread, reinterpret_cast<void *>(this));
	m_pushReceivedPayloadThread->m_bAutoDelete = false;

	m_popReceivedPayloadThread = AfxBeginThread(PopReceivedPayloadFromQueueThread, reinterpret_cast<void *>(this));
	m_popReceivedPayloadThread->m_bAutoDelete = false;
}

unsigned int CUDPSocket::PushReceivedPayloadQueueThread(LPVOID lpParam)
{
	CUDPSocket *pUdpSocket = reinterpret_cast<CUDPSocket *>(lpParam);

	struct sockaddr_in sockClientAddr;
	int iClientAddrSize = sizeof(sockClientAddr);
	
	fd_set fdSet;
	struct timeval timeInterval;
	timeInterval.tv_sec = 0;
	timeInterval.tv_usec = 500000; // 0.5초
	
	//char* cReceivedPayloadBuffer = new char[UDP_PAYLOAD_MAX_LENGTH];
// 	if(pUdpSocket->m_bIsThreadRunning == true)
// 		pUdpSocket->m_pReceivedPayloadBuffer = new char[UDP_PAYLOAD_MAX_LENGTH];

	//pUdpSocket->m_bIsThreadRunning = true;
	while ((pUdpSocket != nullptr) && (pUdpSocket->m_bIsThreadRunning == true))
	{
		FD_ZERO(&fdSet);
		FD_SET(pUdpSocket->m_receiveSocket, &fdSet);

		int iSocketState = 0;
		iSocketState = select(sizeof(fdSet), &fdSet, NULL, NULL, &timeInterval);
		if ((iSocketState > 0) && (FD_ISSET(pUdpSocket->m_receiveSocket, &fdSet)))
		{
			//memset(cReceivedPayloadBuffer, 0x00, sizeof(cReceivedPayloadBuffer));
			memset(pUdpSocket->m_pReceivedPayloadBuffer, 0x00, UDP_PAYLOAD_MAX_LENGTH);

			// 데이터 수신
 			int iReceivedDataByte = recvfrom(pUdpSocket->m_receiveSocket, pUdpSocket->m_pReceivedPayloadBuffer, UDP_PAYLOAD_MAX_LENGTH, 0,
 				reinterpret_cast<struct sockaddr *>(&sockClientAddr), &iClientAddrSize);

			if (iReceivedDataByte > 0)
			{
				pUdpSocket->m_payloadQueueMutex.Lock();

				pUdpSocket->m_payloadQueue.push(pUdpSocket->m_pReceivedPayloadBuffer);
				pUdpSocket->m_payloadSizeQueue.push(iReceivedDataByte);

				pUdpSocket->m_payloadQueueMutex.Unlock();
			}
		}
		Sleep(5);
	}

// 	if(m_pReceivedPayloadBuffer != nullptr)
// 	{
// 		delete[] m_pReceivedPayloadBuffer;
// 		m_pReceivedPayloadBuffer = nullptr;
// 	}


	return 0;
}

unsigned int CUDPSocket::PopReceivedPayloadFromQueueThread(LPVOID lpParam)
{
	CUDPSocket *pUdpSocket = reinterpret_cast<CUDPSocket *>(lpParam);

	//pUdpSocket->m_bIsThreadRunning = true;
	while ((pUdpSocket != nullptr) && (pUdpSocket->m_bIsThreadRunning == true))
	{
		if (pUdpSocket->m_payloadQueue.empty() == false)
		{
			pUdpSocket->m_payloadQueueMutex.Lock();

			char* cPayload = pUdpSocket->m_payloadQueue.front();
			pUdpSocket->m_payloadQueue.pop();
			
			int nRecvSize = pUdpSocket->m_payloadSizeQueue.front();
			pUdpSocket->m_payloadSizeQueue.pop();

			pUdpSocket->ReceivedPayload(nRecvSize, cPayload);

			pUdpSocket->m_payloadQueueMutex.Unlock();
		}
	}

	return 0;
}
