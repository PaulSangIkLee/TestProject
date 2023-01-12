// TcpSocket.cpp: implementation of the CServerSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TcpSocket.h"
#include <malloc.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DWORD _tinet_addr(const TCHAR *cp)
{
#ifdef UNICODE
	char IP[16];
	int Ret = 0;
	Ret = WideCharToMultiByte(CP_ACP, 0, cp, _tcslen(cp), IP, 15, NULL, NULL);
	IP[Ret] = 0;
	return inet_addr(IP);
#endif
#ifndef UNICODE
	return init_addr(cp);
#endif
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
* ������ �Լ�
* ť ���� �� WinSock ���� �ʱ�ȭ
*/
CTcpSocket::CTcpSocket()
{
	m_iCurrentChildId = 0;

	for ( int i = 0; i < MAX_CHILDSOCKET_CNT; i++ )
		m_pstChildSocket[i] = NULL;	

	m_pstAcceptQueue = new SAcceptQueue;
	m_pstEventQueue = new SEventQueue;
	m_pstRcvQueue = new SRcvQueue;
	
	m_pstRcvQueue->uiQueueDataFront = NULL;
	m_pstRcvQueue->uiQueueDataRear = NULL;

	m_bEventLoopStopFlag = FALSE;
	m_bIsClentConnect = FALSE;

	InitSocket();

	m_iChildSocketCnt = NULL;

	m_iServerPort = 0;
	m_strServerIP = "";
	m_eConnState = TCP_CONNECTING;
	m_pConnectionThread = NULL;
	
	m_pAcceptWatchThread = NULL;
	m_pWatchThread = NULL;
	
	m_bIsConnectTryThreadRunning = FALSE;
	m_bThreadStopFlag = TRUE;
}

/**
* �Ҹ��� �Լ�
* ť ���� �� WinSock ���� Ŭ����
*/
CTcpSocket::~CTcpSocket()
{
	m_bEventLoopStopFlag = TRUE;
	m_bThreadStopFlag = FALSE;
	//AfxMessageBox(L"TCP Socket �Ҹ��� ���");
	Sleep(10);
	TerminateWatchThread();

	TerminateConnectionThread();

	TerminateAcceptWatchThread();

	
// 	m_bIsConnectTryThreadRunning = FALSE;
// 	this->CleanupConnectionThread();

	Sleep(200);

	for ( int i = 0; i < MAX_CHILDSOCKET_CNT; i++ )
	{
		if ( m_pstChildSocket[i] )
		{
			delete m_pstChildSocket[i];
			m_pstChildSocket[i] = NULL;
		}
	}	
	
	m_pstRcvQueue->SizeInfo.~CQueue();
	m_pstAcceptQueue->ConnectionType.~CQueue();
	m_pstEventQueue->ChildInfo.~CQueue();

	if(m_pstRcvQueue)
	{
	delete m_pstRcvQueue;
		m_pstRcvQueue = NULL;
	}
	if(m_pstAcceptQueue)
	{
	delete m_pstAcceptQueue;
		m_pstAcceptQueue = NULL;
	}
	if(m_pstEventQueue)
	{
	delete m_pstEventQueue;
		m_pstEventQueue = NULL;
	}

	WSACleanup();
}

/**
* ���� �ʱ�ȭ
* @return TRUE(����) �Ǵ� FALSE(����)
*/
BOOL CTcpSocket::InitSocket()
{
	WORD		wVersionRequested = MAKEWORD(2, 2); 
	WSADATA		wsaData; 
 
	if ( WSAStartup(wVersionRequested, &wsaData) == SOCKET_ERROR ) 	
	{
		WSACleanup();
		return FALSE; 
	}

	if (LOBYTE(wsaData.wVersion)!= 2 || HIBYTE(wsaData.wVersion)!= 2) 
	{ 
		WSACleanup();
		return FALSE;
	}	

	return TRUE;
}
/**
* ������ ���� ������
* @param	1) i_lpData : ������ ���� �� ������ �ν��Ͻ� ������
* @return ������ ���� �� ��ȯ ��. �ǹ� ����
*/
UINT CTcpSocket::TcpWatchProc(LPVOID i_lpData)
{		
	CRITICAL_SECTION cs;

	CTcpSocket* pSocket;
	pSocket = (CTcpSocket*)i_lpData;

	SOCKET CurrentSocket;
	int iChildID;
	
	//Server�� Child
	if ( pSocket->m_iCurrentChildId )
	{
		CurrentSocket = pSocket->m_pstChildSocket[pSocket->m_iCurrentChildId-1]->ChildSocket;
		iChildID = pSocket->m_iCurrentChildId;
	}
	//Client
	else
	{
		CurrentSocket = pSocket->m_Socket;		
		iChildID = 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	//Socket Event	
	WSANETWORKEVENTS event; // Event Storage Structure
	WSAEVENT hRecvEvent = WSACreateEvent(); // Init WinSock And Return Handle	
	WSAEventSelect(CurrentSocket,hRecvEvent,FD_ACCEPT|FD_READ|FD_CLOSE|FD_WRITE);		
	////////////////////////////////////////////////////////////////////////////////////////////

	int iReceivedSize;
	DWORD dwResult;
	BOOL EventLoopStopFlag = FALSE;

    //while ( !(EventLoopStopFlag | pSocket->m_bEventLoopStopFlag) )
	while ( (!(EventLoopStopFlag | pSocket->m_bEventLoopStopFlag)) && pSocket->m_bThreadStopFlag)
    {		
		dwResult = WSAWaitForMultipleEvents( 1, &hRecvEvent, FALSE, THREAD_TIMEOUT, FALSE );

		if ( dwResult == WSA_WAIT_FAILED || dwResult == WSA_WAIT_TIMEOUT )
		{
			continue;
		}
		else if ( dwResult >= WSA_WAIT_EVENT_0 )
		{		
			WSAEnumNetworkEvents(CurrentSocket,hRecvEvent,&event);
			
			if ((event.lNetworkEvents & FD_CLOSE) == FD_CLOSE)
			{	
				closesocket(CurrentSocket);
				InitializeCriticalSection(&cs);
				EnterCriticalSection(&cs);
				
				if ( pSocket->m_iCurrentChildId != 0 )
				{					
					delete pSocket->m_pstChildSocket[iChildID-1];
					pSocket->m_pstChildSocket[iChildID-1] = NULL;
					pSocket->m_iChildSocketCnt--;
				}

				LeaveCriticalSection(&cs);
				DeleteCriticalSection(&cs);				
							
				EventLoopStopFlag = TRUE;
				pSocket->UpdateClientConnectionInfo(CLIENT_DISCONNECT, iChildID);
			}
			//Receive Event
			else if((event.lNetworkEvents & FD_READ) == FD_READ)
			{
				if ( iChildID )
				{
					iReceivedSize = recv(CurrentSocket, (char*)pSocket->m_pstChildSocket[iChildID-1]->BUF, MAX_BUF_SIZE, 0);
					//TRACE("Recv Size : %d\r\n", iReceivedSize);

					if ( SOCKET_ERROR != iReceivedSize )
						pSocket->RecvData(iReceivedSize, (char*)pSocket->m_pstChildSocket[iChildID-1]->BUF, iChildID);
					else
						TRACE("���� ������\r\n");

					if ( iReceivedSize == 0 )
						TRACE("0����\r\n");
				}
				else
				{
					iReceivedSize = recv(CurrentSocket, (char*)pSocket->m_BUF, MAX_BUF_SIZE, 0);
					//TRACE("Recv Size : %d\r\n", iReceivedSize);

					if ( SOCKET_ERROR != iReceivedSize )
						pSocket->RecvData(iReceivedSize, (char*)pSocket->m_BUF);
					else
						TRACE("���� ������\r\n");

					if ( iReceivedSize == 0 )
						TRACE("0����\r\n");
				}				
			}
			else if((event.lNetworkEvents & FD_WRITE) == FD_WRITE)
			{
				// Write Evnet
			}
			else
			{
				// etc Evnet				
			}						
		}
	}
	pSocket->m_bIsClentConnect = FALSE;
	closesocket(CurrentSocket);

	pSocket->m_eConnState = TCP_CONNECTED;
	pSocket->m_bIsClentConnect = FALSE;
	//pSocket->TerminateWatchThread();

	pSocket->RetrySocketConnection();

	return 0;
}

UINT CTcpSocket::SocketConnectionThread(LPVOID i_lpData)
{
	CRITICAL_SECTION cs;

	CTcpSocket* pTcpSocket;

	pTcpSocket = (CTcpSocket*)i_lpData;

	pTcpSocket->m_eConnState = TCP_CONNECTING;
	int EventLoopStopFlag = SOCKET_ERROR;
	BOOL bContinueLoop = TRUE;
	int iErrorCode;
	if(pTcpSocket->IsServerInfoOccured())
	{
		do
		{
			EventLoopStopFlag = connect(pTcpSocket->m_Socket, reinterpret_cast<struct sockaddr *>(&pTcpSocket->m_addrTcpSocket),
				sizeof(struct sockaddr_in));
			if (EventLoopStopFlag == SOCKET_ERROR)
			{
				iErrorCode = WSAGetLastError();
				if (WSAEWOULDBLOCK != iErrorCode)
				{
					//WSACleanup();
					//return false;
					bContinueLoop = TRUE;
				}
				else
				{
					fd_set efd;
					FD_ZERO(&efd);
					FD_SET(pTcpSocket->m_Socket, &efd);

					timeval t;
					t.tv_sec = 1;
					t.tv_usec = 0;

					int iRst = select(SO_OOBINLINE, 0, &efd, 0, &t);

					if (iRst != 1)
					{
						//WSACleanup();
						//return false;

					}
					//bContinueLoop = FALSE;
				}
			}
			Sleep(1000);
		//} while (EventLoopStopFlag == SOCKET_ERROR && pTcpSocket->m_bIsConnectTryThreadRunning == TRUE && bContinueLoop == TRUE);
		} while (EventLoopStopFlag == SOCKET_ERROR && pTcpSocket->m_bIsConnectTryThreadRunning == TRUE && bContinueLoop == TRUE && pTcpSocket->m_bThreadStopFlag);

		if ((bContinueLoop == FALSE || EventLoopStopFlag == 0) && pTcpSocket->m_bThreadStopFlag)
		{
			pTcpSocket->m_eConnState = TCP_CONNECTED;
			pTcpSocket->m_bIsClentConnect = TRUE;
			pTcpSocket->StartWatchThread(pTcpSocket);
		}
		else
			pTcpSocket->m_eConnState = TCP_CONNECTION_FAIL;

	}
	return 0;
}

void CTcpSocket::CleanupConnectionThread(void)
{
	if (m_pConnectionThread != NULL)
	{
		WaitForSingleObject(m_pConnectionThread->m_hThread, INFINITE);
		delete m_pConnectionThread;
		m_pConnectionThread = NULL;
	}
}

// void CTcpSocket::CleanupWatchThread(void)
// {
// 	if (m_pWatchThread != NULL)
// 	{
// 		WaitForSingleObject(m_pWatchThread->m_hThread, INFINITE);
// 		delete m_pWatchThread;
// 		m_pWatchThread = NULL;
// 	}
// }

void CTcpSocket::TerminateConnectionThread(void)
{
	if (m_pConnectionThread != nullptr)
	{
		unsigned long ulExitCode = NULL;
		GetExitCodeThread(m_pConnectionThread->m_hThread, &ulExitCode);
		TerminateThread(m_pConnectionThread->m_hThread, ulExitCode);

		delete m_pConnectionThread;
		m_pConnectionThread = NULL;
	}
}

void CTcpSocket::TerminateAcceptWatchThread(void)
{
	if (m_pAcceptWatchThread != nullptr)
	{
		unsigned long ulExitCode = NULL;
		GetExitCodeThread(m_pAcceptWatchThread->m_hThread, &ulExitCode);
		TerminateThread(m_pAcceptWatchThread->m_hThread, ulExitCode);

		delete m_pAcceptWatchThread;
		m_pAcceptWatchThread = NULL;
	}
}

void CTcpSocket::TerminateWatchThread(void)
{
	if (m_pWatchThread != nullptr)
	{
		unsigned long ulExitCode = NULL;
		GetExitCodeThread(m_pWatchThread->m_hThread, &ulExitCode);
		TerminateThread(m_pWatchThread->m_hThread, ulExitCode);

		delete m_pWatchThread;
		m_pWatchThread = NULL;
	}
}

/**
* Ŭ���̾�Ʈ ���� ���� ������
* @param	1) i_lpData : ������ ���� �� ������ �ν��Ͻ� ������
* @return ������ ���� �� ��ȯ ��. �ǹ� ����.
*/
UINT CTcpSocket::TcpAcceptWatchProc(LPVOID i_lpData)
{	
	CRITICAL_SECTION cs;

	CTcpSocket* pSocket;
	pSocket = (CTcpSocket*)i_lpData;

	///////////////////////////////////////////////////////////////////////////////////////////
	//Socket Define
	SOCKADDR_IN ClientAddress;
	SOCKET ClientSocket;
	int AddressSize = sizeof(ClientAddress);	
	////////////////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////////////////////
	//Socket Event	
	WSANETWORKEVENTS event; // Event Storage Structure
	WSAEVENT hRecvEvent = WSACreateEvent(); // Init WinSock And Return Handle	
	WSAEventSelect(pSocket->m_Socket,hRecvEvent,FD_ACCEPT|FD_READ|FD_CLOSE|FD_WRITE); 
	////////////////////////////////////////////////////////////////////////////////////////////

	DWORD dwResult;
	
	BOOL EventLoopStopFlag = FALSE;

   // while ( !(EventLoopStopFlag | pSocket->m_bEventLoopStopFlag) )
	while ((!(EventLoopStopFlag | pSocket->m_bEventLoopStopFlag)) && pSocket->m_bThreadStopFlag)
    {		
		dwResult = WSAWaitForMultipleEvents( 1, &hRecvEvent, FALSE, THREAD_TIMEOUT, FALSE );

		if ( dwResult == WSA_WAIT_FAILED || dwResult == WSA_WAIT_TIMEOUT )
		{
			continue;
		}
		else if ( dwResult >= WSA_WAIT_EVENT_0 )
		{
			long index = dwResult - WAIT_OBJECT_0;    
			WSAEnumNetworkEvents(pSocket->m_Socket,hRecvEvent,&event);

			if ((event.lNetworkEvents & FD_CLOSE) == FD_CLOSE)
			{
				closesocket(pSocket->m_Socket);
				EventLoopStopFlag = TRUE;			
			}			
			else if ((event.lNetworkEvents & FD_ACCEPT) == FD_ACCEPT)
			{
				if((ClientSocket = accept(pSocket->m_Socket,(sockaddr*)&ClientAddress,&AddressSize)) == INVALID_SOCKET)
				{
					//Accept Fail�� ó��
				}
				else
				{
					// Client Manage					
					for ( int i = 0; i < MAX_CHILDSOCKET_CNT; i++ )
					{
						if ( pSocket->m_pstChildSocket[i] == NULL )
						{
							//if ( pSocket->m_iChildSocketCnt == NULL )
							if (pSocket->m_iChildSocketCnt < MAX_CHILDSOCKET_CNT)
							{
								InitializeCriticalSection(&cs);
								EnterCriticalSection(&cs);

								pSocket->m_pstChildSocket[i] = new SChildSocket;
								pSocket->m_pstChildSocket[i]->ChildSocket = ClientSocket;
								pSocket->m_pstChildSocket[i]->iChildId = i+1;
								pSocket->m_iCurrentChildId = pSocket->m_pstChildSocket[i]->iChildId;

								LeaveCriticalSection(&cs);
								DeleteCriticalSection(&cs);

								pSocket->StartWatchThread(pSocket);						
								pSocket->UpdateClientConnectionInfo(CLIENT_CONNECT, pSocket->m_pstChildSocket[i]->iChildId);

								pSocket->m_iChildSocketCnt++;
							}
							else
							{
								//AfxMessageBox("�ϳ��� ������ 2���� ������ ����Ƿ��Ѵ�.");
								CString strWarning = L"�ϳ��� ������ 2���� ������ ����Ƿ��Ѵ�.";
								printf("%s", strWarning);
							}
							break;
						}
					}
				}
			}		
			else
			{}
		}	
	}
	
	pSocket->TerminateAcceptWatchThread();

	return 0;
}

/**
* Ŭ���̾�Ʈ ���� ������ ť�� �Է�
* @param	1) i_iUpdateType : CLIENT_CONNECT(����) �Ǵ� CLIENT_DISCONNECT(����)
* @param	2) i_iChildId : ���� �Ǵ� ���� �̺�Ʈ�� �߻��� �ڽ� ���� ID
*/
void CTcpSocket::UpdateClientConnectionInfo(int i_iUpdateType, int i_iChildId)
{
	m_pstEventQueue->EventType.Push(EVENT_ACCEPT);
	m_pstEventQueue->ChildInfo.Push(i_iChildId);
	m_pstAcceptQueue->ConnectionType.Push(i_iUpdateType);

	//::SetEvent(m_hEvent);
}

/**
* ������ ���Ž� ������ ť�� �Է�
* @param	1) i_iSize : ���ŵ� ������ ũ��
* @param	2) i_pBuf : ���ŵ� ������ ������
* @param	3) i_iChildId : ���� �̺�Ʈ�� �߻��� �ڽ� ���� ID
*/

void CTcpSocket::RecvData(int i_iSize, char* i_pBuf, int i_iChildId)
{
/*	CString str, strTmp;
	str = "RAW DATA - ";
	for ( int iCnt = 0; iCnt < i_iSize; iCnt++ )
	{
		strTmp.Format(" %02x", (UCHAR)(i_pBuf[iCnt]));
		str = str + strTmp;
	}*/		
	////WRITE_LOG(LOG_OPTION_CODE, 77, 0, str, FALSE);

	if ( m_pstRcvQueue->SizeInfo.Push(i_iSize) )
	{
		m_pstEventQueue->EventType.Push(EVENT_RECV);
		m_pstEventQueue->ChildInfo.Push(i_iChildId);	
		
		// Circular Data Push		
		if ( m_pstRcvQueue->uiQueueDataRear + i_iSize > MAX_QUEUE_DATA_SIZE )
		{
			int iRemainQueueSize = MAX_QUEUE_DATA_SIZE - m_pstRcvQueue->uiQueueDataRear;
			memcpy(&m_pstRcvQueue->aucBuf[m_pstRcvQueue->uiQueueDataRear], i_pBuf, iRemainQueueSize);		
			memcpy(m_pstRcvQueue->aucBuf, &i_pBuf[iRemainQueueSize], i_iSize - iRemainQueueSize);
			
			m_pstRcvQueue->uiQueueDataRear = i_iSize - iRemainQueueSize;
		}
		else
		{
			memcpy(&m_pstRcvQueue->aucBuf[m_pstRcvQueue->uiQueueDataRear], i_pBuf, i_iSize);
			m_pstRcvQueue->uiQueueDataRear = m_pstRcvQueue->uiQueueDataRear + i_iSize;
		}
	}

	//::SetEvent(m_hEvent);	
}

/**
* �̺�Ʈ ���� ȹ��. BL�� ���� �����忡�� ��� �̺�Ʈ �߻� ���θ� �Ǵ����� ���� ȣ���ϴ� �Լ�
* @param	1) i_piEventType : �̺�Ʈ Ÿ�� ȹ���� ���� ������. EVENT_ACCEPT(����/����) �Ǵ� EVENT_RECV(����)
* @param	2) i_piChildId : �̺�Ʈ�� �߻���Ų �ڽ� ���� ID�� ������. Ŭ���̾�Ʈ ������ ���� 0
* return TRUE(ť�� �̺�Ʈ ����) or FALSE(ť�� ��� ����)
*/
BOOL CTcpSocket::GetEventInfo(int* i_piEventType, int* i_piChildId)
{
	if ( this != NULL )
	{
		*i_piEventType = m_pstEventQueue->EventType.Pop();

		if ( *i_piEventType )
		{
			*i_piChildId = m_pstEventQueue->ChildInfo.Pop();		

			return TRUE;
		}

		return FALSE;
	}
	else
        return FALSE;
}

/**
* ���� ������ ȹ��. BL�� ���� �����忡�� ���� �����͸� ȹ���ϱ� ���� ȣ���ϴ� �Լ�. GetEventInfo()�� �̺�Ʈ Ȯ�� �� �� �Լ��� ȣ����
* @param	1) i_piSize : ���� ������ ũ�� ������
* @param	2) i_pBuf : ���� ������ ������
*/
void CTcpSocket::GetRcvData(int* i_piSize, char* i_pBuf)
{
	int iSize = m_pstRcvQueue->SizeInfo.Pop();

	// Circular Queue Pop
	if ( m_pstRcvQueue->uiQueueDataFront + iSize > MAX_QUEUE_DATA_SIZE )
	{
		int iRemainQueueSize = MAX_QUEUE_DATA_SIZE - m_pstRcvQueue->uiQueueDataFront;
		memcpy(i_pBuf, &m_pstRcvQueue->aucBuf[m_pstRcvQueue->uiQueueDataFront], iRemainQueueSize);		
		memcpy(&i_pBuf[iRemainQueueSize], m_pstRcvQueue->aucBuf, iSize - iRemainQueueSize);

		m_pstRcvQueue->uiQueueDataFront = iSize - iRemainQueueSize;
	}
	else
	{
		memcpy(i_pBuf, &m_pstRcvQueue->aucBuf[m_pstRcvQueue->uiQueueDataFront], iSize);
		m_pstRcvQueue->uiQueueDataFront = m_pstRcvQueue->uiQueueDataFront + iSize;
	}

	*i_piSize = iSize;
}

/**
* ���� ���� ȹ��. BL�� ���� �����忡�� ���� �������� ȹ���ϱ� ���� ȣ���ϴ� �Լ�. GetEventInfo()�� �̺�Ʈ Ȯ�� �� �� �Լ��� ȣ����
* @param	1) i_piConnectionType : ���� Ÿ�� ������
*/
void CTcpSocket::GetAcceptInfo(int* i_piConnectionType)
{
	*i_piConnectionType = m_pstAcceptQueue->ConnectionType.Pop();		
}


/**
* ���ϵ� ���� �ݰ�
* @param	1) i_iChildID :  ���ϵ� ����
*/
void CTcpSocket::CloseChildSocket(int i_iChildID)
{	
	if ( m_pstChildSocket[i_iChildID-1] )
	{
		delete m_pstChildSocket[i_iChildID-1];
		m_pstChildSocket[i_iChildID-1] = NULL;
	}
}

void CTcpSocket::CloseAllChildSocket()
{
	for ( int i = 1; i <= MAX_CHILDSOCKET_CNT; i++ )
		CloseChildSocket(i);

	m_iChildSocketCnt = NULL;
	m_iCurrentChildId = NULL;
}
BOOL CTcpSocket::IsConnected()
{
	return m_bIsClentConnect;
}


void CTcpSocket::RetrySocketConnection()
{
	if (m_eConnState != TCP_CONNECTING)
	{
		//m_bIsRecvThreadRunning = false;

		m_bIsConnectTryThreadRunning = FALSE;
		this->CleanupConnectionThread();

		closesocket(m_Socket);
		CreateClientSocket();

		m_bIsConnectTryThreadRunning = TRUE;
		m_pConnectionThread = AfxBeginThread(SocketConnectionThread, this);
		m_pConnectionThread->m_bAutoDelete = FALSE;
	}
}

void CTcpSocket::SetServerInfo(CString strIP, int sPort)
{
	m_strServerIP = strIP;
	m_iServerPort = sPort;
}

BOOL CTcpSocket::IsServerInfoOccured()
{
	BOOL bRetVal = FALSE;
	if ((0 < m_iServerPort && m_iServerPort < 65535) &&
		m_strServerIP.GetLength() > 0)
		bRetVal = TRUE;
	return bRetVal;
}

/**
* ���� ���� ����
* @param	1) i_sPort : ���� ��Ʈ
* @return : TRUE(����) �Ǵ� FALSE(����)
*/
BOOL CTcpSocket::CreateServerSocket(short i_sPort)
{
	SOCKADDR_IN		addr_Srv;	
		
	if ((m_Socket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		WSACleanup();
		return FALSE;
	}		

	memset((char *) &addr_Srv, 0, sizeof(addr_Srv));
	addr_Srv.sin_family      = AF_INET;	
	addr_Srv.sin_port        = htons(i_sPort);

	if ( bind(m_Socket, (sockaddr*)&addr_Srv, sizeof(addr_Srv)) == SOCKET_ERROR )
	{
		WSACleanup();
		return FALSE;
	}		

	if( listen(m_Socket, SOMAXCONN) == SOCKET_ERROR)
	{	
		WSACleanup();
		return FALSE;
	}

	m_pAcceptWatchThread = AfxBeginThread((AFX_THREADPROC)TcpAcceptWatchProc,(LPVOID)this);
	m_pAcceptWatchThread->m_bAutoDelete = FALSE;

	return TRUE;
}

/**
* ���� ���� ����
* @return : TRUE(����) �Ǵ� FALSE(����)
*/
BOOL CTcpSocket::CreateClientSocket()
{
	BOOL bRetVal = TRUE;
	if ((m_Socket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		//WSACleanup();
		BOOL bSW = TRUE;
		while(bSW)
		{
			if ((m_Socket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
				bRetVal = FALSE;
			else
				bSW = FALSE;
			Sleep(500);
		}
	}
	else
	{
// 		int iMode = 1;
// 		ioctlsocket(m_Socket, FIONBIO, (u_long FAR*) &iMode);
		bRetVal = TRUE;
	}
	
	return bRetVal;
}

/**
* Ŭ���̾�Ʈ�� ������ ����
* @param	1) i_iChidId : ������ �ڽ� ���� ID
* @param	2) i_pBuf : ���� ������ ������
* @param	3) i_iSize : ���� ������ ũ��
* @return : TRUE(����) �Ǵ� FALSE(����)
*/
BOOL CTcpSocket::SendToClient(int i_iChildId, char* i_pBuf, int i_iSize)
{
	BOOL bRetVal = TRUE;
	CString str, strTmp;
	str = "RAW DATA - ";
	for ( int iCnt = 0; iCnt < i_iSize; iCnt++ )
	{
		strTmp.Format(L" %02x", (UCHAR)(i_pBuf[iCnt]));
		str = str + strTmp;
	}		

	SOCKET ChildSocket;
	BOOL bIsValidClientId = FALSE;

	for ( int i = 0; i < MAX_CHILDSOCKET_CNT; i++ )
	{
		if ( m_pstChildSocket[i] != NULL )
		{
			if ( m_pstChildSocket[i]->iChildId == i_iChildId )
			{
				ChildSocket = m_pstChildSocket[i]->ChildSocket;
				bIsValidClientId = TRUE;
				break;
			}
		}		
	}

	fd_set wfd;	

	FD_ZERO(&wfd);
	FD_SET(m_Socket, &wfd);	

	if ( bIsValidClientId )
	{
		if( send(ChildSocket, i_pBuf, i_iSize, NULL) == SOCKET_ERROR )
		{
			int iErrorCode = WSAGetLastError();
			if (WSAEWOULDBLOCK != iErrorCode)
				bRetVal = FALSE;
			else
			{
				int iRst = select(0, 0, &wfd, 0, 0);                
				
				if (SOCKET_ERROR == iRst)
					bRetVal = FALSE;
				else
					send(m_Socket, i_pBuf, i_iSize, NULL);
			}						
		}		
	}
	else
		bRetVal = FALSE;
	
	return bRetVal;
}

/**
* ������ ������ ����
* @param	1) i_pBuf : ���� ������ ������
* @param	2) i_iSize : ���� ������ ũ��
* @return : TRUE(����) �Ǵ� FALSE(����)
*/
BOOL CTcpSocket::SendToServer(char* i_pBuf, int i_iSize)
{
	BOOL bRetVal = TRUE;
	if(m_eConnState == TCP_CONNECTED)
	{
		fd_set wfd;

		FD_ZERO(&wfd);
		FD_SET(m_Socket, &wfd);

		if (send(m_Socket, i_pBuf, i_iSize, NULL) == SOCKET_ERROR)
		{
			int iRst = WSAGetLastError();
			if (WSAEWOULDBLOCK != iRst)
			{
				bRetVal = FALSE;
			}
			else
			{
				TRACE("WSAEWOULDBLOCK\r\n");

				int iRst = select(0, 0, &wfd, 0, 0);

				if (SOCKET_ERROR == iRst)
					return FALSE;

				send(m_Socket, i_pBuf, i_iSize, NULL);
			}
		}
	}
	else
	{
		bRetVal = FALSE;
	}

	return bRetVal;
}

/**
* ������ ���� ������ ����
* @param	1) i_pSocket : ���� ������
*/
void CTcpSocket::StartWatchThread(CTcpSocket* i_pSocket)
{
	m_pWatchThread = AfxBeginThread((AFX_THREADPROC)TcpWatchProc,(LPVOID)i_pSocket);
	m_pWatchThread->m_bAutoDelete = FALSE;
}

void CTcpSocket::StartConnectThread(CTcpSocket* i_pSocket)
{
// 	if(m_bIsConnectTryThreadRunning == FALSE && m_eConnState != TCP_CONNECTED)
// 	{
// 		m_bIsConnectTryThreadRunning = TRUE;
// 		m_pConnectionThread = AfxBeginThread((AFX_THREADPROC)SocketConnectionThread, (LPVOID)i_pSocket);
// 		m_pConnectionThread->m_bAutoDelete = FALSE;
// 	}
}

/**
* ���� ����. BL���� ȣ���ϴ� �Լ��� �ڵ��� �����ϸ�, ��Ʈ ��ȣ�� ����/ Ŭ���̾�Ʈ�� �����Ͽ� �����Ѵ�. ��Ʈ ��ȣ�� 0�̸� Ŭ���̾�Ʈ ���� ����
* @param	1) i_hEvent : �ñ׳� �߻��� ���� BL�� ���� ������ �ڵ�
* @param	2) i_sPort : ��Ʈ��ȣ. 0�̸� Ŭ���̾�Ʈ ���� ����, �׿ܿ��� �ش� ��Ʈ ��ȣ�� ���� ���� ����
* @return : TRUE(����) �Ǵ� FALSE(����)
*/
BOOL CTcpSocket::Create(/*HANDLE i_hEvent, */short i_sPort)
{
	BOOL bRst = TRUE;

	if (i_sPort)
		bRst = CreateServerSocket(i_sPort);
	// 	else
	// 		bRst = CreateClientSocket();


	return bRst;
}

/**
* ���� ���Ͽ� ����. Ŭ���̾�Ʈ ���Ͽ��� ��ȿ�ϸ�, BL���� ȣ��
* @param	1) i_sPort : ���� ��Ʈ��ȣ
* @param	2) i_strAddr : ���� �ּ�
* @return : TRUE(����) �Ǵ� FALSE(����)
*/
BOOL CTcpSocket::Connect(int i_sPort, CString i_strAddr)
{
	BOOL bRetVal = TRUE;
	memset((char *) &m_addrTcpSocket, 0, sizeof(m_addrTcpSocket));
	m_addrTcpSocket.sin_family      = AF_INET;
	//addr_Client.sin_addr.s_addr = inet_addr(i_strAddr);
#ifndef UNICODE

	m_addrTcpSocket.sin_addr.s_addr = inet_addr(i_strAddr);
#else
	//m_addrTcpSocket.sin_addr.s_addr = _tinet_addr(i_strAddr.GetBuffer());

	LPHOSTENT    lphostent;
	ULONG        uAddr = INADDR_NONE;
	TCHAR       strLocal[MAX_PATH] = { 0 };
	char strHost[MAX_PATH] = { 0 };
	WideCharToMultiByte(CP_ACP, 0, i_strAddr, -1, strHost, sizeof(strHost), NULL, NULL);

	// Check for an Internet Protocol dotted address string
	uAddr = inet_addr(strHost);

// 	if ((INADDR_NONE == uAddr) && (strcmp(strHost, "255.255.255.255")))
// 	{
// 		// It's not an address, then try to resolve it as a hostname
// 		if (lphostent = gethostbyname(strHost))
// 			uAddr = *((ULONG *)lphostent->h_addr_list[0]);
// 	}
	m_addrTcpSocket.sin_addr.s_addr = htonl(ntohl(uAddr));
#endif
	m_addrTcpSocket.sin_port = htons(i_sPort);
	
	CreateClientSocket();

	m_bIsConnectTryThreadRunning = TRUE;
	m_pConnectionThread = AfxBeginThread(SocketConnectionThread, this);
	m_pConnectionThread->m_bAutoDelete = FALSE;
	/*
 	if ( connect(m_Socket, (sockaddr*)&m_addrTcpSocket, sizeof(m_addrTcpSocket)) == SOCKET_ERROR )
	{
		int iErrorCode = WSAGetLastError();				
		if (WSAEWOULDBLOCK != iErrorCode)
		{
			WSACleanup();
			bRetVal = FALSE;
		}
		else
		{
            fd_set efd;
			FD_ZERO(&efd);
			FD_SET(m_Socket, &efd);
			
			timeval t;
			t.tv_sec = 1;
			t.tv_usec = 0;

			int iRst = select(SO_OOBINLINE , 0,  &efd, 0, &t);
			
			if ( iRst != 1)
			{
				WSACleanup();
				bRetVal = FALSE;
			}			
		}
	}
	else
	{
		bRetVal = TRUE;
		m_eConnState = TCP_CONNECTED;
		m_bIsClentConnect = TRUE;
		StartWatchThread(this);
	}
	*/
	return bRetVal;
}

/**
* ������ ����. �����͸� �����ϱ� ���� BL���� ȣ��
* @param	1) i_pBuf : ���� ������ ������
* @param	2) i_iSize : ���� ������ ũ��
* @param	3) i_iChildId : 0�̸�, Ŭ���̾�Ʈ ������ ���� �������� �����͸� �����ϰ�, �׿ܿ��� ���� ������ �ش� �ڽ� ������ ���ؼ� ����� Ŭ���̾�Ʈ�� �����͸� ����
* @return : TRUE(����) �Ǵ� FALSE(����)
*/
BOOL CTcpSocket::SendData(char* i_pBuf, int i_iSize, int i_iChildId)
{
	BOOL bRst;

	if ( i_iChildId )
		bRst = SendToClient(i_iChildId, i_pBuf, i_iSize);
	else
		bRst = SendToServer(i_pBuf, i_iSize);

	return bRst;
}