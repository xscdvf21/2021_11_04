#include "stdafx.h"
#include "May_Castle.h"
#include "Export_Function.h"
#include "MayCamera.h"
#include "MayCastle_Animation.h"
#include "MainCamera.h"
#include "CollisionMgr.h"
#include "ToySword.h"
#include "Enermy.h"


CMayCastle::CMayCastle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CPlayer(pGraphicDev)
	, m_fDashCool(3.f), m_fSmashCool(3.f), m_fSmashRadius(5.f), m_iSmashDamage(-5), m_iWindDamage(-3), m_fWindDamageTime(0.f)
{
}

CMayCastle::~CMayCastle()
{
}

HRESULT CMayCastle::Ready_Object(void* pArg)
{
	m_fColDis = 0.8f;

	// 콜라이더
	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = m_fColDis;

	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);
	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	CMayCastle_Anim::GetInstance()->ReadyMayAnimation();

	// 플레이어 Hp와 MaxHp 초기화
	// 스테이지마다 Hp가 달라서 Ready에서 초기화


	//m_vSavePoint = { 118.f, 1.f, -148.f };
	m_vSavePoint = { 0.f, 1.f, 0.f };

	m_iMaxHp = 12;
	m_iHp = m_iMaxHp;
	m_iHitCount = 0;

	return S_OK;
}

Engine::_int CMayCastle::Update_Object(const _float& fTimeDelta)
{

	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();

	if (vPos.y >= 1.f)
	{
		m_pTransformCom->Set_Pos(vPos.x, 0.5f, vPos.z);
	}


	if (m_iHitCount >= m_iHitCountMax)
	{
		m_fUltiAttTime = 3.f;
		m_iHitCount = 20;
	}

	m_fWindDamageTime += fTimeDelta;

	if (m_iHp <= 0)
	{
		m_bDead = true;
		m_bUltEffect = false;
		m_ePL_STATE = PL_DEAD;
		//Dead_State(fTimeDelta);
		//CMayCastle_Anim::GetInstance()->PlayAnimation(this, m_pMeshCom, 0.0f, m_ePL_STATE, &m_ePre_State, m_eAtt_State);
		//Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

		return OBJ_NOEVENT;
	}

	// 무기 소유
	if (nullptr == m_pSword)
	{
		m_pSword = (CToySword*)Engine::Get_GameObject(L"GameObject", L"ToySword");
		m_pSword->AddRef();
	}

	Engine::CGameObject::Update_Object(fTimeDelta);
	if (!m_bCutScene &&!m_bDead)
	{
		Key_Input(fTimeDelta);
	}
	Effect(fTimeDelta);
	Set_State(fTimeDelta);
	Update_UI(fTimeDelta);
	CoolTime(fTimeDelta);

	// HP가 최대치보다 낮으면 일정시간후에 HP회복
	Recover(fTimeDelta);

	CMayCastle_Anim::GetInstance()->PlayAnimation(this, m_pMeshCom, fTimeDelta, m_ePL_STATE, &m_ePre_State, m_eAtt_State);

	CCollisionMgr::GetInstance()->Add_PlayerList(this);
	if (!m_bDeadUI)
	{
		Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
		if (m_bStencil)
		{
			Engine::Add_RenderGroup(Engine::RENDER_SHADOW, this);
			Engine::Add_RenderGroup(Engine::RENDER_STENCIL, this);
			m_pSword->Set_Render(true);
		}
		else
			m_pSword->Set_Render(false);
	}
	

	return OBJ_NOEVENT;
}

_int CMayCastle::LateUpdate_Object(const _float & fTimeDelta)
{
	if (true == m_bSmashAtt)
	{
		SmashAtt();
		m_bSmashAtt = false;
	}

	if (true == m_bWindAtt)
	{
		WindAtt();
		m_bWindAtt = false;
	}
	if (m_bStart)
	{
		Create_UI(L"MC_UI");
		m_bStart = false;
	}

	Key_Reset();

	PxTransform tTransform = m_pRigidActor->getGlobalPose();
	if (tTransform.p.y >= 0.8f)
	{
		tTransform.p.y = 0.8f;
		m_pRigidActor->setGlobalPose(tTransform);
	}
	_vec3 vPos;
	vPos.x = tTransform.p.x;
	vPos.y = tTransform.p.y - 0.5f;
	vPos.z = tTransform.p.z;
	m_pTransformCom->Set_Pos(vPos);
	return _int();
}

void CMayCastle::Render_Object(const _int& iIndex)
{
	// 죽었을경우 랜더를 안한다.
	if (m_bDead)
		return;

	if (m_bCutScene)
		return;

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes_VTF(pEffect, 0);

	//pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	CMainCamera* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
	pCamera->Update_View_Proj();

	const D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName("Spine2");
	_float4x4 matBoneMatrix = pBone->CombinedTranformationMatrix;

	//Spine2
	m_tColInfo.matWorld = matBoneMatrix * *m_pTransformCom->Get_WorldMatrix();
	m_pColliderCom->Render_Collider(m_tColInfo.matWorld, false);

	// 디버그용 폰트
#ifdef _DEBUG
	TCHAR szbuf[MAX_PATH] = L"";
	wsprintf(szbuf, L"MayHp : %d", m_iHp);
	Engine::Render_Font(L"Font_Jinji", szbuf, &_vec2(20, 40), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
#endif
}

void CMayCastle::Render_Shadow(_vec3 * vEye, _vec3 * vAt, const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_matrix matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix matView, matInv;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInv, nullptr, &matView);

	CMainCamera* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
	*vEye = pCamera->Get_CameraPos();
	*vAt = pCamera->Get_CameraAt();

	_vec3		vLightEye = _vec3(4.f, 0.f, -4.f);

	_matrix		LightViewMatrix, LightProjMatrix;

	D3DXMatrixLookAtLH(&LightViewMatrix, &(*vEye + vLightEye), &*vAt, &vUP);
	pEffect->SetMatrix("g_matView", &LightViewMatrix);

	D3DXMatrixPerspectiveFovLH(&LightProjMatrix, D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);
	pEffect->SetMatrix("g_matProj", &LightProjMatrix);

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes_VTF(pEffect, 2);

	pEffect->End();

	Safe_Release(pEffect);
}

void CMayCastle::Render_Stencil(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes_VTF(pEffect, 3);

	//pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

void CMayCastle::Effect(const _float & fTimeDelta)
{
	Effect_Sword(fTimeDelta);
	Effect_DefaultAttack(fTimeDelta);
	Effect_Dash(fTimeDelta);
	Effect_DefaultSkill(fTimeDelta);
	Effect_UltimateSkill(fTimeDelta);
	Effect_Walk(fTimeDelta);
	Effect_Dead(fTimeDelta);
	Effect_Respawn(fTimeDelta);
}

void CMayCastle::Effect_Sword(const _float & fTimeDelta)
{
	if (m_bDeadUI || m_bCutScene)
		return;

	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);
	Engine::Set_AnimState(tParticle, 4, 4, 0.5f);

	_matrix matWorld;
	const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("RightAttach");
	const _matrix* pMatrix = &pFrame->CombinedTranformationMatrix;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	matWorld = *pMatrix * matWorld;

	_vec3 vSwordPos = { matWorld._41, matWorld._42, matWorld._43 };
	_vec3 vSwordDir = { matWorld._21, matWorld._22, matWorld._23 };
	D3DXVec3Normalize(&vSwordDir, &vSwordDir);

	tParticle.vPos = vSwordPos + vSwordDir * (CRandoms()(3, 17) * 0.1f);
	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.fLifeTime = CRandoms()(0, 5) * 0.1f + 0.5f;
	tParticle.fRotateSpeed = 0.f;
	tParticle.fVelocity = CRandoms()(0, 10) * 0.1f + 1.f;
	_float fSize = CRandoms()(0, 4) * 0.1f + 0.2f;
	tParticle.vStartSize = _vec3(fSize, fSize, fSize);
	tParticle.vEndSize = _vec3(0.1f, 0.1f, 0.1f);
	tParticle.vDir = { 0.f, CRandoms()(0, 20) * 0.1f + 1.f, 0.f };
	tParticle.vStartColor = { 255.f, 255.f, 255.f, 250.f };
	tParticle.vEndColor = { 255.f, 255.f, 255.f, 0.f };

	Engine::Emit_ImageParticle(tParticle, 8, 9);
}

void CMayCastle::Effect_DefaultAttack(const _float & fTimeDelta)
{
	//SwordTrail
}

