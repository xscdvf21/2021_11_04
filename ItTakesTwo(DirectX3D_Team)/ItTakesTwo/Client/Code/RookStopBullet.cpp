#include "stdafx.h"
#include "RookStopBullet.h"
#include "ChessTile.h"

#include "Export_Function.h"
CRookStopBullet::CRookStopBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CRookStopBullet::~CRookStopBullet()
{
}

HRESULT CRookStopBullet::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vPos = vPos;

	CHESSTILE	tTemp;
	ZeroMemory(&tTemp, sizeof(CHESSTILE));
	tTemp = CChessTile::GetInstance()->Get_Index(m_vPos);

	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(m_vPos);

	return S_OK;


}

_int CRookStopBullet::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);


	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();

	m_fLifeTime += fTimeDelta;
	m_fEffecTime += fTimeDelta;

	if (vPos.x <= -10.f || vPos.z <= -10.f || vPos.x > 10.f || vPos.z > 10.f)
	{
		m_bDead = true;
	}

	m_fLifeTime += fTimeDelta;
	if (m_fLifeTime > 5.f)
	{
		m_bDead = true;
	}


	if (m_bDead)
		return OBJ_DEAD;


	//Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CRookStopBullet::LateUpdate_Object(const _float & fTimeDelta)
{
	Effect();
	return _int();
}

void CRookStopBullet::Render_Object(const _int & iIndex)
{
	//LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	//NULL_CHECK(pEffect);
	//pEffect->AddRef();

	//FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	//_uint	iPassMax = 0;

	//pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	//									//pEffect->BeginPass(0);
	//m_pMeshCom->Render_Meshes(pEffect, 8);

	//pEffect->End();

	//Safe_Release(pEffect);
}

HRESULT CRookStopBullet::Add_Component()
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

HRESULT CRookStopBullet::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CRookStopBullet::Move(const _float & fTimeDelta)
{
}

void CRookStopBullet::Effect()
{
	if (m_fEffecTime > 0.05f)
	{
		PARTICLE tParticle3;
		Engine::Reset_Particle(tParticle3);
		Engine::Set_AnimState(tParticle3, 8, 8);
		tParticle3.fAcc = 1.f;
		tParticle3.fCurrentTime = 0.f;
		_float fSize = CRandoms()(10, 70) * 0.01f + 0.4f;
		tParticle3.vStartSize = _vec3(fSize, fSize, fSize);
		tParticle3.vEndSize = _vec3(0.3f, 0.3f, 0.3f);
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

CRookStopBullet * CRookStopBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CRookStopBullet*	pInstance = new CRookStopBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRookStopBullet::Free(void)
{
	Engine::CGameObject::Free();
}
