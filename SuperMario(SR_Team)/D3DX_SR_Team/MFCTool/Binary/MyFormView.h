#pragma once
#include "afxcmn.h"


class CTap01;
class CTap02;
class CTap03;
class CTap04;
// CMyFormView �� ���Դϴ�.

class CMyFormView : public CFormView
{
	DECLARE_DYNCREATE(CMyFormView)

protected:
	CMyFormView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMyFormView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORMVIEW };
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
	CTabCtrl m_Tab;

	CTap01* m_pTap01 = nullptr;
	CTap02* m_pTap02 = nullptr;
	CTap03* m_pTap03 = nullptr;
	CTap04* m_pTap04 = nullptr;

	virtual void OnInitialUpdate();
	afx_msg void OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);

	void Update_Tap();
};


