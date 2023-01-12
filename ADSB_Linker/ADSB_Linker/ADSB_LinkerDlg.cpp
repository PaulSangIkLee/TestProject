
// ADSB_LinkerDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "ADSB_Linker.h"
#include "ADSB_LinkerDlg.h"
#include "afxdialogex.h"
#include "ReadWriteINI.h"
//test
#include "JSONParserPingStation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CADSBLinkerDlg 대화 상자



CADSBLinkerDlg::CADSBLinkerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADSB_LINKER_DIALOG, pParent)
	, m_pListnerAsterix(NULL)
	, m_pBusinessMng(NULL)
	, m_nDisplayTime(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CADSBLinkerDlg::~CADSBLinkerDlg()
{
	ReleaseAll();
}


void CADSBLinkerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ADSB, m_listADSB_Display);
	DDX_Control(pDX, IDC_STATIC_AIRCRAFT_COUNT, m_ctrlStrAirCraftCount);
	DDX_Control(pDX, IDC_STATIC_CURRENT_TIME, m_ctrlStrCurrentTime);
	DDX_Control(pDX, IDC_STATIC_DB_CONNECTION, m_ctrlStrConnectStatus);
}

BEGIN_MESSAGE_MAP(CADSBLinkerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_DISP_DECODED, RecvDecodeData)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_ADSB, OnCustomdrawList)

	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_TRAYICON_MSG, &CADSBLinkerDlg::TrayIconMsg)
	ON_COMMAND(ID_SHOWHIDE_SHOW, &CADSBLinkerDlg::OnShowhideShow)
	ON_COMMAND(ID_SHOWHIDE_HIDE, &CADSBLinkerDlg::OnShowhideHide)
	ON_COMMAND(ID_SHOWHIDE_EXIT, &CADSBLinkerDlg::OnShowhideExit)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CADSBLinkerDlg 메시지 처리기

BOOL CADSBLinkerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.'
	RegistTrayIcon();
	OnShowhideHide();
	CRect rt;
	m_listADSB_Display.GetWindowRect(&rt);
// 	m_listADSB_Display.SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
// 	m_listADSB_Display.InsertColumn(0, _T("CallSign"), LVCFMT_CENTER, rt.Width()*0.08);
// 	m_listADSB_Display.InsertColumn(1, _T("ICAO"), LVCFMT_CENTER, rt.Width()*0.08);
// 	m_listADSB_Display.InsertColumn(2, _T("SQUAWK"), LVCFMT_CENTER, rt.Width()*0.08);
// 	m_listADSB_Display.InsertColumn(3, _T("Lat"), LVCFMT_CENTER, rt.Width()*0.13);
// 	m_listADSB_Display.InsertColumn(4, _T("Lon"), LVCFMT_CENTER, rt.Width()*0.13);
// 	m_listADSB_Display.InsertColumn(5, _T("Alt(ft)"), LVCFMT_CENTER, rt.Width()*0.1);
// 	m_listADSB_Display.InsertColumn(6, _T("Head"), LVCFMT_CENTER, rt.Width()*0.1);
// 	m_listADSB_Display.InsertColumn(7, _T("Vel(km/h)"), LVCFMT_CENTER, rt.Width()*0.1);
// 	m_listADSB_Display.InsertColumn(8, _T("UTC Time"), LVCFMT_CENTER, rt.Width()*0.2);
	m_listADSB_Display.InsertColumn(0, _T("CallSign"), LVCFMT_CENTER, 70);
	m_listADSB_Display.InsertColumn(1, _T("ICAO"), LVCFMT_CENTER, 60);
	m_listADSB_Display.InsertColumn(2, _T("SQUAWK"), LVCFMT_CENTER, 70);
	m_listADSB_Display.InsertColumn(3, _T("Lat"), LVCFMT_CENTER, 100);
	m_listADSB_Display.InsertColumn(4, _T("Lon"), LVCFMT_CENTER, 100);
	m_listADSB_Display.InsertColumn(5, _T("Alt(ft)"), LVCFMT_CENTER, 90);
	m_listADSB_Display.InsertColumn(6, _T("Head"), LVCFMT_CENTER, 60);
	m_listADSB_Display.InsertColumn(7, _T("Vel(km/h)"), LVCFMT_CENTER, 70);
	m_listADSB_Display.InsertColumn(8, _T("UTC Time"), LVCFMT_CENTER, 160);
	m_listADSB_Display.InsertColumn(9, _T("ElapsedTime"), LVCFMT_CENTER, 0);

	TCHAR path[_MAX_PATH];
	GetModuleFileName(NULL, path, sizeof path);

	CString strINIPath = path;
	int nPos = strINIPath.ReverseFind('\\');//실행 파일 이름을 지우기 위해서 왼쪽에 있는 '/'를 찾는다.
	strINIPath = strINIPath.Left(nPos);//뒤에 있는 현재 실행 파일 이름을 지운다.
	strINIPath += _T("\\config\\config.ini");
	
	CString strSection(_T(""));
	CString strKey(_T(""));
	CString strValue(_T(""));

	int nSiteID = 0;
	int nUpdateTime = 0;

	int nAsterixPort = 0;
	CString strAsterixIP;
	int nJSONPort = 0;
	CString strJSONIP;
	int nOriginalDBPort = 0;
	int nDisplayDBPort = 0;
	CString strDBIP;

	strSection = _T("GENERAL SETTING");
	strKey = _T("SITE ID");
	strValue = CReadWriteINI::GetTextFromINI(strINIPath, strSection, strKey);
	nSiteID = _ttoi(strValue);
	//UPDATE LIMIT TIME 화면갱신 시간