void CMayCastle::Effect_Dash(const _float & fTimeDelta)
{
	if (!m_bDashEffect)
		return;

	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);
	Engine::Set_AnimState(tParticle, 8, 8, CRandoms()(0.5f, 1.f));

	tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f);
	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.fLifeTime = CRandoms()(1.1f, 1.4f);
	tParticle.fRotateSpeed = 0.f;
	tParticle.fVelocity = 0.f;
	tParticle.vStartColor = { 255.f * 1.5f, 255.f * 1.5f, 255.f * 1.5f, 255.f * 1.5f };
	tParticle.vEndColor = { 255.f, 255.f, 255.f, 105.f };
	tParticle.vStartSize = _vec3(3.f, 3.f, 3.f);
	tParticle.vEndSize = _vec3(2.f, 2.f, 2.f);
	tParticle.vDir = { 0.f, 1.f, 0.f };

	Engine::Emit_ImageParticle(tParticle, 5, 4);

	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.fRotateSpeed = 0.f;
	tParticle.fVelocity = 0.f;
	tParticle.vStartSize = _vec3(1.f, 1.f, 1.f);
	tParticle.vEndSize = _vec3(0.5f, 0.5f, 0.5f);
	tParticle.vDir = { 0.f, 1.f, 0.f };

	for (_int i = 0; i < 3; ++i)
	{
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-10, 5) * 0.1f, 1.f, CRandoms()(-10, 5) * 0.1f);
		_float fRand = CRandoms()(5, 12) * 0.1f;
		tParticle.fLifeTime = fRand;
		tParticle.vStartSize = _vec3(fRand, fRand, fRand);
		tParticle.vEndSize = _vec3(0.3f, 0.3f, 0.3f);
		Engine::Emit_ImageParticle(tParticle, 2, 6);
	}

	PARTICLE tParticle4;
	Reset_Particle(tParticle4);
	tParticle4.fAcc = 1.f;
	tParticle4.fCurrentTime = 0.f;
	tParticle4.vStartSize = _vec3(0.05f, 0.05f, 0.05f);
	tParticle4.vEndSize = _vec3(0.05f, 0.05f, 0.05f);
	tParticle4.fRotateSpeed = 0.f;

	tParticle4.vStartColor = { 255.f, 200.f, 40.f, 255.f };
	tParticle4.vEndColor = { 255.f, 200.f, 200.f, 255.f };
	tParticle4.vRot = { 0.f, 0.f, 0.f };
	for (_int i = 0; i < 5; ++i)
	{
		tParticle4.fVelocity = CRandoms()(0, 15) * 0.1f + 1.f;
		tParticle4.fLifeTime = CRandoms()(0, 5) * 0.1f + 1.f;
		tParticle4.vDir = { CRandoms()(-10, 10) * 0.1f, CRandoms()(0, 15) * 0.1f, CRandoms()(-10, 10) * 0.1f };
		tParticle4.vPos = tParticle.vPos + _vec3(CRandoms()(-10, 10) * 0.1f, -1.f, CRandoms()(-10, 10) * 0.1f);
		Engine::Emit_ImageParticle(tParticle4, 15, 11);
	}
}

void CMayCastle::Effect_DefaultSkill(const _float & fTimeDelta)
{
	switch (m_iSkillEffect)
	{
	case 0:
		return;

	case 1: // Effect Start
	{
		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 4, 4, 0.5f);

		_matrix matWorld;
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("RightAttach");
		const _matrix* pMatrix = &pFrame->CombinedTranformationMatrix;

		m_pTransformCom->Get_WorldMatrix(&matWorld);
		matWorld = *pMatrix * matWorld;

		_vec3 vSwordPos = { matWorld._41, matWorld._42, matWorld._43 };
		_vec3 vSwordDir = { matWorld._21, matWorld._22, matWorld._23 };
		D3DXVec3Normalize(&vSwordDir, &vSwordDir);

		tParticle.vPos = vSwordPos + vSwordDir * (CRandoms()(4, 15) * 0.1f);
		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.fLifeTime = 0.8f;
		tParticle.fRotateSpeed = 0.f;
		tParticle.fVelocity = 1.f;
		tParticle.vStartSize = _vec3(1.5f, 1.5f, 1.5f);
		tParticle.vEndSize = _vec3(1.f, 1.f, 1.f);
		tParticle.vDir = { 0.f, 0.f, 0.f };
		tParticle.vStartColor = { 255.f, 255.f, 255.f, 200.f };
		tParticle.vEndColor = { 255.f, 255.f, 255.f, 0.f };

		Engine::Emit_ImageParticle(tParticle, 8, 9);

		m_iSkillEffect = 0;
	}
	return;

	case 2: // Effect End
	{
		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 8, 8, 1.f);

		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.fLifeTime = 0.7f;
		tParticle.fRotateSpeed = 0.f;
		tParticle.fVelocity = 1.f;
		tParticle.vStartSize = _vec3(1.f, 1.f, 1.f);
		tParticle.vEndSize = _vec3(6.f, 6.f, 6.f);
		tParticle.vDir = { 0.f, 1.f, 0.f };
		/*tParticle.vStartColor = { 605.f, 1000.f, 50.f, 300.f };
		tParticle.vEndColor = { 800.f, 1500.f, 50.f, 0.f };*/
		tParticle.vStartColor = { 605.f, 800.f, 255.f, 255.f };
		tParticle.vEndColor = { 255.f, 255.f, 255.f, 0.f };


		PARTICLE tParticle2;
		Reset_Particle(tParticle2);
		tParticle2.fAcc = 1.f;
		tParticle2.fCurrentTime = 0.f;
		tParticle2.vStartSize = _vec3(5.f, 5.f, 5.f);
		tParticle2.vEndSize = _vec3(5.f, 5.f, 5.f);
		tParticle2.fRotateSpeed = 0.f;
		tParticle2.fVelocity = 1.f;
		tParticle2.fLifeTime = 1.5f;
		tParticle2.vDir = { 0.f, 0.f, 0.f };
		tParticle2.vStartColor = { 255.f, 200.f, 40.f, 255.f };
		tParticle2.vEndColor = { -100.f, -100.f, -100.f, 255.f };
		tParticle2.bBill = false;
		tParticle2.vRot = { D3DXToRadian(90.f), 0.f, 0.f };

		PARTICLE tParticle3;
		Reset_Particle(tParticle3);
		tParticle3.fAcc = 1.f;
		tParticle3.fCurrentTime = 0.f;
		tParticle3.vStartSize = _vec3(0.5f, 0.5f, 0.5f);
		tParticle3.vEndSize = _vec3(0.5f, 0.5f, 0.5f);
		tParticle3.fRotateSpeed = 0.f;
		tParticle3.fVelocity = 1.f;
		tParticle3.fLifeTime = 1.5f;
		tParticle3.vDir = { 0.f, 0.f, 0.f };
		tParticle3.vStartColor = { 255.f, 255.f, 40.f, 255.f };
		tParticle3.vEndColor = { 255.f, 255.f, 40.f, 150.f };
		tParticle3.vRot = { 0.f, 0.f, 0.f };

		PARTICLE tParticle4;
		Reset_Particle(tParticle4);
		tParticle4.fAcc = 1.f;
		tParticle4.fCurrentTime = 0.f;
		tParticle4.vStartSize = _vec3(0.05f, 0.05f, 0.05f);
		tParticle4.vEndSize = _vec3(0.05f, 0.05f, 0.05f);
		tParticle4.fRotateSpeed = 0.f;

		tParticle4.vStartColor = { 255.f, 200.f, 40.f, 255.f };
		tParticle4.vEndColor = { 255.f, 200.f, 200.f, 255.f };
		tParticle4.vRot = { 0.f, 0.f, 0.f };

		for (_int i = 0; i < 15; ++i)
		{
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3((CRandoms()(-5, 5) * 0.5f) + _float(CRandoms()(-1, 1)), 1.f, (CRandoms()(-10, 10) * 0.1f) + _float(CRandoms()(-1, 1)));
			Engine::Emit_ImageParticle(tParticle, 1, 7);

			tParticle2.vPos = tParticle.vPos + _vec3(0.f, -0.8f, 0.f);
			Engine::Emit_ImageParticle(tParticle2, 9, 8);
			Engine::Emit_ImageParticle(tParticle2, 9, 8);


			for (_int i = 0; i < 6; ++i)
			{
				tParticle3.vPos = tParticle.vPos + _vec3(CRandoms()(-10, 10) * 0.1f, 0.f, CRandoms()(-10, 10) * 0.1f);
				Engine::Emit_ImageParticle(tParticle3, 2, 6);

				tParticle4.fVelocity = CRandoms()(0, 15) * 0.1f + 1.f;
				tParticle4.fLifeTime = CRandoms()(0, 5) * 0.1f + 1.f;
				tParticle4.vDir = { CRandoms()(-10, 10) * 0.1f, CRandoms()(0, 15) * 0.1f, CRandoms()(-10, 10) * 0.1f };
				tParticle4.vPos = tParticle.vPos + _vec3(CRandoms()(-10, 10) * 0.1f, 0.f, CRandoms()(-10, 10) * 0.1f);
				Engine::Emit_ImageParticle(tParticle4, 15, 11);
			}
		}

		m_iSkillEffect = 0;
	}
	return;
	}
}

void CMayCastle::Effect_UltimateSkill(const _float & fTimeDelta)
{
	if (!m_bUltEffect)
		return;

	m_fUltTime += fTimeDelta;

	//if (m_fUltTime > 0.03f)
	//{
	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);

	tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 1.5f, 0.f);
	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.fLifeTime = 1.2f;
	tParticle.fRotateSpeed = 0.f;
	tParticle.fVelocity = 1.f;
	tParticle.vStartSize = _vec3(12.f, 12.f, 12.f);
	tParticle.vEndSize = _vec3(1.f, 1.f, 1.f);
	tParticle.vDir = { 0.f, -1.5f, 0.f };
	tParticle.vStartColor = { 255.f, 235.f, 0.f, 250.f };
	tParticle.vEndColor = { 255.f, 50.f, 50.f, 0.f };
	tParticle.bBill = false;
	m_fUltAngle -= 20.f;
	tParticle.vRot = { D3DXToRadian(90.f), D3DXToRadian(m_fUltAngle), 0.f };

	Engine::Emit_ImageParticle(tParticle, 2, 10);
	m_fUltTime = 0.f;
	//}

	if (m_fUltAngle <= -360.f)
		m_fUltAngle = 0.f;
}

