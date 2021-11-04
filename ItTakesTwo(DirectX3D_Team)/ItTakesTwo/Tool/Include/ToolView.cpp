
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
#include "Export_Function.h"

#include "MainFrm.h"
#include "MyFormView.h"
#include "MapTool.h"
#include "NaviMeshTool.h"
#include "CharacterTool.h"
#include "NormalTerrainTex.h"
#include "ToolObject.h"
#include "ToolNavi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// 전역변수
_int	g_FillMode = 0;

// CToolView 생성/소멸

CToolView::CToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CToolView::~CToolView()
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();

	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pDeviceClass);
	Engine::Safe_Release(m_pManagementClass);

	Engine::Release_Utility();
	Engine::Release_Resources();
	Engine::Release_System();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (nullptr == m_pManagementClass)
		return;

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
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
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
	CView::OnInitialUpdate();

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());

	RECT rcMain = {};
	pMain->GetWindowRect(&rcMain);
	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);

	RECT rcView;
	GetClientRect(&rcView);
	_int iGapX = rcMain.right - rcView.right;
	_int iGapY = rcMain.bottom - rcView.bottom;

	pMain->SetWindowPos(nullptr, 0, 0, 1280 + iGapX, 720 + iGapY, SWP_NOZORDER | SWP_NOMOVE);

	SetTimer(0, 0, nullptr);

	SetUp_DefaultSetting(&m_pGraphicDev, &m_pManagementClass);
	Resource_Loading();

	Ready_LightInfo();
	Ready_Terrain_Layer(L"Terrain");
	Ready_GameObject_Layer(L"GameObject");
	Engine::Get_Renderer()->End_Split();
	D3DXPlaneFromPoints(&m_tPlane, &_vec3(0.f, 0.f, 0.f), &_vec3(1.f, 0.f, 0.f), &_vec3(0.f, 0.f, 1.f));


}

HRESULT CToolView::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev, Engine::CManagement** ppManagementClass)
{
	Engine::Ready_Timer(L"Timer_Immediate");
	Engine::Ready_Timer(L"Timer_FPS60");
	Engine::Ready_Frame(L"FPS60", 60.f);

	g_hWnd = m_hWnd;
	Engine::Ready_GraphicDev(g_hWnd, Engine::MODE_WIN, 1280, 720, &m_pDeviceClass);

	m_pDeviceClass->AddRef();

	*ppGraphicDev = m_pDeviceClass->GetDevice();
	(*ppGraphicDev)->AddRef();


	Engine::Create_Management(ppManagementClass, *ppGraphicDev);
	(*ppManagementClass)->AddRef();

	//Get_Renderer()->Set_Device(m_pGraphicDev);
	Get_Renderer()->Ready_Renderer(m_pGraphicDev);
	Engine::Ready_InputDev(AfxGetInstanceHandle(), g_hWnd);

	return S_OK;
}