// 	strKey = _T("UPDATE LIMIT TIME");
// 	strValue = CReadWriteINI::GetTextFromINI(strINIPath, strSection, strKey);
// 	nUpdateTime = _ttoi(strValue);
// 	m_nDisplayTime = nUpdateTime;

	strSection = _T("RECV ASTERIX");
	strKey = _T("IP");
	strValue = CReadWriteINI::GetTextFromINI(strINIPath, strSection, strKey);
	strAsterixIP = strValue;

	strKey = _T("PORT");
	strValue = CReadWriteINI::GetTextFromINI(strINIPath, strSection, strKey);
	nAsterixPort = _ttoi(strValue);

	strSection = _T("RECV JSON");
	strKey = _T("IP");
	strValue = CReadWriteINI::GetTextFromINI(strINIPath, strSection, strKey);
	strJSONIP = strValue;

	strKey = _T("PORT");
	strValue = CReadWriteINI::GetTextFromINI(strINIPath, strSection, strKey);
	nJSONPort = _ttoi(strValue);

	strSection = _T("DB SW SETTING");
	strKey = _T("IP");
	strValue = CReadWriteINI::GetTextFromINI(strINIPath, strSection, strKey);
	strDBIP = strValue;
	
	strKey = _T("ORIGINAL_PORT");
	strValue = CReadWriteINI::GetTextFromINI(strINIPath, strSection, strKey);
	nOriginalDBPort = _ttoi(strValue);

	strKey = _T("DISPLAY_PORT");
	strValue = CReadWriteINI::GetTextFromINI(strINIPath, strSection, strKey);
	nDisplayDBPort = _ttoi(strValue);

	m_pBusinessMng = new CBusinessManager;
	m_pBusinessMng->SetSendIPandPortForTCP(strDBIP, nOriginalDBPort, nDisplayDBPort);
	m_pBusinessMng->MakeConnection();

	//m_pListner = new CListnerAstrerix(30003, L"192.168.10.100", L"192.168.10.100");
	m_pListnerAsterix = new CListnerAstrerix(nAsterixPort, strAsterixIP, strAsterixIP);
	m_pListnerAsterix->SetBusinessManager(m_pBusinessMng);

	m_pListnerJSON = new CListnerJSONPingStation(nJSONPort, strJSONIP, strJSONIP);
	//m_pListnerJSON = new CListnerJSONPingStation(40000, L"192.168.0.26", L"192.168.0.26");
	m_pListnerJSON->SetBusinessManager(m_pBusinessMng);

	m_pBusinessMng->SetSiteID(nSiteID);

	SetTimer(TIMERTYPE_DELETE, 500, NULL);
	SetTimer(TIMERTYPE_DISPLAY, 1000, NULL);
	//SetTimer(TIMERTYPE_DEBUG, 500, NULL);
	SetTimer(TIMERTYPE_CONNECT, 500, NULL);

	//TCP 연결
