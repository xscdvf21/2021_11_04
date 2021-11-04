#include "stdafx.h"
#include "CrusherStartDoor.h"
#include "ChessDoorSwitch.h"
#include "Crusher.h"
#include "Export_Function.h"
#include "ChessField.h"

CCrusherStartDoor::CCrusherStartDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_bClose(false)
	, m_fCloseSpeed(D3DXToRadian(5))
	, m_fAngle(90.f)
	, m_fCurrentAngle(0.f)
	, m_bSwitch1(false)
	, m_bSwitch2(false)
	, m_bCrusherCreate(false)
	, m_bCreateTrigger(false)
	, m_pTrigger(nullptr)

{
}

CCrusherStartDoor::~CCrusherStartDoor()
{
}

HRESULT CCrusherStartDoor::Ready_Object(void * pArg)
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

_int CCrusherStartDoor::Update_Object(const _float & fTimeDelta)
{


	CChessDoorSwitch*  pSwitch1 = (CChessDoorSwitch*)Engine::Get_GameObject(L"GameObject", L"CrusherSwitch1");
	CChessDoorSwitch*  pSwitch2 = (CChessDoorSwitch*)Engine::Get_GameObject(L"GameObject", L"CrusherSwitch2");
	m_bSwitch1 = pSwitch1->Get_CrusherMove();
	m_bSwitch2 = pSwitch2->Get_CrusherMove();
	if (m_bSwitch1 && m_bSwitch2)
		m_bClose = true;

	if (m_bClose)
	{
		Move(fTimeDelta);
	}

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CCrusherStartDoor::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CCrusherStartDoor::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 8);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CCrusherStartDoor::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"ChessOpenDoor"));
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
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformCom->Set_Pos(tTemp.vPos);

	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();

	PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(m_pTransformCom->Get_Angle().y, { 0.f,1.f,0.f }));
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxTriangleMeshGeometry pMeshGeo;
	pMeshGeo.triangleMesh = m_pMeshCom->Get_PxMesh();//m_pPhysMesh->Get_PxMesh();
	pMeshGeo.scale = PxMeshScale(PxVec3(tTemp.vScale.x, tTemp.vScale.y, tTemp.vScale.z));

	PxShape* pShape = pPhysics->createShape(pMeshGeo, *pMaterial, true);

	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
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
	pBody->setName((char*) this);

	pBody->attachShape(*pShape);
	return S_OK;
}

HRESULT CCrusherStartDoor::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));

	return S_OK;
}

void CCrusherStartDoor::Move(const _float & fTimeDelta)
{
	if (m_bCrusherCreate)
		return;

	if (!m_bSoundStart)
	{
		StopSound(CSoundMgr::CHANNELID::Effect);
		PlaySoundW(L"DoorOpen.ogg", CSoundMgr::CHANNELID::Effect, 0.3f);

		m_bSoundStart = true;
	}


	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();

	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	pxTransform.p.x = vPos.x;
	pxTransform.p.y = vPos.y;
	pxTransform.p.z = vPos.z;
	_vec3 vRot = m_pTransformCom->Get_Angle();
	pxTransform.q = PxQuat(vRot.x, { 1.f,0.f,0.f })*PxQuat(vRot.y, { 0.f,1.f,0.f })*PxQuat(vRot.z, { 0.f,0.f,1.f });
	m_pRigidActor->setGlobalPose(pxTransform);

	m_fCurrentAngle += D3DXToDegree(m_fCloseSpeed);
	if (m_fAngle >= m_fCurrentAngle)
	{
		m_pTransformCom->Rotation(ROT_Y, m_fCloseSpeed);
	}
	else if (m_fCurrentAngle >= m_fAngle)
	{
		Engine::CLayer*				pLayer = Engine::Get_Layer(L"GameObject");
		Engine::CGameObject*		pGameObject = nullptr;


		OBJINFO tObj;
		tObj.eSceneID = RESOURCE_CF;
		tObj.vAngle = { 0.f, 0.f, 0.f };
		tObj.vScale = { 0.006f, 0.006f, 0.006f };
		tObj.vAngle = { 0.f, 132.f, 0.f };
		tObj.vPos = { 41.f, 0.f, -117.6f };
		pGameObject = CCrusher::Create(m_pGraphicDev, &tObj);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Toy_Crusher", pGameObject, Engine::Get_Scene()), );
		Engine::Get_Scene()->setupFiltering(Engine::Get_Allocator(), pGameObject->Get_RigidActor(), FilterGroup::eCrusher, FilterGroup::eButton);


		CGameObject* pMap = Engine::Get_GameObject(L"GameObject", L"ChessField_Dark13");
		if (pMap)
			pMap->Set_Dead();


		CCrusher* pCrusher = (CCrusher*)Engine::Get_GameObject(L"GameObject", L"Toy_Crusher");
		pCrusher->Set_Move(true);
		m_bCrusherCreate = true;
		
		//////////////////////////////////////////////////////////////////////////

		CChessField* pScene = (CChessField*)Engine::Get_Scene();
		pScene->Set_CrusherStart();
	}



	//m_pRigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
}



CCrusherStartDoor * CCrusherStartDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CCrusherStartDoor*	pInstance = new CCrusherStartDoor(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCrusherStartDoor::Free(void)
{
	Engine::CGameObject::Free();
}
