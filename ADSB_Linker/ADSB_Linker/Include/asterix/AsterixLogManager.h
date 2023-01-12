#pragma once

#include <list>

// Log Type
typedef enum _LogType
{
	LT_INFORMATION = 0,
	LT_ERROR,
	LT_WARNING,
	LT_COUNT
} LogType;

// Log Type ���ڿ�
static LPCTSTR alpszLogType[] =
{
	_T("����"),
	_T("����"),
	_T("���"),
};

#define LOG_UM_MESSAGE			WM_USER+1

typedef struct _AsterixEventLog
{
	LogType		nType;
	SYSTEMTIME	stime;
#ifdef _UNICODE
	TCHAR		szContents[MAX_PATH];
#else
	char		szContents[MAX_PATH];
#endif // _UNICODE
} AsterixEventLog;

typedef std::list<AsterixEventLog*>	LOG_LIST;
typedef LOG_LIST::iterator			LOG_LIST_ITR;

class CAsterixLogManager
{
public:
	CAsterixLogManager();
	virtual ~CAsterixLogManager();

	static CAsterixLogManager* Instance();

	void SetLogWnd(CWnd* pWnd);
	void ClearAllLog();
	BOOL AddLog(const LogType& nType, LPCTSTR lpszContents);
	BOOL StoreLog(LPCTSTR lpszFile);

	LOG_LIST* GetLogList() { return m_pLogList; }

private:
	LOG_LIST*					m_pLogList;
	static CAsterixLogManager*	g_Instance;
	CWnd*						m_pLogWnd;

};

