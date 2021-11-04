#include "stdafx.h"
#include "Cone.h"
#include "May_Castle.h"
#include "MainCamera.h"
#include "CollisionMgr.h"
#include "Export_Function.h"
#include "Cody_Wizard.h"


CCone::CCone(LPDIRECT3DDEVICE9 pGraphicDev)
	: CPlayer(pGraphicDev)
{

}

CCone::~CCone(void)
{

}

HRESULT CCone::Ready_Object(void* pArg)
{
	m_fColDis = 1.0f;

	// 콜라이더
	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = m_fColDis;

	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);


	return S_OK;
}

Engine::_int CCone::Update_Object(const _float& fTimeDelta)
{
	if (m_bStart)
	{
		m_bStart = false;
		return 0;
	}

	m_fTimeDelta += fTimeDelta;

	if (m_iCount == 0 || m_iCount == 1)
	{
		CCodyWizard* pPlayer = (CCodyWizard*)Engine::Get_GameObject(L"GameObject", L"Cody");
		CTransform* pPlayerTrans = (CTransform*)pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC);

		m_pTransformCom->Set_Pos(pPlayerTrans->Get_Position() + _vec3(0.f, 1.f, 0.f));
		m_pTransformCom->Set_Angle(pPlayerTrans->Get_Angle() + _vec3(0.f, D3DXToRadian(-90.f), 0.f));
	}
	else if (m_iCount == 2)
	{
	
	
	}
	else if (m_iCount == 3)
	{

	}

	Engine::CGameObject::Update_Object(fTimeDelta);



	//m_bColl = Collision_ToObject(L"GameLogic", L"Player");

	CCollisionMgr::GetInstance()->Add_PlayerWeaponList(this);
	Engine::Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

_int CCone::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CCone::Render_Object(const _int& iIndex)
{
	if (!m_bActive)
		return;

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)


	if (m_iCount == 0)
		m_pMeshCom->Render_Meshes(pEffect, 10);
	else if (m_iCount == 1)
		m_pMeshCom->Render_Meshes(pEffect, 11);
	else if (m_iCount == 2)
		m_pMeshCom->Render_Meshes(pEffect, 19);
	else if (m_iCount == 3)
		m_pMeshCom->Render_Meshes(pEffect, 20);

	pEffect->End();

	Safe_Release(pEffect);


	_float4x4 matWorld;
	D3DXMatrixTranslation(&matWorld, 0.0f, 80.0f, 0.0f);

	m_tColInfo.matWorld = matWorld * *m_pTransformCom->Get_WorldMatrix();
	m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false);
}

CCone* CCone::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CCone*	pInstance = new CCone(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CCone::Free(void)
{
	CPlayer::Free();
}

HRESULT CCone::Add_Component(void* pArg)
{

	Engine::CComponent*		pComponent = nullptr;
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_PLAYER, L"Cone"));
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

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Particle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	m_pTransformCom->Set_Scale(tTemp.vScale);


	return S_OK;
}

HRESULT CCone::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
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

	if (m_iCount == 0)
	{
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 18);
		m_pTextureCom->Set_Texture(pEffect, "g_Add2Texture", 39);
		pEffect->SetVector("g_vColor", &_vec4{ 150.f * 2.5f, 200.f * 3.5f, 255.f * 4.5f, 100.f });
	}
	else if (m_iCount == 1)
	{
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 40);
		m_pTextureCom->Set_Texture(pEffect, "g_Add2Texture", 39);
		m_pTextureCom->Set_Texture(pEffect, "g_Add3Texture", 41);
		pEffect->SetVector("g_vColor", &_vec4{ 100.f * 5.5f, 150.f * 5.5f, 255.f * 5.5f, 255.f * 3.5f });
	}
	else if (m_iCount == 2)
	{
		m_pTextureCom->Set_Texture(pEffect, "g_Add2Texture", 34);
		pEffect->SetVector("g_vColor", &_vec4{ 255.f * 3.5f, 50.f* 3.5f , 55.f* 3.5f , 205.f  });
	}
	else if (m_iCount == 3)
	{
		m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 40);
		m_pTextureCom->Set_Texture(pEffect, "g_Add2Texture", 39);
		m_pTextureCom->Set_Texture(pEffect, "g_Add3Texture", 41);
		pEffect->SetVector("g_vColor", &_vec4{ 100.f * 5.5f, 150.f * 5.5f, 255.f * 5.5f, 255.f * 3.5f });
	}


	pEffect->CommitChanges();

	return S_OK;
}
