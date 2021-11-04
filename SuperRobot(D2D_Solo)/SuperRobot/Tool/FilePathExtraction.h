#pragma once
#include "afxwin.h"


// CFilePathExtraction ��ȭ �����Դϴ�.

class CFilePathExtraction : public CDialog
{
	DECLARE_DYNAMIC(CFilePathExtraction)

public:
	CFilePathExtraction(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CFilePathExtraction();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEPATHEXTRACTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	void HorizontalScroll();
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	list<PATHINFO*> m_listPathInfo; 
	CListBox m_ListBox;
	afx_msg void OnBnClickedSaveButton();
	afx_msg void OnBnClickedLoadButton();
};
