#include "stdafx.h"
#include "Enermy.h"
#include "MainCamera.h"


CEnermy::CEnermy(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

void CEnermy::Add_CollisionList(CGameObject * pGameObject)
{
	m_vecCollisionList.emplace_back(pGameObject);
}

_bool CEnermy::Check_CollisionList(CGameObject * pGameObject)
{
	for (auto& iter : m_vecCollisionList)
	{
		if (iter == pGameObject)
			return true;
	}
	return false;
}

void CEnermy::Clear_CollisionList()
{
	m_vecCollisionList.clear();
}

void CEnermy::Dead_Effect()
{
	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);
	Engine::Set_AnimState(tParticle, 8, 8, 1.f);
	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vStartSize = _vec3(2.5f, 2.5f, 2.5f);
	tParticle.vEndSize = _vec3(2.5f, 2.5f, 2.5f);
	tParticle.fRotateSpeed = 0.f;
	tParticle.fVelocity = 1.f;
	tParticle.fLifeTime = 2.5f;
	tParticle.vDir = { 0.f, 0.f, 0.f };
	tParticle.vStartColor = { 0.f, 0.f, 0.f, 255.f * 1.5f };
	tParticle.vEndColor = { 0.f, 0.f, 0.f, 100.f };
	tParticle.bBill = false;
	tParticle.vRot = { D3DXToRadian(90.f), 0.f, 0.f };

	tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 0.1f, 0.f);
	Engine::Emit_ImageParticle(tParticle, 6, 4);

	//////////////////////////////////////////////////////////////////////////
	PARTICLE tParticle2;
	Engine::Reset_Particle(tParticle2);

	tParticle2.fAcc = 1.f;
	tParticle2.fCurrentTime = 0.f;
	tParticle2.fRotateSpeed = 0.f;


	tParticle2.vEndSize = _vec3(0.1f, 0.1f, 0.1f);
	tParticle2.vStartColor = { 200.f * 2.5f, 50.f* 2.5f, 255.f* 2.5f, 250.f * 2.5f };
	tParticle2.vEndColor = { 200.f* 2.5f, 50.f* 2.5f, 255.f* 2.5f, 255.f };

	for (_int i = 0; i < 15; ++i)
	{
		Engine::Set_AnimState(tParticle2, 4, 4, CRandoms()(0.5f, 1.f));
		tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.7f, 0.7f), 0.2f, CRandoms()(-0.7f, 0.7f));
		_float fSize = CRandoms()(0.4f, 0.7f);
		tParticle2.vStartSize = _vec3(fSize, fSize, fSize);
		tParticle2.fLifeTime = CRandoms()(0.4f, 1.f);
		tParticle2.fVelocity = CRandoms()(1.f, 3.f);
		tParticle2.vDir = { 0.f, CRandoms()(0.1f, 2.f), 0.f };
		Engine::Emit_ImageParticle(tParticle2, 8, 22);
	}

	for (_int i = 0; i < 15; ++i)
	{
		Engine::Set_AnimState(tParticle2, 4, 4, CRandoms()(0.5f, 1.f));
		tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.7f, 0.7f), 0.2f, CRandoms()(-0.7f, 0.7f));
		_float fSize = CRandoms()(0.4f, 0.7f);
		tParticle2.vStartSize = _vec3(fSize, fSize, fSize);
		tParticle2.fLifeTime = CRandoms()(0.4f, 1.f);
		tParticle2.fVelocity = CRandoms()(1.f, 3.f);
		tParticle2.vStartColor = { 0.f, 0.f, 0.f, 255.f };
		tParticle2.vEndColor = { 0.f, 0.f, 0.f, 255.f };
		tParticle2.vDir = { 0.f, CRandoms()(0.1f, 2.f), 0.f };
		Engine::Emit_ImageParticle(tParticle2, 8, 22);
	}

	PARTICLE tParticle3;
	Engine::Reset_Particle(tParticle3);

	tParticle3.fAcc = 1.f;
	tParticle3.fCurrentTime = 0.f;
	tParticle3.fRotateSpeed = 0.f;


	tParticle3.vEndSize = _vec3(0.1f, 0.1f, 0.1f);
	tParticle3.vStartColor = { 200.f * 4.5f, 50.f* 4.5f, 255.f* 4.5f, 250.f * 4.5f };
	tParticle3.vEndColor = { 200.f* 4.5f, 50.f* 4.5f, 255.f* 4.5f, 255.f };

	for (_int i = 0; i < 15; ++i)
	{
		tParticle3.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.4f, 0.4f), 0.2f, CRandoms()(-0.4f, 0.4f));
		_float fSize = CRandoms()(0.6f, 1.f);
		tParticle3.vStartSize = _vec3(fSize, fSize, fSize);
		tParticle3.fLifeTime = CRandoms()(0.6f, 1.f);
		tParticle3.fVelocity = CRandoms()(1.f, 3.f);
		tParticle3.vDir = { 0.f, CRandoms()(-1.f, 2.f), 0.f };
		Engine::Emit_ImageParticle(tParticle3, 10, 18);
	}

	//////////////////////////////////////////////////////////////////////////
}

