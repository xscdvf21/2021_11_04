#include "stdafx.h"
#include "FuseNPC.h"

#include "Export_Function.h"
#include "PowerSwitch.h"
#include "CodyCamera.h"
#include "MayCamera.h"
#include "Cody.h"
#include "May.h"

CFuseNPC::CFuseNPC(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
	, m_bRun(false)
	, m_bUse(false)
	, m_fDeltaTime(0.f)
	, m_iPattrunStart(0)
	, m_iAniIndex(0)
{
}

CFuseNPC::~CFuseNPC()
{
}

HRESULT CFuseNPC::Ready_Object(void * pArg, _uint iFlag, _uint iEvent, _bool bRun)
{
	m_bRun = bRun;
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	m_iEvent = iEvent;
	m_iFlag = iFlag;
	m_pMeshCom->Set_Animationset(iFlag);
	m_pMeshCom->Play_Animationset(0.5f);
	D3DXMatrixIdentity(&m_matCombinedMatrix);

	return S_OK;
}

_int CFuseNPC::Update_Object(const _float & fTimeDelta)
{
	if (Engine::Key_Pressing(DIK_F7))
	{
		m_fFlyTime = 0.0f;
		m_bJumpStart = true;
	}


	if (Engine::Key_Pressing(DIK_F6))
	{
		m_bWork = true;
	}


	if (m_bStart)
	{
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_pTransformCom->Get_Position(), 0.5f, TRIGGER_FUSE, FilterGroup::eFuse);

		m_bStart = false;
	}

	if (false == m_bRun)
	{
		MoveUse(fTimeDelta);
	}

	FuseEffect();


	if (true == m_bWork && true == m_bRun)
	{
		m_pMeshCom->Play_Animationset(fTimeDelta);
		switch (m_iEvent)
		{
		case 0:
			if (m_pMeshCom->Is_AnimationSetEnd(26.5f))
			{
				_vec3 vDir;
				m_bPlay = true;
				m_bRun = false;
				m_bWork = false;

				// Base
				_matrix matWorld;
				m_pTransformCom->Get_WorldMatrix(&matWorld);
				const D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName("Align");
				_matrix matBoneBase = pBone->CombinedTranformationMatrix;

				matBoneBase *= matWorld;

				m_pTransformCom->Set_Pos(_vec3(matBoneBase._41, matBoneBase._42, matBoneBase._43));
				m_pTrigger->Set_Pos(m_pTransformCom->Get_Position());

				m_pMeshCom->Set_Animationset(FuseCrab_Carried, 1.0f, 0.01f);
				m_pMeshCom->Play_Animationset(0.1f);
			}
			break;
		case 1:
			if (m_pMeshCom->Is_AnimationSetEnd(29.0f))
			{
				m_bRun = false;
				m_bWork = false;
				//m_pTransformCom->Set_Pos(_vec3(-18.335f, 45.22f, -29.22f));
				m_pTransformCom->Set_Pos(_vec3(-20.5572f, 45.5844f, -27.4320f));
				m_pTrigger->Set_Pos(m_pTransformCom->Get_Position());
				m_pMeshCom->Set_Animationset(FuseCrab_Mh, 1.0f, 0.01f);
				m_pMeshCom->Play_Animationset(0.1f);

				//m_pMeshCom->Set_Animationset(FuseCrab_Carried, 1.0f, 0.0f);
				//m_pMeshCom->Play_Animationset(0.1f);

				m_pTransformCom->Set_AngleZ(0.0f);
				m_pTransformCom->Set_AngleY(-90.0f);

				CCody* pCody = Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>();
				if(pCody)
					pCody->Set_FuseRender(true);
				CMay* pMay = Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay>();
				if(pMay)
					pMay->Set_FuseRender(true);

				m_bPlay = true;
			}
			break;
		case 2:
			if (m_pMeshCom->Is_AnimationSetEnd(29.0f))
			{
				if (m_pMeshCom->Is_AnimationSetEnd(26.5f))
				{
					_vec3 vDir;
					m_bPlay = true;
					m_bRun = false;
					m_bWork = false;
					// Base
					_matrix matWorld;
					m_pTransformCom->Get_WorldMatrix(&matWorld);
					const D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName("Align");
					_matrix matBoneBase = pBone->CombinedTranformationMatrix;

					matBoneBase *= matWorld;

					m_pTransformCom->Set_Pos(_vec3(matBoneBase._41, matBoneBase._42, matBoneBase._43));
					m_pTrigger->Set_Pos(m_pTransformCom->Get_Position());

					m_pMeshCom->Set_Animationset(FuseCrab_Carried, 1.0f, 0.01f);
					m_pMeshCom->Play_Animationset(0.1f);
				}
				break;
			}
			break;
		}
	}

	Fuse_JumpStart(fTimeDelta);
	Fuse_Jump(fTimeDelta);
	Fuse_Slide(fTimeDelta);

	if (true == m_bPlay)
	{
		m_pMeshCom->Play_Animationset(fTimeDelta);
	}

	Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	if (m_bDead)
	{
		Engine::StopNPlaySound(L"Play_World_Shed_Awakening_Interactable_FuseSocket_Light.bnk_1.wav", CSoundMgr::CHANNELID::Vacuum_Fuse_Light_On, 0.5f);
		CGameObject* pObj = Engine::Get_GameObject(L"GameObject", L"PowerSwitch");
		pObj->Is<CPowerSwitch>()->Set_Socket(m_iEvent, true);

		return OBJ_DEAD;
	}

	return OBJ_NOEVENT;
}

