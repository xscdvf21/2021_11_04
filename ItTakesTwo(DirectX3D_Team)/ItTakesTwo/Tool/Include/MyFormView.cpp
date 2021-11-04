// MyFormView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "ToolDoc.h"
#include "MyFormView.h"

#include "MapTool.h"
#include "NaviMeshTool.h"
#include "CharacterTool.h"


// CMyFormView

IMPLEMENT_DYNCREATE(CMyFormView, CFormView)

CMyFormView::CMyFormView()
	: CFormView(IDD_MYFORMVIEW)
{
}

CMyFormView::~CMyFormView()
{
	if (m_pMapTool)
	{
		delete m_pMapTool;
		m_pMapTool = nullptr;
	}

	if (m_pObjectTool)
	{
		delete m_pObjectTool;
		m_pObjectTool = nullptr;
	}

	if (m_pNaviMeshTool)
	{
		delete m_pNaviMeshTool;
		m_pNaviMeshTool = nullptr;
	}
}

void CMyFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(CMyFormView, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyFormView::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// CMyFormView 진단입니다.

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


// CMyFormView 메시지 처리기입니다.


void CMyFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_Tab.InsertItem(0, L"Map");
	m_Tab.InsertItem(1, L"Object");
	m_Tab.InsertItem(2, L"Navi");

	m_Tab.SetCurSel(0);

	CRect rc, tRc;
	m_Tab.GetWindowRect(&rc);

	m_pMapTool = new CMapTool; 
	m_pMapTool->Create(IDD_MAPTOOL, &m_Tab);
	m_pMapTool->GetWindowRect(tRc);
	m_pMapTool->MoveWindow(5, 26, rc.Width() - 12, rc.Height() - 100);
	m_pMapTool->ShowWindow(SW_SHOW);
	m_Tab.MoveWindow(15, 15, tRc.Width() + 10, tRc.Height() + 30);

	m_pObjectTool = new CCharacterTool;
	m_pObjectTool->Create(IDD_CHARACTERTOOL, &m_Tab);
	m_pObjectTool->MoveWindow(5, 26, rc.Width() - 12, rc.Height());
	m_pObjectTool->ShowWindow(SW_HIDE);

	m_pNaviMeshTool = new CNaviMeshTool;
	m_pNaviMeshTool->Create(IDD_CAMERATOOL, &m_Tab);
	m_pNaviMeshTool->MoveWindow(5, 26, rc.Width() - 12, rc.Height());
	m_pNaviMeshTool->ShowWindow(SW_HIDE);
}

void CMyFormView::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iSel = m_Tab.GetCurSel();

	switch (iSel)
	{
	case 0:
		m_pMapTool->ShowWindow(SW_SHOW);
		m_pNaviMeshTool->ShowWindow(SW_HIDE);
		m_pObjectTool->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pMapTool->ShowWindow(SW_HIDE);
		m_pObjectTool->ShowWindow(SW_SHOW);
		m_pNaviMeshTool->ShowWindow(SW_HIDE);
		
		break;
	case 2:
		m_pMapTool->ShowWindow(SW_HIDE);
		m_pObjectTool->ShowWindow(SW_HIDE);
		m_pNaviMeshTool->ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}