void CEnermy::Dead_Effect2(_bool Shake)
{
	if (Shake)
	{
		CMainCamera* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
		pCamera->Set_Shake();
	}

	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);
	Engine::Set_AnimState(tParticle, 8, 8);
	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vStartSize = _vec3(4.f, 4.f, 4.f);
	tParticle.vEndSize = _vec3(4.f, 4.f, 4.f);
	tParticle.fRotateSpeed = 0.f;
	tParticle.fVelocity = 3.f;
	tParticle.fLifeTime = 0.8f;
	tParticle.vDir = { 0.f, 1.f, 0.f };
	tParticle.vStartColor = { 0.f, 0.f, 0.f, 255.f * 4.5f };
	tParticle.vEndColor = { 0.f, 0.f, 0.f, 255.f };
	tParticle.vRot = { 0.f, 0.f, 0.f };

	tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 0.5f, 0.f);
	Engine::Emit_ImageParticle(tParticle, 30, 4);

	for (_int i = 0; i < 7; ++i)
	{
		PARTICLE tParticle2;
		Engine::Reset_Particle(tParticle2);
		_float fRand = 0.f;
		if (CRandoms()(0, 1) == 1)
			fRand = 0.f;
		else
			fRand = 0.5f;

		Engine::Set_AnimState(tParticle2, 2, 2, fRand);
		if (CRandoms()(0, 1) == 1)
			tParticle2.fAmount = 0.f;
		else
			tParticle2.fAmount = 0.5f;

		tParticle2.fAcc = 1.f;
		tParticle2.fCurrentTime = 0.f;
		tParticle2.fRotateSpeed = 0.f;

		tParticle2.fLifeTime = 3.f;
		tParticle2.vRot = { 0.f, D3DXToRadian(CRandoms()(0, 360)), 0.f };
		tParticle2.bGravity = true;
		tParticle2.fGravityForce = 2.f;

		tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f);

		_float fSize = CRandoms()(1, 10) * 0.1f + 3.5f;
		tParticle2.vStartColor = { 240.f * fSize, 180.f * fSize , 130.f * fSize, 255.f };
		tParticle2.vEndColor = { 240.f * fSize, 180.f * fSize, 130.f * fSize, 255.f };
		_float fStart = CRandoms()(20, 80) * 0.01f + 0.1f;
		tParticle2.vStartSize = _vec3(fStart, fStart, fStart);
		tParticle2.vEndSize = _vec3(fStart, fStart, fStart);
		tParticle2.fVelocity = _float(CRandoms()(1, 5) + 5.f);
		tParticle2.vDir = { CRandoms()(-100, 100) * 0.1f, CRandoms()(100, 3000) * 0.01f, CRandoms()(-100, 100) * 0.1f };
		Engine::Emit_ImageParticle(tParticle2, 27, 17);
	}

	PARTICLE tParticle3;
	Engine::Reset_Particle(tParticle3);
	tParticle3.fAcc = 1.f;
	tParticle3.fCurrentTime = 0.f;
	tParticle3.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
	tParticle3.fRotateSpeed = 0.f;

	tParticle3.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2, 255.f * 2.5f };
	tParticle3.vEndColor = { 255.f, 255.f, 255.f, 255.f };
	tParticle3.vRot = { 0.f, 0.f, 0.f };

	for (_int i = 0; i < 20; ++i)
	{
		tParticle3.fVelocity = CRandoms()(0, 200) * 0.02f;
		tParticle3.fLifeTime = CRandoms()(0, 2) * 0.1f + 0.5f;
		_float fRand = CRandoms()(5, 10) * 0.01f + 0.1f;
		tParticle3.vStartSize = _vec3(fRand, fRand, fRand);
		tParticle3.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-10, 10) * 0.1f, CRandoms()(5, 20) * 0.1f, CRandoms()(-10, 10) * 0.1f);
		tParticle3.vDir = { CRandoms()(-100, 100) * 0.01f,CRandoms()(-50, 150) * 0.01f , CRandoms()(-100, 100) * 0.01f };
		Engine::Emit_ImageParticle(tParticle3, 15, 11);
	}
}

void CEnermy::Hit_Effect(_vec3 vPos, _float fSize)
{
	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);
	Engine::Set_AnimState(tParticle, 2, 2, 0.9f);
	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vStartSize = _vec3(0.5f, 0.5f, 0.5f);
	tParticle.vEndSize = { fSize , fSize , fSize };
	tParticle.fRotateSpeed = 0.f;
	tParticle.fVelocity = 0.f;
	tParticle.fLifeTime = 0.2f;
	tParticle.vDir = { 0.f, 0.f, 0.f };
	tParticle.vStartColor = { 255.f * 1.5f, 255.f* 1.5f, 255.f* 1.5f, 255.f };
	tParticle.vEndColor = { 255.f* 2.5f, 255.f* 2.5f, 255.f* 2.5f, 255.f };
	tParticle.vRot = { 0.f, 0.f, 0.f };

	tParticle.vPos = vPos;
	Engine::Emit_ImageParticle(tParticle, 44, 25);
}