void CMayCastle::Effect_Walk(const _float & fTimeDelta)
{
	m_fMoveEffectTime += fTimeDelta;

	if (m_ePL_STATE == PL_MOVE)
	{
		if (m_fMoveEffectTime > 0.2f)
		{
			PARTICLE tParticle;
			Engine::Reset_Particle(tParticle);
			Engine::Set_AnimState(tParticle, 8, 8, 2.f);

			tParticle.vPos = m_pTransformCom->Get_Position();
			tParticle.fAcc = 1.f;
			tParticle.fCurrentTime = 0.f;
			tParticle.fLifeTime = 1.0f;
			tParticle.fRotateSpeed = 0.f;
			tParticle.fVelocity = 1.f;
			tParticle.vDir = _vec3(0.f, 0.5f, 0.f);
			tParticle.vStartSize = _vec3(1.f, 1.f, 1.f);
			tParticle.vEndSize = _vec3(2.f, 2.f, 2.f);
			tParticle.vStartColor = { 255.f, 255.f, 255.f, 150.f };
			tParticle.vEndColor = { 150.f, 150.f, 150.f, 0.f };

			Engine::Emit_ImageParticle(tParticle, 1, 4);
			m_fMoveEffectTime = 0.f;
		}
	}
}

void CMayCastle::Effect_Dead(const _float & fTimeDelta)
{
	if (!m_bDeadEffect)
		return;

	m_fAmount += fTimeDelta;
	if (m_fAmount >= 1.f)
	{
		m_bDeadEffect = false;
		m_bMapDead = true;
		m_bDeadUI = true;
		m_fAmount = 1.f;
	}
	else if (m_fAmount < 0.02f)
	{
		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 8, 8);
		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.vStartSize = _vec3(4.f, 4.f, 4.f);
		tParticle.vEndSize = _vec3(6.f, 6.f, 6.f);
		tParticle.fRotateSpeed = 0.f;
		tParticle.fVelocity = 3.f;
		tParticle.fLifeTime = 0.8f;
		tParticle.vDir = { 0.f, 1.f, 0.f };
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 0.5f, 0.f);

		tParticle.vStartColor = { 255.f, 255.f, 55.f, 255.f * 3.5f };
		tParticle.vEndColor = { 255.f, 255.f, 55.f, 155.f };
		Engine::Emit_ImageParticle(tParticle, 30, 4);

		tParticle.vStartColor = { 255.f, 50.f, 50.f, 255.f * 4.5f };
		tParticle.vEndColor = { 255.f, 50.f, 50.f, 255.f };
		Engine::Emit_ImageParticle(tParticle, 29, 4);
	}
	else if (m_fAmount < 0.5f)
	{
		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		Engine::Set_AnimState(tParticle, 4, 4, 0.5f);

		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.fLifeTime = 0.3f;
		tParticle.fRotateSpeed = 0.f;
		tParticle.fVelocity = 3.f;
		tParticle.vStartSize = _vec3(0.2f, 0.2f, 0.2f);
		tParticle.vEndSize = _vec3(0.1f, 0.1f, 0.1f);

		for (_int i = 0; i < 5; ++i)
		{
			tParticle.vStartColor = { 255.f, 50.f, 50.f, 255.f * 3.5f };
			tParticle.vEndColor = { 255.f, 50.f, 50.f, 255.f };
			tParticle.vStartSize = _vec3(0.2f, 0.2f, 0.2f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(0.f, 1.7f), CRandoms()(-0.3f, 0.3f));
			tParticle.vDir = _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(0.f, 1.f), CRandoms()(-0.3f, 0.3f));

			Engine::Emit_ImageParticle(tParticle, 8, 9);

			tParticle.vStartSize = _vec3(0.1f, 0.1f, 0.1f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(0.f, 1.7f), CRandoms()(-0.3f, 0.3f));
			tParticle.vDir = _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(0.f, 1.f), CRandoms()(-0.3f, 0.3f));
			Engine::Emit_ImageParticle(tParticle, 15, 11);

			tParticle.vStartColor = { 255.f, 255.f, 55.f, 255.f * 4.5f };
			tParticle.vEndColor = { 255.f, 255.f, 55.f, 155.f };
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(0.f, 1.7f), CRandoms()(-0.3f, 0.3f));
			tParticle.vDir = _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(0.f, 1.f), CRandoms()(-0.3f, 0.3f));
			Engine::Emit_ImageParticle(tParticle, 15, 11);
		}
	}
}

void CMayCastle::Effect_Respawn(const _float & fTimedelta)
{
	if (!m_bRespawnEffect)
		return;

	m_fAmount -= fTimedelta;
	if (m_fAmount <= 0.f)
	{
		m_bStencil = true;
		m_bRespawnEffect = false;
		m_fAmount = 0.f;
		m_iHp = m_iMaxHp;
	}
	else if (m_fAmount > 0.4f)
	{
		PARTICLE tParticle2;
		Engine::Reset_Particle(tParticle2);

		tParticle2.fAcc = 1.f;
		tParticle2.fCurrentTime = 0.f;
		tParticle2.fLifeTime = 0.3f;
		tParticle2.fRotateSpeed = 0.f;
		tParticle2.fVelocity = 3.f;
		tParticle2.vStartSize = _vec3(0.1f, 0.1f, 0.1f);
		tParticle2.vEndSize = _vec3(0.05f, 0.05f, 0.05f);

		for (_int i = 0; i < 20; ++i)
		{
			tParticle2.vStartColor = { 255.f, 50.f, 50.f, 255.f * 4.5f };
			tParticle2.vEndColor = { 255.f, 50.f, 50.f, 255.f };
			tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-1.f, 1.f), CRandoms()(-0.8f, 2.5f), CRandoms()(-1.f, 1.f));
			_vec3 vDir = (m_pTransformCom->Get_Position() + _vec3(0.f, CRandoms()(0.f, 2.f), 0.f)) - tParticle2.vPos;
			D3DXVec3Normalize(&vDir, &vDir);
			tParticle2.vDir = vDir;
			Engine::Emit_ImageParticle(tParticle2, 15, 11);

			tParticle2.vStartColor = { 255.f, 255.f, 55.f, 255.f * 3.5f };
			tParticle2.vEndColor = { 255.f, 255.f, 55.f, 155.f };
			tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-1.f, 1.f), CRandoms()(-0.8f, 2.5f), CRandoms()(-1.f, 1.f));
			vDir = (m_pTransformCom->Get_Position() + _vec3(0.f, CRandoms()(0.f, 2.f), 0.f)) - tParticle2.vPos;
			D3DXVec3Normalize(&vDir, &vDir);
			tParticle2.vDir = vDir;
			Engine::Emit_ImageParticle(tParticle2, 15, 11);
		}
	}


	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);
	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vStartSize = _vec3(2.5f, 2.5f, 2.5f);
	tParticle.vEndSize = _vec3(5.f, 5.f, 5.f);
	tParticle.fRotateSpeed = 8.f;
	tParticle.fVelocity = 4.f;
	tParticle.fLifeTime = 1.5f;
	tParticle.vDir = { 0.f, -1.f, 0.f };
	tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 1.7f, 0.f);

	//tParticle.bRotAdd = true;
	tParticle.bBill = false;
	tParticle.vRot = { D3DXToRadian(90.f), 0.f,  0.f };

	tParticle.vStartColor = { 255.f, 50.f, 50.f, 255.f * 3.5f };
	tParticle.vEndColor = { 255.f, 50.f, 50.f, 255.f };
	Engine::Emit_ImageParticle(tParticle, 45, 24);

	//tParticle.vStartSize = _vec3(0.5f, 0.5f, 0.5f);
	tParticle.vStartColor = { 255.f, 255.f, 55.f, 255.f  * 3.5f };
	tParticle.vEndColor = { 255.f, 255.f, 55.f, 155.f };
	Engine::Emit_ImageParticle(tParticle, 45, 24);
}