HRESULT CToolView::Resource_Loading()
{
	Engine::Reserve_ContainerSize(1);

	// Component
	Engine::CComponent*	pComponent = nullptr;

	pComponent = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	Engine::Ready_Prototype(L"Proto_Transform", pComponent);

	// Buffer
	//Engine::Ready_Buffer(m_pGraphicDev, 0, L"Buffer_RcTex", Engine::BUFFER_RCTEX);
	//Engine::Ready_Buffer(m_pGraphicDev, 0, L"Buffer_TriCol", Engine::BUFFER_TRICOL);
	//Engine::Ready_Buffer(m_pGraphicDev, 0, L"Buffer_TerrainTex", Engine::BUFFER_TERRAINTEX, 129, 129, 1);
	Engine::Ready_Buffer(m_pGraphicDev, 0, L"Buffer_NormalTerrainTex", Engine::BUFFER_NORMALTERRAINTEX, 129, 129, 1);
	Engine::Ready_Buffer(m_pGraphicDev, 0, L"Buffer_CubeTex", Engine::BUFFER_CUBETEX);
	Engine::Ready_Buffer(m_pGraphicDev, 0, L"Buffer_RoundTex", Engine::BUFFER_ROUNDTEX);


	// Texture
	Engine::Ready_Texture(m_pGraphicDev, 0, L"Sand", Engine::TEX_NORMAL, L"../../Resource/Texture/Terrain/Sand%d.tga", 5);


	// StaticMesh

	Engine::Ready_Meshes(m_pGraphicDev, 0, L"BossPlatform_Main", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Boss/BossPlatform_Main/", L"BossPlatform_Main0.x");

	Engine::Ready_Meshes(m_pGraphicDev, 0, L"BoxTower", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Box/BoxTower/", L"BoxTower0.x");
	Engine::Ready_Meshes(m_pGraphicDev, 0, L"BoxTowerSide", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Box/BoxTowerSide/", L"BoxTowerSide0.x");
	Engine::Ready_Meshes(m_pGraphicDev, 0, L"BoxSide", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Box/BoxSide/", L"BoxSide0.x");
	Engine::Ready_Meshes(m_pGraphicDev, 0, L"BoxDefault", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Box/BoxDefault/", L"BoxDefault0.x");



	// NaviMesh
	Engine::Ready_Meshes(m_pGraphicDev, 0, L"Mesh_Navi", Engine::MESH_NAVI, NULL, NULL);

	return S_OK;
}

HRESULT CToolView::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;



	m_mapLayer.emplace(pLayerTag, pLayer);
	return S_OK;
}

HRESULT CToolView::Ready_Terrain_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;



	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CToolView::Ready_GameObject_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	// dynamicCamera
	pGameObject = CToolCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(1280) / 720, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"MainCamera", pGameObject);

	// Navi
	pGameObject = CToolNavi::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Navi", pGameObject);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CToolView::Ready_LightInfo(void)
{
	D3DLIGHT9				tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	tLightInfo.Direction = _vec3(-1.f, -1.f, 1.f);

	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0);

	return S_OK;
}

void CToolView::OnTimer(UINT_PTR nIDEvent)
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMapTool* pMapTool = dynamic_cast<CMyFormView*>(pMain->m_MainSplitter.GetPane(0, 0))->m_pMapTool;
	CCharacterTool* pObjectTool = dynamic_cast<CMyFormView*>(pMain->m_MainSplitter.GetPane(0, 0))->m_pObjectTool;
	CNaviMeshTool* pNaviTool = dynamic_cast<CMyFormView*>(pMain->m_MainSplitter.GetPane(0, 0))->m_pNaviMeshTool;

	Engine::Set_TimeDelta(L"Timer_Immediate");
	_float fTimeImmediate = Engine::Get_TimeDelta(L"Timer_Immediate");

	if (Engine::IsPermit_Call(L"FPS60", fTimeImmediate))
	{
		Engine::Set_TimeDelta(L"Timer_FPS60");
		m_fTime = Engine::Get_TimeDelta(L"Timer_FPS60");
	}

	for (auto& layer : m_mapLayer)
	{
		layer.second->Update_Layer(m_fTime);
		layer.second->LateUpdate_Layer(m_fTime);
	}

	Engine::Render_Begin(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));
	if (m_bWireFrame)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pManagementClass->Render_Scene(0, g_vBackBufferColor);

	Engine::Render_End();

	Engine::Set_InputDev();

	//////////////////////////////////////////////////////////////////////////
	// TOOL UPDATE //
	pMapTool->Update_MapTool();
	pObjectTool->Update_ObjectTool();
	pNaviTool->Update_NaviMeshTool();

	KeyInput(m_fTime);
	//////////////////////////////////////////////////////////////////////////

	CView::OnTimer(nIDEvent);
}

void CToolView::KeyInput(_float fTimeDelta)
{
	if (GetFocus() != this)
		return;

	Mouse_Picking();
	ObjectMove(fTimeDelta);

	if (Engine::Key_Down(DIK_EQUALS))
		g_FillMode = 0;

	if (Engine::Key_Down(DIK_MINUS))
		g_FillMode = 1;
}