_int CFuseNPC::LateUpdate_Object(const _float & fTimeDelta)
{

	return _int();
}

void CFuseNPC::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 6);

	pEffect->End();

	Safe_Release(pEffect);
	FuseEffect();

}

HRESULT CFuseNPC::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"FuseNPC"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Set_Pos(tTemp.vPos);
	m_pTransformCom->Set_Angle(tTemp.vAngle);
	//m_pTransformCom->Set_AngleZ(D3DXToRadian(-90.0f));

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

HRESULT CFuseNPC::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	if (m_bCarried || m_bFuseDrop)
	{
		matWorld._41 = 0.0f;
		matWorld._42 = 0.0f;
		matWorld._43 = 0.0f;
	}

	matWorld *= m_matCombinedMatrix;

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));
	pEffect->SetFloat("g_fAmount", 0.f);
	pEffect->SetFloat("g_fMaterialIndex", 1.f);

	pEffect->CommitChanges();

	return S_OK;
}

void CFuseNPC::MoveUse(const _float & fTimeDelta)
{
	if (m_bCarried)
	{
		CDynamicMesh* pTargetMesh = (CDynamicMesh*)Engine::Get_Component(L"GameObject", m_tagTargetName, L"Com_Mesh", ID_STATIC);
		CTransform* pTransform = (CTransform*)Engine::Get_Component(L"GameObject", m_tagTargetName, L"Com_Transform", ID_DYNAMIC);
		NULL_CHECK_RETURN(pTargetMesh, );

		const D3DXFRAME_DERIVED* pBone = pTargetMesh->Get_FrameByName("Align");
		NULL_CHECK_RETURN(pBone, );

		_float4x4 matBone = pBone->CombinedTranformationMatrix;
		const _float4x4* matPlayer = pTransform->Get_WorldMatrix();

		m_pTransformCom->Set_Scale(_float3(1.0f, 1.0f, 1.0f));
		m_pTransformCom->Set_AngleY(D3DXToRadian(90.0f));
		m_matCombinedMatrix = matBone * *matPlayer;

		_float4x4 matWorld;
		m_pTransformCom->Get_WorldMatrix(&matWorld);

		matWorld *= matBone * *matPlayer;
		m_pTrigger->Set_Pos(PxMat44(matWorld).getPosition());
	}
}

void CFuseNPC::Fuse_Jump(const _float & fTimeDelta)
{
	if (true == m_bJump && m_iEvent == 1)
	{
		m_pMeshCom->Set_Animationset(FuseCrab_Mh, 1.0f, 0.0f);
		m_pTransformCom->Set_AngleY(D3DXToRadian(-270.0f));
		_vec3 vOut;
		D3DXVec3CatmullRom(&vOut, &_vec3(-20.5572f, 45.5844f - 60.0f, -27.4320f), &_vec3(-20.5572f, 45.5844f, -27.4320f), &_vec3(-10.6f, 48.111f, -29.057f), &_vec3(-10.6f, 48.111f - 40.0f, -29.057f), m_fFlyTime);
		m_pTransformCom->Set_Pos(vOut);
		m_pTrigger->Set_Pos(m_pTransformCom->Get_Position());

		m_fFlyTime += fTimeDelta;
		if (m_fFlyTime >= 1.0f)
		{
			m_bSlide = true;
			m_bJump = false;
			m_fFlyTime = 0.0f;
			return;
		}
	}
}

