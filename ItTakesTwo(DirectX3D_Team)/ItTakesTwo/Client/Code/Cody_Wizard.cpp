#include "stdafx.h"
#include "Cody_Wizard.h"
#include "Export_Function.h"
#include "CodyCamera.h"
#include "CodyWizard_Animation.h"
#include "MainCamera.h"
#include "CodyWizard_IceOrbAtt.h"
#include "CodyWizard_IceOrbAttRB.h"
#include "CollisionMgr.h"
#include "Orb.h"

CCodyWizard::CCodyWizard(LPDIRECT3DDEVICE9 pGraphicDev)
	: CPlayer(pGraphicDev)
	, m_fDashCool(3.f), m_fIceOrbCool(3.f)
{
}

CCodyWizard::~CCodyWizard()
{
}

void CCodyWizard::Check_Ray(const _float& fTimeDelta)
{
	if (!m_bUltEffect) 
		return;

	const PxU32 bufferSize = 256;				// [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];			// [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize); // [out] Blocking and touching hits stored here
	m_fDistance = 100.f;
	//CCody* pPlayer = Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>();
	_vec3 vPos = m_pTransformCom->Get_Position();
	_vec3 vDir;
	PxQueryFilterData data{};
	m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	if (m_pScene->raycast(PxVec3(vPos.x, vPos.y + 1, vPos.z),
		PxVec3(vDir.x, vDir.y, vDir.z),
		m_fMaxDistance,
		buf, PxHitFlag::eDEFAULT, data))
	{
		for (PxU32 i = 0; i < buf.nbTouches; ++i)
		{
			if (buf.getTouch(i).shape->getSimulationFilterData().word0 == FilterGroup::eGround)
			{
				if (m_fDistance > buf.getTouch(i).distance)
					m_fDistance = buf.getTouch(i).distance;
				m_vUltHitPos = ToVec3(buf.getTouch(i).position);

				m_fDistance = buf.getTouch(i).distance;
			}
		}
	}
}

HRESULT CCodyWizard::Ready_Object(void* pArg)
{
	m_fColDis = 0.8f;

	// 콜라이더
	ZeroMemory(&m_tColInfo, sizeof(SPHERE_INFO));
	m_tColInfo.fRadius = m_fColDis;

	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);
	m_pTransformCom->Set_Scale(_vec3(0.01f, 0.01f, 0.01f));
	CCodyWizard_Anim::GetInstance()->ReadyCodyAnimation();

	m_iMaxHp = 12;
	m_iHp = m_iMaxHp;
	m_iHitCount = 0;

	//m_vSavePoint = { 118.f, 1.f, -148.f };
	m_vSavePoint = { 0.f, 1.f, 0.f };
	Ready_Light();

	return S_OK;
}

Engine::_int CCodyWizard::Update_Object(const _float& fTimeDelta)
{
	if (m_bStart)
	{
		m_pAttackTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, { 0.f,0.f,0.f }, 0.2f, 0.2f, 1.f, TRIGGER_ATTACK, FilterGroup::ePlayer, FilterGroup::eMonster);
		m_pAttackTrigger->Set_Interactable(false);
		Create_UI(L"CW_UI");
		Create_Cone(L"Cone");
		m_bStart = false;
	}
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

	if (m_iHp <= 0)
	{
		m_bDead = true;
		m_bUltEffect = false;
		m_ePL_STATE = PL_DEAD;
		//Dead_State(fTimeDelta);
		//CCodyWizard_Anim::GetInstance()->PlayAnimation(this, m_pMeshCom, 0.0f, m_ePL_STATE, &m_ePre_State, m_eAtt_State);
		//Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

		//return OBJ_NOEVENT;
	}


	Check_Ray(fTimeDelta);


	if (Engine::Key_Down(DIK_7))
	{



	}

	// 무기 소유
	if (nullptr == m_pLOrb)
	{
		m_pLOrb = (COrb*)Engine::Get_GameObject(L"GameObject", L"IceOrb_Left");
		m_pLOrb->AddRef();
	}

	if (nullptr == m_pROrb)
	{
		m_pROrb = (COrb*)Engine::Get_GameObject(L"GameObject", L"IceOrb_Right");
		m_pROrb->AddRef();
	}


	Engine::CGameObject::Update_Object(fTimeDelta);
	if (!m_bCutScene && !m_bDead)
	{
		Key_Input(fTimeDelta);
	}
	Set_State(fTimeDelta);
	Effect(fTimeDelta);
	Update_UI(fTimeDelta);
	CoolTime(fTimeDelta);
	Update_Light(fTimeDelta);

	// HP가 최대치보다 낮으면 일정시간후에 HP회복
	Recover(fTimeDelta);

	CCodyWizard_Anim::GetInstance()->PlayAnimation(this, m_pMeshCom, fTimeDelta, m_ePL_STATE, &m_ePre_State, m_eAtt_State);

	CCollisionMgr::GetInstance()->Add_PlayerList(this);

	if (!m_bDeadUI)
	{
		Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
		if (m_bStencil)
		{
			Engine::Add_RenderGroup(Engine::RENDER_SHADOW, this);
			Engine::Add_RenderGroup(Engine::RENDER_STENCIL, this);
			m_pLOrb->Set_Active(true);
			m_pROrb->Set_Active(true);
			m_pCodyLight->Set_LightRender(true);
			m_pCodyLight2->Set_LightRender(true);
		}
		else
		{
			m_pLOrb->Set_Active(false);
			m_pROrb->Set_Active(false);
			m_pCodyLight->Set_LightRender(false);
			m_pCodyLight2->Set_LightRender(false);
		}
	}

	return OBJ_NOEVENT;
}

_int CCodyWizard::LateUpdate_Object(const _float & fTimeDelta)
{
	CPlayer::Clear_HitAble(fTimeDelta);

	Key_Reset();

	// 피직스 없이 이용할때만 false값을줘서 피직스 이동을 막는다.
	if (true == m_bMovePysics)
	{
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
	}
	else
	{
		// 피직스 이동을 하지 않았을경우 위치값을 수정해준다.
		auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);
		_vec3 vPos = m_pTransformCom->Get_Position();
		PxTransform tTransform(vPos.x, vPos.y + 0.5f, vPos.z);

		pBody->setGlobalPose(tTransform);
	}

	// 매프레임마다 피직스는 켜준다.
	m_bMovePysics = true;
	m_bHit = false;

	return _int();
}

void CCodyWizard::Render_Object(const _int& iIndex)
{
	if (m_bBlinkRender)
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
	wsprintf(szbuf, L"CodyHp : %d", m_iHp);

	Engine::Render_Font(L"Font_Jinji", szbuf, &_vec2(20, 60), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
#endif
}

void CCodyWizard::Render_Shadow(_vec3* vEye, _vec3* vAt, const _int& iIndex)
{
	if (m_bBlinkRender)
		return;

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

void CCodyWizard::Render_Stencil(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes_VTF(pEffect, 3);

	pEffect->End();

	Safe_Release(pEffect);
}

void CCodyWizard::Effect(const _float & fTimeDelta)
{
	Effect_Orb(fTimeDelta);
	Effect_DefaultAttack(fTimeDelta);
	Effect_Dash(fTimeDelta);
	Effect_DefaultSkill(fTimeDelta);
	Effect_UltimateSkill(fTimeDelta);
	Effect_UltHitEffect(fTimeDelta);
	Effect_Walk(fTimeDelta);
	Effect_Dead(fTimeDelta);
	Effect_Respawn(fTimeDelta);
}

void CCodyWizard::Effect_Orb(const _float & fTimeDelta)
{
	if (m_bDeadUI || m_bCutScene)
		return;

	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);

	_matrix matWorld, matWorld2;
	const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("RightAttach");
	const Engine::D3DXFRAME_DERIVED* pFrame2 = m_pMeshCom->Get_FrameByName("LeftAttach");
	const _matrix* pMatrix = &pFrame->CombinedTranformationMatrix;
	const _matrix* pMatrix2 = &pFrame2->CombinedTranformationMatrix;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	matWorld = *pMatrix * matWorld;

	m_pTransformCom->Get_WorldMatrix(&matWorld2);
	matWorld2 = *pMatrix2 * matWorld2;

	_vec3 vRightOrbPos = { matWorld._41, matWorld._42, matWorld._43 };
	_vec3 vLeftOrbPos = { matWorld2._41, matWorld2._42, matWorld2._43 };

	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
	tParticle.fRotateSpeed = 0.f;

	tParticle.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2, 255.f * 2.5f };
	tParticle.vEndColor = { 255.f, 255.f, 255.f, 50.f };
	tParticle.vRot = { 0.f, 0.f, 0.f };

	tParticle.fVelocity = CRandoms()(0, 10) * 0.1f;
	tParticle.fLifeTime = CRandoms()(1, 5) * 0.1f + 0.5f;
	_float fRand = CRandoms()(40, 80) * 0.001f;
	tParticle.vStartSize = _vec3(fRand, fRand, fRand);
	tParticle.vDir = { CRandoms()(-100, 100) * 0.01f,CRandoms()(-100, 100) * 0.01f , CRandoms()(-100, 100) * 0.01f };

	tParticle.vPos = vRightOrbPos + _vec3(CRandoms()(-20, 20) * 0.01f, CRandoms()(-20, 20) * 0.01f, CRandoms()(-20, 20) * 0.01f);
	Engine::Emit_ImageParticle(tParticle, 15, 11);

	tParticle.vPos = vLeftOrbPos + _vec3(CRandoms()(-20, 20) * 0.01f, CRandoms()(-20, 20) * 0.01f, CRandoms()(-20, 20) * 0.01f);
	Engine::Emit_ImageParticle(tParticle, 15, 11);



	PARTICLE tParticle2;
	Engine::Reset_Particle(tParticle2);
	Engine::Set_AnimState(tParticle2, 8, 8, 1.f);
	tParticle2.fAcc = 1.f;
	tParticle2.fCurrentTime = 0.f;
	tParticle2.vStartSize = _vec3(3.f, 3.f, 3.f);
	tParticle2.vEndSize = _vec3(2.f, 2.f, 2.f);
	tParticle2.fRotateSpeed = 0.f;
	tParticle2.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 5.f };
	tParticle2.vEndColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 0.f };
	tParticle2.vRot = { 0.f, 0.f, 0.f };
	tParticle2.fVelocity = 0.f;
	tParticle2.fLifeTime = 1.f;

	tParticle2.vRot = { 0.f, _float(CRandoms()(1, 360)), 0.f };

	tParticle2.vDir = { 0.f, 0.f, 0.f };
	tParticle2.vPos = vRightOrbPos;
	Engine::Emit_ImageParticle(tParticle2, 23, 4);
	tParticle2.vPos = vLeftOrbPos;
	Engine::Emit_ImageParticle(tParticle2, 23, 4);
}