void CToolView::Mouse_Picking()
{
	if (Engine::Get_DIMouseState(Engine::DIM_LB) & 0x80 && Engine::Get_DIKeyState(DIK_SPACE) & 0x80)
	{
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CMapTool* pMapTool = dynamic_cast<CMyFormView*>(pMain->m_MainSplitter.GetPane(0, 0))->m_pMapTool;

		/* 레이캐스트 */
		auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(L"GameObject"));
		if (iter == m_mapLayer.end())
			return;
		CToolCamera* pCamera = (CToolCamera*)iter->second->Get_GameObject(L"MainCamera");

		iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(L"Terrain"));
		if (iter == m_mapLayer.end())
			return;
		CToolTerrain* pTerrain = (CToolTerrain*)iter->second->Get_GameObject(L"Terrain");

		_matrix matTerrainWorld;
		D3DXMatrixIdentity(&matTerrainWorld);
		_vec3 vCubePos, vGoalPos, vTemp;
		_float fDis = 0.f, fTemp = 0;
		_bool bStart = false;
		_bool bCreate = false;

		_bool bCol = pTerrain->m_pBufferCom->IsMousePicking<VTXTEX>(
			&vTemp,
			&fTemp,
			g_hWnd,
			1280,
			720,
			pCamera,
			&matTerrainWorld,
			pMapTool->m_fHeight * pMapTool->m_Mul,
			pMapTool->m_fRange,
			pMapTool->m_Round1Edge2Flat3);
	}



	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CNaviMeshTool* pNaviTool = dynamic_cast<CMyFormView*>(pMain->m_MainSplitter.GetPane(0, 0))->m_pNaviMeshTool;
	CCharacterTool* pObjectTool = dynamic_cast<CMyFormView*>(pMain->m_MainSplitter.GetPane(0, 0))->m_pObjectTool;

	if (Engine::Key_Down(Engine::DIM_LB) && pNaviTool->m_iMouseState == 1)  // NaviMesh Picking
	{
		auto pLayer = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(L"GameObject"));
		if (pLayer == m_mapLayer.end())
			return;
		CToolCamera* pCamera = (CToolCamera*)pLayer->second->Get_GameObject(L"MainCamera");
		CToolObject* pObject = (CToolObject*)pLayer->second->Get_GameObject(pObjectTool->m_Map);
		CToolNavi*	 pNavi = (CToolNavi*)pLayer->second->Get_GameObject(L"Navi");
		if (pCamera == nullptr || pObject == nullptr)
			return;

		_matrix meshWorldMat = *pObject->m_pTransformCom->Get_WorldMatrix();
		_vec3 vPos = _vec3(0.f, 0.f, 0.f);

		if (!Translation_ToLocalSpace(g_hWnd, 1280, 720, pCamera, &meshWorldMat))
			return;
		//RayPicking(&vPos, pObject->m_pStaticMeshCom->Get_VtxPos(), pObject->m_pStaticMeshCom->Get_VtxCnt(), m_vRayPos, m_vRayDir);

		pNaviTool->UpdateData(TRUE);
		if (pNaviTool->m_iPickCount == 0 &&
			RayPicking(&vPos, pObject->m_pStaticMeshCom->Get_VtxPos(), pObject->m_pStaticMeshCom->Get_VtxCnt(), m_vRayPos, m_vRayDir))
		{
			D3DXVec3TransformCoord(&vPos, &vPos, &meshWorldMat);

			pNaviTool->m_FirstX = std::to_wstring(vPos.x).c_str();
			pNaviTool->m_FirstY = std::to_wstring(vPos.y).c_str();
			pNaviTool->m_FirstZ = std::to_wstring(vPos.z).c_str();

			m_vFirst = vPos;
			m_bNavi = true;
		}
		else if (pNaviTool->m_iPickCount == 1 &&
			RayPicking(&vPos, pObject->m_pStaticMeshCom->Get_VtxPos(), pObject->m_pStaticMeshCom->Get_VtxCnt(), m_vRayPos, m_vRayDir))
		{
			D3DXVec3TransformCoord(&vPos, &vPos, &meshWorldMat);

			pNaviTool->m_SecondX = std::to_wstring(vPos.x).c_str();
			pNaviTool->m_SecondY = std::to_wstring(vPos.y).c_str();
			pNaviTool->m_SecondZ = std::to_wstring(vPos.z).c_str();

			m_vSecond = vPos;
			m_bNavi = true;
		}
		else if (pNaviTool->m_iPickCount == 2 &&
			RayPicking(&vPos, pObject->m_pStaticMeshCom->Get_VtxPos(), pObject->m_pStaticMeshCom->Get_VtxCnt(), m_vRayPos, m_vRayDir))
		{
			D3DXVec3TransformCoord(&vPos, &vPos, &meshWorldMat);

			pNaviTool->m_ThirdX = std::to_wstring(vPos.x).c_str();
			pNaviTool->m_ThirdY = std::to_wstring(vPos.y).c_str();
			pNaviTool->m_ThirdZ = std::to_wstring(vPos.z).c_str();

			m_vThird = vPos;

			pNavi->m_pNaviMeshCom->Add_Cell(m_vFirst, m_vSecond, m_vThird, pNaviTool->m_CellGroupIndex, pNaviTool->m_eCellState, pNaviTool->m_bLock);
			vector<vector<CCell*>> pvecCell = pNavi->m_pNaviMeshCom->Get_VecCell();

			pNaviTool->m_NaviList.ResetContent();
			for (_uint i = 0; i < pvecCell[pNaviTool->m_CellGroupIndex].size(); ++i)
				pNaviTool->m_NaviList.AddString(std::to_wstring(i).c_str());
			m_bNavi = true;
		}
		if (m_bNavi)
		{
			++pNaviTool->m_iPickCount;
			if (pNaviTool->m_iPickCount > 2)
				pNaviTool->m_iPickCount = 0;
			m_bNavi = false;
		}
		pNaviTool->UpdateData(FALSE);
	}

	else if (Engine::Key_Pressing(Engine::DIM_LB) && pNaviTool->m_iMouseState == 2) // Object Position Mesh Picking 
	{
		auto pLayer = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(L"GameObject"));

		if (pLayer == m_mapLayer.end())
			return;
		CToolCamera* pCamera = (CToolCamera*)pLayer->second->Get_GameObject(L"MainCamera");
		CToolObject* pMap = (CToolObject*)pLayer->second->Get_GameObject(pObjectTool->m_Map);
		CToolObject* pObject = (CToolObject*)pLayer->second->Get_GameObject(pObjectTool->m_Texture);

		if (pCamera == nullptr || pMap == nullptr)
			return;

		_matrix meshWorldMat = *pMap->m_pTransformCom->Get_WorldMatrix();
		_vec3 vPos = _vec3(0.f, 0.f, 0.f);

		if (!Translation_ToLocalSpace(g_hWnd, 1280, 720, pCamera, &meshWorldMat))
			return;

		if (RayPicking(&vPos, pMap->m_pStaticMeshCom->Get_VtxPos(), pMap->m_pStaticMeshCom->Get_VtxCnt(), m_vRayPos, m_vRayDir))
		{
			D3DXVec3TransformCoord(&vPos, &vPos, &meshWorldMat);
			pObject->m_pTransformCom->Set_Pos(vPos);
		}
	}
	else if (Engine::Key_Pressing(Engine::DIM_LB) && pNaviTool->m_iMouseState == 3)  // Object Position Height Picking 
	{
		auto pLayer = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(L"GameObject"));
		if (pLayer == m_mapLayer.end())
			return;

		CToolCamera* pCamera = (CToolCamera*)pLayer->second->Get_GameObject(L"MainCamera");
		//CToolObject* pMap = (CToolObject*)pLayer->second->Get_GameObject(pObjectTool->m_Map);
		CToolObject* pObject = (CToolObject*)pLayer->second->Get_GameObject(pObjectTool->m_Texture);

		//if (pCamera == nullptr || pMap == nullptr)
		//	return;

		_matrix PlaneWorldMat;
		_vec3 vPos = _vec3(0.f, 0.f, 0.f);

		Translation_ToWorldSpace(g_hWnd, 1280, 720, pCamera);

		//
		//_vec3 vPos = _vec3(0.f, 0.f, 0.f);
		D3DXPlaneIntersectLine(&vPos, &m_tPlane, &m_vRayPos, &(m_vRayPos + m_vRayDir * 10000.f));

		pObject->m_pTransformCom->Set_Pos((vPos));
	}
	else if (Engine::Key_Down(Engine::DIM_LB) && pNaviTool->m_iMouseState == 5)
	{
		auto pLayer = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(L"GameObject"));

		if (pLayer == m_mapLayer.end())
			return;
		CToolCamera* pCamera = (CToolCamera*)pLayer->second->Get_GameObject(L"MainCamera");
		CToolObject* pMap = (CToolObject*)pLayer->second->Get_GameObject(pObjectTool->m_Map);
		CToolObject* pObject = (CToolObject*)pLayer->second->Get_GameObject(pObjectTool->m_Texture);

		if (pCamera == nullptr || pMap == nullptr)
			return;

		_matrix meshWorldMat = *pMap->m_pTransformCom->Get_WorldMatrix();
		_vec3 vPos = _vec3(0.f, 0.f, 0.f);

		if (!Translation_ToLocalSpace(g_hWnd, 1280, 720, pCamera, &meshWorldMat))
			return;

		if (RayPicking(&vPos, pMap->m_pStaticMeshCom->Get_VtxPos(), pMap->m_pStaticMeshCom->Get_VtxCnt(), m_vRayPos, m_vRayDir))
		{
			D3DXVec3TransformCoord(&vPos, &vPos, &meshWorldMat);
			//pObject->m_pTransformCom->Set_Pos(vPos);

			//피킹 좌표 저장
			//3개 됐을경우 3점 탐색 
			//없는 버텍스만 추가 
			//새로운 인덱스 3개 추가 각점에 순서 가져옴
			//있을경우 개수 +1

			//삭제시 
			//3점  개수 탐색
			//타겟 인덱스 삭제
			//개수가 1이면 삭제
			//아닐시 -1


		}
	}
}