void CMayCastle::Create_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, );

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_STATIC;

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pProfile = pGameObject->Is<CImageUI>();
	m_pProfile->Set_Image(L"MC_ImageUI");
	m_pProfile->Set_Size(_vec3{ 128.f, 128.f, 0.f });
	m_pProfile->Set_Pos(_vec3{ 60.f, -880.f, 0.f });
	m_pProfile->Set_RenderID(RENDER_MENUUI);
	m_pProfile->Set_PassIndex(2);
	m_pProfile->Set_TextureIndex(0);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Profile", pGameObject), );
	m_pProfile->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pHp = pGameObject->Is<CImageUI>();
	m_pHp->Set_Image(L"MC_ImageUI");
	m_pHp->Set_Size(_vec3{ 160.f, 160.f, 0.f });
	m_pHp->Set_Pos(_vec3{ 44.f, -864.f, 0.f });
	m_pHp->Set_RenderID(RENDER_UI);
	m_pHp->Set_PassIndex(14);
	m_pHp->Set_TextureIndex(1);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Hp", pGameObject), );
	m_pHp->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pChargeframe = pGameObject->Is<CImageUI>();
	m_pChargeframe->Set_Image(L"MC_ImageUI");
	m_pChargeframe->Set_Size(_vec3{ 270.f, 38.f, 0.f });
	m_pChargeframe->Set_Pos(_vec3{ 200.f, -940.f, 0.f });
	m_pChargeframe->Set_RenderID(RENDER_UI);
	m_pChargeframe->Set_PassIndex(8);
	m_pChargeframe->Set_TextureIndex(2);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Chargeframe", pGameObject), );
	m_pChargeframe->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pCharge = pGameObject->Is<CImageUI>();
	m_pCharge->Set_Image(L"MC_ImageUI");
	m_pCharge->Set_Size(_vec3{ 270.f, 38.f, 0.f });
	m_pCharge->Set_Pos(_vec3{ 200.f, -940.f, 0.f });
	m_pCharge->Set_RenderID(RENDER_UI);
	m_pCharge->Set_PassIndex(9);
	m_pCharge->Set_TextureIndex(2);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Charge", pGameObject), );
	m_pCharge->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkill[0] = pGameObject->Is<CImageUI>();
	m_pSkill[0]->Set_Image(L"MC_ImageUI");
	m_pSkill[0]->Set_Size(_vec3{ 55.f, 55.f, 0.f });
	m_pSkill[0]->Set_Pos(_vec3{ 208.f, -877.f, 0.f });
	m_pSkill[0]->Set_RenderID(RENDER_PRIUI);
	m_pSkill[0]->Set_PassIndex(2);
	m_pSkill[0]->Set_TextureIndex(3);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Skill0", pGameObject), );
	m_pSkill[0]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkill[1] = pGameObject->Is<CImageUI>();
	m_pSkill[1]->Set_Image(L"MC_ImageUI");
	m_pSkill[1]->Set_Size(_vec3{ 55.f, 55.f, 0.f });
	m_pSkill[1]->Set_Pos(_vec3{ 278.f, -877.f, 0.f });
	m_pSkill[1]->Set_RenderID(RENDER_PRIUI);
	m_pSkill[1]->Set_PassIndex(2);
	m_pSkill[1]->Set_TextureIndex(4);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Skill1", pGameObject), );
	m_pSkill[1]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkill[2] = pGameObject->Is<CImageUI>();
	m_pSkill[2]->Set_Image(L"MC_ImageUI");
	m_pSkill[2]->Set_Size(_vec3{ 55.f, 55.f, 0.f });
	m_pSkill[2]->Set_Pos(_vec3{ 348.f, -877.f, 0.f });
	m_pSkill[2]->Set_RenderID(RENDER_PRIUI);
	m_pSkill[2]->Set_PassIndex(2);
	m_pSkill[2]->Set_TextureIndex(5);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Skill2", pGameObject), );
	m_pSkill[2]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkill[3] = pGameObject->Is<CImageUI>();
	m_pSkill[3]->Set_Image(L"MC_ImageUI");
	m_pSkill[3]->Set_Size(_vec3{ 60.f, 60.f, 0.f });
	m_pSkill[3]->Set_Pos(_vec3{ 485.f, -927.f, 0.f });
	m_pSkill[3]->Set_RenderID(RENDER_PRIUI);
	m_pSkill[3]->Set_PassIndex(2);
	m_pSkill[3]->Set_TextureIndex(6);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Skill3", pGameObject), );
	m_pSkill[3]->AddRef();

	//////////////////////////////////////////////////////////////////////////

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkillBox[0] = pGameObject->Is<CImageUI>();
	m_pSkillBox[0]->Set_Image(L"MC_ImageUI");
	m_pSkillBox[0]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pSkillBox[0]->Set_Pos(_vec3{ 200.f, -870.f, 0.f });
	m_pSkillBox[0]->Set_RenderID(RENDER_MENUUI);
	m_pSkillBox[0]->Set_PassIndex(6);
	m_pSkillBox[0]->Set_TextureIndex(8);
	m_pSkillBox[0]->Set_Amount(1.f);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_SkillBox0", pGameObject), );
	m_pSkillBox[0]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkillBox[1] = pGameObject->Is<CImageUI>();
	m_pSkillBox[1]->Set_Image(L"MC_ImageUI");
	m_pSkillBox[1]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pSkillBox[1]->Set_Pos(_vec3{ 270.f, -870.f, 0.f });
	m_pSkillBox[1]->Set_RenderID(RENDER_MENUUI);
	m_pSkillBox[1]->Set_PassIndex(6);
	m_pSkillBox[1]->Set_TextureIndex(8);
	m_pSkillBox[1]->Set_Amount(1.f);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_SkillBox1", pGameObject), );
	m_pSkillBox[1]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkillBox[2] = pGameObject->Is<CImageUI>();
	m_pSkillBox[2]->Set_Image(L"MC_ImageUI");
	m_pSkillBox[2]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pSkillBox[2]->Set_Pos(_vec3{ 340.f, -870.f, 0.f });
	m_pSkillBox[2]->Set_RenderID(RENDER_MENUUI);
	m_pSkillBox[2]->Set_PassIndex(6);
	m_pSkillBox[2]->Set_TextureIndex(8);
	m_pSkillBox[2]->Set_Amount(1.f);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_SkillBox2", pGameObject), );
	m_pSkillBox[2]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkillBox[3] = pGameObject->Is<CImageUI>();
	m_pSkillBox[3]->Set_Image(L"MC_ImageUI");
	m_pSkillBox[3]->Set_Size(_vec3{ 80.f, 80.f, 0.f });
	m_pSkillBox[3]->Set_Pos(_vec3{ 475.f, -920.f, 0.f });
	m_pSkillBox[3]->Set_RenderID(RENDER_MENUUI);
	m_pSkillBox[3]->Set_PassIndex(6);
	m_pSkillBox[3]->Set_TextureIndex(8);
	m_pSkillBox[3]->Set_Amount(1.f);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_SkillBox3", pGameObject), );
	m_pSkillBox[3]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkillBox[4] = pGameObject->Is<CImageUI>();
	m_pSkillBox[4]->Set_Image(L"MC_ImageUI");
	m_pSkillBox[4]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pSkillBox[4]->Set_Pos(_vec3{ 200.f, -870.f, 0.f });
	m_pSkillBox[4]->Set_RenderID(RENDER_UI);
	m_pSkillBox[4]->Set_PassIndex(7);
	m_pSkillBox[4]->Set_TextureIndex(8);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_SkillBox4", pGameObject), );
	m_pSkillBox[4]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkillBox[5] = pGameObject->Is<CImageUI>();
	m_pSkillBox[5]->Set_Image(L"MC_ImageUI");
	m_pSkillBox[5]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pSkillBox[5]->Set_Pos(_vec3{ 270.f, -870.f, 0.f });
	m_pSkillBox[5]->Set_RenderID(RENDER_UI);
	m_pSkillBox[5]->Set_PassIndex(7);
	m_pSkillBox[5]->Set_TextureIndex(8);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_SkillBox5", pGameObject), );
	m_pSkillBox[5]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkillBox[6] = pGameObject->Is<CImageUI>();
	m_pSkillBox[6]->Set_Image(L"MC_ImageUI");
	m_pSkillBox[6]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pSkillBox[6]->Set_Pos(_vec3{ 340.f, -870.f, 0.f });
	m_pSkillBox[6]->Set_RenderID(RENDER_UI);
	m_pSkillBox[6]->Set_PassIndex(7);
	m_pSkillBox[6]->Set_TextureIndex(8);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_SkillBox6", pGameObject), );
	m_pSkillBox[6]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkillBox[7] = pGameObject->Is<CImageUI>();
	m_pSkillBox[7]->Set_Image(L"MC_ImageUI");
	m_pSkillBox[7]->Set_Size(_vec3{ 80.f, 80.f, 0.f });
	m_pSkillBox[7]->Set_Pos(_vec3{ 475.f, -920.f, 0.f });
	m_pSkillBox[7]->Set_RenderID(RENDER_UI);
	m_pSkillBox[7]->Set_PassIndex(7);
	m_pSkillBox[7]->Set_TextureIndex(8);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_SkillBox7", pGameObject), );
	m_pSkillBox[7]->AddRef();

	//////////////////////////////////////////////////////////////////////////

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pKey[0] = pGameObject->Is<CImageUI>();
	m_pKey[0]->Set_Image(L"MC_ImageUI");
	m_pKey[0]->Set_Size(_vec3{ 40.f, 40.f, 0.f });
	m_pKey[0]->Set_Pos(_vec3{ 215.f, -815.f, 0.f });
	m_pKey[0]->Set_RenderID(RENDER_MENUUI);
	m_pKey[0]->Set_PassIndex(2);
	m_pKey[0]->Set_TextureIndex(9);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Key0", pGameObject), );
	m_pKey[0]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pKey[1] = pGameObject->Is<CImageUI>();
	m_pKey[1]->Set_Image(L"MC_ImageUI");
	m_pKey[1]->Set_Size(_vec3{ 40.f, 40.f, 0.f });
	m_pKey[1]->Set_Pos(_vec3{ 285.f, -815.f, 0.f });
	m_pKey[1]->Set_RenderID(RENDER_MENUUI);
	m_pKey[1]->Set_PassIndex(2);
	m_pKey[1]->Set_TextureIndex(10);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Key1", pGameObject), );
	m_pKey[1]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pKey[2] = pGameObject->Is<CImageUI>();
	m_pKey[2]->Set_Image(L"MC_ImageUI");
	m_pKey[2]->Set_Size(_vec3{ 40.f, 40.f, 0.f });
	m_pKey[2]->Set_Pos(_vec3{ 355.f, -815.f, 0.f });
	m_pKey[2]->Set_RenderID(RENDER_MENUUI);
	m_pKey[2]->Set_PassIndex(2);
	m_pKey[2]->Set_TextureIndex(11);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Key2", pGameObject), );
	m_pKey[2]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pKey[3] = pGameObject->Is<CImageUI>();
	m_pKey[3]->Set_Image(L"MC_ImageUI");
	m_pKey[3]->Set_Size(_vec3{ 40.f, 40.f, 0.f });
	m_pKey[3]->Set_Pos(_vec3{ 495.f, -865.f, 0.f });
	m_pKey[3]->Set_RenderID(RENDER_MENUUI);
	m_pKey[3]->Set_PassIndex(2);
	m_pKey[3]->Set_TextureIndex(12);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Key3", pGameObject), );
	m_pKey[3]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pKey[4] = pGameObject->Is<CImageUI>();
	m_pKey[4]->Set_Image(L"MC_ImageUI");
	m_pKey[4]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pKey[4]->Set_Pos(_vec3{ 200.f, -800.f, 0.f });
	m_pKey[4]->Set_RenderID(RENDER_UI);
	m_pKey[4]->Set_PassIndex(2);
	m_pKey[4]->Set_TextureIndex(13);
	m_pKey[4]->Set_Color(_vec4(255.f, 213.f, 64.f, 255.f));
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Key4", pGameObject), );
	m_pKey[4]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pKey[5] = pGameObject->Is<CImageUI>();
	m_pKey[5]->Set_Image(L"MC_ImageUI");
	m_pKey[5]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pKey[5]->Set_Pos(_vec3{ 270.f, -800.f, 0.f });
	m_pKey[5]->Set_RenderID(RENDER_UI);
	m_pKey[5]->Set_PassIndex(2);
	m_pKey[5]->Set_TextureIndex(13);
	m_pKey[5]->Set_Color(_vec4(255.f, 213.f, 64.f, 255.f));
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Key5", pGameObject), );
	m_pKey[5]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pKey[6] = pGameObject->Is<CImageUI>();
	m_pKey[6]->Set_Image(L"MC_ImageUI");
	m_pKey[6]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pKey[6]->Set_Pos(_vec3{ 340.f, -800.f, 0.f });
	m_pKey[6]->Set_RenderID(RENDER_UI);
	m_pKey[6]->Set_PassIndex(2);
	m_pKey[6]->Set_TextureIndex(13);
	m_pKey[6]->Set_Color(_vec4(255.f, 213.f, 64.f, 255.f));
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Key6", pGameObject), );
	m_pKey[6]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pKey[7] = pGameObject->Is<CImageUI>();
	m_pKey[7]->Set_Image(L"MC_ImageUI");
	m_pKey[7]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pKey[7]->Set_Pos(_vec3{ 480.f, -850.f, 0.f });
	m_pKey[7]->Set_RenderID(RENDER_UI);
	m_pKey[7]->Set_PassIndex(2);
	m_pKey[7]->Set_TextureIndex(13);
	m_pKey[7]->Set_Color(_vec4(255.f, 213.f, 64.f, 255.f));
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Key7", pGameObject), );
	m_pKey[7]->AddRef();

	//////////////////////////////////////////////////////////////////////////

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[0] = pGameObject->Is<CImageUI>();
	m_pDeadUI[0]->Set_Image(L"MC_ImageUI");
	m_pDeadUI[0]->Set_Size(_vec3{ 400.f, 400.f, 0.f });
	m_pDeadUI[0]->Set_Pos(_vec3{ 70.f, -540.f, 0.f });
	m_pDeadUI[0]->Set_RenderID(RENDER_MENUUI);
	m_pDeadUI[0]->Set_PassIndex(11);
	m_pDeadUI[0]->Set_TextureIndex(14);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Dead", pGameObject), );
	m_pDeadUI[0]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[1] = pGameObject->Is<CImageUI>();
	m_pDeadUI[1]->Set_Image(L"MC_ImageUI");
	m_pDeadUI[1]->Set_Size(_vec3{ 400.f, 400.f, 0.f });
	m_pDeadUI[1]->Set_Pos(_vec3{ 70.f, -540.f, 0.f });
	m_pDeadUI[1]->Set_RenderID(RENDER_UI);
	m_pDeadUI[1]->Set_PassIndex(12);
	m_pDeadUI[1]->Set_TextureIndex(14);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Dead2", pGameObject), );
	m_pDeadUI[1]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[2] = pGameObject->Is<CImageUI>();
	m_pDeadUI[2]->Set_Image(L"MC_ImageUI");
	m_pDeadUI[2]->Set_Size(_vec3{ 400.f, 400.f, 0.f });
	m_pDeadUI[2]->Set_Pos(_vec3{ 70.f, -540.f, 0.f });
	m_pDeadUI[2]->Set_RenderID(RENDER_UI);
	m_pDeadUI[2]->Set_PassIndex(13);
	m_pDeadUI[2]->Set_TextureIndex(14);
	m_pDeadUI[2]->Set_Color(_vec4(231.f, 76.f, 60.f, 255.f));
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Dead3", pGameObject), );
	m_pDeadUI[2]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[3] = pGameObject->Is<CImageUI>();
	m_pDeadUI[3]->Set_Image(L"MC_ImageUI");
	m_pDeadUI[3]->Set_Size(_vec3{ 60.f, 60.f, 0.f });
	m_pDeadUI[3]->Set_Pos(_vec3{ 240.f, -860.f, 0.f });
	m_pDeadUI[3]->Set_RenderID(RENDER_MENUUI);
	m_pDeadUI[3]->Set_PassIndex(2);
	m_pDeadUI[3]->Set_TextureIndex(13);
	m_pDeadUI[3]->Set_Color(236.f, 216.f, 162.f, 255.f);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Dead4", pGameObject), );
	m_pDeadUI[3]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[4] = pGameObject->Is<CImageUI>();
	m_pDeadUI[4]->Set_Image(L"MC_ImageUI");
	m_pDeadUI[4]->Set_Size(_vec3{ 40.f, 40.f, 0.f });
	m_pDeadUI[4]->Set_Pos(_vec3{ 250.f, -870.f, 0.f });
	m_pDeadUI[4]->Set_RenderID(RENDER_MENUUI);
	m_pDeadUI[4]->Set_PassIndex(2);
	m_pDeadUI[4]->Set_TextureIndex(12);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Dead5", pGameObject), );
	m_pDeadUI[4]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[5] = pGameObject->Is<CImageUI>();
	m_pDeadUI[5]->Set_Image(L"MC_ImageUI");
	m_pDeadUI[5]->Set_Size(_vec3{ 128.f, 128.f, 0.f });
	m_pDeadUI[5]->Set_Pos(_vec3{ 206.f, -830.f, 0.f });
	m_pDeadUI[5]->Set_RenderID(RENDER_MENUUI);
	m_pDeadUI[5]->Set_PassIndex(2);
	m_pDeadUI[5]->Set_TextureIndex(15);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MC_Dead6", pGameObject), );
	m_pDeadUI[5]->AddRef();

	for (_int i = 0; i < 6; ++i)
	{
		m_pDeadUI[i]->Set_Visible(false);
		m_pDeadUI[i]->Change_Size_From_To(0.1f, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f));
	}

	Engine::Add_Layer(pLayerTag, pLayer);
}