//	m_pBusinessMng->Connect(nOriginalDBPort, strDBIP);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CADSBLinkerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CADSBLinkerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CADSBLinkerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	

	if (nIDEvent == TIMERTYPE_DELETE)
	{
		ADSB_List_Delete();
	}

// 	else if (nIDEvent == TIMERTYPE_OVER)
// 	{
// 
// 	}

	else if (nIDEvent == TIMERTYPE_DISPLAY)
	{
		ADSB_List_Display();
	}

	else if (nIDEvent == TIMERTYPE_CONNECT)
	{
		if (m_pBusinessMng != NULL)
		{
			if (m_pBusinessMng->IsTCPConnect())
			{
				m_ctrlStrConnectStatus.SetWindowTextW(_T("성공"));
			}
			else
			{
				m_ctrlStrConnectStatus.SetWindowTextW(_T("해제"));
			}
		}
	}

	else if (nIDEvent == TIMERTYPE_DEBUG)
	{
		char arTest[4];
		arTest[0] = 0xff;
		arTest[1] = 0xff;
		arTest[2] = 0x00;
		arTest[0] = 0xff;
		int nSize = sizeof(arTest);
		m_pBusinessMng->SendMessage(WM_SEND_DEBUG, (WPARAM)arTest, (LPARAM)&nSize);
	}

	else
	{ }

	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CADSBLinkerDlg::RecvDecodeData(WPARAM wParam, LPARAM lParam)
{
	stADSBCommon stRecvDecode = *((stADSBCommon*)wParam);

	stADSBCommonDisp stDispData;
	stDispData.Common = stRecvDecode;

	//CTime timeCurr = CTime::GetCurrentTime();
	SYSTEMTIME curTime;
	::GetSystemTime(&curTime);
	CTime timeCurr = CTime(curTime);
	DWORD dwDispTime = timeCurr.GetTime();
	stDispData.dwUpdateTime = dwDispTime;

	MAP_ADSB_ITR itr;
	//CString strCallSign;
	//strCallSign = stRecvDecode.archCallSign;
	unsigned long ulICAOAddress = stRecvDecode.ulICAOAddress;

	//itr = m_mapADSB.find(strCallSign);
	itr = m_mapADSB.find(ulICAOAddress);
	if (itr == m_mapADSB.end())
		//m_mapADSB.insert(make_pair(strCallSign, stDispData));
		m_mapADSB.insert(make_pair(ulICAOAddress, stDispData));
	else
		itr->second = stDispData;

	return LRESULT();
}

void CADSBLinkerDlg::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	//CString strProtocol, strRSTFIN;
	//BOOL bEmerFlag = FALSE;
	CString strElapsedTime;
	BOOL bChange = FALSE;

	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;

// 	strProtocol = m_ctrlTCPlist.GetItemText(pLVCD->nmcd.dwItemSpec, 1);
// 	strRSTFIN = m_ctrlTCPlist.GetItemText(pLVCD->nmcd.dwItemSpec, 9);
// 
// 	if ((strRSTFIN.Find("RST") != -1) || (strRSTFIN.Find("FIN") != -1))
// 	{
// 		bEmerFlag = TRUE;
// 	}
	strElapsedTime = m_listADSB_Display.GetItemText(pLVCD->nmcd.dwItemSpec, 9);
	if (strElapsedTime == L"1")
	{
		bChange = TRUE;
	}

	*pResult = 0;

	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
		*pResult = CDRF_NOTIFYITEMDRAW;

	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
 		if (bChange) // 곧 지워질 색상 플래그 일떄
 		{
			pLVCD->clrText = RGB(169, 109, 199);
			//pLVCD->clrText = RGB(0, 162, 232);
			//pLVCD->clrText = RGB(255, 102, 102);
			//pLVCD->clrTextBk = RGB(237, 255, 255);
		}

 		if (!bChange) // 긴급 플래그가 아닐때
 		{
 			pLVCD->clrText = RGB(0, 0, 0);
			//pLVCD->clrTextBk = RGB(255, 255, 255);
 		}
		*pResult = CDRF_DODEFAULT;
	}
}