void CToolView::ObjectMove(_float fTimeDelta)
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CCharacterTool* pObjectTool = dynamic_cast<CMyFormView*>(pMain->m_MainSplitter.GetPane(0, 0))->m_pObjectTool;

	if (Engine::Get_DIKeyState(DIK_X) & 0x80)
	{
		pObjectTool->m_SelectPos[0].SetCheck(TRUE);
		pObjectTool->m_SelectPos[1].SetCheck(FALSE);
		pObjectTool->m_SelectPos[2].SetCheck(FALSE);
	}
	if (Engine::Get_DIKeyState(DIK_Y) & 0x80)
	{
		pObjectTool->m_SelectPos[0].SetCheck(FALSE);
		pObjectTool->m_SelectPos[1].SetCheck(TRUE);
		pObjectTool->m_SelectPos[2].SetCheck(FALSE);
	}
	if (Engine::Get_DIKeyState(DIK_Z) & 0x80)
	{
		pObjectTool->m_SelectPos[0].SetCheck(FALSE);
		pObjectTool->m_SelectPos[1].SetCheck(FALSE);
		pObjectTool->m_SelectPos[2].SetCheck(TRUE);
	}

	auto pLayer = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(L"GameObject"));
	if (pLayer == m_mapLayer.end())
		return;
	map<_wstr, CGameObject*> pGameObject = pLayer->second->Get_GameObjectMap();

	auto pObject = find_if(pGameObject.begin(), pGameObject.end(), CTag_Finder(pObjectTool->m_Texture));
	if (pObject == pGameObject.end())
		return;

	if (Engine::Key_Pressing(DIK_MINUS))
	{
		if (pObjectTool->m_CheckPos == 0)
			dynamic_cast<CToolObject*>(pObject->second)->m_pTransformCom->Move_Pos(&_vec3(1.f, 0.f, 0.f), -3.f, fTimeDelta);
		else if (pObjectTool->m_CheckPos == 1)
			dynamic_cast<CToolObject*>(pObject->second)->m_pTransformCom->Move_Pos(&_vec3(0.f, 1.f, 0.f), -3.f, fTimeDelta);
		else if (pObjectTool->m_CheckPos == 2)
			dynamic_cast<CToolObject*>(pObject->second)->m_pTransformCom->Move_Pos(&_vec3(0.f, 0.f, 1.f), -3.f, fTimeDelta);
	}
	if (Engine::Key_Pressing(DIK_EQUALS))
	{
		if (pObjectTool->m_CheckPos == 0)
			dynamic_cast<CToolObject*>(pObject->second)->m_pTransformCom->Move_Pos(&_vec3(1.f, 0.f, 0.f), 3.f, fTimeDelta);
		else if (pObjectTool->m_CheckPos == 1)
			dynamic_cast<CToolObject*>(pObject->second)->m_pTransformCom->Move_Pos(&_vec3(0.f, 1.f, 0.f), 3.f, fTimeDelta);
		else if (pObjectTool->m_CheckPos == 2)
			dynamic_cast<CToolObject*>(pObject->second)->m_pTransformCom->Move_Pos(&_vec3(0.f, 0.f, 1.f), 3.f, fTimeDelta);
	}
	if (Engine::Key_Pressing(DIK_9))
	{
		if (pObjectTool->m_CheckPos == 0)
			dynamic_cast<CToolObject*>(pObject->second)->m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(-90.f * fTimeDelta));
		else if (pObjectTool->m_CheckPos == 1)
			dynamic_cast<CToolObject*>(pObject->second)->m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(-90.f * fTimeDelta));
		else if (pObjectTool->m_CheckPos == 2)
			dynamic_cast<CToolObject*>(pObject->second)->m_pTransformCom->Rotation(Engine::ROT_Z, D3DXToRadian(-90.f * fTimeDelta));
	}
	if (Engine::Key_Pressing(DIK_0))
	{
		if (pObjectTool->m_CheckPos == 0)
			dynamic_cast<CToolObject*>(pObject->second)->m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(90.f * fTimeDelta));
		else if (pObjectTool->m_CheckPos == 1)
			dynamic_cast<CToolObject*>(pObject->second)->m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(90.f * fTimeDelta));
		else if (pObjectTool->m_CheckPos == 2)
			dynamic_cast<CToolObject*>(pObject->second)->m_pTransformCom->Rotation(Engine::ROT_Z, D3DXToRadian(90.f * fTimeDelta));
	}
}

