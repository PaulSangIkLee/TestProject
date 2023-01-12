#pragma once
#include <queue>
#include <WinSock2.h>
#include <afxsock.h>


#define UDP_PAYLOAD_MAX_LENGTH 4096 // PAYLOAD �ִ� ũ��


/**
*  (1) ���
* - UDP ��� ���
*/
class CUDPSocket
{
public:
	/**
	* UDP Send ���� ������. �ش� ������ Send �� �� ��ȸ������ �����ϰ�, ���� �� ����.
	* @param const bool bIsOnlySend Send ����
	*/CUDPSocket(const bool bIsOnlySend);
	
	  /**
	* CUDPSocket ������. cMulticastIP�� �� ���� ������ ����ĳ��Ʈ ���� ����
	* @param const unsigned short usPort ���� ��Ʈ
	* @param const char * cMulticatIP ��Ƽĳ��Ʈ IP
	*/
#ifndef UNICODE
	  CUDPSocket(const unsigned short usPort = 62710, const char* cMulticatIP = nullptr, const char* cNicCardIP = nullptr);
#else
	  CUDPSocket(const unsigned short usPort = 62710, const wchar_t* cMulticatIP = nullptr, const wchar_t* cNicCardIP = nullptr);
#endif
	/**
	* CUDPSocket �Ҹ���
	* @param void 
	*/virtual ~CUDPSocket(void);
	
	/**
	* ������ ����
	* @param const char * cPayload ������
	* @param const char * destinationIP ������ IP
	* @param const unsigned short destinationPort ������ Port
	* @param const int iPayloadLength ������ ũ��
	* @return bool ���� �� ����
	*/bool SendPayload(const char* cPayload, const char* destinationIP, const unsigned short destinationPort, const int iPayloadLength = UDP_PAYLOAD_MAX_LENGTH);

protected:
	/**
	* ������ ����
	* @param const char * cPayload ���� ���� ������
	* @param const size_t uiPayloadLen ���� ���� ������ ũ��
	* @return void 
	*/
	//virtual void ReceivedPayload(const char* cPayload) = 0;
	virtual void ReceivedPayload(int nRecvSize, const char* cPayload) = 0;

private:
	/**
	* ���� ����. ��Ƽĳ��ƮIP �� �Է� �� ����ĳ��Ʈ�� ���� ����.
	* @param cMulticastIP ���� ���� ��Ƽĳ��Ʈ IP
	* @param usPort ���� ���� Port
	* @return void 
	*/
#ifndef  UNICODE
	void CreateSocket(const char* cMulticastIP, const unsigned short usPort, const char* cNicCardIP = nullptr);
#else
	void CreateSocket(const wchar_t* cMulticastIP, const unsigned short usPort, const wchar_t* cNicCardIP = nullptr);
#endif
	/**
	* ���� �ۼ��� ������ ����
	* @return void 
	*/void ReceivePayloadFromSocket();
	 
	/**
	* Socket���� ������ �������� ���� ������
	* @param lpParam ���� ����
	* @return unsigned int ������ ���� ��
	*/static unsigned int PushReceivedPayloadQueueThread(LPVOID lpParam);
	
	/**
	* Queue���� ���� �����͸� ������ ����̽� Ŭ������ ����
	* @param lpParam ���� ����
	* @return unsigned int ������ ���� ��
	*/static unsigned int PopReceivedPayloadFromQueueThread(LPVOID lpParam);
	 
	/**
	* ���� �ʱ�ȭ
	* @return void 
	*/void InitSocket();

private:
	/**
	* ���� ����
	*/
	SOCKET m_receiveSocket;
	/**
	* ���� ����
	*/
	SOCKET m_sendSocket;
	/**
	* Queue�� ���� ���� ����
	*/
	CMutex m_payloadQueueMutex;
	/**
	* ������ Queue
	*/
	std::queue<char*> m_payloadQueue;
	/**
	* ������ Size Queue
	*/
	std::queue<int> m_payloadSizeQueue;


	/**
	* ���ŵ� �����͸� Queue�� �����ϴ� ������
	*/
	CWinThread *m_pushReceivedPayloadThread;
	/**
	* Queue�� ����� �����͸� ������ ������
	*/
	CWinThread *m_popReceivedPayloadThread;
	/**
	* ������ ��� �÷��� ����
	*/
	bool m_bIsThreadRunning;
	/**
	* ���� ���� ���� �÷���
	*/
	bool m_bIsOnlySend;

	//static char* m_pReceivedPayloadBuffer;
	char m_pReceivedPayloadBuffer[UDP_PAYLOAD_MAX_LENGTH];
};
