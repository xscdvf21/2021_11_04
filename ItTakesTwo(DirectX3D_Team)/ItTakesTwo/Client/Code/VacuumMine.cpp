#include "stdafx.h"
#include "VacuumMine.h"
#include "Cody.h"
#include "May.h"
#include "Export_Function.h"



CVacuumMine::CVacuumMine(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_bTram(false), m_bTramStart(true), m_fTramPower(0.f)
	, m_fTramTime(0.f), m_fTramHeight(0.f), m_fDuration(2.f)

{
}

CVacuumMine::~CVacuumMine()
{
}

HRESULT CVacuumMine::Ready_Object(_vec3 vPos, _vec3 vTargetPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(vPos);
	m_vTargetPos = vTargetPos;

	return S_OK;

}

_int CVacuumMine::Update_Object(const _float & fTimeDelta)
{
	m_fLifeTime += fTimeDelta;

	if (m_fLifeTime > 4.f)
	{
		// DEAD EFFECT
		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 8, 8, 1.f);

		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.vStartSize = _vec3(5.f, 5.f, 5.f);
		tParticle.vEndSize = _vec3(5.f, 5.f, 5.f);
		tParticle.fRotateSpeed = 0.f;
		tParticle.vStartColor = { 100.f , 100.f, 100.f, 255.f };
		tParticle.vEndColor = { 100.f, 100.f, 100.f , 255.f };
		tParticle.vRot = { 0.f, 0.f, 0.f };
		tParticle.fVelocity = 0.f;
		tParticle.fLifeTime = 1.f;
		tParticle.vRot = { 0.f, 0.f, 0.f };

		tParticle.vDir = { 0.f, 0.f, 0.f };
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 0.6f, 0.f);
		Engine::Emit_ImageParticle(tParticle, 23, 4);


		PARTICLE tParticle2;
		Engine::Reset_Particle(tParticle2);
		Engine::Set_AnimState(tParticle2, 7, 7, 1.f);

		tParticle2.fAcc = 1.f;
		tParticle2.fCurrentTime = 0.f;
		tParticle2.vStartSize = _vec3(3.f, 3.f, 3.f);
		tParticle2.vEndSize = _vec3(3.f, 3.f, 3.f);
		tParticle2.fRotateSpeed = 0.f;
		tParticle2.vStartColor = { 255.f * 4.f , 110.f * 4.f, 90.f * 4.f, 255.f * 4.f };
		tParticle2.vEndColor = { 255.f * 4.f, 110.f * 4.f, 90.f * 4.f, 255.f };
		tParticle2.vRot = { 0.f, 0.f, 0.f };
		tParticle2.fVelocity = 0.f;
		tParticle2.fLifeTime = 0.4f;
		tParticle2.vRot = { 0.f, 0.f, 0.f };

		tParticle2.vDir = { 0.f, 0.f, 0.f };
		tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 0.6f, 0.f);
		Engine::Emit_ImageParticle(tParticle2, 11, 4);


		CRandoms Random;
		_uint	iRandom = Random(1,2);

		if (iRandom == 1)
		{

			Engine::PlaySoundW(L"Vacuum_MineDead0.wav", CSoundMgr::CHANNELID::Vacuum_Mine, 0.005f);
		}
		else if (iRandom == 2)
		{
			Engine::PlaySoundW(L"Vacuum_MineDead1.wav", CSoundMgr::CHANNELID::Vacuum_Mine, 0.005f);
		}
		else if (iRandom == 3)
		{
			//Engine::StopSound(CSoundMgr::CHANNELID::Vacuum_Mine);
			Engine::PlaySoundW(L"Vacuum_MineDead2.wav", CSoundMgr::CHANNELID::Vacuum_Mine, 0.005f);

		}

		CCody*	pCody = (CCody*)Engine::Get_GameObject(L"GameObject", L"Cody");
		CTransform*	 pCodyTrans = (CTransform*)pCody->Get_Component(L"Com_Transform", ID_DYNAMIC);

		_vec3 vPos = m_pTransformCom->Get_Position();
		_vec3 vCodyPos = pCodyTrans->Get_Position();
		_vec3 vDir = vPos - vCodyPos;
		_float fDir;
		fDir = D3DXVec3Length(&vDir);

		if (fDir < 2.f)
		{
			pCody->Set_Damage(-1);
		}
	
		CMay*	pMay = (CMay*)Engine::Get_GameObject(L"GameObject", L"May");
		CTransform*	 pMayTrans = (CTransform*)pMay->Get_Component(L"Com_Transform", ID_DYNAMIC);

		_vec3 vMayPos = pMayTrans->Get_Position();
		_vec3 vDir2 = vPos - vMayPos;
		_float fDir2;
		fDir2 = D3DXVec3Length(&vDir2);

		if (fDir2 < 2.f)
		{
			pMay->Set_Damage(-1);
		}


		return OBJ_DEAD;
	}


	Move(fTimeDelta, m_vTargetPos);


	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CVacuumMine::LateUpdate_Object(const _float & fTimeDelta)
{
	// IDLE EFFECT
	m_fEffectTime += fTimeDelta;

	if (m_fEffectTime > 0.1f)
	{
		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 8, 8, 1.f);

		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.vStartSize = _vec3(1.f, 1.f, 1.f);
		tParticle.vEndSize = _vec3(1.f, 1.f, 1.f);
		tParticle.fRotateSpeed = 0.f;
		tParticle.vStartColor = { 255.f * 2.f, 255.f * 1.5f, 50.f * 1.5f, 255.f * 4.5f };
		tParticle.vEndColor = { 255.f * 2.f, 255.f * 1.5f, 50.f * 1.5f, 255.f* 2.5f };
		tParticle.vRot = { 0.f, 0.f, 0.f };
		tParticle.fVelocity = 0.f;
		tParticle.fLifeTime = 0.03f;
		tParticle.vRot = { 0.f, 0.f, 0.f };

		tParticle.vDir = { 0.f, 0.f, 0.f };
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 0.6f, 0.f);
		Engine::Emit_ImageParticle(tParticle, 15, 16);


		m_fEffectTime = 0.f;
	}


	return _int();
}

