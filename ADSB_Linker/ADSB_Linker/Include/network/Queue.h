#ifndef __SAFE_QUEUE_
#define __SAFE_QUEUE_

#include <windows.h>
#include <queue>

using namespace std;

#define MAX_QUEUE_SIZE 100

/**
* [common class] STL queue�� ����� Queue Class
* queue�� ����� �� critical section ���
*/

template <typename T> class CQueue
{

public:
	CQueue();
	~CQueue();
	bool Push(T T);
	T Pop();
	int Size();

private:
	queue <T> m_Ts; ///< Queue
	CRITICAL_SECTION m_cs; ///< Critical Section
	queue <int>::size_type m_iQueueSize;
};

/**
* ������
*/
template <typename T> CQueue <T>::CQueue()
{
	InitializeCriticalSection(&m_cs);	
}

/**
* �Ҹ���
*/
template <typename T> CQueue <T>::~CQueue()
{
	T T;
	DeleteCriticalSection(&m_cs);
	while( !m_Ts.empty() )	
	{
		T = m_Ts.front();		
		m_Ts.pop();
	}
}

/**
* Queue�� ������ �Է�
*/
template <typename T> bool CQueue <T>::Push(T T)
{
	m_iQueueSize =  m_Ts.size();
	if ( m_iQueueSize < MAX_QUEUE_SIZE )
	{
		EnterCriticalSection(&m_cs);
		m_Ts.push(T);
		LeaveCriticalSection(&m_cs);
		return true;
	}
	return false;
}

/**
* Queue���� ������ ȹ��
*/
template <typename T> T CQueue <T>::Pop()
{
	T T;
	if( !m_Ts.empty() )	
	{
		EnterCriticalSection(&m_cs);
		T = m_Ts.front();
		m_Ts.pop();	
		LeaveCriticalSection(&m_cs);
	}
	else
	{
		T = NULL;
	}
	return T;
} 

/**
* Queue�� ������ �Է�
*/
template <typename T> int CQueue <T>::Size()
{
	int nReturnSize =  m_Ts.size();
	return nReturnSize;
}

#endif

