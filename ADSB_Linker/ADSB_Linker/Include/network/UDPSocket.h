#pragma once
#include <queue>
#include <WinSock2.h>
#include <afxsock.h>


#define UDP_PAYLOAD_MAX_LENGTH 4096 // PAYLOAD 최대 크기


/**
*  (1) 기능
* - UDP 통신 모듈
*/
class CUDPSocket
{
public:
	/**
	* UDP Send 전용 생성자. 해당 소켓은 Send 할 때 일회성으로 생성하고, 전송 후 닫음.
	* @param const bool bIsOnlySend Send 전용
	*/CUDPSocket(const bool bIsOnlySend);
	
	  /**
	* CUDPSocket 생성자. cMulticastIP에 빈 값을 넣으면 유니캐스트 소켓 생성
	* @param const unsigned short usPort 수신 포트
	* @param const char * cMulticatIP 멀티캐스트 IP
	*/
#ifndef UNICODE
	  CUDPSocket(const unsigned short usPort = 62710, const char* cMulticatIP = nullptr, const char* cNicCardIP = nullptr);
#else
	  CUDPSocket(const unsigned short usPort = 62710, const wchar_t* cMulticatIP = nullptr, const wchar_t* cNicCardIP = nullptr);
#endif
	/**
	* CUDPSocket 소멸자
	* @param void 
	*/virtual ~CUDPSocket(void);
	
	/**
	* 데이터 전송
	* @param const char * cPayload 데이터
	* @param const char * destinationIP 목적지 IP
	* @param const unsigned short destinationPort 목적지 Port
	* @param const int iPayloadLength 데이터 크기
	* @return bool 성공 및 실패
	*/bool SendPayload(const char* cPayload, const char* destinationIP, const unsigned short destinationPort, const int iPayloadLength = UDP_PAYLOAD_MAX_LENGTH);

protected:
	/**
	* 데이터 수신
	* @param const char * cPayload 수신 받은 데이터
	* @param const size_t uiPayloadLen 수신 받은 데이터 크기
	* @return void 
	*/
	//virtual void ReceivedPayload(const char* cPayload) = 0;
	virtual void ReceivedPayload(int nRecvSize, const char* cPayload) = 0;

private:
	/**
	* 소켓 생성. 멀티캐스트IP 미 입력 시 유니캐스트로 소켓 생성.
	* @param cMulticastIP 수신 받을 멀티캐스트 IP
	* @param usPort 수신 받을 Port
	* @return void 
	*/
#ifndef  UNICODE
	void CreateSocket(const char* cMulticastIP, const unsigned short usPort, const char* cNicCardIP = nullptr);
#else
	void CreateSocket(const wchar_t* cMulticastIP, const unsigned short usPort, const wchar_t* cNicCardIP = nullptr);
#endif
	/**
	* 소켓 송수신 스레드 생성
	* @return void 
	*/void ReceivePayloadFromSocket();
	 
	/**
	* Socket에서 데이터 가져오기 위한 스레드
	* @param lpParam 전달 인자
	* @return unsigned int 스레드 종료 값
	*/static unsigned int PushReceivedPayloadQueueThread(LPVOID lpParam);
	
	/**
	* Queue에서 수신 데이터를 꺼내서 디바이스 클래스로 전달
	* @param lpParam 전달 인자
	* @return unsigned int 스레드 종료 값
	*/static unsigned int PopReceivedPayloadFromQueueThread(LPVOID lpParam);
	 
	/**
	* 소켓 초기화
	* @return void 
	*/void InitSocket();

private:
	/**
	* 수신 소켓
	*/
	SOCKET m_receiveSocket;
	/**
	* 전송 소켓
	*/
	SOCKET m_sendSocket;
	/**
	* Queue에 동시 접근 방지
	*/
	CMutex m_payloadQueueMutex;
	/**
	* 데이터 Queue
	*/
	std::queue<char*> m_payloadQueue;
	/**
	* 데이터 Size Queue
	*/
	std::queue<int> m_payloadSizeQueue;


	/**
	* 수신된 데이터를 Queue에 저장하는 스레드
	*/
	CWinThread *m_pushReceivedPayloadThread;
	/**
	* Queue에 저장된 데이터를 빼내는 스레드
	*/
	CWinThread *m_popReceivedPayloadThread;
	/**
	* 스레드 사용 플래그 변수
	*/
	bool m_bIsThreadRunning;
	/**
	* 전송 소켓 전용 플래그
	*/
	bool m_bIsOnlySend;

	//static char* m_pReceivedPayloadBuffer;
	char m_pReceivedPayloadBuffer[UDP_PAYLOAD_MAX_LENGTH];
};