void CFuseNPC::Fuse_Slide(const _float & fTimeDelta)
{
	if (m_iEvent != 1)
		return;

	if (true == m_bSlide)
	{
		if (false == m_bSlideStart)
		{
			CCody* pCody = Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody>();
			if (pCody)
				pCody->Set_FuseRender(false);
			CMay* pMay = Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay>();
			if (pMay)
				pMay->Set_FuseRender(false);

			m_pMeshCom->Set_Animationset(FuseCrab_Carried, 1.0f, 0.1f);
			m_bSlideStart = true;
			m_pTransformCom->Set_AddAngleX(D3DXToRadian(20.0f));
		}

		_vec3 vSlidingDir;
		_vec3 vDir = _vec3(56.63f, 9.565f, -29.057f) - _vec3(-10.6f, 48.111f, -29.057f);
		D3DXVec3Normalize(&vDir, &vDir);
		vSlidingDir = vDir * 30.0f * fTimeDelta;

		m_pTransformCom->Move_Pos(&vSlidingDir, 1.0f, 1.0f);
		m_pTrigger->Set_Pos(m_pTransformCom->Get_Position());

		if (m_pTransformCom->Get_Position().y <= 9.565)
		{
			m_bSlide = false;
			m_bSlideJump = true;
			m_vSlideJumpStart = m_pTransformCom->Get_Position();
			m_fFlyTime = 0.0f;
			m_pTransformCom->Set_AddAngleX(D3DXToRadian(-20.0f));
		}
	}
	else if (m_bSlideJump == true)
	{
		_vec3 vOut;
		m_fFlyTime += fTimeDelta;
		if (m_fFlyTime >= 1.0f)
		{
			m_fFlyTime = 1.0f;
			m_bSlideJump = false;
		}
		D3DXVec3CatmullRom(&vOut, &_vec3(m_vSlideJumpStart.x, m_vSlideJumpStart.y - 60.0f, m_vSlideJumpStart.z), &_vec3(m_vSlideJumpStart.x, m_vSlideJumpStart.y, m_vSlideJumpStart.z), &_vec3(86.1f, -4.358f, m_vSlideJumpStart.z), &_vec3(86.1f, -4.358f - 60.0f, m_vSlideJumpStart.z), m_fFlyTime);
		m_pTransformCom->Set_Pos(vOut);
		m_pTrigger->Set_Pos(m_pTransformCom->Get_Position());
	}
}

void CFuseNPC::Fuse_JumpStart(const _float & fTimeDelta)
{
	if (m_bJumpStart && m_iEvent == 1)
	{
		m_pMeshCom->Set_Animationset(FuseCrab_Taunt, 1.0f, 0.1f);
		if (m_pMeshCom->Is_AnimationSetEnd(3.3f))
		{
			m_bJump = true;
			m_bJumpStart = false;
		}
	}
}

void CFuseNPC::Set_Carried(_tchar* tagTargetName)
{

	m_bCarried = true;
	lstrcpy(m_tagTargetName, tagTargetName);
}

void CFuseNPC::Set_Drop()
{
	//m_bDrop = false;
	m_bCarried = false;
	m_bRotationStop = false;
	m_bRotation = false;
	m_bFuseDrop = true;
	//m_pTransformCom->Set_AngleY(D3DXToRadian(0.0f));
	//m_pTransformCom->Set_Scale(_float3(0.01f, 0.01f, 0.01f));

	_float4x4 matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	matWorld._41 = 0.0f;
	matWorld._42 = 0.0f;
	matWorld._43 = 0.0f;

	matWorld *= m_matCombinedMatrix;

	m_pTrigger->Set_Pos(PxMat44(matWorld).getPosition());
	return;
}

void CFuseNPC::Set_TriggerOff()
{
	if (m_pTrigger)
		m_pTrigger->Set_Interactable(false);

}

void CFuseNPC::EventStart()
{
	m_fFlyTime = 0.0f;
	m_bJumpStart = true;
}