void CVacuumMine::Render_Object(const _int & iIndex)
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
}

HRESULT CVacuumMine::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_VB, L"VacuumMine"));
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

HRESULT CVacuumMine::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CVacuumMine::Move(const _float & fTimeDelta, const _vec3 & TargetPos)
{
	_float fStartPower = 0.f;

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


		CRandoms Random;
		_uint	iRandom = Random(1, 5);


		if (!m_bSound)
		{
			if (iRandom == 1)
			{
				Engine::PlaySoundW(L"Vacuum_Mine0.wav", CSoundMgr::CHANNELID::Vacuum_Mine, 0.05f);
			}
			else if (iRandom == 2)
			{
				Engine::PlaySoundW(L"Vacuum_Mine1.wav", CSoundMgr::CHANNELID::Vacuum_Mine, 0.05f);
			}
			else if (iRandom == 3)
			{
				Engine::PlaySoundW(L"Vacuum_Mine2.wav", CSoundMgr::CHANNELID::Vacuum_Mine, 0.05f);
			}
			else if (iRandom == 4)
			{
				Engine::PlaySoundW(L"Vacuum_Mine3.wav", CSoundMgr::CHANNELID::Vacuum_Mine, 0.05f);
			}

			else if (iRandom == 5)
			{
				Engine::PlaySoundW(L"Vacuum_Mine4.wav", CSoundMgr::CHANNELID::Vacuum_Mine, 0.05f);
			}
			m_bSound = true;
		}


		_vec3 vPosTemp = { m_vSetPos.x + 3.f, m_vSetPos.y, m_vSetPos.z };
		m_pTransformCom->Set_Pos(m_vSetPos + _vec3(0.f, 0.2f, 0.f));
	}
}

CVacuumMine * CVacuumMine::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vTargetPos)
{
	CVacuumMine*	pInstance = new CVacuumMine(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos, vTargetPos)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVacuumMine::Free(void)
{
	Engine::CGameObject::Free();
}
