
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "MFCTool.h"

#include "MainFrm.h"

#include "MFCToolView.h"
#include "MyFormView.h"

extern CMFCToolView*	g_pView;



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: ���� ������ ��ŷ�� �� ���� �Ϸ��� �� �� ���� �����Ͻʽÿ�.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	CRect rect;
	GetClientRect(&rect);

	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		return FALSE;
	}

	int nSizeX = rect.right - rect.left;
	int nMenuSize = nSizeX / 6;

	//if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CMFCToolView/*�� �߰�.*/), CSize(nSizeX - nMenuSize, 0), pContext))
	//{
	//	return FALSE;
	//}
	//if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CMyFormView/*�� �߰�*/), CSize(nMenuSize, 0), pContext))
	//{
	//	return FALSE;
	//}


	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CMFCToolView/*�� �߰�.*/), CSize(nSizeX - nMenuSize, 0), pContext))
	{
		return FALSE;
	}
	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CMyFormView/*�� �߰�*/), CSize(nMenuSize, 0), pContext))
	{
		return FALSE;
	}

	m_wndSplitter.SetColumnInfo(0, rect.Width() / 2, 50);
	m_wndSplitter.SetColumnInfo(1, rect.Width() / 2, 50);

	int num = nSizeX - nMenuSize;
	int num2 = rect.Width() / 2;
	
	g_pView = (CMFCToolView*)m_wndSplitter.GetPane(0, 0); //0, 0 �� View // 0, 1 �� Form

	return TRUE;
	//return m_wndSplitter.Create(this,
	//	2, 2,               // TODO: �� �� ���� ������ �����մϴ�.
	//	CSize(10, 10),      // TODO: �ּ� â ũ�⸦ �����մϴ�.
	//	pContext);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame �޽��� ó����



BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	return FALSE;
	//return CFrameWnd::OnEraseBkgnd(pDC);
}