void CCodyWizard::Effect_DefaultAttack(const _float & fTimeDelta)
{
	switch (m_iDefaultAttack)
	{
	case 0:
		return;
	case 1: // First Attack Effect
	{
		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.vStartSize = _vec3(3.f, 1.f, 1.f);
		tParticle.vEndSize = _vec3(4.f, 2.f, 2.f);
		tParticle.fRotateSpeed = 0.f;
		tParticle.vStartColor = { 180.f * 3, 200.f * 3, 255.f * 4.f, 255.f };
		tParticle.vEndColor = { 180.f * 3, 200.f * 3, 255.f * 4.f, 0.f };
		tParticle.vRot = { 0.f, 0.f, 0.f };
		tParticle.fVelocity = 40.f;
		tParticle.fLifeTime = 0.15f;
		tParticle.bBill = false;
		tParticle.vRot = { D3DXToRadian(90.f), m_pTransformCom->Get_Angle().y, 0.f };

		_matrix matWorld;
		m_pTransformCom->Get_WorldMatrix(&matWorld);
		_vec3 vLook = { matWorld._31, 0.f, matWorld._33 };

		tParticle.vDir = vLook;
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f);
		Engine::Emit_ImageParticle(tParticle, 16, 18);


		PARTICLE tParticle2;
		Engine::Reset_Particle(tParticle2);
		tParticle2.fAcc = 1.f;
		tParticle2.fCurrentTime = 0.f;
		tParticle2.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
		tParticle2.fRotateSpeed = 0.f;

		tParticle2.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2, 255.f * 2.5f };
		tParticle2.vEndColor = { 255.f, 255.f, 255.f, 255.f };
		tParticle2.vRot = { 0.f, 0.f, 0.f };
		tParticle2.bGravity = true;
		tParticle2.fGravityForce = 1.8f;

		for (_int i = 0; i < 100; ++i)
		{
			tParticle2.fVelocity = CRandoms()(0, 200) * 0.1f + 5.f;
			tParticle2.fLifeTime = CRandoms()(0, 2) * 0.1f + 0.2f;
			_float fRand = CRandoms()(10, 15) * 0.01f;
			tParticle2.vStartSize = _vec3(fRand, fRand, fRand);
			tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-2, 2) * 0.1f, 1.f, CRandoms()(-2, 2) * 0.1f);
			_matrix matRot;
			vLook = { matWorld._31, CRandoms()(10, 50) * 0.0001f, matWorld._33 };
			D3DXMatrixRotationY(&matRot, D3DXToRadian(CRandoms()(-15, 15) * 0.1f));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
			tParticle2.vDir = vLook;
			Engine::Emit_ImageParticle(tParticle2, 15, 11);
		}

		PARTICLE tParticle3;
		Engine::Reset_Particle(tParticle3);
		tParticle3.fAcc = 1.f;
		tParticle3.fCurrentTime = 0.f;
		tParticle3.vEndSize = _vec3(0.001f, 0.001f, 0.001f);
		tParticle3.fRotateSpeed = 0.f;

		tParticle3.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2.3f, 255.f * 5 };
		tParticle3.vEndColor = { 180.f * 2, 200.f * 2, 255.f * 2.3f, 255.f * 5 };
		tParticle3.vRot = { 0.f, m_pTransformCom->Get_Angle().y + D3DXToRadian(45.f), 0.f };
		tParticle3.bBill = false;
		tParticle3.bGravity = true;
		tParticle3.fGravityForce = 2.5f;

		for (_int i = 0; i < 50; ++i)
		{
			tParticle3.fVelocity = CRandoms()(0, 200) * 0.1f + 5.f;
			tParticle3.fLifeTime = CRandoms()(0, 2) * 0.1f + 0.1f;
			_float fRand = CRandoms()(30, 50) * 0.0001f;
			tParticle3.vStartSize = _vec3(fRand, fRand, fRand);

			tParticle3.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-1, 1) * 0.1f, 1.f, CRandoms()(-1, 1) * 0.1f);
			_matrix matRot;
			vLook = { matWorld._31, CRandoms()(10, 50) * 0.0001f, matWorld._33 };
			D3DXMatrixRotationY(&matRot, D3DXToRadian(CRandoms()(-40, 40) * 0.2f));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
			tParticle3.vDir = vLook;
			Engine::Emit_MeshParticle(tParticle3, L"BreakIce2", 13);
		}

		PARTICLE tParticle4;
		Engine::Reset_Particle(tParticle4);
		tParticle4.fAcc = 1.f;
		tParticle4.fCurrentTime = 0.f;
		tParticle4.vStartSize = _vec3(3.f, 5.f, 5.f);
		tParticle4.vEndSize = _vec3(3.f, 5.f, 5.f);
		tParticle4.fRotateSpeed = 0.f;
		tParticle4.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 150.f };
		tParticle4.vEndColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 0.f };
		tParticle4.vRot = { 0.f, 0.f, 0.f };
		tParticle4.fVelocity = 0.f;
		tParticle4.fLifeTime = 1.5f;
		tParticle4.bBill = false;
		tParticle4.vRot = { D3DXToRadian(90.f), m_pTransformCom->Get_Angle().y + D3DXToRadian(180.f), 0.f };

		_matrix matRot;
		vLook = { matWorld._31, 0.f, matWorld._33 };
		tParticle4.vDir = vLook;
		tParticle4.vPos = (m_pTransformCom->Get_Position() + _vec3(0.f, 0.7f, 0.f)) + vLook * 350.f;
		Engine::Emit_ImageParticle(tParticle4, 15, 14);

		m_iDefaultAttack = 0;
	}
	return;

	case 2: // Second Attack Effect
	{
		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.vStartSize = _vec3(4.f, 1.5f, 1.5f);
		tParticle.vEndSize = _vec3(5.f, 2.5f, 2.5f);
		tParticle.fRotateSpeed = 0.f;
		tParticle.vStartColor = { 180.f * 3, 200.f * 3, 255.f * 4.f, 255.f };
		tParticle.vEndColor = { 180.f * 3, 200.f * 3, 255.f * 4.f, 0.f };
		tParticle.vRot = { 0.f, 0.f, 0.f };
		tParticle.fVelocity = 40.f;
		tParticle.fLifeTime = 0.2f;
		tParticle.bBill = false;
		tParticle.vRot = { D3DXToRadian(90.f), m_pTransformCom->Get_Angle().y, 0.f };

		_matrix matWorld;
		m_pTransformCom->Get_WorldMatrix(&matWorld);
		_vec3 vLook = { matWorld._31, 0.f, matWorld._33 };
		tParticle.vDir = vLook;
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f);
		Engine::Emit_ImageParticle(tParticle, 16, 18);


		PARTICLE tParticle2;
		Engine::Reset_Particle(tParticle2);
		tParticle2.fAcc = 1.f;
		tParticle2.fCurrentTime = 0.f;
		tParticle2.vEndSize = _vec3(0.05f, 0.05f, 0.05f);
		tParticle2.fRotateSpeed = 0.f;

		tParticle2.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 255.f * 2 };
		tParticle2.vEndColor = { 255.f, 255.f, 255.f, 255.f };
		tParticle2.vRot = { 0.f, 0.f, 0.f };
		tParticle2.bGravity = true;
		tParticle2.fGravityForce = 1.8f;

		for (_int i = 0; i < 100; ++i)
		{
			tParticle2.fVelocity = CRandoms()(0, 200) * 0.1f + 5.f;
			tParticle2.fLifeTime = CRandoms()(0, 2) * 0.1f + 0.3f;
			_float fRand = CRandoms()(10, 20) * 0.02f;
			tParticle2.vStartSize = _vec3(fRand, fRand, fRand);
			tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-2, 2) * 0.1f, 1.f, CRandoms()(-2, 2) * 0.1f);
			_matrix matRot;
			vLook = { matWorld._31, CRandoms()(10, 50) * 0.0001f, matWorld._33 };

			D3DXMatrixRotationY(&matRot, D3DXToRadian(CRandoms()(-15, 15) * 0.1f));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
			tParticle2.vDir = vLook;
			Engine::Emit_ImageParticle(tParticle2, 15, 11);
		}

		PARTICLE tParticle3;
		Engine::Reset_Particle(tParticle3);
		tParticle3.fAcc = 1.f;
		tParticle3.fCurrentTime = 0.f;
		tParticle3.vEndSize = _vec3(0.001f, 0.001f, 0.001f);
		tParticle3.fRotateSpeed = 0.f;

		tParticle3.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2.3f, 255.f * 5 };
		tParticle3.vEndColor = { 180.f * 2, 200.f * 2, 255.f * 2.3f, 255.f * 5 };
		tParticle3.vRot = { 0.f, m_pTransformCom->Get_Angle().y + D3DXToRadian(45.f), 0.f };
		tParticle3.bBill = false;
		tParticle3.bGravity = true;
		tParticle3.fGravityForce = 2.5f;

		for (_int i = 0; i < 70; ++i)
		{
			tParticle3.fVelocity = CRandoms()(0, 200) * 0.1f + 5.f;
			tParticle3.fLifeTime = CRandoms()(0, 2) * 0.1f + 0.3f;
			_float fRand = CRandoms()(30, 50) * 0.0001f;
			tParticle3.vStartSize = _vec3(fRand, fRand, fRand);

			tParticle3.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-2, 2) * 0.1f, 1.f, CRandoms()(-2, 2) * 0.1f);
			_matrix matRot;
			vLook = { matWorld._31, CRandoms()(10, 50) * 0.0001f, matWorld._33 };

			D3DXMatrixRotationY(&matRot, D3DXToRadian(CRandoms()(-80, 80) * 0.2f));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
			tParticle3.vDir = vLook;
			Engine::Emit_MeshParticle(tParticle3, L"BreakIce2", 13);
		}

		PARTICLE tParticle4;
		Engine::Reset_Particle(tParticle4);
		tParticle4.fAcc = 1.f;
		tParticle4.fCurrentTime = 0.f;
		tParticle4.vStartSize = _vec3(4.f, 7.f, 7.f);
		tParticle4.vEndSize = _vec3(4.f, 7.f, 7.f);
		tParticle4.fRotateSpeed = 0.f;
		tParticle4.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 150.f };
		tParticle4.vEndColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 0.f };
		tParticle4.vRot = { 0.f, 0.f, 0.f };
		tParticle4.fVelocity = 0.f;
		tParticle4.fLifeTime = 1.5f;
		tParticle4.bBill = false;
		tParticle4.vRot = { D3DXToRadian(90.f), m_pTransformCom->Get_Angle().y + D3DXToRadian(180.f), 0.f };

		_matrix matRot;
		vLook = { matWorld._31, 0.f, matWorld._33 };
		tParticle4.vDir = vLook;
		tParticle4.vPos = (m_pTransformCom->Get_Position() + _vec3(0.f, 0.7f, 0.f)) + vLook * 400.f;
		Engine::Emit_ImageParticle(tParticle4, 15, 14);


		m_iDefaultAttack = 0;
	}
	return;

	case 3: // Third Attack Effect
	{
		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.vStartSize = _vec3(5.f, 2.f, 2.f);
		tParticle.vEndSize = _vec3(6.f, 3.f, 3.f);
		tParticle.fRotateSpeed = 0.f;
		tParticle.vStartColor = { 180.f * 3, 200.f * 3, 255.f * 4.f, 255.f };
		tParticle.vEndColor = { 180.f * 3, 200.f * 3, 255.f * 4.f, 0.f };
		tParticle.vRot = { 0.f, 0.f, 0.f };
		tParticle.fVelocity = 40.f;
		tParticle.fLifeTime = 0.25f;
		tParticle.bBill = false;
		tParticle.vRot = { D3DXToRadian(90.f), m_pTransformCom->Get_Angle().y, 0.f };

		_matrix matWorld;
		m_pTransformCom->Get_WorldMatrix(&matWorld);
		_vec3 vLook = { matWorld._31, 0.f, matWorld._33 };
		tParticle.vDir = vLook;
		tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f);
		Engine::Emit_ImageParticle(tParticle, 16, 18);


		PARTICLE tParticle2;
		Engine::Reset_Particle(tParticle2);
		tParticle2.fAcc = 1.f;
		tParticle2.fCurrentTime = 0.f;
		tParticle2.vEndSize = _vec3(0.05f, 0.05f, 0.05f);
		tParticle2.fRotateSpeed = 0.f;

		tParticle2.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 255.f * 2 };
		tParticle2.vEndColor = { 255.f, 255.f, 255.f, 255.f };
		tParticle2.vRot = { 0.f, 0.f, 0.f };
		tParticle2.bGravity = true;
		tParticle2.fGravityForce = 1.8f;

		for (_int i = 0; i < 150; ++i)
		{
			tParticle2.fVelocity = CRandoms()(0, 200) * 0.1f + 5.f;
			tParticle2.fLifeTime = CRandoms()(0, 2) * 0.1f + 0.4f;
			_float fRand = CRandoms()(10, 20) * 0.02f;
			tParticle2.vStartSize = _vec3(fRand, fRand, fRand);
			tParticle2.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-2, 2) * 0.1f, 1.f, CRandoms()(-2, 2) * 0.1f);
			_matrix matRot;
			vLook = { matWorld._31, CRandoms()(10, 50) * 0.0001f, matWorld._33 };

			D3DXMatrixRotationY(&matRot, D3DXToRadian(CRandoms()(-15, 15) * 0.1f));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
			tParticle2.vDir = vLook;
			Engine::Emit_ImageParticle(tParticle2, 15, 11);
		}

		PARTICLE tParticle3;
		Engine::Reset_Particle(tParticle3);
		tParticle3.fAcc = 1.f;
		tParticle3.fCurrentTime = 0.f;
		tParticle3.vEndSize = _vec3(0.001f, 0.001f, 0.001f);
		tParticle3.fRotateSpeed = 0.f;

		tParticle3.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2.3f, 255.f * 5 };
		tParticle3.vEndColor = { 180.f * 2, 200.f * 2, 255.f * 2.3f, 255.f * 5 };
		tParticle3.vRot = { 0.f, m_pTransformCom->Get_Angle().y + D3DXToRadian(45.f), 0.f };
		tParticle3.bBill = false;
		tParticle3.bGravity = true;
		tParticle3.fGravityForce = 2.5f;

		for (_int i = 0; i < 90; ++i)
		{
			tParticle3.fVelocity = CRandoms()(0, 200) * 0.1f + 5.f;
			tParticle3.fLifeTime = CRandoms()(0, 2) * 0.1f + 0.4f;
			_float fRand = CRandoms()(30, 50) * 0.0001f;
			tParticle3.vStartSize = _vec3(fRand, fRand, fRand);

			tParticle3.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-3, 3) * 0.1f, 1.f, CRandoms()(-3, 3) * 0.1f);
			_matrix matRot;
			vLook = { matWorld._31, CRandoms()(10, 50) * 0.0001f, matWorld._33 };

			D3DXMatrixRotationY(&matRot, D3DXToRadian(CRandoms()(-110, 110) * 0.2f));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
			tParticle3.vDir = vLook;
			Engine::Emit_MeshParticle(tParticle3, L"BreakIce2", 13);
		}

		PARTICLE tParticle4;
		Engine::Reset_Particle(tParticle4);
		tParticle4.fAcc = 1.f;
		tParticle4.fCurrentTime = 0.f;
		tParticle4.vStartSize = _vec3(5.f, 9.f, 9.f);
		tParticle4.vEndSize = _vec3(5.f, 9.f, 9.f);
		tParticle4.fRotateSpeed = 0.f;
		tParticle4.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 150.f };
		tParticle4.vEndColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 0.f };
		tParticle4.vRot = { 0.f, 0.f, 0.f };
		tParticle4.fVelocity = 0.f;
		tParticle4.fLifeTime = 1.5f;
		tParticle4.bBill = false;
		tParticle4.vRot = { D3DXToRadian(90.f), m_pTransformCom->Get_Angle().y + D3DXToRadian(180.f), 0.f };

		_matrix matRot;
		vLook = { matWorld._31, 0.f, matWorld._33 };
		tParticle4.vDir = vLook;
		tParticle4.vPos = (m_pTransformCom->Get_Position() + _vec3(0.f, 0.7f, 0.f)) + vLook * 500.f;
		Engine::Emit_ImageParticle(tParticle4, 15, 14);

		m_iDefaultAttack = 0;
	}
	return;
	}
}

