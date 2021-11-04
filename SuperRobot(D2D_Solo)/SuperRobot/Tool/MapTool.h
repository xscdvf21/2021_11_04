#pragma once
#include "afxwin.h"


// MapTool ��ȭ �����Դϴ�.

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapTool();
public:
	int Get_DrawID() { return m_iDrawID; }
// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
