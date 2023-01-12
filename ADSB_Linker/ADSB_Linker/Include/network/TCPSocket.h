// TcpSocket.h: interface for the CServerSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPSOCKET_H_INCLUDED_)
#define AFX_TCPSOCKET_H_INCLUDED_

#pragma once

/**
* TCP 연결 상태.
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
* (1) 기능
*	- [common class] WinSock 2.2 기반의 Tcp/Ip 통신을 위한 클래스.
*	해당 클래스 사용을 위해 "winsock2.h" include 및 "ws2_32.lib" link 필요.
*	TCP/IP 서버 및 클라이언트 소켓 생성. 연결 감시 및 데이터 감시. 데이터 송/ 수신.
* (2) 설계결정사항
*	- 데이터 송/ 수신은 신뢰성 기반의 프로토콜인 TCP/IP 사용.
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
	* 소켓 연결 스레드 시작
	* @param lpParam 전달 인자
	* @return unsigned int 스레드 종료 값
	*/static UINT SocketConnectionThread(LPVOID i_lParam);

	/**
	* 소켓 연결 스레드 종료
	* @param  void
	* @return void
	*/inline void CleanupConnectionThread(void);

	//inline void CleanupWatchThread(void);
	/**
	* 소켓 연결 스레드 강제 종료
	* @param  void
	* @return void
	*/inline void TerminateConnectionThread(void);
	/**
	* 소켓 연결 스레드 강제 종료
	* @param  void
	* @return void
	*/inline void TerminateAcceptWatchThread(void);
	/**
	* 소켓 연결 스레드 강제 종료
	* @param  void
	* @return void
	*/inline void TerminateWatchThread(void);

	void RecvData(int i_iSize, char* i_buf, int i_iChildId = NULL); //event 발생. 메모리 할당	
	void UpdateClientConnectionInfo(int i_iUpdateType, int i_iChildId);

private:
	
	/**
	* 클래스에 유일한 소켓 인스턴스.
	*/	
	SOCKET m_Socket;

	/**
	* 수신 저장소
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
	* 수신 데이터 큐
	*/	
	SRcvQueue *m_pstRcvQueue;

	/**
	* 연결 정보 큐
	*/	
	SAcceptQueue *m_pstAcceptQueue;

	/**
	* 소켓 이벤트 큐
	*/	
	SEventQueue *m_pstEventQueue;

	/**
	* 시그널을 전달하기 위한 핸들. BL함수 실행요구 감시 쓰레드의 핸들을 저장
	*/	
	//HANDLE m_hEvent;

	/**
	* 연결된 클라이언트와 데이터를 송/수신하기 위한 자식 소켓
	*/	
	SChildSocket *m_pstChildSocket[MAX_CHILDSOCKET_CNT];
	
	/**
	* 현재 시점에서 이벤트를 발생한 자식 소켓의 ID
	*/	
	int m_iCurrentChildId;
	
	/**
	* 실행중인 쓰레드의 일괄 종료를 위한 종료 플래그. 연결 해제 이벤트 발생 시는 로컬 변수에 의한 자동 해제
	*/	
	BOOL m_bEventLoopStopFlag;

	int  m_iChildSocketCnt;

	BOOL m_bIsClentConnect;

	/**
	* 소켓 연결 스레드 핸들
	*/
	CWinThread* m_pConnectionThread;

	/**
	* 소켓 연결 스레드 핸들
	*/
	CWinThread* m_pAcceptWatchThread;

	/**
	* 소켓 연결 스레드 핸들
	*/
	CWinThread* m_pWatchThread;

	/**
	* 연결 시도 스레드 구동 플래그 변수
	*/
	bool m_bIsConnectTryThreadRunning;

	/**
	* TCP Socket Address
	*/
	sockaddr_in m_addrTcpSocket;

	/**
	* TCP 소켓 연결 상태 변수
	*/
	E_TCP_CONNECTING_STATE m_eConnState;

	/**
	* TCP 클라이언트로 실행시 접속해야할 서버의 IP/Port
	*/
	short m_iServerPort;

	/**
	* 이벤트 핸들. 쓰레드로 시그널을 보내기 위한 핸들
	*/
	HANDLE		m_hEvent;

	/**
	* 쓰레드 종료를 위한 플래그
	*/
	BOOL		m_bThreadStopFlag;
	CString m_strServerIP;
};

#endif // !defined(AFX_TCPSOCKET_H_INCLUDED_)

