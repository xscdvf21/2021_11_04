#include "stdafx.h"
#include "OgreChain.h"

#include "Export_Function.h"

COgreChain::COgreChain(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_bMoveStart(false),m_iAniIndex(0)
	, m_bOgreCol(false)
{
}

COgreChain::~COgreChain()
{
}

HRESULT COgreChain::Ready_Object(_vec3 vPos, void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	m_pTransformCom->Set_Scale(tTemp.vScale);
	//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(35.f));
	m_pTransformCom->Set_Pos(tTemp.vPos);

	PARTICLE tParticle;
	Engine::Reset_Particle(tParticle);
	Engine::Set_AnimState(tParticle, 8, 8, CRandoms()(5000.f, 6000.f));

	tParticle.vPos = m_pTransformCom->Get_Position() + vPos;
	tParticle.fAcc = 1.f;
	tParticle.fCurrentTime = 0.f;
	tParticle.fLifeTime = 10000.f;
	tParticle.fRotateSpeed = 0.f;
	tParticle.fVelocity = 0.f;
	tParticle.vStartColor = { 255.f * 2.f, 255.f * 2.f, 255.f * 2.f, 255.f * 1.5f };
	tParticle.vEndColor = { 255.f, 255.f, 255.f, 105.f };
	tParticle.vStartSize = _vec3(2.5f, 2.5f, 2.5f);
	tParticle.vEndSize = _vec3(2.5f, 2.5f, 2.5f);
	tParticle.vDir = { 0.f, 1.f, 0.f };

	m_pParticle = Engine::Emit_ImageParticle(tParticle, 5, 4);

	D3DLIGHT9				tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));


	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);


	tLightInfo.Diffuse = D3DXCOLOR(1.f * 0.8f, 0.7f * 0.8f, 0.f * 0.8f, 1.f);
	tLightInfo.Position = m_pTransformCom->Get_Position() + vPos;
	tLightInfo.Range = 7.f;
	m_pLight = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 42);

	return S_OK;
}

_int COgreChain::Update_Object(const _float & fTimeDelta)
{
	if (m_bOgreCol && !m_bMoveStart)
		Move(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int COgreChain::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void COgreChain::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 4);

	pEffect->End();

	Safe_Release(pEffect);
}

void COgreChain::Set_Sound()
{
}

HRESULT COgreChain::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	pComponent = m_pMeshCom = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"OgreChain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = static_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_fColDis, _vec4(50.f, 244.f, 234.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_VTF"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);


	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformCom->Set_Pos(tTemp.vPos);

	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();

	PxTransform tTransform(vPos.x, vPos.y, vPos.z);

	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxShape* pTrigger = pPhysics->createShape(PxBoxGeometry(1, 1, 1), *pMaterial, true);

	pTrigger->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pTrigger->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	pTrigger->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);;

	m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pTrigger, 1);

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
	// 그래비티를 끈다.
	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	// 질량설정
	pBody->setMass(0);
	//이동 제동력
	pBody->setName((char*)this);

	PxTransform Transform = pBody->getGlobalPose();

	PxQuat pxQ = Transform.q;

	PxTransform TriggerTransform(0, 0, 0);

	pTrigger->setLocalPose(TriggerTransform);

	return S_OK;
}

HRESULT COgreChain::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{

	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));
	pEffect->SetFloat("g_fAmount", 0.f);
	pEffect->CommitChanges();

	return S_OK;
}

void COgreChain::Move(const _float & fTimeDelta)
{

	//_vec3 vPos;
	//vPos = m_pTransformCom->Get_Position();

	//PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	//pxTransform.p.x = vPos.x;
	//pxTransform.p.y = vPos.y;
	//pxTransform.p.z = vPos.z;
	//_vec3 vRot = m_pTransformCom->Get_Angle();
	//pxTransform.q = PxQuat(vRot.x, { 1.f,0.f,0.f })*PxQuat(vRot.y, { 0.f,1.f,0.f })*PxQuat(vRot.z, { 0.f,0.f,1.f });
	//m_pRigidActor->setGlobalPose(pxTransform);


	m_iAniIndex = m_pMeshCom->Get_AniIndex();
	m_pMeshCom->Set_Animationset(animID::ToyChain_Break);
	m_pMeshCom->Play_Animationset(fTimeDelta);
	if (!m_bSound[Sound_break])
	{
		StopSound(CSoundMgr::CHANNELID::OgreChain);
		PlaySoundW(L"Ogre_ChainBreak.wav", CSoundMgr::CHANNELID::OgreChain, 0.2f);
		m_bSound[Sound_break] = true;
		m_pParticle->Set_Active(false);
		m_pLight->Set_LightRender(false);


		_vec3 vLook, vUp, vDir;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		m_pTransformCom->Get_Info(INFO_UP, &vUp);
		D3DXVec3Normalize(&vDir, &(vLook + vUp));

		PARTICLE tParticle;
		Engine::Reset_Particle(tParticle);
		tParticle.fAcc = 1.f;
		tParticle.fCurrentTime = 0.f;

		tParticle.vEndSize = _vec3(0.01f, 0.01f, 0.01f);
		tParticle.fRotateSpeed = 0.f;

		tParticle.vStartColor = { 255.f * 3.5f, 200.f * 3.5f, 120.f * 3.5f, 255.f * 2.f };
		tParticle.vEndColor = { 255.f * 3.5f, 200.f * 3.5f, 120.f * 3.5f, 255.f * 2.f };
		tParticle.bGravity = true;


		for (_int i = 0; i < 100; ++i)
		{
			tParticle.fGravityForce = CRandoms()(0.5f, 1.5f);
			_vec3 vAddDir = { CRandoms()(-0.5f, 0.5f), CRandoms()(0.f, 8.f) , CRandoms()(-0.5f, 0.5f) };
			D3DXVec3Normalize(&vAddDir, &(vDir + vAddDir));
			tParticle.vDir = vAddDir;
			tParticle.fLifeTime = CRandoms()(0.8f, 1.2f);
			tParticle.vPos = m_pTransformCom->Get_Position() + _vec3(CRandoms()(-0.3f, 0.3f), CRandoms()(0.5f, 2.5f), CRandoms()(-0.3f, 0.3f));
			_float fSize = CRandoms()(0.2f, 0.4f);
			tParticle.vStartSize = _vec3(fSize, fSize, fSize);
			tParticle.fVelocity = CRandoms()(10.f, 15.f);
			Engine::Emit_ImageParticle(tParticle, 15, 11);
		}

		
	}
	if (m_iAniIndex == animID::ToyChain_Break && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_bMoveStart = true;
	}
}

COgreChain * COgreChain::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, void * pArg)
{
	COgreChain*	pInstance = new COgreChain(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos, pArg)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void COgreChain::Free(void)
{
	Engine::CGameObject::Free();
}
