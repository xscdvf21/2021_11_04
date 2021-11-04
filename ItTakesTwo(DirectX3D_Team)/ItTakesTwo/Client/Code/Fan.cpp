#include "stdafx.h"
#include "Fan.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "PhysXMesh.h"
#include "Blow.h"


CFan::CFan(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CFan::~CFan(void)
{
}

HRESULT CFan::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;

	return S_OK;
}

Engine::_int CFan::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	if (m_bStart)
	{
		CTrigger::CreateBoxTrigger(m_pScene, this, m_tObjInfo.vPos, 1.2f, 2.0f, 1.2f, TRIGGERTYPE::TRIGGER_WING);
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_tObjInfo.vPos, m_tObjInfo.vScale.x, m_tObjInfo.vScale.y, m_tObjInfo.vScale.z, TRIGGERTYPE::TRIGGER_FLOAT);
		m_bStart = false;
	}
	// BlowTransform
	CTransform* pBlowTransform = static_cast<CTransform*>(m_pBlow->Get_Component(L"Com_Transform", ID_DYNAMIC));
	if (m_pTransformCom->Get_Angle().y >= 3.14f * 2.0f || m_pTransformCom->Get_Angle().y <= -3.14f * 2.0f)
	{
		m_pTransformCom->Set_AngleY(0.0f);
		//Blow
		pBlowTransform->Set_AngleY(0.0f);
	}

	_float fDir = -1.0f;
	if (m_bUp)
		fDir = -1.0f;
	else
		fDir = 1.0f;

	m_pTransformCom->Set_AddAngleY(fTimeDelta * 3.0f * fDir);
	//Blow
	//pBlowTransform->Set_AddAngleY(fTimeDelta * 3.0f * fDir);

	if (!m_bUp)
		m_pBlow->Update_Object(fTimeDelta);


	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CFan::LateUpdate_Object(const _float & fTimeDelta)
{
	if (!m_bUp)
	{
		m_pBlow->LateUpdate_Object(fTimeDelta);
		PARTICLE tParticle;
		_float fRand = 0.f;
		if (CRandoms()(0, 1) == 1)
			fRand = 0.f;
		else
			fRand = 0.5f;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 2, 2, fRand);

		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.fLifeTime = 0.3f;
		tParticle.fRotateSpeed = 0.f;
		tParticle.fVelocity = 20.f;

		if (CRandoms()(0, 1) == 1)
			tParticle.fAmount = 0.f;
		else
			tParticle.fAmount = 0.5f;

		_vec3 vLook = {0.f, 1.f, 0.f};
		
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-2.f, 2.f), CRandoms()(-2.f, 2.f), CRandoms()(-2.f, 2.f)) + (vLook * (10.f + CRandoms()(0.f, 2.f)));
		_vec3 vDir = m_pTransformCom->Get_Position() - tParticle.vPos;
		D3DXVec3Normalize(&vDir, &vDir);
		tParticle.vDir = vDir;
		tParticle.vStartSize = _vec3(0.5f, 0.5f, 0.5f);
		tParticle.vEndSize = _vec3(0.2f, 0.2f, 0.2f);
		tParticle.vStartColor = { 255.f * 3.5f, 255.f * 3.5f, 255.f * 3.5f, 250.f };
		tParticle.vEndColor = { 255.f * 3.5f, 255.f * 3.5f, 255.f * 3.5f, 150.f };

		tParticle.bBill = false;
		tParticle.vRot = { CRandoms()(0.f, 360.f), CRandoms()(0.f, 360.f), CRandoms()(0.f, 360.f) };

		Engine::Emit_ImageParticle(tParticle, 43, 23);
	}
	else
	{
		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 8, 8, 2.f);

		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.fLifeTime = 0.8f;
		tParticle.fRotateSpeed = 0.f;
		tParticle.fVelocity = 15.f;

		tParticle.vDir = { 0.f, 1.f, 0.f };
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-1.f, 1.f), 0.f, CRandoms()(-1.f, 1.f));
		tParticle.vStartSize = _vec3(1.5f, 1.5f, 1.5f);
		tParticle.vEndSize = _vec3(4.f, 4.f, 4.f);
		tParticle.vStartColor = { 255.f * 3.5f, 255.f * 3.5f, 255.f * 3.5f, 255.f };
		tParticle.vEndColor = { 255.f * 2.5f, 255.f * 2.5f, 255.f * 2.5f, 0.f };

		Engine::Emit_ImageParticle(tParticle, 1, 4);
	}

	return _int();
}

void CFan::Render_Object(const _int& iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 8);


	pEffect->CommitChanges();
	pEffect->SetFloat("g_fMaterialIndex", 0.f);

	_matrix matWorld;
	m_pBodyTransformCom->Get_WorldMatrix(&matWorld);
	pEffect->SetMatrix("g_matWorld", &matWorld);
	m_pBodyMeshCom->Render_Meshes(pEffect, 8);

	//m_pHolderMeshCom->Render_Meshes(pEffect);

	pEffect->End();

	Safe_Release(pEffect);
}

void* CFan::On_Active(void* pCaller)
{
	m_bUp = !m_bUp;
	return nullptr;
}

CFan* CFan::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CFan*	pInstance = new CFan(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CFan::Free(void)
{
	Safe_Release(m_pBlow);
	Engine::CGameObject::Free();
}

HRESULT CFan::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	m_bUp = tTemp.eMeshType ? true : false;

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"FanHead"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_HeadMesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(tTemp.vPos);
	m_pTransformCom->Set_AddAngleX(D3DXToRadian(90.0f));

	// BodyCStaticMesh
	pComponent = m_pBodyMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"FanBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_BodyMesh", pComponent);

	//// HolderCStaticMesh
	//pComponent = m_pHolderMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"FanHolder"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_HolderMesh", pComponent);

	// Transform
	pComponent = m_pBodyTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_BodyTransform", pComponent);
	m_pBodyTransformCom->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	m_pBodyTransformCom->Set_Pos(tTemp.vPos);
	m_pBodyTransformCom->Set_AddAngleX(D3DXToRadian(-270.0f));

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);


	m_pBlow = CBlow::Create(m_pGraphicDev, pArg);
	CTransform* pTransform = static_cast<CTransform*>(m_pBlow->Get_Component(L"Com_Transform", ID_DYNAMIC));
	_float3 vBlowPos = tTemp.vPos;
	vBlowPos.y += 2.0f;
	pTransform->Set_Pos(vBlowPos);
	pTransform->Set_Scale(0.01f, 0.01f, 0.01f);

	return S_OK;
}

HRESULT CFan::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	pEffect->CommitChanges();
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));
	pEffect->SetFloat("g_fAmount", 0.f);
	pEffect->SetFloat("g_fMaterialIndex", 1.f);
	pEffect->CommitChanges();

	return S_OK;
}