void CCodyWizard::Effect_Dash(const _float & fTimeDelta)
{
	switch (m_iDash)
	{
	case 0:
		return;

	case 1: // Dash Start Effect
	{
		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);

		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
		tParticle.fRotateSpeed = 0.f;

		tParticle.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2, 255.f * 2.5f };
		tParticle.vEndColor = { 255.f, 255.f, 255.f, 50.f };
		tParticle.vRot = { 0.f, 0.f, 0.f };

		for (_int i = 0; i < 300; ++i)
		{
			tParticle.fVelocity = CRandoms()(0, 10) * 0.1f;
			tParticle.fLifeTime = CRandoms()(1, 5) * 0.1f + 0.5f;
			_float fRand = CRandoms()(50, 120) * 0.001f;
			tParticle.vStartSize = _vec3(fRand, fRand, fRand);

			tParticle.vDir = { CRandoms()(-100, 100) * 0.01f, CRandoms()(-100, 100) * 0.01f , CRandoms()(-100, 100) * 0.01f };
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f) + _vec3(CRandoms()(-50, 50) * 0.01f, CRandoms()(-50, 50) * 0.01f, CRandoms()(-50, 50) * 0.01f);
			Engine::Emit_ImageParticle(tParticle, 15, 11);
		}
		m_iDash = 0;
	}
	return;

	case 2: // Dash End Effect
	{
		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);

		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;
		tParticle.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
		tParticle.fRotateSpeed = 0.f;

		tParticle.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2, 255.f * 2.5f };
		tParticle.vEndColor = { 255.f, 255.f, 255.f, 50.f };
		tParticle.vRot = { 0.f, 0.f, 0.f };

		for (_int i = 0; i < 300; ++i)
		{
			tParticle.fVelocity = CRandoms()(0, 10) * 0.1f;
			tParticle.fLifeTime = CRandoms()(1, 5) * 0.1f + 0.5f;
			_float fRand = CRandoms()(50, 120) * 0.001f;
			tParticle.vStartSize = _vec3(fRand, fRand, fRand);

			tParticle.vDir = { CRandoms()(-100, 100) * 0.01f, CRandoms()(-100, 100) * 0.01f , CRandoms()(-100, 100) * 0.01f };
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f) + _vec3(CRandoms()(-50, 50) * 0.01f, CRandoms()(-50, 50) * 0.01f, CRandoms()(-50, 50) * 0.01f);
			Engine::Emit_ImageParticle(tParticle, 15, 11);
		}

		PARTICLE tParticle2;
		Engine::Reset_Particle(tParticle2);
		Engine::Set_AnimState(tParticle2, 8, 8, 1.f);
		tParticle2.fAcc = 1.f;
		tParticle2.fCurrentTime = 0.f;
		tParticle2.vStartSize = _vec3(3.f, 3.f, 3.f);
		tParticle2.vEndSize = _vec3(3.f, 3.f, 3.f);
		tParticle2.fRotateSpeed = 0.f;
		tParticle2.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 255.f };
		tParticle2.vEndColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 0.f };
		tParticle2.vRot = { 0.f, 0.f, 0.f };
		tParticle2.fVelocity = 0.f;
		tParticle2.fLifeTime = 1.f;
		tParticle2.vRot = { 0.f, 0.f, 0.f };

		tParticle2.vDir = { 0.f, 0.f, 0.f };
		for (_int i = 0; i < 5; ++i)
		{
			tParticle2.vPos = (m_pTransformCom->Get_Position() + _vec3(CRandoms()(-10, 10) * 0.1f, 1.f, CRandoms()(-10, 10) * 0.1f));
			Engine::Emit_ImageParticle(tParticle2, 23, 4);
		}

		PARTICLE tParticle3;
		Engine::Reset_Particle(tParticle3);
		tParticle3.fAcc = 1.f;
		tParticle3.fCurrentTime = 0.f;
		tParticle3.vStartSize = _vec3(2.5f, 2.5f, 2.5f);
		tParticle3.vEndSize = _vec3(4.f, 4.f, 4.f);
		tParticle3.fRotateSpeed = 0.f;
		tParticle3.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 255.f };
		tParticle3.vEndColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 55.f };

		tParticle3.vRot = { 0.f, 0.f, 0.f };
		tParticle3.fVelocity = 0.f;
		tParticle3.fLifeTime = 2.5f;
		tParticle3.vRot = { 0.f, 0.f, 0.f };

		tParticle3.vDir = { 0.f, 0.f, 0.f };
		tParticle3.fAmount = 1.f;

		tParticle3.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f);
		Engine::Emit_ImageParticle(tParticle3, 15, 15);


		m_iDash = 0;
	}
	return;
	}
}

