#include "stdafx.h"
#include "Orb.h"
#include "May_Castle.h"
#include "MainCamera.h"
#include "CollisionMgr.h"
#include "Export_Function.h"


COrb::COrb(LPDIRECT3DDEVICE9 pGraphicDev)
	: CPlayer(pGraphicDev)
{

}

COrb::~COrb(void)
{

}

HRESULT COrb::Ready_Object(char* pBoneName,void* pArg)
{
	m_fColDis = 1.0f;
	strcpy_s(strBoneName, pBoneName);

	// 콜라이더
	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = m_fColDis;

	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);
	m_pTransformCom->Set_Scale(_float3(0.2f, 0.2f, 0.2f));

	m_tShader.m_fRimDist = 1.f;
	m_tShader.m_iRimPower = 100;
	m_tShader.m_vRimColor = _vec4(0.3f, 0.7f, 1.f, 1.f);


	return S_OK;
}

Engine::_int COrb::Update_Object(const _float& fTimeDelta)
{
	if (m_bStart)
	{
		m_bStart = false;
		return 0;
	}

	if (nullptr == m_pParentBoneMatrix)
	{
		CMayCastle* pPlayer = (CMayCastle*)Engine::Get_GameObject(L"GameObject", L"Cody");
		Engine::CDynamicMesh*	pPlayerMeshCom = (CDynamicMesh*)pPlayer->Get_Component(L"Com_Mesh", ID_STATIC);
		NULL_CHECK_RETURN(pPlayerMeshCom, 0);

		const Engine::D3DXFRAME_DERIVED* pFrame = pPlayerMeshCom->Get_FrameByName(strBoneName);

		m_pParentBoneMatrix = &pFrame->CombinedTranformationMatrix;

		Engine::CTransform*		pSpongeBobTransformCom = dynamic_cast<CTransform*>(pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC));

		m_pParentWorldMatrix = pSpongeBobTransformCom->Get_WorldMatrix();
	}

	Engine::CGameObject::Update_Object(fTimeDelta);


	_matrix matTrans;
	_vec3 vTrans = _vec3(10.f, 10.f, 0.f);
	D3DXMatrixTranslation(&matTrans, vTrans.x, vTrans.y, vTrans.z);

	m_pTransformCom->Set_ParentMatrix((matTrans * *m_pParentBoneMatrix * *m_pParentWorldMatrix));


	//m_bColl = Collision_ToObject(L"GameLogic", L"Player");

	CCollisionMgr::GetInstance()->Add_PlayerWeaponList(this);
	if (m_bActive)
		Engine::Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

_int COrb::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void COrb::Render_Object(const _int& iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)

	m_pMeshCom->Render_Meshes(pEffect, 1);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	CMainCamera* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
	pCamera->Update_View_Proj();

	//Spine2
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	_float4x4 matWorld;
	D3DXMatrixTranslation(&matWorld, 0.0f, 80.0f, 0.0f);

	m_tColInfo.matWorld = matWorld * *m_pTransformCom->Get_WorldMatrix();
	m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false);
}

COrb* COrb::Create(LPDIRECT3DDEVICE9 pGraphicDev, char* pBoneName, void* pArg)
{
	COrb*	pInstance = new COrb(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pBoneName,pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void COrb::Free(void)
{
	CPlayer::Free();
}

HRESULT COrb::Add_Component(void* pArg)
{

	Engine::CComponent*		pComponent = nullptr;
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_PLAYER, L"IceOrb"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, 1.0f, _vec4(100.f, 255.f, 0.0f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT COrb::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj, matInv;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matInv, nullptr, &matView);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetFloat("g_fRimDist", m_tShader.m_fRimDist);
	pEffect->SetInt("g_iRimPower", m_tShader.m_iRimPower);
	pEffect->SetVector("g_vRimColor", &m_tShader.m_vRimColor);

	pEffect->SetVector("g_vCamPos", &_vec4(matInv._41, matInv._42, matInv._43, 1.f));

	pEffect->CommitChanges();

	return S_OK;
}
