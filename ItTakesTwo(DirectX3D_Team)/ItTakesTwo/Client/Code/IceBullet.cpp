#include "stdafx.h"
#include "IceBullet.h"
#include "CollisionMgr.h"
#include "MainCamera.h"

#include "Export_Function.h"


CIceBullet::CIceBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnermy(pGraphicDev)
	, m_fScale(0.001f)
{
}

CIceBullet::~CIceBullet()
{
}

HRESULT CIceBullet::Ready_Object(_vec3 vPos, _vec3 vDir)
{
	m_fColDis = 0.8f;

	// 콜라이더
	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = m_fColDis;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(m_fScale, m_fScale, m_fScale));
	m_pTransformCom->Set_Pos(vPos);

	m_vDir = vDir;
	D3DXVec3Normalize(&m_vDir, &m_vDir); //혹시 몰라서 한번더해줌, 애초에 인자로 던질때 노말라이즈 해주긴할것.

	m_tShader.m_fRimDist = 10.f;
	m_tShader.m_iRimPower = 50;
	m_tShader.m_vRimColor = _vec4(0.15f, 0.15f, 0.15f, 1.f);

	return S_OK;
}

_int CIceBullet::Update_Object(const _float & fTimeDelta)
{
	m_pTransformCom->Set_Scale(_vec3(m_fScale, m_fScale, m_fScale));

	
	m_fScale += 0.0005f;
	if (m_fLifeTime > 1.5f) // 현재는 라이프타임  2초로 설정해둠. 나중에 플레이어나 벽충돌하면 없어지게해야할듯
	{
		m_bDead = true;
	}
	if (m_bDead)
	{
		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 8, 8, 1.f);
		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.vStartSize = _vec3(5.f, 5.f, 5.f);
		tParticle.vEndSize = _vec3(5.f, 5.f, 5.f);
		tParticle.fRotateSpeed = 0.f;
		tParticle.vStartColor = { 200.f * 1.5f, 125.f * 1.5f, 255.f * 2.f, 255.f };
		tParticle.vEndColor = { 200.f * 1.5f, 125.f * 1.5f, 255.f * 2.f, 0.f };
		tParticle.fVelocity = 0.f;
		tParticle.fLifeTime = 1.f;

		tParticle.vRot = { 0.f, 0.f, 0.f };
		tParticle.vDir = { 0.f, 0.f, 0.f };

		tParticle.vPos = m_pTransformCom->Get_Position();
		Engine::Emit_ImageParticle(tParticle, 23, 4);


		Engine::Set_AnimState(tParticle, 8, 8, 1.f);
		tParticle.fLifeTime = 1.f;
		tParticle.vStartSize = _vec3(4.f, 4.f, 4.f);
		tParticle.vEndSize = _vec3(4.f, 4.f, 4.f);
		tParticle.vStartColor = { 200.f * 5.5f, 125.f * 5.5f, 255.f * 7.5f, 255.f * 3.5f };
		tParticle.vPos = m_pTransformCom->Get_Position();
		Engine::Emit_ImageParticle(tParticle, 29, 4);

		tParticle.vStartSize = _vec3(1.5f, 1.5f, 1.5f);
		tParticle.vEndSize = _vec3(1.5f, 1.5f, 1.5f);
		Engine::Emit_ImageParticle(tParticle, 30, 4);

		tParticle.vStartSize = _vec3(4.f, 4.f, 4.f);
		tParticle.vEndSize = _vec3(4.f, 4.f, 4.f);
		Engine::Emit_ImageParticle(tParticle, 29, 4);


		PARTICLE tParticle2;
		Engine::Reset_Particle(tParticle2);
		tParticle2.fAcc = 1.f;
		tParticle2.fCurrentTime = 0.f;
		tParticle2.fRotateSpeed = 0.f;
		tParticle2.vEndSize = _vec3(0.001f, 0.001f, 0.001f);

		tParticle2.vStartColor = { 200.f * 2, 125.f * 2, 255.f * 2.3f, 255.f * 5 };
		tParticle2.vEndColor = { 200.f * 2, 125.f * 2, 255.f * 2.3f, 255.f * 5 };
		tParticle2.bBill = false;
		tParticle2.bGravity = true;
		tParticle2.fGravityForce = 1.5f;
		tParticle2.vPos = m_pTransformCom->Get_Position();

		PARTICLE tParticle3;
		Engine::Reset_Particle(tParticle3);

		tParticle3.fAcc = 1.f;
		tParticle3.fCurrentTime = 0.f;
		tParticle3.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
		tParticle3.fRotateSpeed = 0.f;

		tParticle3.vStartColor = { 200.f * 2, 125.f * 2, 255.f * 2, 255.f * 2.5f };
		tParticle3.vEndColor = { 255.f, 255.f, 255.f, 50.f };
		tParticle3.vRot = { 0.f, 0.f, 0.f };

		for (_int i = 0; i < 50; ++i)
		{
			_float fRand = CRandoms()(50, 100) * 0.0015f;
			tParticle3.vStartSize = _vec3(fRand, fRand, fRand);
			tParticle3.fVelocity = CRandoms()(0, 10) * 0.1f + 2.5f;
			tParticle3.vDir = { CRandoms()(-1.f, 1.f), CRandoms()(-1.f, 1.f), CRandoms()(-1.f, 1.f)};
			tParticle3.fLifeTime = CRandoms()(1, 5) * 0.1f + 0.8f;
			tParticle3.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-30, 30) * 0.01f, CRandoms()(-30, 30) * 0.01f, CRandoms()(-30, 30) * 0.01f);
			Engine::Emit_ImageParticle(tParticle3, 15, 11);
		}


		PARTICLE tParticle4;
		Engine::Reset_Particle(tParticle4);
		Engine::Set_AnimState(tParticle4, 2, 2, 5.f);
		tParticle4.fAcc = 1.f;
		tParticle4.fCurrentTime = 0.f;
		tParticle4.fRotateSpeed = 0.f;
		tParticle4.vEndSize = _vec3(0.1f, 0.1f, 0.1f);

		tParticle4.vStartColor = { 200.f * 2, 125.f * 2, 255.f * 3.f, 255.f * 5 };
		tParticle4.vEndColor = { 200.f * 2, 125.f * 2, 255.f * 3.f, 255.f * 5 };
		tParticle4.bBill = false;
		tParticle4.vPos = m_pTransformCom->Get_Position();

		for (_int i = 0; i < 30; ++i)
		{
			tParticle4.fLifeTime = CRandoms()(0.3f, 0.5f);
			_float fRand = CRandoms()(1.f, 4.f);
			tParticle4.vStartSize = _vec3(CRandoms()(1.f, 2.f), fRand, 2.f);
			tParticle4.fVelocity = CRandoms()(6.f, 10.f);

			//tParticle4.vDir = { 1.f, 0.f, 0.f };
			tParticle4.vDir = { CRandoms()(-100, 100) * 0.01f, 0.f, CRandoms()(-100, 100) * 0.01f };
			_float fRadian = atan2f(tParticle4.vDir.x, tParticle4.vDir.z);
			tParticle4.vRot = { D3DXToRadian(90.f), fRadian, 0.f };

			Engine::Emit_ImageParticle(tParticle4, 15, 21);
		}


		return OBJ_DEAD;
	}

	
	if (m_fScale > 0.01f)
	{
		m_fLifeTime += fTimeDelta;
		m_pTransformCom->Set_AddPos(m_vDir * m_fLifeTime * 0.3f);
		m_fScale = 0.01f;
		CCollisionMgr::GetInstance()->Add_EnermyBulletList(this);
	}
	m_pTransformCom->Set_Scale(_vec3(m_fScale, m_fScale, m_fScale));

	Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	
	return OBJ_NOEVENT;
}