void CCodyWizard::Effect_DefaultSkill(const _float & fTimeDelta)
{
}

void CCodyWizard::Effect_UltimateSkill(const _float & fTimeDelta)
{
	if (!m_bUltEffect)
		return;

	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);

	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	_float fRand = CRandoms()(0.05f, 0.2f);
	tParticle.vStartSize = _vec3(fRand, fRand, fRand);
	tParticle.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
	tParticle.fRotateSpeed = 0.f;

	tParticle.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2, 255.f * 2.5f };
	tParticle.vEndColor = { 255.f, 255.f, 255.f, 50.f };
	tParticle.vRot = { 0.f, 0.f, 0.f };

	tParticle.fVelocity = CRandoms()(20.f, 30.f);
	tParticle.fLifeTime = CRandoms()(0.1f, 0.3f);

	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	tParticle.vDir = vLook;
	tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f) + _vec3(CRandoms()(-50, 50) * 0.01f, CRandoms()(-50, 50) * 0.01f, CRandoms()(-50, 50) * 0.01f);
	Engine::Emit_ImageParticle(tParticle, 15, 11);

	////


	PARTICLE tParticle2;
	Engine::Reset_Particle(tParticle2);
	Engine::Set_AnimState(tParticle2, 8, 8);

	_matrix matWorld, matWorld2;
	const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("RightAttach");
	const Engine::D3DXFRAME_DERIVED* pFrame2 = m_pMeshCom->Get_FrameByName("LeftAttach");
	const _matrix* pMatrix = &pFrame->CombinedTranformationMatrix;
	const _matrix* pMatrix2 = &pFrame2->CombinedTranformationMatrix;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	matWorld = *pMatrix * matWorld;

	m_pTransformCom->Get_WorldMatrix(&matWorld2);
	matWorld2 = *pMatrix2 * matWorld2;

	_vec3 vRightOrbPos = { matWorld._41, matWorld._42, matWorld._43 };
	_vec3 vLeftOrbPos = { matWorld2._41, matWorld2._42, matWorld2._43 };
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);

	tParticle2.fAcc = 1.f;
	tParticle2.fCurrentTime = 0.f;
	tParticle2.vEndSize = _vec3(4.f, 4.f, 4.f);
	tParticle2.fRotateSpeed = 0.f;

	tParticle2.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2, 100.f };
	tParticle2.vEndColor = { 180.f, 200.f, 255.f, 50.f };
	tParticle2.vRot = { 0.f, 0.f, 0.f };

	tParticle2.fVelocity = CRandoms()(0.1f, 2.f);
	tParticle2.fLifeTime = CRandoms()(1, 5) * 0.1f + 0.5f;
	fRand = CRandoms()(2.f, 3.f);
	tParticle2.vStartSize = _vec3(fRand, fRand, fRand);
	tParticle2.vDir = vLook;

	tParticle2.vPos = vRightOrbPos;
	Engine::Emit_ImageParticle(tParticle2, 29, 4);

	tParticle2.vPos = vLeftOrbPos;
	Engine::Emit_ImageParticle(tParticle2, 29, 4);


	////	
	PARTICLE tParticle4;
	Engine::Reset_Particle(tParticle4);
	Engine::Set_AnimState(tParticle4, 2, 2, 5.f);
	tParticle4.fAcc = 1.f;
	tParticle4.fCurrentTime = 0.f;
	tParticle4.fRotateSpeed = 0.f;
	tParticle4.vEndSize = _vec3(0.1f, 0.1f, 0.1f);

	tParticle4.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 3.f, 255.f * 5 };
	tParticle4.vEndColor = { 180.f * 2, 200.f * 2, 255.f * 3.f, 255.f * 5 };
	tParticle4.bBill = false;
	tParticle4.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-1.3f, 1.3f), 1.f, CRandoms()(-1.3f, 1.3f)) + (vLook * CRandoms()((300.f * (m_fDistance / m_fMaxDistance)), (2000.f * (m_fDistance / m_fMaxDistance))));

	tParticle4.fLifeTime = CRandoms()(0.2f, 0.4f);
	fRand = CRandoms()(1.f, 4.f);
	tParticle4.vStartSize = _vec3(CRandoms()(1.f, 2.f), fRand, 2.f);
	tParticle4.fVelocity = CRandoms()(15.f, 20.f);

	//tParticle4.vDir = { 1.f, 0.f, 0.f };
	tParticle4.vDir = vLook;
	_float fRadian = atan2f(tParticle4.vDir.x, tParticle4.vDir.z);
	tParticle4.vRot = { D3DXToRadian(90.f), fRadian, 0.f };

	Engine::Emit_ImageParticle(tParticle4, 15, 21);
}

void CCodyWizard::Effect_UltHitEffect(const _float & fTimeDelta)
{
	if (!m_bUltEffect)
		return;


	_float test = m_fDistance / m_fMaxDistance;
	m_pCone->Set_Amount(m_fDistance / m_fMaxDistance);
	m_pCone2->Set_Amount(m_fDistance / m_fMaxDistance);

	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);

	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
	tParticle.fRotateSpeed = 0.f;

	tParticle.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2, 255.f * 2.5f };
	tParticle.vEndColor = { 255.f, 255.f, 255.f, 50.f };
	tParticle.vRot = { 0.f, 0.f, 0.f };

	for (_int i = 0; i < 15; ++i)
	{
		tParticle.fVelocity = CRandoms()(1.5f, 2.5f);
		tParticle.fLifeTime = CRandoms()(1.f, 1.5f);
		_float fRand = CRandoms()(50, 120) * 0.001f;
		tParticle.vStartSize = _vec3(fRand, fRand, fRand);

		tParticle.vDir = { CRandoms()(-100, 100) * 0.01f, CRandoms()(-100, 100) * 0.01f , CRandoms()(-100, 100) * 0.01f };
		tParticle.vPos = m_vUltHitPos + _vec3(CRandoms()(-1.f, 1.f), CRandoms()(-1.f, 1.f), CRandoms()(-1.f, 1.f));
		Engine::Emit_ImageParticle(tParticle, 15, 11);
	}

	PARTICLE tParticle2;
	Engine::Reset_Particle(tParticle2);
	Engine::Set_AnimState(tParticle2, 8, 8, 1.f);
	tParticle2.fAcc = 1.f;
	tParticle2.fCurrentTime = 0.f;
	tParticle2.vStartSize = _vec3(3.f, 3.f, 3.f);
	tParticle2.vEndSize = _vec3(3.f, 3.f, 3.f);
	tParticle2.fRotateSpeed = 0.f;
	tParticle2.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 255.f };
	tParticle2.vEndColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 0.f };
	tParticle2.vRot = { 0.f, 0.f, 0.f };
	tParticle2.fVelocity = 0.f;
	tParticle2.fLifeTime = 1.f;
	tParticle2.vRot = { 0.f, 0.f, 0.f };

	tParticle2.vDir = { 0.f, 0.f, 0.f };
	for (_int i = 0; i < 5; ++i)
	{
		tParticle2.vPos = m_vUltHitPos + _vec3(CRandoms()(-10, 10) * 0.1f, 0.f, CRandoms()(-10, 10) * 0.1f);
		Engine::Emit_ImageParticle(tParticle2, 23, 4);
	}

	PARTICLE tParticle3;
	Engine::Reset_Particle(tParticle3);
	tParticle3.fAcc = 1.f;
	tParticle3.fCurrentTime = 0.f;
	tParticle3.vStartSize = _vec3(2.f, 2.f, 2.f);
	tParticle3.vEndSize = _vec3(3.f, 3.f, 3.f);
	tParticle3.fRotateSpeed = 0.f;
	tParticle3.vStartColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 255.f };
	tParticle3.vEndColor = { 180.f * 2, 200.f * 2, 255.f * 2.5f, 55.f };

	tParticle3.vRot = { 0.f, 0.f, 0.f };
	tParticle3.fVelocity = 0.f;
	tParticle3.fLifeTime = 2.5f;
	tParticle3.vRot = { 0.f, 0.f, 0.f };

	tParticle3.vDir = { 0.f, 0.f, 0.f };
	tParticle3.fAmount = 1.f;

	tParticle3.vPos = m_vUltHitPos;

	Engine::Emit_ImageParticle(tParticle3, 15, 15);
}

