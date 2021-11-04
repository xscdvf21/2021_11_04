#include "stdafx.h"
#include "VacuumStone.h"
#include "CollisionMgr.h"
#include "Export_Function.h"
#include "CodyCamera.h"
#include "MayCamera.h"

CVacuumStone::CVacuumStone(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnermy(pGraphicDev)
	, m_bTram(false), m_bTramStart(true), m_fTramPower(0.f)
	, m_fTramTime(0.f), m_fTramHeight(0.f), m_fDuration(2.f)
	, m_bAnglecheck(false)
{
}

CVacuumStone::~CVacuumStone()
{
}

//생성 할 뼈이름 Neck 
HRESULT CVacuumStone::Ready_Object(void* pArg, _vec3 vTargetPos)
{
	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = 1.f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformCom->Set_Pos(tTemp.vPos);

	CRandoms CRandom;
	m_vRot.x = CRandom(-1.f, 1.f);
	m_vRot.y = CRandom(-1.f, 1.f);
	m_vRot.z = CRandom(-1.f, 1.f);

	m_vTargetPos = vTargetPos;

	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);

	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vStartSize = _vec3(3.f, 3.f, 3.f);
	tParticle.vEndSize = _vec3(7.f, 7.f, 7.f);
	tParticle.fRotateSpeed = 0.f;
	tParticle.vStartColor = { 30.f, 30.f, 30.f, 120.f };
	tParticle.vEndColor = { 30.f, 30.f, 30.f, 255.f };
	tParticle.fVelocity = 0.f;
	tParticle.fLifeTime = 2.f;
	tParticle.bBill = false;
	tParticle.vRot = { D3DXToRadian(90.f), 0.f, 0.f };

	tParticle.vDir = { 0.f, 0.f, 0.f };
	tParticle.vPos = m_vTargetPos + _vec3(0.f, 0.1f, 0.f);
	Engine::Emit_ImageParticle(tParticle, 15, 11);

	return S_OK;
}

_int CVacuumStone::Update_Object(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		m_pTrigger = CTrigger::CreateSphereTirgger(m_pScene, this, m_pTransformCom->Get_Position(), 3.f, TRIGGER_VACUUMBOSS_STONE, FilterGroup::eDefaultTrigger);
		m_bStart = false;
	}

	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();
	m_pTrigger->Set_Pos(m_pTransformCom->Get_Position());

	if (m_bDead)
	{
		CCodyCamera* pCodycam = (CCodyCamera*)Engine::Get_GameObject(L"GameObject", L"CodyCamera");
		CMayCamera* pMaycam = (CMayCamera*)Engine::Get_GameObject(L"GameObject", L"MayCamera");
		pCodycam->Set_Shake();
		pMaycam->Set_Shake();

		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);

		_float fRand = 0.f;
		if (CRandoms()(0, 1) == 1)
			fRand = 0.f;
		else
			fRand = 0.5f;
		Engine::Set_AnimState(tParticle, 2, 2, fRand);

		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.fRotateSpeed = 0.f;

		tParticle.fLifeTime = 3.f;
		tParticle.bGravity = true;
		tParticle.fGravityForce = 2.f;

		tParticle.vPos = m_vTargetPos + _vec3(0.f, 1.3f, 0.f);

		for (_int i = 0; i < 15; ++i)
		{
			if (CRandoms()(0, 1) == 1)
				tParticle.fAmount = 0.f;
			else
				tParticle.fAmount = 0.5f;

			tParticle.vRot = { 0.f, D3DXToRadian(CRandoms()(0, 360)), 0.f };
			_float fSize = CRandoms()(1, 10) * 0.1f + 1.5f;
			tParticle.vStartColor = { 255.f * fSize, 255.f * fSize , 255.f * fSize, 255.f };
			tParticle.vEndColor = { 255.f * fSize, 255.f * fSize, 255.f * fSize, 255.f };
			_float fStart = CRandoms()(1, 100) * 0.01f + 0.5f;
			tParticle.vStartSize = _vec3(fStart, fStart, fStart);
			tParticle.vEndSize = _vec3(fStart, fStart, fStart);
			tParticle.fVelocity = _float(CRandoms()(1, 10) + 5.f);
			tParticle.vDir = { CRandoms()(-100, 100) * 0.1f, CRandoms()(100, 3000) * 0.01f, CRandoms()(-100, 100) * 0.1f };
			Engine::Emit_ImageParticle(tParticle, 27, 17);
		}

		for (_int i = 0; i < 30; ++i)
		{
			if (CRandoms()(0, 1) == 1)
				tParticle.fAmount = 0.f;
			else
				tParticle.fAmount = 0.5f;

			tParticle.vRot = { 0.f, D3DXToRadian(CRandoms()(0, 360)), 0.f };
			_float fSize = CRandoms()(1, 10) * 0.1f + 1.5f;
			tParticle.vStartColor = { 255.f * fSize, 255.f * fSize , 255.f * fSize, 255.f };
			tParticle.vEndColor = { 255.f * fSize, 255.f * fSize, 255.f * fSize, 255.f };
			_float fStart = CRandoms()(1, 20) * 0.01f;
			tParticle.vStartSize = _vec3(fStart, fStart, fStart);
			tParticle.vEndSize = _vec3(fStart, fStart, fStart);
			tParticle.fVelocity = _float(CRandoms()(1, 10) + 5.f);
			tParticle.vDir = { CRandoms()(-100, 100) * 0.1f, CRandoms()(100, 500) * 0.1f, CRandoms()(-100, 100) * 0.1f };
			Engine::Emit_ImageParticle(tParticle, 27, 17);
		}


		PARTICLE tParticle2;
		Engine::Reset_Particle(tParticle2);
		Engine::Set_AnimState(tParticle2, 8, 8, 1.f);

		tParticle2.fAcc = 1.f;
		tParticle2.fCurrentTime = 0.f;
		tParticle2.vStartSize = _vec3(10.f, 10.f, 10.f);
		tParticle2.vEndSize = _vec3(10.f, 10.f, 10.f);
		tParticle2.fRotateSpeed = 0.f;
		tParticle2.vStartColor = { 60.f , 60.f, 60.f, 255.f };
		tParticle2.vEndColor = { 60.f, 60.f, 60.f , 100.f };
		tParticle2.vRot = { 0.f, 0.f, 0.f };
		tParticle2.fVelocity = 0.f;
		tParticle2.fLifeTime = 1.f;
		tParticle2.vRot = { 0.f, 0.f, 0.f };

		tParticle2.vDir = { 0.f, 0.f, 0.f };
		for (_int i = 0; i < 7; ++i)
		{
			tParticle2.vPos = m_vTargetPos + _vec3(CRandoms()(-20, 20) * 0.1f, 1.3f + CRandoms()(0, 15) * 0.1f, CRandoms()(-20, 20) * 0.1f);
			Engine::Emit_ImageParticle(tParticle2, 23, 4);
		}

		CRandoms Random;
		_uint	iRandom = Random(1, 3);
		if (iRandom == 1)
		{
			Engine::StopSound(CSoundMgr::CHANNELID::Vacuum_StoneDead);
			Engine::PlaySoundW(L"Vacuum_StoneDead0.wav", CSoundMgr::CHANNELID::Vacuum_StoneDead, 0.3f);
		}
		else if (iRandom == 2)
		{
			Engine::StopSound(CSoundMgr::CHANNELID::Vacuum_StoneDead);
			Engine::PlaySoundW(L"Vacuum_StoneDead1.wav", CSoundMgr::CHANNELID::Vacuum_StoneDead, 0.3f);
		}
		else if (iRandom == 3)
		{
			Engine::StopSound(CSoundMgr::CHANNELID::Vacuum_StoneDead);
			Engine::PlaySoundW(L"Vacuum_StoneDead2.wav", CSoundMgr::CHANNELID::Vacuum_StoneDead, 0.3f);
		}

		m_pTrigger->Set_Interactable(false);
		

		return OBJ_DEAD;
	}
	Engine::CGameObject::Update_Object(fTimeDelta);

	Move(fTimeDelta, m_vTargetPos);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;

}