void CMayCastle::Update_UI(const _float & fTimeDelta)
{
	HP_UI(fTimeDelta);
	Skill_UI(fTimeDelta);
	Dead_UI(fTimeDelta);
}

void CMayCastle::HP_UI(const _float & fTimeDelta)
{
	m_pHp->Set_Amount(_float(m_iHp) / m_iMaxHp);
	m_pCharge->Set_Amount(_float(m_iHitCount) / m_iHitCountMax);
}

void CMayCastle::Skill_UI(const _float & fTimeDelta)
{
	if (Engine::Key_Down(DIK_5))
	{
		for (_int i = 0; i < 4; ++i)
			m_pSkillBox[i]->Set_Amount(0.f);
	}

	for (_int i = 0; i < 4; ++i)
	{
		if (m_pSkillBox[i]->Get_Amount() >= 1.f)
		{
			m_pKey[i]->Set_Color(_vec4(255.f, 255.f, 255.f, 255.f));
			m_pKey[i + 4]->Set_Color(_vec4(255.f, 213.f, 64.f, 255.f));
		}
		else
		{
			m_pKey[i]->Set_Color(_vec4(155.f, 155.f, 155.f, 255.f));
			m_pKey[i + 4]->Set_Color(_vec4(155.f, 155.f, 155.f, 255.f));
		}
	}
}