void CCodyWizard::Effect_Walk(const _float & fTimeDelta)
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

void CCodyWizard::Effect_Dead(const _float & fTimeDelta)
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

void CCodyWizard::Effect_Respawn(const _float & fTimedelta)
{
	if (!m_bRespawnEffect)
		return;

	m_fAmount -= fTimedelta;
	if (m_fAmount <= 0.f)
	{
		m_iHp = m_iMaxHp;
		m_bRespawnEffect = false;
		m_bStencil = true;
		m_fAmount = 0.f;
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

void CCodyWizard::Create_UI(const _tchar* pLayerTag)
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
	m_pProfile->Set_Image(L"CW_ImageUI");
	m_pProfile->Set_Size(_vec3{ 128.f, 128.f, 0.f });
	m_pProfile->Set_Pos(_vec3{ 1732.f, -880.f, 0.f });
	m_pProfile->Set_RenderID(RENDER_UI);
	m_pProfile->Set_PassIndex(2);
	m_pProfile->Set_TextureIndex(0);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_Profile", pGameObject), );
	m_pProfile->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pHp = pGameObject->Is<CImageUI>();
	m_pHp->Set_Image(L"CW_ImageUI");
	m_pHp->Set_Size(_vec3{ 160.f, 160.f, 0.f });
	m_pHp->Set_Pos(_vec3{ 1716.f, -864.f, 0.f });
	m_pHp->Set_RenderID(RENDER_UI);
	m_pHp->Set_PassIndex(14);
	m_pHp->Set_TextureIndex(1);
	m_pHp->Set_Amount(1.f);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_Hp", pGameObject), );
	m_pHp->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pChargeframe = pGameObject->Is<CImageUI>();
	m_pChargeframe->Set_Image(L"CW_ImageUI");
	m_pChargeframe->Set_Size(_vec3{ 270.f, 38.f, 0.f });
	m_pChargeframe->Set_Pos(_vec3{ 1450.f, -940.f, 0.f });
	m_pChargeframe->Set_RenderID(RENDER_UI);
	m_pChargeframe->Set_PassIndex(8);
	m_pChargeframe->Set_TextureIndex(2);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_Chargeframe", pGameObject), );
	m_pChargeframe->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pCharge = pGameObject->Is<CImageUI>();
	m_pCharge->Set_Image(L"CW_ImageUI");
	m_pCharge->Set_Size(_vec3{ 270.f, 38.f, 0.f });
	m_pCharge->Set_Pos(_vec3{ 1450.f, -940.f, 0.f });
	m_pCharge->Set_RenderID(RENDER_UI);
	m_pCharge->Set_PassIndex(10);
	m_pCharge->Set_TextureIndex(2);
	m_pCharge->Set_Amount(0.f);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_Charge", pGameObject), );
	m_pCharge->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkill[0] = pGameObject->Is<CImageUI>();
	m_pSkill[0]->Set_Image(L"CW_ImageUI");
	m_pSkill[0]->Set_Size(_vec3{ 55.f, 55.f, 0.f });
	m_pSkill[0]->Set_Pos(_vec3{ 1658.f, -877.f, 0.f });
	m_pSkill[0]->Set_RenderID(RENDER_PRIUI);
	m_pSkill[0]->Set_PassIndex(2);
	m_pSkill[0]->Set_TextureIndex(3);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_Skill0", pGameObject), );
	m_pSkill[0]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkill[1] = pGameObject->Is<CImageUI>();
	m_pSkill[1]->Set_Image(L"CW_ImageUI");
	m_pSkill[1]->Set_Size(_vec3{ 55.f, 55.f, 0.f });
	m_pSkill[1]->Set_Pos(_vec3{ 1588.f, -877.f, 0.f });
	m_pSkill[1]->Set_RenderID(RENDER_PRIUI);
	m_pSkill[1]->Set_PassIndex(2);
	m_pSkill[1]->Set_TextureIndex(4);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_Skill1", pGameObject), );
	m_pSkill[1]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkill[2] = pGameObject->Is<CImageUI>();
	m_pSkill[2]->Set_Image(L"CW_ImageUI");
	m_pSkill[2]->Set_Size(_vec3{ 55.f, 55.f, 0.f });
	m_pSkill[2]->Set_Pos(_vec3{ 1518.f, -877.f, 0.f });
	m_pSkill[2]->Set_RenderID(RENDER_PRIUI);
	m_pSkill[2]->Set_PassIndex(2);
	m_pSkill[2]->Set_TextureIndex(5);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_Skill2", pGameObject), );
	m_pSkill[2]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkill[3] = pGameObject->Is<CImageUI>();
	m_pSkill[3]->Set_Image(L"CW_ImageUI");
	m_pSkill[3]->Set_Size(_vec3{ 60.f, 60.f, 0.f });
	m_pSkill[3]->Set_Pos(_vec3{ 1375.f, -927.f, 0.f });
	m_pSkill[3]->Set_RenderID(RENDER_PRIUI);
	m_pSkill[3]->Set_PassIndex(2);
	m_pSkill[3]->Set_TextureIndex(6);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_Skill3", pGameObject), );
	m_pSkill[3]->AddRef();

	//////////////////////////////////////////////////////////////////////////

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkillBox[0] = pGameObject->Is<CImageUI>();
	m_pSkillBox[0]->Set_Image(L"MC_ImageUI");
	m_pSkillBox[0]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pSkillBox[0]->Set_Pos(_vec3{ 1650.f, -870.f, 0.f });
	m_pSkillBox[0]->Set_RenderID(RENDER_MENUUI);
	m_pSkillBox[0]->Set_PassIndex(6);
	m_pSkillBox[0]->Set_TextureIndex(8);
	m_pSkillBox[0]->Set_Amount(1.f);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_SkillBox0", pGameObject), );
	m_pSkillBox[0]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkillBox[1] = pGameObject->Is<CImageUI>();
	m_pSkillBox[1]->Set_Image(L"MC_ImageUI");
	m_pSkillBox[1]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pSkillBox[1]->Set_Pos(_vec3{ 1580.f, -870.f, 0.f });
	m_pSkillBox[1]->Set_RenderID(RENDER_MENUUI);
	m_pSkillBox[1]->Set_PassIndex(6);
	m_pSkillBox[1]->Set_TextureIndex(8);
	m_pSkillBox[1]->Set_Amount(1.f);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_SkillBox1", pGameObject), );
	m_pSkillBox[1]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkillBox[2] = pGameObject->Is<CImageUI>();
	m_pSkillBox[2]->Set_Image(L"MC_ImageUI");
	m_pSkillBox[2]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pSkillBox[2]->Set_Pos(_vec3{ 1510.f, -870.f, 0.f });
	m_pSkillBox[2]->Set_RenderID(RENDER_MENUUI);
	m_pSkillBox[2]->Set_PassIndex(6);
	m_pSkillBox[2]->Set_TextureIndex(8);
	m_pSkillBox[2]->Set_Amount(1.f);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_SkillBox2", pGameObject), );
	m_pSkillBox[2]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkillBox[3] = pGameObject->Is<CImageUI>();
	m_pSkillBox[3]->Set_Image(L"MC_ImageUI");
	m_pSkillBox[3]->Set_Size(_vec3{ 80.f, 80.f, 0.f });
	m_pSkillBox[3]->Set_Pos(_vec3{ 1365.f, -920.f, 0.f });
	m_pSkillBox[3]->Set_RenderID(RENDER_MENUUI);
	m_pSkillBox[3]->Set_PassIndex(6);
	m_pSkillBox[3]->Set_TextureIndex(8);
	m_pSkillBox[3]->Set_Amount(1.f);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_SkillBox3", pGameObject), );
	m_pSkillBox[3]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkillBox[4] = pGameObject->Is<CImageUI>();
	m_pSkillBox[4]->Set_Image(L"MC_ImageUI");
	m_pSkillBox[4]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pSkillBox[4]->Set_Pos(_vec3{ 1650.f, -870.f, 0.f });
	m_pSkillBox[4]->Set_RenderID(RENDER_UI);
	m_pSkillBox[4]->Set_PassIndex(7);
	m_pSkillBox[4]->Set_TextureIndex(8);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_SkillBox4", pGameObject), );
	m_pSkillBox[4]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkillBox[5] = pGameObject->Is<CImageUI>();
	m_pSkillBox[5]->Set_Image(L"MC_ImageUI");
	m_pSkillBox[5]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pSkillBox[5]->Set_Pos(_vec3{ 1580.f, -870.f, 0.f });
	m_pSkillBox[5]->Set_RenderID(RENDER_UI);
	m_pSkillBox[5]->Set_PassIndex(7);
	m_pSkillBox[5]->Set_TextureIndex(8);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_SkillBox5", pGameObject), );
	m_pSkillBox[5]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkillBox[6] = pGameObject->Is<CImageUI>();
	m_pSkillBox[6]->Set_Image(L"MC_ImageUI");
	m_pSkillBox[6]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pSkillBox[6]->Set_Pos(_vec3{ 1510.f, -870.f, 0.f });
	m_pSkillBox[6]->Set_RenderID(RENDER_UI);
	m_pSkillBox[6]->Set_PassIndex(7);
	m_pSkillBox[6]->Set_TextureIndex(8);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_SkillBox6", pGameObject), );
	m_pSkillBox[6]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pSkillBox[7] = pGameObject->Is<CImageUI>();
	m_pSkillBox[7]->Set_Image(L"MC_ImageUI");
	m_pSkillBox[7]->Set_Size(_vec3{ 80.f, 80.f, 0.f });
	m_pSkillBox[7]->Set_Pos(_vec3{ 1365.f, -920.f, 0.f });
	m_pSkillBox[7]->Set_RenderID(RENDER_UI);
	m_pSkillBox[7]->Set_PassIndex(7);
	m_pSkillBox[7]->Set_TextureIndex(8);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_SkillBox7", pGameObject), );
	m_pSkillBox[7]->AddRef();

	//////////////////////////////////////////////////////////////////////////

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pKey[0] = pGameObject->Is<CImageUI>();
	m_pKey[0]->Set_Image(L"CW_ImageUI");
	m_pKey[0]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pKey[0]->Set_Pos(_vec3{ 1650.f, -800.f, 0.f });
	m_pKey[0]->Set_RenderID(RENDER_MENUUI);
	m_pKey[0]->Set_PassIndex(2);
	m_pKey[0]->Set_TextureIndex(9);
	m_pKey[0]->Set_Color(_vec4(255.f, 213.f, 64.f, 255.f));
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_Key0", pGameObject), );
	m_pKey[0]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pKey[1] = pGameObject->Is<CImageUI>();
	m_pKey[1]->Set_Image(L"CW_ImageUI");
	m_pKey[1]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pKey[1]->Set_Pos(_vec3{ 1370.f, -850.f, 0.f });
	m_pKey[1]->Set_RenderID(RENDER_MENUUI);
	m_pKey[1]->Set_PassIndex(2);
	m_pKey[1]->Set_TextureIndex(10);
	m_pKey[1]->Set_Color(_vec4(255.f, 213.f, 64.f, 255.f));
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_Key1", pGameObject), );
	m_pKey[1]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pKey[2] = pGameObject->Is<CImageUI>();
	m_pKey[2]->Set_Image(L"CW_ImageUI");
	m_pKey[2]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pKey[2]->Set_Pos(_vec3{ 1510.f, -800.f, 0.f });
	m_pKey[2]->Set_RenderID(RENDER_MENUUI);
	m_pKey[2]->Set_PassIndex(2);
	m_pKey[2]->Set_TextureIndex(11);
	m_pKey[2]->Set_Color(_vec4(255.f, 213.f, 64.f, 255.f));
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_Key2", pGameObject), );
	m_pKey[2]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pKey[3] = pGameObject->Is<CImageUI>();
	m_pKey[3]->Set_Image(L"CW_ImageUI");
	m_pKey[3]->Set_Size(_vec3{ 60.f, 60.f, 0.f });
	m_pKey[3]->Set_Pos(_vec3{ 1585.f, -805.f, 0.f });
	m_pKey[3]->Set_RenderID(RENDER_MENUUI);
	m_pKey[3]->Set_PassIndex(2);
	m_pKey[3]->Set_TextureIndex(12);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_Key3", pGameObject), );
	m_pKey[3]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pKey[4] = pGameObject->Is<CImageUI>();
	m_pKey[4]->Set_Image(L"CW_ImageUI");
	m_pKey[4]->Set_Size(_vec3{ 70.f, 70.f, 0.f });
	m_pKey[4]->Set_Pos(_vec3{ 1580.f, -800.f, 0.f });
	m_pKey[4]->Set_RenderID(RENDER_UI);
	m_pKey[4]->Set_PassIndex(2);
	m_pKey[4]->Set_TextureIndex(13);
	m_pKey[4]->Set_Color(_vec4(255.f, 213.f, 64.f, 255.f));
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_Key4", pGameObject), );
	m_pKey[4]->AddRef();

	//////////////////////////////////////////////////////////////////////////

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[0] = pGameObject->Is<CImageUI>();
	m_pDeadUI[0]->Set_Image(L"CW_ImageUI");
	m_pDeadUI[0]->Set_Size(_vec3{ 400.f, 400.f, 0.f });
	m_pDeadUI[0]->Set_Pos(_vec3{ 1370.f, -540.f, 0.f });
	m_pDeadUI[0]->Set_RenderID(RENDER_MENUUI);
	m_pDeadUI[0]->Set_PassIndex(11);
	m_pDeadUI[0]->Set_TextureIndex(14);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_Dead", pGameObject), );
	m_pDeadUI[0]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[1] = pGameObject->Is<CImageUI>();
	m_pDeadUI[1]->Set_Image(L"CW_ImageUI");
	m_pDeadUI[1]->Set_Size(_vec3{ 400.f, 400.f, 0.f });
	m_pDeadUI[1]->Set_Pos(_vec3{ 1370.f, -540.f, 0.f });
	m_pDeadUI[1]->Set_RenderID(RENDER_UI);
	m_pDeadUI[1]->Set_PassIndex(12);
	m_pDeadUI[1]->Set_TextureIndex(14);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_Dead2", pGameObject), );
	m_pDeadUI[1]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[2] = pGameObject->Is<CImageUI>();
	m_pDeadUI[2]->Set_Image(L"CW_ImageUI");
	m_pDeadUI[2]->Set_Size(_vec3{ 400.f, 400.f, 0.f });
	m_pDeadUI[2]->Set_Pos(_vec3{ 1370.f, -540.f, 0.f });
	m_pDeadUI[2]->Set_RenderID(RENDER_UI);
	m_pDeadUI[2]->Set_PassIndex(13);
	m_pDeadUI[2]->Set_TextureIndex(14);
	m_pDeadUI[2]->Set_Color(_vec4(231.f, 76.f, 60.f, 255.f));
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_Dead3", pGameObject), );
	m_pDeadUI[2]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[3] = pGameObject->Is<CImageUI>();
	m_pDeadUI[3]->Set_Image(L"CW_ImageUI");
	m_pDeadUI[3]->Set_Size(_vec3{ 60.f, 60.f, 0.f });
	m_pDeadUI[3]->Set_Pos(_vec3{ 1540.f, -860.f, 0.f });
	m_pDeadUI[3]->Set_RenderID(RENDER_MENUUI);
	m_pDeadUI[3]->Set_PassIndex(2);
	m_pDeadUI[3]->Set_TextureIndex(13);
	m_pDeadUI[3]->Set_Color(236.f, 216.f, 162.f, 255.f);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_Dead4", pGameObject), );
	m_pDeadUI[3]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[4] = pGameObject->Is<CImageUI>();
	m_pDeadUI[4]->Set_Image(L"CW_ImageUI");
	m_pDeadUI[4]->Set_Size(_vec3{ 40.f, 40.f, 0.f });
	m_pDeadUI[4]->Set_Pos(_vec3{ 1550.f, -870.f, 0.f });
	m_pDeadUI[4]->Set_RenderID(RENDER_MENUUI);
	m_pDeadUI[4]->Set_PassIndex(2);
	m_pDeadUI[4]->Set_TextureIndex(18);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_Dead5", pGameObject), );
	m_pDeadUI[4]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pDeadUI[5] = pGameObject->Is<CImageUI>();
	m_pDeadUI[5]->Set_Image(L"CW_ImageUI");
	m_pDeadUI[5]->Set_Size(_vec3{ 128.f, 128.f, 0.f });
	m_pDeadUI[5]->Set_Pos(_vec3{ 1506.f, -830.f, 0.f });
	m_pDeadUI[5]->Set_RenderID(RENDER_MENUUI);
	m_pDeadUI[5]->Set_PassIndex(2);
	m_pDeadUI[5]->Set_TextureIndex(15);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CW_Dead6", pGameObject), );
	m_pDeadUI[5]->AddRef();

	for (_int i = 0; i < 6; ++i)
	{
		m_pDeadUI[i]->Set_Visible(false);
		m_pDeadUI[i]->Change_Size_From_To(0.1f, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f));
	}

	Engine::Add_Layer(pLayerTag, pLayer);
}