void CADSBLinkerDlg::ADSB_List_Delete()
{
	if (m_mapADSB.size() > 0)
	{
		//CTime timeCurrent = CTime::GetCurrentTime();
		SYSTEMTIME curTime;
		::GetSystemTime(&curTime);
		CTime timeCurrent = CTime(curTime);

		MAP_ADSB_ITR itr;
		CTimeSpan tOverTime(0, 0, 0, 5);

		if(m_nDisplayTime > 3)
			tOverTime = CTimeSpan(0, 0, 0, m_nDisplayTime);
		
		itr = m_mapADSB.begin();
		while (itr != m_mapADSB.end())
		{
			CTime tRecvTime = itr->second.dwUpdateTime;

			if (tRecvTime + tOverTime < timeCurrent)
			{
				m_mapADSB.erase(itr++);
			}
			else
			{
				itr++;
			}
		}
	}
}

void CADSBLinkerDlg::ADSB_List_Display()
{
	m_listADSB_Display.SetRedraw(FALSE);

	//삭제 기준시간 확인
	TCHAR path[_MAX_PATH];
	GetModuleFileName(NULL, path, sizeof path);

	CString strINIPath = path;
	int nPos = strINIPath.ReverseFind('\\');//실행 파일 이름을 지우기 위해서 왼쪽에 있는 '/'를 찾는다.
	strINIPath = strINIPath.Left(nPos);//뒤에 있는 현재 실행 파일 이름을 지운다.
	strINIPath += _T("\\config\\config.ini");

	CString strSection(_T(""));
	CString strKey(_T(""));
	CString strValue(_T(""));

	int nUpdateTime = 0;

	strSection = _T("GENERAL SETTING");
	//UPDATE LIMIT TIME 화면갱신 시간
	strKey = _T("UPDATE LIMIT TIME");
	strValue = CReadWriteINI::GetTextFromINI(strINIPath, strSection, strKey);
	nUpdateTime = _ttoi(strValue);
	m_nDisplayTime = nUpdateTime;
	//삭제 기준시간 확인

	//전체 데이터 삭제
	int nItemCnt = m_listADSB_Display.GetItemCount();
	int nMapCnt = m_mapADSB.size();

	m_listADSB_Display.DeleteAllItems();

	SYSTEMTIME curTime;
	::GetSystemTime(&curTime);
	CTime timeCurrent = CTime(curTime);

	CString strTmpInfo;
	strTmpInfo.Format(_T("%d"), nMapCnt);
	m_ctrlStrAirCraftCount.SetWindowTextW(strTmpInfo);
	m_ctrlStrCurrentTime.SetWindowTextW(timeCurrent.Format("%X-%Y/%m/%d"));

	if (m_mapADSB.size() > 0)
	{		
		//CTime timeCurrent = CTime::GetCurrentTime();
		MAP_ADSB_ITR itr;
		CTimeSpan tOverTime(0, 0, 0, 3);
		CString strTmp;
		stADSBCommon sDisplayData;

		itr = m_mapADSB.begin();
		int nIdx = 0;
		if((m_nDisplayTime /2) > 3)
			tOverTime = CTimeSpan(0, 0, 0, (m_nDisplayTime/2));
		while (itr != m_mapADSB.end())
		{
			CTime tUpdateTime = itr->second.dwUpdateTime;
			sDisplayData = itr->second.Common;

			strTmp = sDisplayData.archCallSign;
			m_listADSB_Display.InsertItem(nIdx, strTmp);
			
			if (tUpdateTime + tOverTime < timeCurrent)
			{
				strTmp = L"1";
				m_listADSB_Display.SetItemText(nIdx, 9, strTmp);
			}

			strTmp.Format(L"%X", sDisplayData.ulICAOAddress);
			m_listADSB_Display.SetItemText(nIdx, 1, strTmp);

			strTmp.Format(L"%04d", sDisplayData.nSQUAWK);
			m_listADSB_Display.SetItemText(nIdx, 2, strTmp);
			
			strTmp.Format(L"%3.8f", sDisplayData.dLatitude);
			m_listADSB_Display.SetItemText(nIdx, 3, strTmp);
			
			strTmp.Format(L"%3.8f", sDisplayData.dLongitude);
			m_listADSB_Display.SetItemText(nIdx, 4, strTmp);
			
			//strTmp.Format(L"%.2f", sDisplayData.dAltitude * 3.28084);
			strTmp.Format(L"%.2f", sDisplayData.dAltitude);
			m_listADSB_Display.SetItemText(nIdx, 5, strTmp);
			
			strTmp.Format(L"%3.3f", sDisplayData.dHeading);
			m_listADSB_Display.SetItemText(nIdx, 6, strTmp);
			
			strTmp.Format(L"%3.2f", sDisplayData.dVelocity * 1.94384);//meter per second to knot
			m_listADSB_Display.SetItemText(nIdx, 7, strTmp);
			
			CTime tRecvTime = sDisplayData.dwRecvTime;
			strTmp = tRecvTime.Format("%X - %Y/%m/%d");
			m_listADSB_Display.SetItemText(nIdx, 8, strTmp);

			itr++;
			nIdx++;
		}
	}

	m_listADSB_Display.SetRedraw(TRUE);
}