void CMayCastle::Dead_UI(const _float & fTimeDelta)
{
	if (!m_bDeadUI)
		return;

	if (Engine::Key_Pressing(DIC_Y) && m_pDeadUI[0]->Get_Amount() < 1.f)
	{
		m_pDeadUI[2]->Change_Color_From_To(0.5f, _vec4(255.f, 255.f, 255.f, 255), _vec4(231.f, 76.f, 60.f, 255.f));
		m_pDeadUI[2]->Change_Size_From_To(0.15f, _vec3(500.f, 500.f, 0.f), _vec3(400.f, 400.f, 0.f));
		m_pDeadUI[3]->Change_Size_From_To(0.15f, _vec3(30.f, 30.f, 0.f), _vec3(60.f, 60.f, 0.f));
		m_pDeadUI[4]->Change_Size_From_To(0.15f, _vec3(20.f, 20.f, 0.f), _vec3(40.f, 40.f, 0.f));

		m_pDeadUI[0]->Set_AddAmount(0.01f);
	}

	if (m_bDeadSign)
	{
		m_fDeadSign += 32.f * fTimeDelta * 5.f;
		if (m_fDeadSign >= 128.f)
			m_bDeadSign = false;
	}
	else
	{
		m_fDeadSign -= 32.f * fTimeDelta * 5.f;
		if (m_fDeadSign <= 96.f)
			m_bDeadSign = true;
	}
	m_pDeadUI[5]->Set_Size(_vec3(m_fDeadSign, m_fDeadSign, 0.f));

	if (m_pDeadUI[0]->Get_Amount() >= 1.f)
	{
		if (m_bMapDead)
		{
			for (_int i = 0; i < 6; ++i)
				m_pDeadUI[i]->Change_Color_From_To(0.2f, _vec4(255.f, 255.f, 255.f, 255.f), _vec4(255.f, 255.f, 255.f, 0.f));

			for (_int i = 0; i < 3; ++i)
				m_pDeadUI[i]->Change_Size_From_To(0.2f, _vec3(400.f, 400.f, 0.f), _vec3(0.f, 0.f, 0.f));
			m_pDeadUI[3]->Change_Size_From_To(0.2f, _vec3(60.f, 60.f, 0.f), _vec3(0.f, 0.f, 0.f));
			m_pDeadUI[4]->Change_Size_From_To(0.2f, _vec3(40.f, 40.f, 0.f), _vec3(0.f, 0.f, 0.f));
			m_pDeadUI[5]->Change_Size_From_To(0.2f, _vec3(128.f, 128.f, 0.f), _vec3(0.f, 0.f, 0.f));

			m_bMapDead = false;
		}

		m_bDeadStart = false;
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_DEAD;
		m_bRespawnEffect = true;

		auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);
		PxTransform Transform = pBody->getGlobalPose();
		Transform.p.x = m_vSavePoint.x;
		Transform.p.y = m_vSavePoint.y + 0.5f;
		Transform.p.z = m_vSavePoint.z;

		pBody->setGlobalPose(Transform);
		m_bDeadUI = false;

		Clear(PL_DEAD);

		for (_int i = 0; i < 6; ++i)
			m_pDeadUI[i]->Set_Visible(false);

		m_pProfile->Set_Visible(true);
		m_pHp->Set_Visible(true);
		m_pCharge->Set_Visible(true);
		m_pChargeframe->Set_Visible(true);

		for (_int i = 0; i < 4; ++i)
			m_pSkill[i]->Set_Visible(true);

		for (_int i = 0; i < 8; ++i)
		{
			m_pSkillBox[i]->Set_Visible(true);
			m_pKey[i]->Set_Visible(true);
		}
	}
}

void CMayCastle::KeyEvent_UI(CImageUI* pUI)
{
	if (pUI == nullptr)
		return;

	pUI->Change_Color_From_To(1.f, _vec4(255.f, 213.f, 64.f, 255.f), _vec4(255.f, 255.f, 255.f, 255.f));
}

CMayCastle* CMayCastle::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CMayCastle*	pInstance = new CMayCastle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CMayCastle::Free()
{
	Safe_Release(m_pSword);
	Safe_Release(m_pProfile);
	Safe_Release(m_pHp);
	Safe_Release(m_pCharge);
	Safe_Release(m_pChargeframe);
	for (_int i = 0; i < 4; ++i)
		Safe_Release(m_pSkill[i]);

	for (_int i = 0; i < 8; ++i)
	{
		Safe_Release(m_pSkillBox[i]);
		Safe_Release(m_pKey[i]);
	}

	for (_int i = 0; i < 6; ++i)
		Safe_Release(m_pDeadUI[i]);

	CMayCastle_Anim::DestroyInstance();
	CPlayer::Free();
}

HRESULT CMayCastle::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	// CDynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_PLAYER, L"May_Castle"));
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
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_fColDis, _vec4(100.f, 255.f, 255.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_VTF"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Dissolve"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	//m_pTransformCom->Set_Pos(5.0f, 1.0f, 0.0f);

	m_pTransformCom->Set_Pos(tTemp.vPos);
	//m_pTransformCom->Set_Pos(0.f, 1.f, 0.f);

	m_vSavePoint = tTemp.vPos;
	//PhysX
	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();
	PxTransform tTransform(vPos.x, vPos.y, vPos.z);

	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxShape* pShape = pPhysics->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *pMaterial, true);
	//밑 두개중 하나는 false 둘다 true안?
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);

	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pShape, 1);

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	// 트랜스폼은 가지고있지만 움직이지 않는애들
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);

	// 그래비티를 끈다.
	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);

	// 질량설정
	pBody->setMass(5);
	pBody->setName((char*)this);
	pShape->setName((char*)this);
	pBody->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
	//이동 제동력
	pBody->setLinearDamping(0.05f);

	return S_OK;
}

void CMayCastle::Key_Input(const _float& fTimeDelta)
{
	//if (Engine::Key_Pressing(DIK_NUMPAD8))
	//	m_iKeyState |= MOVE_UP;

	//if (Engine::Key_Pressing(DIK_NUMPAD5))
	//	m_iKeyState |= MOVE_DOWN;

	//if (Engine::Key_Pressing(DIK_NUMPAD6))
	//	m_iKeyState |= MOVE_RIGHT;

	//if (Engine::Key_Pressing(DIK_NUMPAD4))
	//	m_iKeyState |= MOVE_LEFT;

	if (Engine::Key_Pressing(DIK_L))
		m_vSavePoint = m_pTransformCom->Get_Position();

	_float fDIC_Y = 0;
	_float fCheck = 0.0f;
	fDIC_Y = Engine::Get_DIPadMove(PADMOVESTATE::DIC_LS_Y);
	if (fabsf(fDIC_Y) <= 0.1f) {}
	else
	{
		if (fDIC_Y > 0)
			m_iKeyState |= MOVE_UP;
		else if (fDIC_Y < 0)
			m_iKeyState |= MOVE_DOWN;
	}

	_float fDIC_X = 0;
	fDIC_X = Engine::Get_DIPadMove(PADMOVESTATE::DIC_LS_X);
	if (fabs(fDIC_X) <= 0.1f) {}
	else
	{
		if (fDIC_X > 0)
			m_iKeyState |= MOVE_RIGHT;
		else if (fDIC_X < 0)
			m_iKeyState |= MOVE_LEFT;
	}

	// 패드로 누른 방향
	m_vMoveDir = _float3(fDIC_X, 0.0f, fDIC_Y);
	D3DXVec3Normalize(&m_vMoveDir, &m_vMoveDir);

	// 이동 방향을 구하기위한 각도 구하기
	{
		_float fCosTh = D3DXVec3Dot(&m_vMoveDir, &_float3(0.0f, 0.0f, 1.0f));
		_float fRad = acosf(fCosTh);
		if (fDIC_X <= 0)
			fRad = 3.14f * 2 - fRad;

		m_fDirAngle = D3DXToDegree(fRad);
	}

	if (Engine::Key_Down(DIC_A))
	{
		m_iKeyState |= ATT;
		m_eAtt_State = ATT_BASIC;
		KeyEvent_UI(m_pSkillBox[0]);
	}

	if (Engine::Key_Down(DIC_B) && m_fSmashCoolTime <= 0.f)
	{
		m_iKeyState |= ATT;
		m_eAtt_State = ATT_SMASH;
		KeyEvent_UI(m_pSkillBox[2]);
		//m_iSkillEffect = 1;
	}

	if (Engine::Key_Pressing(DIC_Y) && m_fUltiAttTime > 0.f)
	{
		m_iKeyState |= (ATT | ULTIMATE);
		m_eAtt_State = ATT_ULTIMATE;
		KeyEvent_UI(m_pSkillBox[3]);
		m_bUltEffect = true;
	}

	if (Engine::Key_Down(DIC_X) && m_fDashCoolTime <= 0.f)
	{
		KeyEvent_UI(m_pSkillBox[1]);
		m_bDashEffect = true;
		m_iKeyState |= DASH;
	}
}

HRESULT CMayCastle::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int& iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	if (m_bStencil)
		pEffect->SetVector("g_vColor", &_vec4(75.f * 3.5f, 155.f * 3.5f, 255.f * 3.5f, 255.f));
	else
		pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));

	if (m_bRespawnEffect)
	{
		if (m_fAmount >= 3.14f / 2.f)
		{
			m_bDead = true;
			m_fAmount = 3.14f / 2.f;
		}

		_float fSin = sinf(m_fAmount);
		pEffect->SetFloat("g_fAmount", fSin);
	}
	else
	{
		pEffect->SetFloat("g_fAmount", 0.f);
	}

	if (m_bDeadEffect)
	{
		if (m_fAmount >= 3.14f / 2.f)
		{
			m_bDead = true;
			m_fAmount = 3.14f / 2.f;
		}

		_float fSin = sinf(m_fAmount);
		pEffect->SetFloat("g_fAmount", fSin);
	}

	m_pTextureCom->Set_Texture(pEffect, "g_DissolveTexture", 0);
	m_pTextureCom->Set_Texture(pEffect, "g_BurnTexture", 3);
	pEffect->SetFloat("g_fMaterialIndex", 1.f);

	return S_OK;
}