void CEnermy::Hit_Damage(_float fPosY)
{
	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);

	tParticle.vDir = { CRandoms()(-1.f, 1.f), CRandoms()(0.f, 2.f), CRandoms()(-1.f, 1.f) };
	tParticle.vStartColor = { _float(CRandoms()(0, 255)), _float(CRandoms()(0, 255)), _float(CRandoms()(0, 255)), 255.f };
	tParticle.vEndColor = { tParticle.vStartColor.r, tParticle.vStartColor.g, tParticle.vStartColor.b, 55.f };
	tParticle.vRot = { 0.f, 0.f, 0.f };
	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.5f, 0.5f), CRandoms()(-0.5f, 0.5f) + fPosY, CRandoms()(-0.5f, 0.5f));

	_float fRand = CRandoms()(0.5f, 0.8f);
	tParticle.vStartSize = _vec3(fRand, fRand, fRand);
	tParticle.vEndSize = { 0.2f , 0.2f , 0.2f };
	tParticle.fLifeTime = 1.f;
	tParticle.fRotateSpeed = 0.f;
	tParticle.fAmount = _float(CRandoms()(0, 15));

	_int i = CRandoms()(0, 1);
	if (i == 0)
	{
		tParticle.fVelocity = 5.f;
		tParticle.bGravity = true;
		tParticle.fGravityForce = 1.5f;

		Engine::Emit_ImageParticle(tParticle, 48, 27);
	}
	else if (i == 1)
	{
		tParticle.fVelocity = 1.f;

		Engine::Emit_ImageParticle(tParticle, 48, 27);
	}
}

void CEnermy::Hit_Rim()
{
	m_bRimLight = true;
}

void CEnermy::Hit_RimLight(const _float & fTimeDelta)
{
	if (!m_bRimLight)
		return;

	m_fRimLightTime += fTimeDelta;

	if (m_fRimLightTime > 0.17f)
	{
		m_bRimLight = false;
		m_fRimLightTime = 0.f;
	}
}

void CEnermy::Set_Damage(_int iHp)
{
	m_iHP += iHp;
	m_bHit = true;

	//Hit_Effect(m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f), 2.f);
}

void CEnermy::Free()
{
	CGameObject::Free();
}

void CEnermy::Create_DefaultUI(_float fSize, _float fPosY)
{
	m_fPosY = fPosY;

	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);
	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vStartSize = { fSize , 0.1f , 1.f };
	tParticle.vEndSize = { fSize , 0.1f , 1.f };
	tParticle.fRotateSpeed = 0.f;
	tParticle.fVelocity = 0.f;
	tParticle.fLifeTime = 100000.f;
	tParticle.vDir = { 0.f,0.f, 0.f };

	tParticle.vRot = { D3DXToRadian(90.f), D3DXToRadian(-120.f), 0.f };
	tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, fPosY, 0.f);
	tParticle.bBill = false;

	tParticle.vStartColor = { 255.f, 255.f, 255.f, 255.f };
	tParticle.vEndColor = { 255.f, 255.f, 255.f, 255.f };
	m_pEnermyMaxHp = Engine::Emit_ImageParticle(tParticle, 49, 28);
	
	tParticle.vStartColor = { 255.f , 55.f, 55.f, 255.f  * 3.5f };
	tParticle.vEndColor = { 255.f, 55.f, 55.f, 255.f  * 3.5f };
	m_pEnermyHp = Engine::Emit_ImageParticle(tParticle, 49, 28);
}

void CEnermy::Update_DefaultUI(const _float & fTimeDelta)
{
	m_pEnermyHp->Set_Amount((_float)m_iHP / (_float)m_iHPMax);
	m_pEnermyMaxHp->Set_Amount(1.f);

	m_pEnermyHp->Set_ParticlePos(m_pTransformCom->Get_Position() + _vec3(0.f, m_fPosY, 0.f));
	m_pEnermyMaxHp->Set_ParticlePos(m_pTransformCom->Get_Position() + _vec3(0.f, m_fPosY, 0.f));
}

void CEnermy::Delete_UI()
{
	if (m_pEnermyHp != nullptr)
	{
		m_pEnermyHp->Set_Active(false);
		m_pEnermyHp = nullptr;
	}
	if (m_pEnermyMaxHp != nullptr)
	{
		m_pEnermyMaxHp->Set_Active(false);
		m_pEnermyMaxHp = nullptr;
	}
}

void CEnermy::Tick_Damage(const _float& fTimeDelta)
{
	if (!m_bTick)
		return;

	if (m_bTickFinite)
		m_fTickTime -= fTimeDelta;

	if (m_fTickTime <= 0.f)
		m_bTick = false;

	m_iHP -= m_iTickDamge;
	m_bHit = true;
}

void CEnermy::Set_TickDamage(const _bool& bState, const _int& iDamage, const _bool& bIsFinite, const _float& fTickDuration, const _float& fTickCool)
{
	m_bTickFinite = bIsFinite;
	m_fTickTime = fTickDuration;
	m_iTickDamge = iDamage;
	m_bTick = bState;
}

void CEnermy::Set_TickOff(const _bool& bState)
{
	m_bTick = bState;
}