void CADSBLinkerDlg::ReleaseAll()
{
	if (m_pListnerAsterix != NULL)
	{
		TRACE("m_pListnerAsterix : 0x%x\n", m_pListnerAsterix);
		delete m_pListnerAsterix;
		m_pListnerAsterix = NULL;
	}

	if (m_pListnerJSON != NULL)
	{
		TRACE("m_pListnerJSON : 0x%x\n", m_pListnerJSON);
		delete m_pListnerJSON;
		m_pListnerJSON = NULL;
	}

	if (m_pBusinessMng != NULL)
	{
		delete m_pBusinessMng;
		m_pBusinessMng = NULL;
	}

	//if (m_pBusinessMng != NULL)
	//{
	//	KillTimer(TIMERTYPE_DELETE);
	//	KillTimer(TIMERTYPE_DISPLAY);
	//}

	//	KillTimer(TIMERTYPE_DEBUG);
}

void CADSBLinkerDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ReleaseAll();
	CDialogEx::OnClose();
}

LRESULT CADSBLinkerDlg::TrayIconMsg(WPARAM wParam, LPARAM lParam)
{
	if (wParam == IDR_TRAY_MENU)
	{
		switch (lParam)
		{
		case WM_RBUTTONDOWN:

			CMenu menu, *pMenu;
			CPoint pt = 0;
			menu.LoadMenu(IDR_TRAY_MENU);
			pMenu = menu.GetSubMenu(0);
			GetCursorPos(&pt);

			SetForegroundWindow();
			pMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this, NULL);
			SetForegroundWindow();
			break;
		}
	} // if(lParam== WM_RBUTTONDOWN)
	return TRUE;
}

void CADSBLinkerDlg::OnShowhideShow()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bShowStatus = TRUE;
	ShowWindow(SW_SHOW);
}


void CADSBLinkerDlg::OnShowhideHide()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bShowStatus = FALSE;
	ShowWindow(SW_HIDE);
}


void CADSBLinkerDlg::OnShowhideExit()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	this->SendMessage(WM_CLOSE);
}


void CADSBLinkerDlg::RegistTrayIcon()
{
	NOTIFYICONDATA nid;
	//구조체 초기화
	ZeroMemory(&nid, sizeof(nid));

	//구조체 설정
	nid.cbSize = sizeof(nid);
	nid.hWnd = m_hWnd;//이 다이얼로그의 핸들
	nid.uID = IDR_TRAY_MENU;//트레이 아이콘 리소스 ID
	nid.uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE;
	nid.uCallbackMessage = WM_TRAYICON_MSG;//콜백 메시지 설정
	nid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);//아이콘 로드

	lstrcpy(nid.szTip, _T("ADSB_LINKER"));
	Shell_NotifyIcon(NIM_ADD, &nid);
	SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)nid.hIcon);
}

void CADSBLinkerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (nID == SC_CLOSE)
	{
		OnShowhideHide();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}