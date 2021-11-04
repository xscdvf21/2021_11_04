#pragma once



// CMFormView 폼 뷰입니다.
#include "UnitTool.h"
#include "MapTool.h"
#include "FilePathExtraction.h"
class CMFormView : public CFormView
{
	DECLARE_DYNCREATE(CMFormView)
public:
	CUnitTool m_tUnitTool;
	CMapTool m_tMapTool; 
	CFilePathExtraction m_tExtraction; 
protected:
	CMFormView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMFormView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFORMVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedUnit();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedPathExtraction();
};


