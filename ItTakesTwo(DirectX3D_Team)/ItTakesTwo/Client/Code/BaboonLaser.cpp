#include "stdafx.h"
#include "BaboonLaser.h"
#include "CollisionMgr.h"
#include "Export_Function.h"
#include "SpaceBaboon.h"
CBaboonLaser::CBaboonLaser(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEnermy(pGraphicDev), m_fLifeTime(0.f)
{
}

CBaboonLaser::~CBaboonLaser()
{
}

HRESULT CBaboonLaser::Ready_Object(void * pArg, CGameObject* pTarget)
{
	m_fColDis = 1.0f;

	// 콜라이더
	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = m_fColDis;

	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	m_pTarget = pTarget;

	CTransform*		pTargetTransform = (CTransform*)m_pTarget->Get_Component(L"Com_Transform", ID_DYNAMIC);
	m_vStartPos = pTargetTransform->Get_Position();


	return S_OK;

}

_int CBaboonLaser::Update_Object(const _float & fTimeDelta)
{
	m_fTimeDelta += fTimeDelta;
	if (m_bStart)
	{
		//Engine::CGameObject::Update_Object(fTimeDelta);
		m_bStart = false;
	}
	Move(fTimeDelta);
	//m_bColl = Collision_ToObject(L"GameLogic", L"Player");
	CCollisionMgr::GetInstance()->Add_EnermyWeaponList(this);
	Engine::CGameObject::Update_Object(fTimeDelta);
	if (m_bActive)
		Engine::Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

_int CBaboonLaser::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CBaboonLaser::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)


	m_pMeshCom->Render_Meshes(pEffect, 21);


	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CBaboonLaser::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_PLAYER, L"SpaceLaser"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Particle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);


	m_pTransformCom->Set_Scale(tTemp.vScale);

	return S_OK;
}

HRESULT CBaboonLaser::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj, matInv;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matInv, nullptr, &matView);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetFloat("g_DeltaTime", m_fTimeDelta);
	pEffect->SetFloat("g_fDistance", m_fAmount);
	m_pTextureCom->Set_Texture(pEffect, "g_DistortionTexture", 21);


	m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 15);
	//m_pTextureCom->Set_Texture(pEffect, "g_Add2Texture", 39);
	pEffect->SetVector("g_vColor", &_vec4{ 255.f * 3.5f, 125.f * 3.5f, 85.f * 3.5f, 255.f });


	pEffect->CommitChanges();

	return S_OK;
}

void CBaboonLaser::Move(const _float & fTimeDelta)
{


}

CBaboonLaser * CBaboonLaser::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, CGameObject* pTarget)
{
	CBaboonLaser*	pInstance = new CBaboonLaser(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg, pTarget)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaboonLaser::Free(void)
{

	CEnermy::Free();
}
