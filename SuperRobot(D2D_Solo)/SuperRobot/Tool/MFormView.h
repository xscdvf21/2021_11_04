#pragma once



// CMFormView �� ���Դϴ�.
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
	CMFormView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedUnit();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedPathExtraction();
};