void CMayCastle::Set_State(const _float & fTimeDelta)
{
	switch (m_ePL_STATE)
	{
	case CMayCastle::PL_IDLE:
		Idle_State(fTimeDelta);
		break;
	case CMayCastle::PL_MOVE:
		Move_State(fTimeDelta);
		break;
	case CMayCastle::PL_ATT:
		Att_State(fTimeDelta);
		break;
	case CMayCastle::PL_DASH:
		Dash_State(fTimeDelta);
		break;
	case CMayCastle::PL_HIT:
		Hit_State(fTimeDelta);
		break;
	case CMayCastle::PL_DEAD:
		Dead_State(fTimeDelta);
		break;
	}
}

void CMayCastle::Move(const _float & fTimeDelta, _float3 * vResultPos, _float3 * vResultDir, _float fRotSpeed)
{
	if (m_bDead)
	{
		m_ePL_STATE = PL_DEAD;
		m_bDead = false;
		return;
	}

	_float3 vPos, vDir;
	_float fCosTh;
	_int iRotDir;

	vPos = m_pTransformCom->Get_Position();
	_float fAngle = fRotSpeed;

	Set_Dir(&vDir, &iRotDir, &fCosTh);

	fAngle *= iRotDir;

	_float fRatio = 1.0f;
	if (fCosTh > 0.75f)
		fRatio = -1.0f / 0.2f * (fCosTh - 0.8f) + 1.0f;

	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fAngle * fTimeDelta * fRatio));

	*vResultPos = vPos;
	*vResultDir = vDir;
}

void CMayCastle::Set_SmashCoolTime()
{
	m_fSmashCoolTime = m_fSmashCool;
	m_pSkillBox[2]->Set_CoolTime(m_fSmashCool);
}

void CMayCastle::Set_DashCoolTime()
{
	m_fDashCoolTime = m_fDashCool;
	m_pSkillBox[1]->Set_CoolTime(m_fDashCool);
}

void CMayCastle::SmashAtt()
{
	vector<CEnermy*>*		vecEnermyList;
	vecEnermyList = CCollisionMgr::GetInstance()->Get_EnermyList();

	_vec3	vPos;
	_vec3	vDir;
	_float	fDir;

	m_iSmashDamage = CRandoms()(5, 10);

	vPos = m_pTransformCom->Get_Position();
	for (_uint i = 0; i < vecEnermyList->size(); ++i)
	{
		_vec3 vEnermyPos;
		vEnermyPos = (*vecEnermyList)[i]->Get_Component(L"Com_Transform", ID_DYNAMIC)->Is<CTransform>()->Get_Position();
		vDir = vPos - vEnermyPos;
		fDir = D3DXVec3Length(&vDir);

		if (fDir < m_fSmashRadius)
		{
			(*vecEnermyList)[i]->Set_Damage(-m_iSmashDamage);

			CPlayer* pMay = (CPlayer*)Engine::Get_GameObject(L"GameObject", L"May");
			pMay->Add_UltiStack(3);
		}
	}
}

void CMayCastle::WindAtt()
{
	vector<CEnermy*>*		vecEnermyList;
	vecEnermyList = CCollisionMgr::GetInstance()->Get_EnermyList();

	_vec3	vPos;
	_vec3	vDir;
	_float	fDir;



	vPos = m_pTransformCom->Get_Position();
	if (m_fWindDamageTime >= 0.5f)
	{
		for (_uint i = 0; i < vecEnermyList->size(); ++i)
		{
			_vec3 vEnermyPos;
			vEnermyPos = (*vecEnermyList)[i]->Get_Component(L"Com_Transform", ID_DYNAMIC)->Is<CTransform>()->Get_Position();
			vDir = vPos - vEnermyPos;
			fDir = D3DXVec3Length(&vDir);

			if (fDir < m_fSmashRadius)
			{
				m_iWindDamage = CRandoms()(5, 10);
			
				(*vecEnermyList)[i]->Set_Damage(-m_iWindDamage);

			}
		}
		m_fWindDamageTime = 0.f;
	}
}

void CMayCastle::CoolTime(const _float & fTimeDelta)
{

	m_fDashCoolTime -= fTimeDelta;
	if (m_fDashCoolTime < 0.f)
	{
		m_fDashCoolTime = 0.f;
	}

	m_fSmashCoolTime -= fTimeDelta;
	if (m_fSmashCoolTime < 0.f)
	{
		m_fSmashCoolTime = 0.f;
	}
}

void CMayCastle::Idle_State(const _float & fTimeDelta)
{
	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);
	pBody->setLinearVelocity(PxVec3(0, 0, 0));

	if (true == m_bDead)
	{
		m_ePL_STATE = PL_DEAD;
		Clear(PL_DEAD);
		return;
	}
	else if (m_bHit)
	{
		m_ePL_STATE = PL_HIT;
		m_ePre_State = PL_IDLE;
		m_bHit = false;
	}
	else if (m_iKeyState & DASH)
	{
		m_ePL_STATE = PL_DASH;
		m_ePre_State = PL_IDLE;
	}
	else if (m_iKeyState & ATT)
	{
		m_ePL_STATE = PL_ATT;
		m_ePre_State = PL_IDLE;
	}
	else if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
	{
		m_ePL_STATE = PL_MOVE;
		m_ePre_State = PL_IDLE;
	}
}

void CMayCastle::Move_State(const _float & fTimeDelta)
{
	if (true == m_bDead)
	{
		m_ePL_STATE = PL_DEAD;
		Clear(PL_DEAD);
		return;
	}
	else if (m_bHit)
	{
		m_ePL_STATE = PL_HIT;
		m_ePre_State = PL_MOVE;
		m_bHit = false;
	}
	else if (m_iKeyState & DASH)
	{
		m_ePL_STATE = PL_DASH;
		m_ePre_State = PL_MOVE;
	}
	else if (m_iKeyState & ATT)
	{
		m_ePL_STATE = PL_ATT;
		m_ePre_State = PL_MOVE;
	}
	else if (!(m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT)))
	{
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_MOVE;
		auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);
		pBody->setLinearVelocity(PxVec3(0, 0, 0));
	}
	else
	{
		auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

		_float3 vPos, vDir;

		Move(fTimeDelta, &vPos, &vDir);

		m_fCurMoveTime += fTimeDelta;
		if (m_fCurMoveTime < m_fGoalTime)
			m_fSpeed = m_fMaxSpeed * m_fCurMoveTime * (1.0f / m_fGoalTime);
		else
			m_fSpeed = m_fMaxSpeed;

		_vec3 vDstPos = vDir * m_fSpeed;
		pBody->setLinearVelocity(PxVec3(0, 0, 0));
		pBody->addForce(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), PxForceMode::eIMPULSE);
	}
}

void CMayCastle::Att_State(const _float & fTimeDelta)
{
	if (true == m_bDead)
	{
		m_ePL_STATE = PL_DEAD;
		Clear_Dead();
		return;
	}
	else if (m_bHit)
	{
		m_bHit = false;
		if (!(ATT_ULTIMATE == m_eAtt_State))
		{
			m_ePL_STATE = PL_HIT;
			m_ePre_State = PL_ATT;
		}
	}
	else if (m_bAttEnd)
	{
		if (m_bUltEffect)
			m_bUltEffect = false;

		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_ATT;
		Clear(PL_ATT);
		return;
	}

	// 무기의 공격 가능상태
	m_pSword->Set_AttAble(m_bAttAble);

	switch (m_eAtt_State)
	{
	case CMayCastle::ATT_BASIC:
		BasicAtt(fTimeDelta);
		break;
	case CMayCastle::ATT_ULTIMATE:
		Ultimate(fTimeDelta);
		break;
	case CMayCastle::ATT_SMASH:
		Smash(fTimeDelta);
		break;
	}
}

void CMayCastle::Dash_State(const _float & fTimeDelta)
{
	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	if (true == m_bDead)
	{
		m_ePL_STATE = PL_DEAD;
		Clear(PL_DEAD);
		return;
	}
	else if (m_bDashEnd)
	{
		Clear(PL_DASH);
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_DASH;

		//PxVec3 vDir = pBody->getLinearVelocity();
		//pBody->setLinearVelocity(vDir * 0.5f);

		pBody->setLinearVelocity(PxVec3(0, 0, 0));
		return;
	}

	_float3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	vLook.y = 0.0f;
	D3DXVec3Normalize(&vLook, &vLook);

	_vec3 vDstPos = vLook * m_fMaxSpeed * m_fDashRatio;
	pBody->setLinearVelocity(PxVec3(0, 0, 0));
	pBody->addForce(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), PxForceMode::eIMPULSE);

}

void CMayCastle::Hit_State(const _float & fTimeDelta)
{
	if (true == m_bDead)
	{
		m_ePL_STATE = PL_DEAD;
		Clear(PL_DEAD);
		return;
	}
	else if (m_bKnockback)
	{
		auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);
		_vec3 vDstPos = m_vKnockBackDir * m_fMaxSpeed * 0.5f;
		pBody->setLinearVelocity(PxVec3(0, 0, 0));
		pBody->addForce(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), PxForceMode::eIMPULSE);
	}
	else if (m_bHitEnd)
	{
		m_bHitEnd = false;
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_HIT;
	}
	else if (m_iKeyState & ATT)
	{
		m_bHitEnd = false;
		m_bHit = false;
		m_ePL_STATE = PL_ATT;
		m_ePre_State = PL_HIT;
	}
	else if (m_iKeyState & DASH)
	{
		m_ePL_STATE = PL_DASH;
		m_ePre_State = PL_HIT;
	}
}

