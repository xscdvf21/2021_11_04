
#include "stdafx.h"
#include "BaboonBombUpBullet.h"

#include "Export_Function.h"


CBaboonBombUpBullet::CBaboonBombUpBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEnermy(pGraphicDev)
{
}

CBaboonBombUpBullet::~CBaboonBombUpBullet()
{
}

HRESULT CBaboonBombUpBullet::Ready_Object(_vec3 vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.005f, 0.005f, 0.005f));
	m_pTransformCom->Set_Pos(vPos);

	m_tShader.m_fRimDist = 10.f;
	m_tShader.m_iRimPower = 50;
	m_tShader.m_vRimColor = _vec4(0.15f, 0.15f, 0.15f, 1.f);

	return S_OK;
}

_int CBaboonBombUpBullet::Update_Object(const _float & fTimeDelta)
{
	m_pTransformCom->Move_Pos(&_vec3(0.f, 1.f, 0.f), 20.f, fTimeDelta);

	m_fLifeTime += fTimeDelta;

	if (m_fLifeTime > 3.f)
		m_bDead = true;

	if (m_bDead)
		return OBJ_DEAD;


	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CBaboonBombUpBullet::LateUpdate_Object(const _float & fTimeDelta)
{
	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);

	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
	tParticle.fRotateSpeed = 0.f;

	tParticle.vStartColor = { 255.f * 3.5f, 54.f* 3.5f, 0.f* 3.5f, 255.f };
	tParticle.vEndColor = { 255.f, 255.f, 255.f, 255.f };
	tParticle.vRot = { 0.f, 0.f, 0.f };
	tParticle.bGravity = true;
	tParticle.fGravityForce = 1.5f;
	_float fRand;
	for (_int i = 0; i < 5; ++i)
	{
		tParticle.fVelocity = CRandoms()(2.5f, 3.5f);
		tParticle.fLifeTime = CRandoms()(0.5f, 1.f);
		fRand = CRandoms()(0.1f, 0.2f);
		tParticle.vStartSize = _vec3(fRand, fRand, fRand);

		tParticle.vDir = { CRandoms()(-1.f, 1.f),  CRandoms()(0.f, 1.f), CRandoms()(-1.f, 1.f) };
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f));
		Engine::Emit_ImageParticle(tParticle, 15, 11);
	}

	for (_int i = 0; i < 3; ++i)
	{
		tParticle.fVelocity = CRandoms()(2.5f, 3.5f);
		tParticle.fLifeTime = CRandoms()(0.3f, 0.5f);
		fRand = CRandoms()(0.2f, 0.4f);
		tParticle.vStartSize = _vec3(fRand, fRand, fRand);
		tParticle.vStartColor = { 255.f * 3.5f, 255.f* 3.5f, 255.f* 3.5f, 255.f };

		tParticle.vDir = { CRandoms()(-1.f, 1.f),  CRandoms()(0.f, 1.f), CRandoms()(-1.f, 1.f) };
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f));
		Engine::Emit_ImageParticle(tParticle, 15, 11);
	}

	PARTICLE tParticle2;
	Engine::Reset_Particle(tParticle2);
	Engine::Set_AnimState(tParticle2, 8, 8, 1.f);
	tParticle2.fAcc = 1.f;
	tParticle2.fCurrentTime = 0.f;
	tParticle2.vStartSize = _vec3(2.3f, 2.3f, 2.3f);
	tParticle2.vEndSize = _vec3(0.5f, 0.5f, 0.5f);
	tParticle2.fRotateSpeed = 0.f;
	tParticle2.vStartColor = { 30.f, 30.f, 30.f, 255.f * 1.5f };
	tParticle2.vEndColor = { 30.f, 30.f, 30.f, 0.f };
	tParticle2.fVelocity = 0.f;
	tParticle2.fLifeTime = 0.5f;
	tParticle2.vRot = { 0.f, 0.f, 0.f };

	tParticle2.vDir = { 0.f, 0.f, 0.f };
	tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.1f, 0.1f), 0.f, CRandoms()(-0.1f, 0.1f));
	Engine::Emit_ImageParticle(tParticle2, 23, 4);


	tParticle2.vStartSize = _vec3(1.8f, 1.8f, 1.8f);
	tParticle2.fLifeTime = 0.35f;
	tParticle2.vStartColor = { 255.f * 3.5f, 54.f * 3.5f, 0.f * 3.5f, 255.f };
	tParticle2.vEndColor = { 255.f * 2.5f, 54.f * 2.5f, 0.f * 2.5f, 255.f };
	tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.1f, 0.1f), 0.f, CRandoms()(-0.1f, 0.1f));
	Engine::Emit_ImageParticle(tParticle2, 30, 26);

	return _int();
}

void CBaboonBombUpBullet::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 28);

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CBaboonBombUpBullet::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_SB, L"IceOrb"));
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
	pComponent = m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Resource(0, L"Particle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	return S_OK;
}

HRESULT CBaboonBombUpBullet::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj, matInv;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_Add1Texture", 15);

	pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));
	pEffect->SetFloat("g_fAmount", 0.f);
	pEffect->SetFloat("g_fMaterialIndex", 0.f);

	pEffect->SetFloat("g_fRimDist", m_tShader.m_fRimDist);
	pEffect->SetInt("g_iRimPower", m_tShader.m_iRimPower);
	pEffect->SetVector("g_vRimColor", &m_tShader.m_vRimColor);
	pEffect->SetBool("g_bRim", m_bRimLight);

	D3DXMatrixInverse(&matInv, nullptr, &matView);
	pEffect->SetVector("g_vCamPos", &_vec4(matInv._41, matInv._42, matInv._43, 1.f));

	return S_OK;
}

void CBaboonBombUpBullet::Move(const _float & fTimeDelta)
{
}

CBaboonBombUpBullet * CBaboonBombUpBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CBaboonBombUpBullet*	pInstance = new CBaboonBombUpBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaboonBombUpBullet::Free(void)
{
	CEnermy::Free();
}
