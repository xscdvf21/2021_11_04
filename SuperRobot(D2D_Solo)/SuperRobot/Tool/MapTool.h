#pragma once
#include "afxwin.h"


// MapTool 대화 상자입니다.

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();
public:
	int Get_DrawID() { return m_iDrawID; }
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	int m_iDrawID; 
	CListBox m_ListBox;
	CStatic m_Picture;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	void HorizontalScroll(); 
	afx_msg void OnLbnSelchangePicture();
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