void CMayCastle::Dead_State(const _float & fTimeDelta)
{
	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);
	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	pBody->setLinearVelocity(PxVec3(0, 0, 0));

	if (false == m_bDeadStart)
	{
		m_bDeadStart = true;
		m_bDeadEffect = true;
		m_bStencil = false;
		//m_bStopGravity = true;

		m_pDeadUI[0]->Set_Amount(0.f);
		for (_int i = 0; i < 6; ++i)
		{
			m_pDeadUI[i]->Change_Color_From_To(0.4f, _vec4(255.f, 255.f, 255.f, 0.f), _vec4(255.f, 255.f, 255.f, 255.f));
			m_pDeadUI[i]->Set_Visible(true);
		}
		m_pDeadUI[2]->Change_Color_From_To(0.4f, _vec4(255.f, 255.f, 255.f, 0.f), _vec4(231.f, 76.f, 60.f, 255.f));
		m_pDeadUI[3]->Change_Color_From_To(0.4f, _vec4(255.f, 255.f, 255.f, 0.f), _vec4(236.f, 216.f, 162.f, 255.f));

		for (_int i = 0; i < 3; ++i)
			m_pDeadUI[i]->Change_Size_From_To(0.2f, _vec3(1600.f, 1600.f, 0.f), _vec3(400.f, 400.f, 0.f));
		m_pDeadUI[3]->Change_Size_From_To(0.2f, _vec3(240.f, 240.f, 0.f), _vec3(60.f, 60.f, 0.f));
		m_pDeadUI[4]->Change_Size_From_To(0.2f, _vec3(160.f, 160.f, 0.f), _vec3(40.f, 40.f, 0.f));
		m_pDeadUI[5]->Change_Size_From_To(0.2f, _vec3(500.f, 500.f, 0.f), _vec3(128.f, 128.f, 0.f));

		m_pProfile->Set_Visible(false);
		m_pHp->Set_Visible(false);
		m_pCharge->Set_Visible(false);
		m_pChargeframe->Set_Visible(false);

		for (_int i = 0; i < 4; ++i)
			m_pSkill[i]->Set_Visible(false);

		for (_int i = 0; i < 8; ++i)
		{
			m_pSkillBox[i]->Set_Visible(false);
			m_pKey[i]->Set_Visible(false);
		}


	}
}

void CMayCastle::Recover(const _float & fTimeDelta)
{
	if (m_iHp < m_iMaxHp)
		m_bRecover = true;
	else
		m_bRecover = false;

	if (true == m_bRecover)
	{
		m_fRecoverTime += fTimeDelta;
		if (m_fRecoverTime > m_fRecoverGoalTime)
		{
			m_iHp += m_iRecoverHp;
			m_fRecoverTime = 0.0f;
		}
	}
}

void CMayCastle::BasicAtt(const _float & fTimeDelta)
{
	m_fAttTime += fTimeDelta;

	if (!m_bAttStart)
	{
		m_bAttStart = true;
		m_fAttTime = 0.0f;
		// 충돌리스트 초기화 애니메이션 매니저에서도 초기화하는곳이 있음

	}
	else if (m_iKeyState & ATT && m_fAttTime >= m_fAttLimitTime)
	{
		m_fAttTime = 0.0f;
		CMayCastle_Anim::GetInstance()->Set_AttState(this);
	}
}

void CMayCastle::Ultimate(const _float & fTimeDelta)
{
	//if (!(m_iKeyState & ULTIMATE))
	//	m_bAttEnd = true;

	m_fUltiAttTime -= fTimeDelta;

	m_iHitCount = 0;
	if (m_fUltiAttTime <= 0.f)
	{
		m_bAttEnd = true;
	}

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	_float3 vPos, vDir;

	Move(fTimeDelta, &vPos, &vDir, 0.0f);

	m_fCurMoveTime += fTimeDelta;
	if (m_fCurMoveTime < m_fGoalTime)
		m_fSpeed = m_fMaxSpeed * m_fCurMoveTime * (1.0f / m_fGoalTime);
	else
		m_fSpeed = m_fMaxSpeed;

	_vec3 vDstPos = vDir * m_fSpeed;

	if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
	{
		pBody->setLinearVelocity(PxVec3(0, 0, 0));
		pBody->addForce(PxVec3(vDstPos.x, vDstPos.y, vDstPos.z), PxForceMode::eIMPULSE);
	}

}

void CMayCastle::Smash(const _float & fTimeDelta)
{

}

void CMayCastle::Set_Hit(HITTYPE eType)
{
	switch (eType)
	{
	case DEFAULT:
	{
		if (m_ePL_STATE == PL_ATT && m_eAtt_State == ATT_SMASH)
			m_bHit = false;
		else
			m_bHit = true;
		break;
	}
	case CHESSBOSS:
		if (m_ePL_STATE == PL_ATT && m_eAtt_State == ATT_SMASH)
			m_bHit = false;
		else
			m_bHit = true;
		m_bKnockback = true;
		break;
	}
}

void CMayCastle::Set_Dir(_float3 * vDir, _int * iRotDir, _float * fCosTh)
{
	_float4x4		matCamWorld;
	CMainCamera*	pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
	matCamWorld = pCamera->Get_CamMatrix();
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	_float3 vPlayerLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vPlayerLook);
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	// 이동할 방향 구하기
	_float3 vCamDir = _float3(0.0f, 0.0f, 0.0f);
	/*if (m_iKeyState & MOVE_UP)
	{
		_float3 vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_float3));
		vLook.y = 0;
		D3DXVec3Normalize(&vLook, &vLook);
		vCamDir += vLook;
	}
	if (m_iKeyState & MOVE_DOWN)
	{
		_float3 vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_float3));
		vLook.y = 0;
		D3DXVec3Normalize(&vLook, &vLook);
		vCamDir += -vLook;
	}
	if (m_iKeyState & MOVE_LEFT)
	{
		_float3 vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_float3));
		vRight.y = 0;
		D3DXVec3Normalize(&vRight, &vRight);
		vCamDir += -vRight;
	}
	if (m_iKeyState & MOVE_RIGHT)
	{
		_float3 vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_float3));
		vRight.y = 0;
		D3DXVec3Normalize(&vRight, &vRight);
		vCamDir += vRight;
	}

	D3DXVec3Normalize(&vCamDir, &vCamDir);
	if (nullptr != vDir)
		*vDir = vCamDir;*/

		// 전방향 회전을 하기위한 식
	{
		// 카메라의 Look
		memcpy(&vCamDir, &matCamWorld.m[2][0], sizeof(_float3));
		vCamDir.y = 0;
		D3DXVec3Normalize(&vCamDir, &vCamDir);

		// 패드를 기준으로 회전
		_float4x4 matRotY;
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_fDirAngle));
		D3DXVec3TransformCoord(&vCamDir, &vCamDir, &matRotY);

		if (nullptr != vDir)
			*vDir = vCamDir;
	}

	// 회전할 방향을 구하자
	_float3 vPerp;
	D3DXVec3Cross(&vPerp, &_float3(0.0f, 1.0f, 0.0f), &vCamDir);
	D3DXVec3Normalize(&vPerp, &vPerp);
	_float fDir = D3DXVec3Dot(&vPerp, &vPlayerLook);

	if (nullptr != iRotDir)
	{
		if (fDir > 0.0f)
			*iRotDir = -1;
		else
			*iRotDir = 1;
	}

	// 코사인TH를 구하자
	_float _fCosTh = D3DXVec3Dot(&vCamDir, &vPlayerLook);
	if (nullptr != fCosTh)
		*fCosTh = _fCosTh;


}

void CMayCastle::Key_Reset()
{
	m_iKeyState = NULL;
	m_bHit = false;
}

void CMayCastle::Clear(PLAYER_STATE eState)
{
	switch (eState)
	{
	case CMayCastle::PL_IDLE:
		Clear_Idle();
		break;
	case CMayCastle::PL_MOVE:
		Clear_Move();
		break;
	case CMayCastle::PL_ATT:
		Clear_Att();
		break;
	case CMayCastle::PL_DASH:
		Clear_Dash();
		break;
	case CMayCastle::PL_HIT:
		Clear_Hit();
		break;
	case CMayCastle::PL_DEAD:
		Clear_Dead();
		break;
	}
}

void CMayCastle::Clear_Idle()
{
}

void CMayCastle::Clear_Move()
{
}

void CMayCastle::Clear_Att()
{
	m_bAttEnd = false;
	m_bAttStart = false;
	m_bAttAble = false;
	m_pSword->Set_AttAble(m_bAttAble);
	m_fAttTime = 0.0f;
	CMayCastle_Anim::GetInstance()->Set_ClearIndex();
}

void CMayCastle::Clear_Dash()
{
	m_bDashEnd = false;
	m_bDashEffect = false;
}

void CMayCastle::Clear_Hit()
{
	m_bHitEnd = false;
	m_bHit = false;
}

void CMayCastle::Clear_Dead()
{
	Clear_Idle();
	Clear_Move();
	Clear_Dash();
	Clear_Att();
	Clear_Hit();
	m_bDead = false;
	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);
	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
}