void CCodyWizard::Update_UI(const _float & fTimeDelta)
{
	HP_UI(fTimeDelta);
	Skill_UI(fTimeDelta);
	Dead_UI(fTimeDelta);
}

void CCodyWizard::HP_UI(const _float & fTimeDelta)
{
	m_pHp->Set_Amount(_float(m_iHp) / m_iMaxHp);
	m_pCharge->Set_Amount(_float(m_iHitCount) / m_iHitCountMax);
	_float test = m_pHp->Get_Amount();
}

void CCodyWizard::Skill_UI(const _float & fTimeDelta)
{
	if (Engine::Key_Down(DIK_4))
	{
		for (_int i = 0; i < 4; ++i)
			m_pSkillBox[i]->Set_Amount(0.f);
	}
	if (m_pSkillBox[3]->Get_Amount() >= 1.f)
		m_pKey[1]->Set_Color(_vec4(255.f, 213.f, 64.f, 255.f));
	else
		m_pKey[1]->Set_Color(_vec4(155.f, 155.f, 155.f, 255.f));

	if (m_pSkillBox[2]->Get_Amount() >= 1.f)
		m_pKey[2]->Set_Color(_vec4(255.f, 213.f, 64.f, 255.f));
	else
		m_pKey[2]->Set_Color(_vec4(155.f, 155.f, 155.f, 255.f));

	if (m_pSkillBox[1]->Get_Amount() >= 1.f)
	{
		m_pKey[3]->Set_Color(_vec4(255.f, 255.f, 255.f, 255.f));
		m_pKey[4]->Set_Color(_vec4(255.f, 213.f, 64.f, 255.f));
	}
	else
	{
		m_pKey[3]->Set_Color(_vec4(155.f, 155.f, 155.f, 255.f));
		m_pKey[4]->Set_Color(_vec4(155.f, 155.f, 155.f, 255.f));
	}

}

void CCodyWizard::Dead_UI(const _float & fTimeDelta)
{
	if (!m_bDeadUI)
		return;

	if (Engine::Key_Pressing(DIK_E) && m_pDeadUI[0]->Get_Amount() < 1.f)
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
			m_pSkillBox[i]->Set_Visible(true);

		for (_int i = 0; i < 5; ++i)
			m_pKey[i]->Set_Visible(true);
	}
}

void CCodyWizard::KeyEvent_UI(CImageUI * pUI)
{
	if (pUI == nullptr)
		return;

	pUI->Change_Color_From_To(1.f, _vec4(255.f, 213.f, 64.f, 255.f), _vec4(255.f, 255.f, 255.f, 255.f));
}

