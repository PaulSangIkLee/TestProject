//#include "stdafx.h"
#include "stdafx.h"
#include "ReadWriteINI.h"


CReadWriteINI::CReadWriteINI(void)
{
}


CReadWriteINI::~CReadWriteINI(void)
{
}

CString CReadWriteINI::GetTextFromINI(CString i_strFilePath, LPCTSTR lpGrpName, LPCTSTR lpCtrlName)
{
	CString strText;

#ifndef UNICODE
	char szValue[MAX_PATH];
#else
	wchar_t szValue[MAX_PATH];
#endif
	memset(szValue, 0, MAX_PATH);

	BOOL bResult = ::GetPrivateProfileString(lpGrpName,
		lpCtrlName,
		_T(""),
		szValue,
		MAX_PATH,
		LPCTSTR(i_strFilePath));

	strText = szValue;

	return strText;
}

void CReadWriteINI::WriteTextToINI(CString i_strFilePath, CString i_strGrpName, CString i_strItem, CString i_strContent)
{
	CString strContent;

	//따옴표 붙이기
	strContent = "\"";
	strContent += i_strContent;
	strContent += "\""; 

	::WritePrivateProfileString(i_strGrpName, i_strItem, strContent, i_strFilePath);
}
