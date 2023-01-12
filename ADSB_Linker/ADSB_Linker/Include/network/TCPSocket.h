// TcpSocket.h: interface for the CServerSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPSOCKET_H_INCLUDED_)
#define AFX_TCPSOCKET_H_INCLUDED_

#pragma once

/**
* TCP ���� ����.
*/
typedef enum E_TCP_CONNECTING_STATE
{
	TCP_CONNECTING = 0,
	TCP_CONNECTED,
	TCP_CONNECTION_FAIL,
};

#include <afxmt.h> //mutex
#include <WinSock2.h>
#include "Queue.h"
#define MAX_UI_QUEUE_CNT				100
#define MAX_QUEUE_DATA_CNT			100
#define MAX_QUEUE_DATA_SIZE		204800

#define THREAD_TIMEOUT					100
#define WATCH_QUEUE_ID				1
#define MAX_BUF_SIZE						81920
#define MAX_CHILDSOCKET_CNT		10

#define MAX_BL_TIMER_CNT	100
#define MAX_SEND_QUEUE_CNT	120
#define MAX_BL_BUF_SIZE		40960

#define MSG_ACK_TIME_OUT		20000
#define TIMER_USER_ID_INIT_VAL	(0xffff)


#define CLIENT_CONNECT		1
#define CLIENT_DISCONNECT	2

#define EVENT_ACCEPT		1
#define EVENT_RECV			2

/**
* (1) ���
*	- [common class] WinSock 2.2 ����� Tcp/Ip ����� ���� Ŭ����.
*	�ش� Ŭ���� ����� ���� "winsock2.h" include �� "ws2_32.lib" link �ʿ�.
*	TCP/IP ���� �� Ŭ���̾�Ʈ ���� ����. ���� ���� �� ������ ����. ������ ��/ ����.
* (2) �����������
*	- ������ ��/ ������ �ŷڼ� ����� ���������� TCP/IP ���.
*/



class CTcpSocket
{

	typedef struct SChildSocket
	{
		SOCKET	ChildSocket;
		int	iChildId;
		BYTE BUF[MAX_BUF_SIZE];
	}SChildSocket;


public:	
	CTcpSocket();
	virtual ~CTcpSocket();
	BOOL Create(/*HANDLE i_hEvent, */short i_sPort = NULL);
	BOOL Connect(int i_sPort, CString i_strAddr);
	BOOL SendData(char* i_pBuf, int i_iSize, int i_iChildId = NULL);		
	BOOL GetEventInfo(int* i_piEventType, int* i_piChildId);
	void GetRcvData(int* i_piSize, char* i_pBuf);
	void GetAcceptInfo(int* i_piConnectionType);
	void CloseChildSocket(int i_iChildID);
	void CloseAllChildSocket();
	BOOL IsConnected();
	void RetrySocketConnection();
	void SetServerInfo(CString strIP, int sPort);
	BOOL IsServerInfoOccured();
	void StartConnectThread(CTcpSocket* i_pSocket);


	BOOL InitSocket();
	BOOL CreateServerSocket(short i_sPort);
	BOOL CreateClientSocket();
	BOOL SendToClient(int i_iChildId, char* i_pBuf, int i_iSize);
	BOOL SendToServer(char* i_pBuf, int i_iSize);
	void StartWatchThread(CTcpSocket* i_pSocket);
	static UINT TcpAcceptWatchProc(LPVOID i_lParam);	
	static UINT TcpWatchProc(LPVOID i_lParam);	
	/**
	* ���� ���� ������ ����
	* @param lpParam ���� ����
	* @return unsigned int ������ ���� ��
	*/static UINT SocketConnectionThread(LPVOID i_lParam);

	/**
	* ���� ���� ������ ����
	* @param  void
	* @return void
	*/inline void CleanupConnectionThread(void);

	//inline void CleanupWatchThread(void);
	/**
	* ���� ���� ������ ���� ����
	* @param  void
	* @return void
	*/inline void TerminateConnectionThread(void);
	/**
	* ���� ���� ������ ���� ����
	* @param  void
	* @return void
	*/inline void TerminateAcceptWatchThread(void);
	/**
	* ���� ���� ������ ���� ����
	* @param  void
	* @return void
	*/inline void TerminateWatchThread(void);

	void RecvData(int i_iSize, char* i_buf, int i_iChildId = NULL); //event �߻�. �޸� �Ҵ�	
	void UpdateClientConnectionInfo(int i_iUpdateType, int i_iChildId);

private:
	
	/**
	* Ŭ������ ������ ���� �ν��Ͻ�.
	*/	
	SOCKET m_Socket;

	/**
	* ���� �����
	*/
	BYTE m_BUF[MAX_BUF_SIZE];
		
	struct SAcceptQueue
	{		
		CQueue<int> ConnectionType;		
	};

	struct SRcvQueue
	{	
		
		CQueue<int> SizeInfo;
		BYTE aucBuf[MAX_QUEUE_DATA_SIZE];
		UINT uiQueueDataFront;
		UINT uiQueueDataRear;
	};

	struct SEventQueue
	{		
		CQueue<int> EventType;
		CQueue<int> ChildInfo;
	};
	
	/**
	* ���� ������ ť
	*/	
	SRcvQueue *m_pstRcvQueue;

	/**
	* ���� ���� ť
	*/	
	SAcceptQueue *m_pstAcceptQueue;

	/**
	* ���� �̺�Ʈ ť
	*/	
	SEventQueue *m_pstEventQueue;

	/**
	* �ñ׳��� �����ϱ� ���� �ڵ�. BL�Լ� ����䱸 ���� �������� �ڵ��� ����
	*/	
	//HANDLE m_hEvent;

	/**
	* ����� Ŭ���̾�Ʈ�� �����͸� ��/�����ϱ� ���� �ڽ� ����
	*/	
	SChildSocket *m_pstChildSocket[MAX_CHILDSOCKET_CNT];
	
	/**
	* ���� �������� �̺�Ʈ�� �߻��� �ڽ� ������ ID
	*/	
	int m_iCurrentChildId;
	
	/**
	* �������� �������� �ϰ� ���Ḧ ���� ���� �÷���. ���� ���� �̺�Ʈ �߻� �ô� ���� ������ ���� �ڵ� ����
	*/	
	BOOL m_bEventLoopStopFlag;

	int  m_iChildSocketCnt;

	BOOL m_bIsClentConnect;

	/**
	* ���� ���� ������ �ڵ�
	*/
	CWinThread* m_pConnectionThread;

	/**
	* ���� ���� ������ �ڵ�
	*/
	CWinThread* m_pAcceptWatchThread;

	/**
	* ���� ���� ������ �ڵ�
	*/
	CWinThread* m_pWatchThread;

	/**
	* ���� �õ� ������ ���� �÷��� ����
	*/
	bool m_bIsConnectTryThreadRunning;

	/**
	* TCP Socket Address
	*/
	sockaddr_in m_addrTcpSocket;

	/**
	* TCP ���� ���� ���� ����
	*/
	E_TCP_CONNECTING_STATE m_eConnState;

	/**
	* TCP Ŭ���̾�Ʈ�� ����� �����ؾ��� ������ IP/Port
	*/
	short m_iServerPort;

	/**
	* �̺�Ʈ �ڵ�. ������� �ñ׳��� ������ ���� �ڵ�
	*/
	HANDLE		m_hEvent;

	/**
	* ������ ���Ḧ ���� �÷���
	*/
	BOOL		m_bThreadStopFlag;
	CString m_strServerIP;
};

#endif // !defined(AFX_TCPSOCKET_H_INCLUDED_)

