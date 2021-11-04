#include "stdafx.h"
#include "MorningStar.h"
#include "Knight.h"
#include "MainCamera.h"
#include "Export_Function.h"
#include "CollisionMgr.h"

CMorningStar::CMorningStar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnermy(pGraphicDev)
{
}

CMorningStar::~CMorningStar()
{
}

HRESULT CMorningStar::Ready_Object(void * pArg)
{
	m_fColDis = 0.8f;

	// 콜라이더
	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = m_fColDis;

	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));


	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	lstrcpy(m_tParentBone, tTemp.tParentBone);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

_int CMorningStar::Update_Object(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Engine::CGameObject::Update_Object(fTimeDelta);


	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*		pKnight = static_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"GameObject", m_tParentBone, L"Com_Mesh", Engine::ID_STATIC));
		NULL_CHECK_RETURN(pKnight, 0);

		const Engine::D3DXFRAME_DERIVED* pFrame = pKnight->Get_FrameByName("RightHand");

		m_pParentBoneMatrix = &pFrame->CombinedTranformationMatrix;
		
		Engine::CTransform*		pKnightTransformCom = static_cast<Engine::CTransform*>(Engine::Get_Component(L"GameObject", m_tParentBone, L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pKnight, 0);


		m_pParentWorldMatrix = pKnightTransformCom->Get_WorldMatrix();

	}

	m_pTransformCom->Set_ParentMatrix((*m_pParentBoneMatrix * *m_pParentWorldMatrix));

	_matrix temp;
	//무기 디테일 위치.
	m_pTransformCom->Get_WorldMatrix(&temp);
	temp._41 = temp._41 + 0.05f;
	temp._42 = temp._42 - 0.1f;
	m_pTransformCom->Set_WorldMatrix(temp);

	if (m_bAttAble)
		CCollisionMgr::GetInstance()->Add_EnermyWeaponList(this);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CMorningStar::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CMorningStar::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 8);

	pEffect->End();

	Safe_Release(pEffect);

	CMainCamera* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
	pCamera->Update_View_Proj();

	//Spine2
	m_tColInfo.matWorld = *m_pTransformCom->Get_WorldMatrix();
	m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false);
}

HRESULT CMorningStar::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	// CStaticMesh
	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_CF, L"ToyKnight_MorningStar"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = static_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_fColDis, _vec4(50.f, 244.f, 234.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CMorningStar::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	return S_OK;
}

CMorningStar * CMorningStar::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CMorningStar*	pInstance = new CMorningStar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMorningStar::Free(void)
{
	CEnermy::Free();
}