_bool CToolView::RayPicking(_vec3* vPos, const _vec3 * vtxpos, _ulong vtxcnt, _vec3 raypos, _vec3 raydir)
{
	_vec3 q, v, c, q_minus_c;
	float A, B, C, D;
	float fBest = 10000.f;
	float fDist = 0.f;
	bool bResult = false;
	for (_ulong i = 0; i < vtxcnt; ++i)
	{
		q = raypos;
		v = raydir;
		c = vtxpos[i];
		q_minus_c = q - c;

		A = D3DXVec3Dot(&v, &v);
		B = D3DXVec3Dot(&v, &q_minus_c);
		C = D3DXVec3Dot(&q_minus_c, &q_minus_c) - 30.f * 30.f;

		D = B * B - A * C;

		if (D < 0)
			continue;

		float t0 = -(B + sqrt(D)) / A;
		float t1 = -(B - sqrt(D)) / A;
		if (t0 < 0 && t1 < 0)
			continue;

		fDist = D3DXVec3Length(&(c - raypos));
		if (fBest > fDist)
		{

			fBest = fDist;
			*vPos= c;
		}
		bResult = true;
	}

	return bResult;
}


_bool CToolView::Translation_ToLocalSpace(HWND hWnd, _uint iWinCX, _uint iWinCY, CCamera * pCamera, _matrix * pWorldMatrix)
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(&ptMouse);

	if (ptMouse.x >= (LONG)iWinCX || ptMouse.x <= 0 || ptMouse.y >= (LONG)iWinCY || ptMouse.y <= 0)
		return false;

	/* 뷰포트 -> 투영스페이스 */
	m_vMouse = _vec3(0.f, 0.f, 0.f);
	m_vMouse.x = ptMouse.x / (iWinCX * 0.5f) - 1.f;
	m_vMouse.y = 1.f - ptMouse.y / (iWinCY * 0.5f);

	/* 투영스페이스 -> 뷰스페이스 */
	_matrix matProj = pCamera->Get_Projection();
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&m_vMouse, &m_vMouse, &matProj);

	m_vRayPos = _vec3(0.f, 0.f, 0.f);
	m_vRayDir = m_vMouse - m_vRayPos;
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);

	/* 뷰스페이스 -> 월드스페이스 */
	_matrix matView = pCamera->Get_View();
	D3DXMatrixInverse(&matView, 0, &matView);
	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &matView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matView);



	/* 월드스페이스 -> 로컬스페이스 */
	_matrix matInvWorld;
	D3DXMatrixInverse(&matInvWorld, 0, pWorldMatrix);
	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &matInvWorld);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matInvWorld);

	return true;
}

