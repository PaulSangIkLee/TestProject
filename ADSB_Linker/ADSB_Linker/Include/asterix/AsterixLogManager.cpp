#include "stdafx.h"
#include "AsterixLogManager.h"

CAsterixLogManager* CAsterixLogManager::g_Instance = 0;

CAsterixLogManager* CAsterixLogManager::Instance()
{
	if (g_Instance == 0) {
		g_Instance = new CAsterixLogManager;
	}

	return g_Instance;
}

CAsterixLogManager::CAsterixLogManager()
{
	m_pLogList	= NULL;
	m_pLogWnd	= NULL;

	m_pLogList = new LOG_LIST;
}


CAsterixLogManager::~CAsterixLogManager()
{
	if (m_pLogList)
	{
		for (LOG_LIST_ITR itr = m_pLogList->begin(); itr != m_pLogList->end(); ++itr)
		{
			AsterixEventLog* pLog = *itr;

			if (pLog) {
				delete pLog, pLog = NULL;
			}
		}

		m_pLogList->clear();

		delete m_pLogList;
		m_pLogList = NULL;
	}
}

void CAsterixLogManager::ClearAllLog()
{
	if (m_pLogList)
	{
		for (LOG_LIST_ITR itr = m_pLogList->begin(); itr != m_pLogList->end(); ++itr)
		{
			AsterixEventLog* pLog = *itr;

			if (pLog) {
				delete pLog, pLog = NULL;
			}
		}

		m_pLogList->clear();

		if (m_pLogWnd)
			m_pLogWnd->PostMessage(LOG_UM_MESSAGE, 0, 0);

	}
}


BOOL CAsterixLogManager::AddLog(const LogType& nType, LPCTSTR lpszContents)
{
	if (m_pLogList == NULL) return FALSE;

	AsterixEventLog* pLog = new AsterixEventLog;

	CTime time = CTime::GetCurrentTime();

	time.GetAsSystemTime(pLog->stime);
	pLog->nType = nType;

	//190527 익 - 멀티바이트에서 에러가 나므로 추가함.
#ifndef _UNICODE
// 	size_t newsize = strlen(pLog->szContents) + 1;
// 	wchar_t * wcstring = new wchar_t[newsize];
// 	size_t convertedChars = 0;
// 	mbstowcs_s(&convertedChars, wcstring, newsize, pLog->szContents, _TRUNCATE);
// 	wcscpy(wcstring, T2CW(lpszContents));
	strcpy(pLog->szContents, lpszContents);
#else
	wcscpy(pLog->szContents, lpszContents);
#endif

	m_pLogList->push_back(pLog);
	

	if (m_pLogWnd)
		m_pLogWnd->PostMessage(LOG_UM_MESSAGE, 0, 0);

	return TRUE;
}

void CAsterixLogManager::SetLogWnd(CWnd* pWnd)
{
	ASSERT(pWnd);
	m_pLogWnd = pWnd;
}

BOOL CAsterixLogManager::StoreLog(LPCTSTR lpszFile)
{
	if (m_pLogList == NULL) return FALSE;

	CFile file;
	CFileException ex;

	if (!file.Open(lpszFile, CFile::modeReadWrite | CFile::modeCreate, &ex)) {
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);

		TRACE(L"Store Log File Failed! Error = %s\n", szError);
		return FALSE;
	}

	CString strTitle;

	strTitle = "종류,날짜,시간,내용\n";
	file.Write(strTitle, strTitle.GetLength());

	for (LOG_LIST_ITR itr = m_pLogList->begin(); itr != m_pLogList->end(); ++itr)
	{
		AsterixEventLog* pLog = *itr;

		if (pLog) {
			CTime time(pLog->stime);

			CString strLog;
			CString strDate = time.Format(L"%Y-%m-%d");
			CString strTime = time.Format(L"%p %I:%M:%S");

			strLog.Format(_T("%s,%s,%s,%s\n"), alpszLogType[pLog->nType], strDate, strTime, pLog->szContents);

			file.Write(strLog, strLog.GetLength());
		}
	}

	file.Close();

	return TRUE;

}