_int CIceBullet::LateUpdate_Object(const _float & fTimeDelta)
{
	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);

	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
	tParticle.fRotateSpeed = 0.f;

	tParticle.vStartColor = { 200.f * 2.f, 125.f* 2.f, 250.f* 2.f, 255.f };
	tParticle.vEndColor = { 200.f, 125.f, 250.f, 85.f };
	tParticle.vRot = { 0.f, 0.f, 0.f };
	tParticle.bGravity = true;
	tParticle.fGravityForce = 1.5f;
	_float fRand;
	for (_int i = 0; i < 3; ++i)
	{
		tParticle.fVelocity = CRandoms()(2.5f, 3.5f);
		tParticle.fLifeTime = CRandoms()(0.5f, 1.f);
		fRand = CRandoms()(0.2f, 0.4f);
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
		tParticle.vStartColor = { 200.f * 2.f, 125.f* 2.f, 250.f* 2.f, 255.f };

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
	tParticle2.vStartColor = { 200.f * 3.5f, 125.f * 3.5f, 250.f * 3.5f, 255.f };
	tParticle2.vEndColor = { 200.f * 2.5f, 125.f * 2.5f, 250.f * 2.5f, 255.f };
	tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.1f, 0.1f), 0.f, CRandoms()(-0.1f, 0.1f));
	Engine::Emit_ImageParticle(tParticle2, 30, 26);

	return _int();
}

void CIceBullet::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 27);

	pEffect->End();

	Safe_Release(pEffect);

	CMainCamera* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
	pCamera->Update_View_Proj();

	//Spine2
	m_tColInfo.matWorld = *m_pTransformCom->Get_WorldMatrix();
	m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false);
}

HRESULT CIceBullet::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_CF, L"ToyMage_Bullet"));
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

	// Texture
	pComponent = m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Resource(0, L"Particle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CIceBullet::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

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

	_matrix matViewInv;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matViewInv);
	D3DXMatrixInverse(&matViewInv, nullptr, &matViewInv);
	pEffect->SetVector("g_vCamPos", &_vec4(matViewInv._41, matViewInv._42, matViewInv._43, 1.f));

	return S_OK;
}

void CIceBullet::Move(const _float & fTimeDelta)
{
}

CIceBullet * CIceBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vDir)
{
	CIceBullet*	pInstance = new CIceBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos, vDir)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceBullet::Free(void)
{
	CEnermy::Free();
}