void CFuseNPC::FuseEffect()
{
	if (!m_bWork)
		return;

	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);

	_matrix matWorld;
	const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("Base");
	const _matrix* pMatrix = &pFrame->CombinedTranformationMatrix;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	matWorld = *pMatrix * matWorld;

	_vec3 vPos = { matWorld._41, matWorld._42, matWorld._43 };

	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
	tParticle.fRotateSpeed = 0.f;

	tParticle.vStartColor = { 85.f * 3.5f, 125.f* 3.5f, 255.f* 3.5f, 255.f * 3.5f };
	tParticle.vEndColor = { 255.f, 125.f, 255.f, 85.f };
	tParticle.vRot = { 0.f, 0.f, 0.f };
	tParticle.bGravity = true;
	tParticle.fGravityForce = 0.5f;

	for (_int i = 0; i < 5; ++i)
	{
		tParticle.fVelocity = CRandoms()(3.5f, 5.5f);
		tParticle.fLifeTime = CRandoms()(0.5f, 1.f);
		_float fRand = CRandoms()(30, 80) * 0.001f;
		tParticle.vStartSize = _vec3(fRand, fRand, fRand);

		tParticle.vDir = { CRandoms()(-1.f, 1.f),  CRandoms()(0.f, 1.f), CRandoms()(-1.f, 1.f) };
		tParticle.vPos = vPos + _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f));
		Engine::Emit_ImageParticle(tParticle, 15, 11);
	}

	for (_int i = 0; i < 3; ++i)
	{
		tParticle.fVelocity = CRandoms()(10.5f, 15.5f);
		tParticle.fLifeTime = CRandoms()(0.3f, 0.5f);
		tParticle.vStartSize = _vec3(0.02f, 0.02f, 0.02f);
		tParticle.vStartColor = { 85.f * 3.5f, 125.f* 3.5f, 255.f* 3.5f, 255.f };

		tParticle.vDir = { CRandoms()(-1.f, 1.f),  CRandoms()(0.f, 1.f), CRandoms()(-1.f, 1.f) };
		tParticle.vPos = vPos + _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f), CRandoms()(-0.3f, 0.3f));
		Engine::Emit_ImageParticle(tParticle, 15, 11);
	}

	PARTICLE tParticle2;
	Engine::Reset_Particle(tParticle2);
	Engine::Set_AnimState(tParticle2, 8, 8, 1.f);
	tParticle2.fAcc = 1.f;
	tParticle2.fCurrentTime = 0.f;
	tParticle2.vStartSize = _vec3(5.f, 5.f, 5.f);
	tParticle2.vEndSize = _vec3(5.f, 5.f, 5.f);
	tParticle2.fRotateSpeed = 0.f;
	tParticle2.vStartColor = { 30.f, 30.f, 30.f, 255.f * 1.5f };
	tParticle2.vEndColor = { 30.f, 30.f, 30.f, 0.f };
	tParticle2.vRot = { 0.f, 0.f, D3DXToRadian(CRandoms()(0.f, 360.f)) };
	tParticle2.fVelocity = 0.f;
	tParticle2.fLifeTime = 0.5f;
	tParticle2.vRot = { 0.f, 0.f, 0.f };

	tParticle2.vDir = { 0.f, 0.f, 0.f };
	tParticle2.vPos = vPos + _vec3(CRandoms()(-0.1f, 0.1f), 0.f, CRandoms()(-0.1f, 0.1f));
	Engine::Emit_ImageParticle(tParticle2, 23, 4);


	tParticle2.vStartSize = _vec3(2.f, 2.f, 2.f);
	tParticle2.vEndSize = _vec3(3.2f, 3.2f, 3.2f);
	tParticle2.fLifeTime = 0.4f;
	tParticle2.vStartColor = { 85.f * 3.5f, 125.f* 3.5f, 255.f* 3.5f, 255.f };
	tParticle2.vEndColor = { 255.f, 65.f, 35.f, 0.f };
	tParticle2.vPos = vPos + _vec3(CRandoms()(-0.1f, 0.1f), 0.f, CRandoms()(-0.1f, 0.1f));
	Engine::Emit_ImageParticle(tParticle2, 30, 26);
}

CFuseNPC * CFuseNPC::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iFlag, _uint iEvent, void * pArg, _bool bRun)
{
	CFuseNPC*	pInstance = new CFuseNPC(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg, iFlag, iEvent, bRun)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFuseNPC::Free(void)
{
	Engine::CGameObject::Free();
}