CCodyWizard* CCodyWizard::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CCodyWizard*	pInstance = new CCodyWizard(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CCodyWizard::Free()
{
	Safe_Release(m_pProfile);
	Safe_Release(m_pHp);
	Safe_Release(m_pCharge);
	Safe_Release(m_pChargeframe);
	for (_int i = 0; i < 4; ++i)
		Safe_Release(m_pSkill[i]);

	for (_int i = 0; i < 8; ++i)
		Safe_Release(m_pSkillBox[i]);

	for (_int i = 0; i < 5; ++i)
		Safe_Release(m_pKey[i]);

	for (_int i = 0; i < 6; ++i)
		Safe_Release(m_pDeadUI[i]);

	Safe_Release(m_pCone);
	Safe_Release(m_pCone2);

	Safe_Release(m_pLOrb);
	Safe_Release(m_pROrb);

	CCodyWizard_Anim::DestroyInstance();
	CPlayer::Free();
}

HRESULT CCodyWizard::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;
	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = tTemp;
	// CDynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_PLAYER, L"Cody_Wizard"));
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
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_VTF"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Dissolve"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	m_pTransformCom->Set_Pos(tTemp.vPos);

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

void CCodyWizard::Key_Input(const _float& fTimeDelta)
{

	if (Engine::Key_Pressing(DIK_L))
		m_vSavePoint = m_pTransformCom->Get_Position();


	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
		m_iKeyState |= MOVE_UP;

	if (Engine::Get_DIKeyState(DIK_S) & 0x80)
		m_iKeyState |= MOVE_DOWN;

	if (Engine::Get_DIKeyState(DIK_D) & 0x80)
		m_iKeyState |= MOVE_RIGHT;

	if (Engine::Get_DIKeyState(DIK_A) & 0x80)
		m_iKeyState |= MOVE_LEFT;

	if (Engine::Key_Down(DIM_LB))
	{
		KeyEvent_UI(m_pSkillBox[0]);
		m_iKeyState |= (ATT | BASIC);
		//m_eAtt_State = ATT_BASIC;
		BasicShoot_Light(35.f);
	}

	else if (Engine::Key_Down(DIM_RB) && m_fIceOrbCoolTime <= 0.f)
	{
		KeyEvent_UI(m_pSkillBox[2]);
		m_iKeyState |= (ATT | ORB);
		//m_eAtt_State = ATT_ORB;
	}

	else if (Engine::Key_Pressing(DIM_MB) && m_fUltiAttTime > 0.f)
	{
		KeyEvent_UI(m_pSkillBox[3]);
		m_iKeyState |= (ATT | ULTIMATE);
		//m_eAtt_State = ATT_ULTIMATE;
	}


	if (Engine::Key_Down(DIK_LSHIFT) && m_fDashCoolTime <= 0.f)
	{
		KeyEvent_UI(m_pSkillBox[1]);
		m_iKeyState |= DASH;
	}

}

HRESULT CCodyWizard::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int& iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	if (m_bStencil)
		pEffect->SetVector("g_vColor", &_vec4(175.f * 2.5f, 255.f * 2.5f, 225.f * 2.5f, 255.f));
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
	pEffect->SetFloat("g_fMaterialIndex", 0.f);

	return S_OK;
}

void CCodyWizard::Set_State(const _float & fTimeDelta)
{
	switch (m_ePL_STATE)
	{
	case CCodyWizard::PL_IDLE:
		Idle_State(fTimeDelta);
		break;
	case CCodyWizard::PL_MOVE:
		Move_State(fTimeDelta);
		break;
	case CCodyWizard::PL_ATT:
		Att_State(fTimeDelta);
		break;
	case CCodyWizard::PL_DASH:
		Dash_State(fTimeDelta);
		break;
	case CCodyWizard::PL_HIT:
		Hit_State(fTimeDelta);
		break;
	case CCodyWizard::PL_DEAD:
		Dead_State(fTimeDelta);
		break;
	}
}

void CCodyWizard::Ready_Light()
{
	D3DLIGHT9				tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	tLightInfo.Position = _vec3(38.56f, 8.54f, 2.5f);
	tLightInfo.Range = 1.5f;


	tLightInfo.Diffuse = D3DXCOLOR(0.3f, 0.4f, 1.f * 1.3f, 0.5f);
	m_pCodyLight = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0);

	tLightInfo.Diffuse = D3DXCOLOR(0.3f, 0.4f, 1.f * 1.3f, 0.5f);
	m_pCodyLight2 = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 1);
}

void CCodyWizard::Update_Light(const _float& fTimeDelta)
{
	_matrix matWorld, matWorld2;
	const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("RightAttach");
	const Engine::D3DXFRAME_DERIVED* pFrame2 = m_pMeshCom->Get_FrameByName("LeftAttach");
	const _matrix* pMatrix = &pFrame->CombinedTranformationMatrix;
	const _matrix* pMatrix2 = &pFrame2->CombinedTranformationMatrix;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	matWorld = *pMatrix * matWorld;

	m_pTransformCom->Get_WorldMatrix(&matWorld2);
	matWorld2 = *pMatrix2 * matWorld2;

	m_vRightOrbPos = { matWorld._41, matWorld._42, matWorld._43 };
	m_vLeftOrbPos = { matWorld2._41, matWorld2._42, matWorld2._43 };

	if (!m_bAttLight)
		m_pCodyLight->Set_LightPos(m_vRightOrbPos);
	else
	{
		m_fAttLight += fTimeDelta;

		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_LOOK, &vPos);
		D3DXVec3Normalize(&vPos, &vPos);
		m_pCodyLight->Set_LightAddPos(vPos * fTimeDelta * m_fLightSpeed);
		m_pCodyLight->Set_LightSize(3.5f);

		if (m_fAttLight > 0.2f)
		{
			m_pCodyLight->Set_LightPos(m_vRightOrbPos);
			m_pCodyLight->Set_LightSize(1.5f);
			m_fAttLight = 0.f;
			m_bAttLight = false;
		}
	}

	if (!m_bOrbAttLight)
	{
		m_pCodyLight2->Set_LightPos(m_vLeftOrbPos);
		m_pCodyLight2->Set_LightSize(1.5f);
	}
}

void CCodyWizard::BasicShoot_Light(_float fSpeed)
{
	m_bAttLight = true;
	m_pCodyLight->Set_LightSize(1.5f);
	m_pCodyLight->Set_LightPos(m_vRightOrbPos);
	m_fLightSpeed = fSpeed;
}

void CCodyWizard::Move(const _float & fTimeDelta, _float3 * vResultPos, _float3 * vResultDir, _float fRotSpeed)
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

	if (nullptr != vResultPos)
		*vResultPos = vPos;
	if (nullptr != vResultDir)
		*vResultDir = vDir;
}

void CCodyWizard::Idle_State(const _float & fTimeDelta)
{
	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);
	pBody->setLinearVelocity(PxVec3(0, 0, 0));

	if (m_bDead)
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
		if (m_iKeyState & BASIC)
		{
			m_eAtt_State = ATT_BASIC;
		}
		else if (m_iKeyState & ORB)
		{
			m_eAtt_State = ATT_ORB;
		}
		else
		{
			m_eAtt_State = ATT_ULTIMATE;
		}
	}
	else if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
	{
		m_ePL_STATE = PL_MOVE;
		m_ePre_State = PL_IDLE;
	}
}

void CCodyWizard::Move_State(const _float & fTimeDelta)
{
	if (m_bDead)
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
	else if ((m_iKeyState & DASH))
	{
		m_ePL_STATE = PL_DASH;
		m_ePre_State = PL_IDLE;
	}
	else if (m_iKeyState & ATT)
	{
		m_ePL_STATE = PL_ATT;
		m_ePre_State = PL_MOVE;
		if (m_iKeyState & BASIC)
			m_eAtt_State = ATT_BASIC;
		else if (m_iKeyState & ORB)
			m_eAtt_State = ATT_ORB;
		else
			m_eAtt_State = ATT_ULTIMATE;
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

void CCodyWizard::Att_State(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		m_ePL_STATE = PL_DEAD;
		Clear(PL_DEAD);
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
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_ATT;
		m_bAttEnd = false;
		m_bAttStart = false;
		return;
	}

	switch (m_eAtt_State)
	{
	case CCodyWizard::ATT_BASIC:
		BasicAtt(fTimeDelta);
		break;
	case CCodyWizard::ATT_ULTIMATE:
		Ultimate(fTimeDelta);
		break;
	case CCodyWizard::ATT_ORB:
		OrbAtt(fTimeDelta);
		break;
	}
}

void CCodyWizard::Dash_State(const _float & fTimeDelta)
{
	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	// Blink애니메이션이 종료되면 m_bDashEnd는 true가 된다.
	if (m_bDead)
	{
		m_ePL_STATE = PL_DEAD;
		Clear(PL_DEAD);
		return;
	}
	else if (m_bDashEnd)
	{
		m_bDashEnd = false;
		m_bBlinkAnimation = false;
		m_ePL_STATE = PL_IDLE;
		m_ePre_State = PL_DASH;
		m_fDashDis = 0.0f;


		//PxVec3 vDir = pBody->getLinearVelocity();
		//pBody->setLinearVelocity(vDir * 0.5f);

		pBody->setLinearVelocity(PxVec3(0, 0, 0));
		return;
	}

	// 피직스로 이동을 끈다.
	m_bMovePysics = false;


	// 이동방향, 현재위치, 이동한거리
	_float3 vLook, vPos, vDis;

	// 목표만큼의 거리를 이동하면 Blink애니메이션 재생
	if (m_fDashDis >= m_fDashGoalDis)
	{
		m_bBlinkAnimation = true;
		m_bBlinkRender = false;
	}
	else
	{
		if (m_bDashEffect)
		{

			Set_DashEffect(1);
			m_bDashEffect = false;
		}


		// 대쉬중일때 랜더를 끈다.
		m_bBlinkRender = true;

		// 이동방향
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		vLook.y = 0.0f;
		D3DXVec3Normalize(&vLook, &vLook);

		// 플레이어의 현재 위치
		vPos = m_pTransformCom->Get_Position();

		// 이동거리
		vDis = vLook * m_fMaxSpeed * m_fDashRatio * fTimeDelta;

		// 이동거리의 누적값
		m_fDashDis += D3DXVec3Length(&vDis);

		// 이동
		m_pTransformCom->Set_Pos(vPos + vDis);
	}
}

void CCodyWizard::Hit_State(const _float & fTimeDelta)
{
	if (m_bDead)
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

void CCodyWizard::Dead_State(const _float & fTimeDelta)
{
	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);
	pBody->setLinearVelocity(PxVec3(0, 0, 0));
	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

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
			m_pSkillBox[i]->Set_Visible(false);

		for (_int i = 0; i < 5; ++i)
			m_pKey[i]->Set_Visible(false);
	}
}

