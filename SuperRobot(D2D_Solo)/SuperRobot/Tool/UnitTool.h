#pragma once
#include "afxwin.h"


// CUnitTool 대화 상자입니다.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:// Test임
	CString m_string1;
	CString m_string2;
	afx_msg void OnBnClickedButton1();
public:	// UnitData
	int m_iAtt;
	int m_iHP;
	CListBox m_ListBox;
	CString m_strName;
	afx_msg void OnBnClickedAdd();
public:
	map<CString, UNITINFO*> m_mapUnitInfo; 
	afx_msg void OnLbnSelchangeFindName();
	CButton m_Class[3];
	CButton m_Item[3];
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
};
