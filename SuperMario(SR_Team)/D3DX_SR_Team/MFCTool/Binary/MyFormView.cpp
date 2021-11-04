// MyFormView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "MyFormView.h"

#include "Tap01.h";
#include "Tap02.h";
#include "Tap03.h";
#include "Tap04.h"

// CMyFormView

IMPLEMENT_DYNCREATE(CMyFormView, CFormView)

CMyFormView::CMyFormView()
	: CFormView(IDD_MYFORMVIEW)
{

}

CMyFormView::~CMyFormView()
{
}

void CMyFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(CMyFormView, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyFormView::OnSelchangeTab1)
END_MESSAGE_MAP()


// CMyFormView �����Դϴ�.

#ifdef _DEBUG
void CMyFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyFormView �޽��� ó�����Դϴ�.


void CMyFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_Tab.InsertItem(0, _T("�� �� 01"));
	m_Tab.InsertItem(1, _T("�� �� 02"));
	m_Tab.InsertItem(2, _T("EFFECT"));
	m_Tab.InsertItem(3, _T("MAP"));

	m_Tab.SetCurSel(0);

	CRect Rect;
	m_Tab.GetWindowRect(&Rect);

	m_pTap01 = new CTap01;
	m_pTap01->Create(IDD_TAP01, &m_Tab);
	m_pTap01->MoveWindow(0, 25, Rect.Width(), Rect.Height());
	m_pTap01->ShowWindow(SW_HIDE);

	m_pTap02 = new CTap02;
	m_pTap02->Create(IDD_TAP02, &m_Tab);
	m_pTap02->MoveWindow(0, 25, Rect.Width(), Rect.Height());
	m_pTap02->ShowWindow(SW_HIDE);

	m_pTap03 = new CTap03;
	m_pTap03->Create(IDD_TAP03, &m_Tab);
	m_pTap03->MoveWindow(0, 25, Rect.Width(), Rect.Height());
	m_pTap03->ShowWindow(SW_HIDE);

	m_pTap04 = new CTap04;
	m_pTap04->Create(IDD_Tap04, &m_Tab);
	m_pTap04->MoveWindow(0, 25, Rect.Width(), Rect.Height());
	m_pTap04->ShowWindow(SW_SHOW);
	
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}


void CMyFormView::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int sel = m_Tab.GetCurSel();

	switch (sel)
	{
	case 0:
		m_pTap01->ShowWindow(SW_SHOW);
		m_pTap02->ShowWindow(SW_HIDE);
		m_pTap03->ShowWindow(SW_HIDE);
		m_pTap04->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pTap01->ShowWindow(SW_HIDE);
		m_pTap02->ShowWindow(SW_SHOW);
		m_pTap03->ShowWindow(SW_HIDE);
		m_pTap04->ShowWindow(SW_HIDE);
		break;
	case 2:
		m_pTap01->ShowWindow(SW_HIDE);
		m_pTap02->ShowWindow(SW_HIDE);
		m_pTap03->ShowWindow(SW_SHOW);
		m_pTap04->ShowWindow(SW_HIDE);
		break;
	case 3:
		m_pTap01->ShowWindow(SW_HIDE);
		m_pTap02->ShowWindow(SW_HIDE);
		m_pTap03->ShowWindow(SW_HIDE);
		m_pTap04->ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}

void CMyFormView::Update_Tap()
{
//	m_pTap03->Showing_Cube();
}
