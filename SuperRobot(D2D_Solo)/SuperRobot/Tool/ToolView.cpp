
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CToolView 생성/소멸
HWND g_hWND;
CToolView::CToolView()
//:m_pSingletexture(nullptr)
	: m_fAngle(0.f)
	, m_pTerrain(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.

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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView 그리기

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


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
	
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


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
	// 이시키는 스크린좌표를 반환. 
	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top); 
	RECT rcView = {}; 
	GetClientRect(&rcView);

	int iWidth = rcMain.right - rcView.right; 
	int iHeight = rcMain.bottom - rcView.bottom;

	pMain->SetWindowPos(nullptr, 0, 0, WINCX + iWidth + 18, WINCY + iHeight, SWP_NOZORDER); 
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	Invalidate(0);
	CScrollView::OnTimer(nIDEvent);
}
