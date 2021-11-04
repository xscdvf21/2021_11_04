
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "Texture_Manager.h"
#include "Single_Texture.h"
#include "Terrain.h"
#include "MainFrm.h"
#include "MiniView.h"
#include "MapTool.h"
#include "MFormView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�
HWND g_hWND;
CToolView::CToolView()
//:m_pSingletexture(nullptr)
	: m_fAngle(0.f)
	, m_pTerrain(nullptr)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CToolView::~CToolView()
{
// 	if (m_pSingletexture)
// 	{
// 		delete m_pSingletexture; 
// 		m_pSingletexture = nullptr; 
// 	}
	//KillTimer(1); 
	Safe_Delete(m_pTerrain); 
	CGraphic_Device::Destroy_Instance(); 
	CTexture_Manager::Destroy_Instance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView �׸���

void CToolView::OnDraw(CDC*/* pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	CGraphic_Device::Get_Instance()->Render_Begin(); 
	m_pTerrain->Render_Terrain(); 
	CGraphic_Device::Get_Instance()->Render_End();
}


// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


void CToolView::OnInitialUpdate()
{
	
	CScrollView::OnInitialUpdate();
	SIZE tScrollsize = {TILECX * TILEX, (TILECY >> 1)* TILEY };
	SetScrollSizes(MM_TEXT,tScrollsize );
	//m_iTimerID = SetTimer(0, 0, nullptr);
	g_hWND = m_hWnd; 
	//SetTimer()
	if (FAILED(CGraphic_Device::Get_Instance()->Ready_Graphic_Device()))
	{
		ERR_MSG(L"Graphic_device Failed");
		return; 
	}

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_TextureMgr(
		CTexture_Manager::MULTI_TEX,
		L"../Texture/Map/Stage%d.png",
		L"Map", L"Stage",
		38)))
		return;

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_TextureMgr(
		CTexture_Manager::MULTI_TEX,
		L"../Texture/Stage/Player/Attack/AKIHA_AKI01_00%d.png",
		L"Player", L"Attack",
		6)))
		return; 
	if (FAILED(CTexture_Manager::Get_Instance()->Insert_TextureMgr(
		CTexture_Manager::MULTI_TEX,
		L"../Texture/Stage/Player/Dash/AKIHA_AKI13_00%d.png",
		L"Player", L"Dash", 11)))
		return;


	if (!m_pTerrain)
	{
		m_pTerrain = new CTerrain; 
		m_pTerrain->Ready_Terrain(); 
		m_pTerrain->Set_View(this);
	}

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	RECT rcMain = {}; 
	pMain->GetWindowRect(&rcMain);
	// �̽�Ű�� ��ũ����ǥ�� ��ȯ. 
	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top); 
	RECT rcView = {}; 
	GetClientRect(&rcView);

	int iWidth = rcMain.right - rcView.right; 
	int iHeight = rcMain.bottom - rcView.bottom;

	pMain->SetWindowPos(nullptr, 0, 0, WINCX + iWidth + 18, WINCY + iHeight, SWP_NOZORDER); 
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMain->m_SecondSplitter.GetPane(0, 0));
	CMapTool* pMapTool = &dynamic_cast<CMFormView*>(pMain->m_SecondSplitter.GetPane(1, 0))->m_tMapTool;
	int iDrawID = pMapTool->Get_DrawID();

	D3DXVECTOR3 vMouse = { float(point.x + GetScrollPos(0)) , float(point.y + GetScrollPos(1)),0.f };
	m_pTerrain->Tile_Change_Terrain(vMouse, iDrawID);
	
	//TCHAR szPoint[64] = {}; 
	//wsprintf(szPoint, L"Pt.x : %d, Pt.y : %d", point.x, point.y); 
	/*ERR_MSG(szPoint);*/
	Invalidate(0);

	pMiniView->Invalidate(FALSE);
	CScrollView::OnLButtonDown(nFlags, point);
}


void CToolView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	Invalidate(0);
	CScrollView::OnTimer(nIDEvent);
}
