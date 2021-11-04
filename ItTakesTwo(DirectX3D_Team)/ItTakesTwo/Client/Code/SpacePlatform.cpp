#include "stdafx.h"
#include "SpacePlatform.h"
#include "Export_Function.h"
#include "SpacePlatformWall.h"
#include "May_Space.h"
#include "Cody_Space.h"
#include "SpaceLever.h"



CSpacePlatform::CSpacePlatform(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CSpacePlatform::~CSpacePlatform()
{
}

void CSpacePlatform::Move1stDoor()
{
	CSpacePlatformWall* pWall = nullptr;
	pWall =Engine::Get_GameObject(L"GameObject", L"MoveWall1")->Is<CSpacePlatformWall>();
	pWall->Set_Move({ 0.f,-5.f,0.f });

	pWall = Engine::Get_GameObject(L"GameObject", L"MoveWall2")->Is<CSpacePlatformWall>();
	pWall->Set_Move({ 0.f,5.f,0.f });

	CSoundMgr::GetInstance()->StopNPlaySound(L"2D_LaserDoorOpen.wav", CSoundMgr::CHANNELID::Space_LaserDoor, g_fEffecctSound);
}

void CSpacePlatform::Move2ndDoor()
{
	CSpacePlatformWall* pWall = nullptr;
	pWall = Engine::Get_GameObject(L"GameObject", L"MoveWall3")->Is<CSpacePlatformWall>();
	pWall->Set_Move({ 0.f,-5.f,0.f });

	pWall = Engine::Get_GameObject(L"GameObject", L"MoveWall4")->Is<CSpacePlatformWall>();
	pWall->Set_Move({ 0.f,5.f,0.f });

	CSoundMgr::GetInstance()->StopNPlaySound(L"2D_LaserDoorOpen.wav", CSoundMgr::CHANNELID::Space_LaserDoor, g_fEffecctSound);
}

HRESULT CSpacePlatform::Ready_Object(void * pArg)
{

	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	return S_OK;
}

_int CSpacePlatform::Update_Object(const _float & fTimeDelta)
{
	
	if (m_bStart)
	{
		m_pCodyTransform = Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", ID_DYNAMIC)->Is<CTransform>();
		m_pMayTransform = Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", ID_DYNAMIC)->Is<CTransform>();
		m_bStart = false;
	}
	_float3 vDis1 = m_pCodyTransform->Get_Position() - m_pTransformCom->Get_Position();
	_float3 vDis2 = m_pMayTransform->Get_Position() - m_pTransformCom->Get_Position();

	_float fDis1 = D3DXVec3Length(&vDis1);
	_float fDis2 = D3DXVec3Length(&vDis2);

	if (fDis2 <= 50.0f || fDis1 <= 50.0f)
	{
		CSoundMgr::GetInstance()->PlaySoundW(L"2D_Laser.wav", CSoundMgr::CHANNELID::Space_Laser, g_fEffecctSound * (1 - fDis1 / 35.0f));

		if (fDis1 < fDis2)
			m_fVolume = fDis1;
		else
			m_fVolume = fDis2;
	}

	if (m_fVolume >= 50.0f)
		m_fVolume = 50.0f;

	CSoundMgr::GetInstance()->Set_Volum(g_fEffecctSound * (1 - m_fVolume / 35.0f), CSoundMgr::CHANNELID::Space_Laser);

	//if (m_iCount > 0)
	//{
	//	m_fTime += fTimeDelta;
	//	if (m_fTime > 1.f)
	//	{
	//		--m_iCount;
	//		m_fTime = 0.f;
	//	}
	//}
	Move(fTimeDelta);
	if (Engine::Key_Down(DIK_F))
	{
		On_Active(nullptr);
	}

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CSpacePlatform::LateUpdate_Object(const _float & fTimeDelta)
{
	_bool bCodyDead = Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody_Space>()->Get_PlayerDead();
	_bool bMayDead = Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay_Space>()->Get_PlayerDead();

	if (bCodyDead && bMayDead && !m_bArrive && m_bMove)
	{
		m_bMove = false;
		Reset_State();
	}

	return _int();
}

void CSpacePlatform::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);			//	1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	m_pMeshCom->Render_Meshes(pEffect, 8);	//	pEffect->BeginPass(0);
											//	스태틱매쉬8
											//	다이나믹6
	pEffect->End();

	Safe_Release(pEffect);
}

void* CSpacePlatform::On_Active(void* pCaller)
{
	++m_iCount;
	m_fPushTime = 0;
	if (m_iCount >= 2)
	{

		m_bMove = true;
		((CGameObject*)m_pInteractActor)->On_Active(&m_bMove);
		Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay_Space>()->Set_LeverEnd(true);
		Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody_Space>()->Set_LeverEnd(true);
		Move1stDoor();
	}
	return nullptr;
}

void* CSpacePlatform::On_Trigger_Active(void* pTrigger, void* pCaller)
{
	--m_iCount;
	return nullptr;
}

