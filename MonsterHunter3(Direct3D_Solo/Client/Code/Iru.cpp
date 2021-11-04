#include "stdafx.h"
#include "Iru.h"

#include "Player.h"
#include "NaviMgr.h"
#include "SoundMgr.h"
#include "Aura.h"

#include "Export_Fucntion.h"



CIru::CIru(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CIru::~CIru()
{
}

HRESULT CIru::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = _vec3(0.02f, 0.02f, 0.02f);

	m_pTransformCom->Set_Pos(_vec3(30.f, 1.f, 220.f));

	m_pMeshCom->Set_Animationset(3);
	return S_OK;
}

_int CIru::Update_Object(const _float & fTimeDelta)
{

	m_fBuffTime += fTimeDelta;

	CManagement*	pManagement = CManagement::GetInstance();
	
	CTransform*		pPlayerTrans = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));

	Engine::CGameObject::Update_Object(fTimeDelta);

	_vec3 vPos;
	_vec3 vPlayerPos;

	_vec3 vDir;

	pPlayerTrans->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);


	vDir = vPlayerPos - vPos;

	_float fDir;


	fDir = D3DXVec3Length(&vDir);



	_vec3  vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);

	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vLook, &vLook);
	vDir.y = 0.f;
	vLook.y = 0.f;


	_vec3 vRight;
	_vec3 vLeft; 

	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vRight, &vRight);
	vLeft = vRight * -1.f;

	_float fAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vDir, &vLook)));


	_float fRAngle = D3DXVec3Dot(&vDir, &vRight);
	_float fLAngle = D3DXVec3Dot(&vDir, &vLeft);


	
	if (fDir >= 3.f && false == m_bBuffing )
	{
		m_pMeshCom->Set_Animationset(3);
		m_pTransformCom->Move_Pos(&vLook, 4.f, fTimeDelta);
		if (fAngle >= D3DXToDegree(m_TurnSpeed))
		{
			if (fRAngle > 0 && fLAngle < 0.f)
			{
 				m_pTransformCom->Rotation(Engine::ROT_Y, m_TurnSpeed);
			}
			else if (fRAngle < 0 && fLAngle > 0.f)
			{
				m_pTransformCom->Rotation(Engine::ROT_Y, -m_TurnSpeed);
			}
		}

	}
	else if(false == m_bBuffing)
	{
		m_pMeshCom->Set_Animationset(2);
	}


	if (false == m_bBuffing && m_fBuffTime > 40.f  || Engine::Get_DIKeyState(DIK_0) & 0x8000)
	{
		m_bBuffing = true;
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::IRU);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Bell.mp3", CSoundMgr::CHANNELID::IRU);
		m_pMeshCom->Set_Animationset(6);
	}

	if (true == m_bBuffing && true == m_pMeshCom->Is_AnimationSetEnd())
	{
		Engine::CLayer*			pLayer = nullptr;

		pLayer = pManagement->Get_Layer(L"GameLogic");
		NULL_CHECK_RETURN(pLayer, E_FAIL);

		// 오브젝트 추가
		Engine::CGameObject*		pGameObject = nullptr;

		pGameObject = CAura::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Aura_Buff", pGameObject), E_FAIL);
		
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::IRU);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Iru.mp3", CSoundMgr::CHANNELID::IRU);

		m_fBuffTime = 0.f;
		m_bBuffing = false;
	}


	m_pMeshCom->Play_Animationset(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);


	return 0;
}

void CIru::Render_Object(void)
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	/*m_pNaviMeshCom->Render_NaviMesh();*/

	m_pMeshCom->Render_Meshes();
}

HRESULT CIru::Add_Component(void)
{



	Engine::CComponent*		pComponent = nullptr;
	// CDynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Iru"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);


	pComponent = m_pNaviMeshCom = CNaviMgr::GetInstance()->Get_NaviMesh(RESOURCE_STAGE, NAVI_NORMAL);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Navi", pComponent);


	// NaviMesh
	//pComponent = m_pNaviMeshCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Navi"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Navi", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);


	return S_OK;
}

CIru * CIru::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CIru*	pInstance = new CIru(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CIru::Free(void)
{
	Engine::CGameObject::Free();
}