void CCodyWizard::Recover(const _float & fTimeDelta)
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

void CCodyWizard::BasicAtt(const _float & fTimeDelta)
{
	m_fAttTime += fTimeDelta;

	if (m_iKeyState & ORB)
	{
		m_eAtt_State = ATT_ORB;
		return;
	}
	else if (m_iKeyState & ULTIMATE)
	{
		m_eAtt_State = ATT_ULTIMATE;
		return;
	}

	if (!m_bAttStart)
	{
		m_bAttStart = true;
		m_fAttTime = 0.0f;
		// 충돌리스트 초기화 애니메이션 매니저에서도 초기화하는곳이 있음

	}
	else if (m_iKeyState & ATT && m_fAttTime >= m_fAttLimitTime)
	{
		m_fAttTime = 0.0f;
		CCodyWizard_Anim::GetInstance()->Set_AttState();
	}

	if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
	{
		auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);
		pBody->setLinearVelocity(PxVec3(0, 0, 0));
		Move(fTimeDelta, nullptr, nullptr, 360.0f);
	}
	else
	{
		auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);
		pBody->setLinearVelocity(PxVec3(0, 0, 0));
	}

}

void CCodyWizard::Ultimate(const _float & fTimeDelta)
{
	//if (!(m_iKeyState & ULTIMATE))
	//	m_bAttEnd = true;

	m_fUltiAttTime -= fTimeDelta;

	m_iHitCount = 0;

	if (m_fUltiAttTime <= 0.f)
	{

		m_pCone->Set_Active(false);
		m_pCone2->Set_Active(false);
		m_bAttEnd = true;
		//m_fUltiAttTime = 3.f;
		m_bUltEffect = false;
		m_bUltimate = false;
		m_bUltimateStart = true;
		m_pAttackTrigger->Set_Interactable(false);

		return;
	}
	//if (m_bUltimateStart)
	//{
	//	m_bUltimateStart = false;
	//}
	//else
	//{
	//	m_pAttackTrigger->Set_Scale(1 / m_fLastDist);
	//}
	//m_fLastDist = m_fDistance;

	_vec3 vPos = m_pTransformCom->Get_Position();


	_vec3 vDir;
	m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	m_pAttackTrigger->Set_Rotation({ 0.f,atan2f(vDir.x,vDir.z),0.f });
	m_pAttackTrigger->Set_Pos(vPos + vDir*(m_fDistance / 2));
	m_pAttackTrigger->Set_Scale(m_fDistance / 2);
	m_pAttackTrigger->Set_Interactable(true);

	m_bUltimate = true;
	m_pCone->Set_Active(true);
	m_pCone2->Set_Active(true);
	m_bUltEffect = true;

	if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
	{
		auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);
		pBody->setLinearVelocity(PxVec3(0, 0, 0));
		Move(fTimeDelta, nullptr, nullptr, 360.0f);
	}
	else
	{
		auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);
		pBody->setLinearVelocity(PxVec3(0, 0, 0));
	}

}

void CCodyWizard::OrbAtt(const _float & fTimeDelta)
{
	if (m_iKeyState & ULTIMATE)
	{
		m_eAtt_State = ATT_ULTIMATE;
		return;
	}

	if (m_iKeyState & (MOVE_DOWN | MOVE_UP | MOVE_LEFT | MOVE_RIGHT))
	{
		auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

		pBody->setLinearVelocity(PxVec3(0, 0, 0));
	}

	//m_bIceCoolTime = true;
}

void CCodyWizard::CoolTime(const _float & fTimeDelta)
{

	m_fDashCoolTime -= fTimeDelta;
	if (m_fDashCoolTime < 0.f)
	{
		m_fDashCoolTime = 0.f;
	}

	m_fIceOrbCoolTime -= fTimeDelta;
	if (m_fIceOrbCoolTime < 0.f)
	{
		m_fIceOrbCoolTime = 0.f;
	}

}

void CCodyWizard::CreateIceOrbLB(_float fLifeDir)
{
	CLayer*				pLayer = nullptr;
	pLayer = Engine::Get_Layer(L"GameObject");
	Engine::CGameObject*		pGameObject = nullptr;

	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	OBJINFO pObj;
	pObj.vScale = { 0.01f, 0.01f, 0.01f };
	pObj.vAngle = { 0.f, 0.f, 0.f };
	pObj.vPos = m_pTransformCom->Get_Position() + vLook * 1.f;
	pObj.eSceneID = m_tObjInfo.eSceneID;

	_tchar tagTemp[MAX_PATH];
	wsprintf(tagTemp, L"IceOrbAtt %d", m_iIceOrbCount);


	pGameObject = CCodyWizard_IceOrbAtt::Create(m_pGraphicDev, m_iDamage, vLook, &pObj, fLifeDir);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject, Engine::Get_Scene()), );
	Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eIceOrb, FilterGroup::eGround | FilterGroup::eWall);

	m_iIceOrbCount++;

}

void CCodyWizard::CreateIceOrbRB()
{

	CLayer*				pLayer = nullptr;
	pLayer = Engine::Get_Layer(L"GameObject");
	Engine::CGameObject*		pGameObject = nullptr;

	OBJINFO pObj;
	pObj.vScale = { 0.01f, 0.01f, 0.01f };
	pObj.vAngle = { 0.f, 0.f, 0.f };
	pObj.vPos = m_pTransformCom->Get_Position() + _vec3(0.f, 1.f, 0.f);
	pObj.eSceneID = m_tObjInfo.eSceneID;

	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);


	_tchar tagTemp[MAX_PATH];
	wsprintf(tagTemp, L"IceOrbAtt %d", m_iIceOrbCount);


	pGameObject = CCodyWizard_IceOrbAttRB::Create(m_pGraphicDev, vLook, m_pCodyLight2, &pObj);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(tagTemp, pGameObject, Engine::Get_Scene()), );
	Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eIceOrb, FilterGroup::eGround | FilterGroup::eWall);

	m_bOrbAttLight = true;

	m_iIceOrbCount++;
}

void CCodyWizard::Set_IceCoolTime()
{
	m_fIceOrbCoolTime = m_fIceOrbCool;
	m_pSkillBox[2]->Set_CoolTime(m_fIceOrbCool);
}

void CCodyWizard::Set_DashCoolTime()
{
	m_fDashCoolTime = m_fDashCool;
	m_pSkillBox[1]->Set_CoolTime(m_fDashCool);
}


void CCodyWizard::Set_Dir(_float3 * vDir, _int * iRotDir, _float * fCosTh)
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
	if (m_iKeyState & MOVE_UP)
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
		*vDir = vCamDir;

	_float3 vTest = _float3(0.1f, 0.0f, 0.1f);
	D3DXVec3Normalize(&vTest, &vTest);
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

void CCodyWizard::Key_Reset()
{
	m_iKeyState = NULL;
}

void CCodyWizard::Clear(PLAYER_STATE eState)
{
	switch (eState)
	{
	case CCodyWizard::PL_IDLE:
		Clear_Idle();
		break;
	case CCodyWizard::PL_MOVE:
		Clear_Move();
		break;
	case CCodyWizard::PL_ATT:
		Clear_Att();
		break;
	case CCodyWizard::PL_DASH:
		Clear_Dash();
		break;
	case CCodyWizard::PL_HIT:
		Clear_Hit();
		break;
	case CCodyWizard::PL_DEAD:
		Clear_Dead();
		break;
	}
}

void CCodyWizard::Clear_Idle()
{
}

void CCodyWizard::Clear_Move()
{
}

void CCodyWizard::Clear_Att()
{
	m_bAttEnd = false;
	m_bAttStart = false;
	CCodyWizard_Anim::GetInstance()->Set_ClearIndex();
	m_pCone->Set_Active(false);
	m_pCone2->Set_Active(false);
	m_bUltEffect = false;
	m_bUltimate = false;
	m_bUltimateStart = true;
}

void CCodyWizard::Clear_Dash()
{
	m_bDashEnd = false;
	m_bBlinkAnimation = false;
	m_fDashDis = 0.0f;
	m_bMovePysics = true;
	m_bBlinkRender = false;
	m_bDashEffect = false;
}

void CCodyWizard::Clear_Hit()
{
	m_bHitEnd = false;
	m_bHit = false;
}

void CCodyWizard::Clear_Dead()
{
	Clear_Idle();
	Clear_Move();
	Clear_Att();
	Clear_Dash();
	Clear_Hit();
	m_bDead = false;
	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);
	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
}

void CCodyWizard::Create_Cone(const _tchar* pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, );

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = m_tObjInfo.eSceneID;
	tObj.vScale = { 0.01f, 0.01f, 0.07f };

	pGameObject = CCone::Create(m_pGraphicDev, &tObj);
	m_pCone = pGameObject->Is<CCone>();
	m_pCone->Set_Index(0);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cone", pGameObject), );
	m_pCone->AddRef();

	tObj.vScale = { 0.01f, 0.01f, 0.07f };
	pGameObject = CCone::Create(m_pGraphicDev, &tObj);
	m_pCone2 = pGameObject->Is<CCone>();
	m_pCone2->Set_Index(1);
	NULL_CHECK_RETURN(pGameObject, );
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Cone1", pGameObject), );
	m_pCone2->AddRef();

	Engine::Add_Layer(pLayerTag, pLayer);
}
