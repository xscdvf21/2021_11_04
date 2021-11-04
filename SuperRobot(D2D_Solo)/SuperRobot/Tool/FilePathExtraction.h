#pragma once
#include "afxwin.h"


// CFilePathExtraction 대화 상자입니다.

class CFilePathExtraction : public CDialog
{
	DECLARE_DYNAMIC(CFilePathExtraction)

public:
	CFilePathExtraction(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFilePathExtraction();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEPATHEXTRACTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	void HorizontalScroll();
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	list<PATHINFO*> m_listPathInfo; 
	CListBox m_ListBox;
	afx_msg void OnBnClickedSaveButton();
	afx_msg void OnBnClickedLoadButton();
};
