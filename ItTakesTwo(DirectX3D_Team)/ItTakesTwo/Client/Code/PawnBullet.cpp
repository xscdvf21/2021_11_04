#include "stdafx.h"
#include "PawnBullet.h"

#include "Export_Function.h"



CPawnBullet::CPawnBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CPawnBullet::~CPawnBullet()
{
}

HRESULT CPawnBullet::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(vPos);

	return S_OK;
}

_int CPawnBullet::Update_Object(const _float & fTimeDelta)
{

	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();

	if (vPos.x <= -11.f || vPos.z <= -11.f || vPos.x > 10.f || vPos.z > 10.f)
	{
		m_bDead = true;
	}

	m_fLifeTime += fTimeDelta;
	m_fEffecTime += fTimeDelta;
	if (m_fLifeTime > 2.f) // 현재는 라이프타임  2초로 설정해둠. 나중에 플레이어나 벽충돌하면 없어지게해야할듯
	{
		m_bDead = true;
	}


	if (m_bDead)
		return OBJ_DEAD;

	Engine::CGameObject::Update_Object(fTimeDelta);

	//Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;

}

_int CPawnBullet::LateUpdate_Object(const _float & fTimeDelta)
{
	Effect();
	return _int();
}

void CPawnBullet::Render_Object(const _int & iIndex)
{

}

HRESULT CPawnBullet::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// CStaticMesh
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
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_fColDis, _vec4(50.f, 244.f, 234.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CPawnBullet::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CPawnBullet::Move(const _float & fTimeDelta)
{
}

void CPawnBullet::Effect()
{
	if (m_fEffecTime > 0.05f)
	{
		PARTICLE tParticle3;
		Engine::Reset_Particle(tParticle3);
		Engine::Set_AnimState(tParticle3, 8, 8);
		tParticle3.fAcc = 1.f;
		tParticle3.fCurrentTime = 0.f;
		_float fSize = CRandoms()(0.8f, 1.2f);
		tParticle3.vStartSize = _vec3(fSize, fSize, fSize);
		tParticle3.vEndSize = _vec3(0.1f, 0.1f, 0.1f);
		tParticle3.fRotateSpeed = 0.f;
		tParticle3.fVelocity = 1.f;
		tParticle3.fLifeTime = 0.8f;
		tParticle3.vDir = { 0.f, 0.f, 0.f };
		tParticle3.vStartColor = { 30.f, 30.f, 30.f, 255.f * 3.5f };
		tParticle3.vEndColor = { 30.f, 30.f, 30.f, 255.f* 3.5f };
		tParticle3.vRot = { 0.f, 0.f, 0.f };

		tParticle3.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-10, 10) * 0.1f, 0.3f, CRandoms()(-10, 10) * 0.1f);
		Engine::Emit_ImageParticle(tParticle3, 5, 19);

		tParticle3.vStartSize = _vec3(fSize - 0.2f, fSize - 0.2f, fSize - 0.2f);
		tParticle3.vEndSize = _vec3(0.2f, 0.2f, 0.2f);
		Engine::Emit_ImageParticle(tParticle3, 5, 4);
		m_fEffecTime = 0.f;
	}

	if (!m_bEffect)
	{
		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 8, 8, 0.7f);
		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.vStartSize = _vec3(3.5f, 3.5f, 3.5f);
		tParticle.vEndSize = _vec3(3.5f, 3.5f, 3.5f);
		tParticle.fRotateSpeed = 0.f;
		tParticle.fVelocity = 1.f;
		tParticle.fLifeTime = 4.f;
		tParticle.vDir = { 0.f, 0.f, 0.f };
		tParticle.vStartColor = { 0.f, 0.f, 0.f, 255.f * 4.5f };
		tParticle.vEndColor = { 0.f, 0.f, 0.f, 255.f };
		tParticle.bBill = false;
		tParticle.vRot = { D3DXToRadian(90.f), 0.f, 0.f };

		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 0.1f, 0.f);
		Engine::Emit_ImageParticle(tParticle, 6, 4);

		m_bEffect = true;
	}
}

CPawnBullet * CPawnBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CPawnBullet*	pInstance = new CPawnBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPawnBullet::Free(void)
{
	Engine::CGameObject::Free();
}