_int CVacuumStone::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CVacuumStone::Render_Object(const _int & iIndex)
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


	m_tColInfo.matWorld = *m_pTransformCom->Get_WorldMatrix();
	m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false);


}

HRESULT CVacuumStone::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;



	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_VB, L"VacuumDebris01"));
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
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_tColInfo.fRadius, _vec4(50.f, 244.f, 234.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CVacuumStone::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CVacuumStone::Move(const _float & fTimeDelta, const _vec3 & TargetPos)
{
	_float fStartPower = 0.f;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(m_vRot.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(m_vRot.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(m_vRot.z));

	if (m_bTramStart)
	{
		m_vSetPos = TargetPos;
		m_vTramDir = TargetPos - m_pTransformCom->Get_Position();
		D3DXVec3Normalize(&m_vTramDir, &m_vTramDir);

		m_fTramPower = ((m_vTramDir.y + 1.f) - m_pTransformCom->Get_Position().y - (m_fDuration * m_fDuration) * -GRAVITY) / m_fDuration;

		m_vTramDis = TargetPos - m_pTransformCom->Get_Position();
		m_vTramDis.y = 0.f;
		m_fTramHeight = m_pTransformCom->Get_Position().y;
		m_bTramStart = false;
	}

	m_fTramTime += fTimeDelta;

	_float fHeight = m_fTramHeight + ((m_fTramTime * m_fTramTime) * (-GRAVITY) / 1) + (m_fTramTime * m_fTramPower);
	_vec3 vDstPos = m_pTransformCom->Get_Position() + (m_vTramDir * D3DXVec3Length(&m_vTramDis) * fTimeDelta / m_fDuration);

	m_pTransformCom->Set_Pos(_vec3(vDstPos.x, fHeight, vDstPos.z));
	if (m_pTransformCom->Get_Position().y <= 0.f)
	{
		m_bDead = true;
	}
}



CVacuumStone * CVacuumStone::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg, _vec3 vTargetPos)
{
	CVacuumStone*	pInstance = new CVacuumStone(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg, vTargetPos)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVacuumStone::Free(void)
{
	CEnermy::Free();
}