void CSpacePlatform::Reset_State()
{
	((CGameObject*)m_pInteractActor)->On_Active(&m_bMove);

	ZeroMemory(m_bDoorActive, 2);
	m_fTime = 0.f;
	m_pTransformCom->Set_Pos(m_vStartPos);
	m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(PxTransform(m_vStartPos.x, m_vStartPos.y, m_vStartPos.z));
	m_iCount = 0;
	CSpacePlatformWall* pWall = nullptr;

	pWall = Engine::Get_GameObject(L"GameObject", L"MoveWall1")->Is<CSpacePlatformWall>();
	pWall->Reset_State();
	pWall = Engine::Get_GameObject(L"GameObject", L"MoveWall2")->Is<CSpacePlatformWall>();
	pWall->Reset_State();
	pWall = Engine::Get_GameObject(L"GameObject", L"MoveWall3")->Is<CSpacePlatformWall>();
	pWall->Reset_State();
	pWall = Engine::Get_GameObject(L"GameObject", L"MoveWall4")->Is<CSpacePlatformWall>();
	pWall->Reset_State();

	CSoundMgr::GetInstance()->StopNPlaySound(L"2D_LaserDoorClose.wav", CSoundMgr::CHANNELID::Space_LaserDoor, g_fEffecctSound);

	Engine::Get_GameObject(L"GameObject", L"SpaceLever")->Is<CSpaceLever>()->Reset_State();
	Engine::Get_GameObject(L"GameObject", L"SpaceLever2")->Is<CSpaceLever>()->Reset_State();

	Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay_Space>()->Set_SceneWait(false);
	Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody_Space>()->Set_SceneWait(false);
}

HRESULT CSpacePlatform::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	


	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"Move_Platform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformCom->Set_Pos(tTemp.vPos);
	m_vStartPos = tTemp.vPos;

	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();

	PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(tTemp.vAngle.y, { 0.f,1.f,0.f }));
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxTriangleMeshGeometry pMeshGeo;
	pMeshGeo.triangleMesh = m_pMeshCom->Get_PxMesh();//m_pPhysMesh->Get_PxMesh();
	pMeshGeo.scale = PxMeshScale(PxVec3(tTemp.vScale.x, tTemp.vScale.y, tTemp.vScale.z));

	PxShape* pShape = pPhysics->createShape(pMeshGeo, *pMaterial, true);


	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);

	m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pShape, 1);

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	// 트랜스폼은 가지고있지만 움직이지 않는애들
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	//pBody->setKinematicTarget()
	// 그래비티를 끈다.
	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	// 질량설정
	pBody->setMass(0);
	//이동 제동력
	pBody->setName((char*)this);



	return S_OK;
}

HRESULT CSpacePlatform::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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
	pEffect->SetFloat("g_fMaterialIndex", 0.f);
	pEffect->CommitChanges();

	return S_OK;
}

CSpacePlatform * CSpacePlatform::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CSpacePlatform*	pInstance = new CSpacePlatform(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpacePlatform::Free(void)
{
	Engine::CGameObject::Free();
}

void CSpacePlatform::Move(const _float & fTimeDelta)
{
	if (!m_bMove)
		return;


	//Move2ndDoor();
	if (m_bMove && m_pTransformCom->Get_Position().x >= 111.084f)
	{
		Move2ndDoor();
		m_bMove = false;
		m_iCount = 0;
		m_bArrive = true;

		_bool bActive = false;
		((CGameObject*)m_pInteractActor)->On_Active(&bActive);
	}

	m_fTime+=fTimeDelta;
	_vec3 vPos = m_pTransformCom->Get_Position();
	_vec3 vGap= m_vGoalPos - vPos;
	if (D3DXVec3Length(&vGap) < 1.f)
	{
		m_bMove = false;
		m_pTransformCom->Set_Pos(m_vGoalPos);
		vPos = m_pTransformCom->Get_Position();
		m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(PxTransform(vPos.x, vPos.y, vPos.z));
		return;
	}
	
	CTransform* Lever1 = (CTransform*)Engine::Get_GameObject(L"GameObject", L"SpaceLever")->Is<CSpaceLever>()->Get_Component(L"Com_Transform", ID_DYNAMIC);
	CTransform* Lever2 = (CTransform*)Engine::Get_GameObject(L"GameObject", L"SpaceLever2")->Is<CSpaceLever>()->Get_Component(L"Com_Transform", ID_DYNAMIC);

	Lever1->Move_Pos(&_vec3{ 1.f,0.f,0.f }, 4.0f, fTimeDelta);
	Lever2->Move_Pos(&_vec3{ 1.f,0.f,0.f }, 4.0f, fTimeDelta);

	m_pTransformCom->Move_Pos(&_vec3{ 1.f,0.f,0.f }, 4.0f, fTimeDelta);
	vPos = m_pTransformCom->Get_Position();
	m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(PxTransform(vPos.x, vPos.y, vPos.z));


	if (D3DXVec3Length(&vGap) < 20.f && m_bDoorActive[0])
	{
		m_bDoorActive[0] = true;
		Move1stDoor();
	}

	if (D3DXVec3Length(&vGap) < 5.f && m_bDoorActive[1])
	{
		m_bDoorActive[1] = true;
		Move2ndDoor();
	}
}
