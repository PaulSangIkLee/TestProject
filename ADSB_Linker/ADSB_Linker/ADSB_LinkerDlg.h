
// ADSB_LinkerDlg.h: 헤더 파일
//

#pragma once
#include "ListnerAstrerix.h"
#include "ListnerJSONPingStation.h"
#include "BusinessManager.h"
#include "ADSBProtocol.h"

typedef enum DISP_TIMER_TYPE
{
	TIMERTYPE_DISPLAY,
	TIMERTYPE_OVER,
	TIMERTYPE_DELETE,
	TIMERTYPE_DEBUG,
	TIMERTYPE_CONNECT,
};

// CADSBLinkerDlg 대화 상자
class CADSBLinkerDlg : public CDialogEx
{
// 생성입니다.
public:
	CADSBLinkerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CADSBLinkerDlg();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADSB_LINKER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listADSB_Display;
	MAP_ADSB m_mapADSB;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//SendMessage를 통해 비행체 자료 수신
	afx_msg LRESULT RecvDecodeData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);

	void ADSB_List_Delete();
	void ADSB_List_Display();
	void ReleaseAll();
	CListnerAstrerix* m_pListnerAsterix;
	CListnerJSONPingStation* m_pListnerJSON;
	CBusinessManager* m_pBusinessMng;
	int m_nDisplayTime;
	CStatic m_ctrlStrAirCraftCount;
	CStatic m_ctrlStrCurrentTime;
	afx_msg void OnClose();
	CStatic m_ctrlStrConnectStatus;
	BOOL m_bShowStatus;

	LRESULT TrayIconMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShowhideShow();
	afx_msg void OnShowhideHide();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnShowhideExit();
	void RegistTrayIcon();
};