_bool CToolView::Translation_ToWorldSpace(HWND hWnd, _uint iWinCX, _uint iWinCY, CCamera * pCamera)
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(&ptMouse);

	if (ptMouse.x >= (LONG)iWinCX || ptMouse.x <= 0 || ptMouse.y >= (LONG)iWinCY || ptMouse.y <= 0)
		return false;

	/* 뷰포트 -> 투영스페이스 */
	m_vMouse = _vec3(0.f, 0.f, 0.f);
	m_vMouse.x = ptMouse.x / (iWinCX * 0.5f) - 1.f;
	m_vMouse.y = 1.f - ptMouse.y / (iWinCY * 0.5f);

	/* 투영스페이스 -> 뷰스페이스 */
	_matrix matProj = pCamera->Get_Projection();
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&m_vMouse, &m_vMouse, &matProj);

	m_vRayPos = _vec3(0.f, 0.f, 0.f);
	m_vRayDir = m_vMouse - m_vRayPos;
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);


	/* 뷰스페이스 -> 월드스페이스 */
	_matrix matView = pCamera->Get_View();
	D3DXMatrixInverse(&matView, 0, &matView);
	D3DXVec3TransformCoord(&m_vRayPos, &m_vRayPos, &matView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matView);
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);

	return true;
}
