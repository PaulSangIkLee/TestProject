#pragma once
class CReadWriteINI
{
public:
	CReadWriteINI(void);
	~CReadWriteINI(void);

	static CString GetTextFromINI(CString i_strFilePath, LPCTSTR lpGrpName, LPCTSTR lpCtrlName);
	static void WriteTextToINI(CString i_strFilePath, CString i_strGrpName, CString i_strItem, CString i_strContent);

};

