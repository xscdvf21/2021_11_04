#include "stdafx.h"
#include "OgreStartDoor.h"
#include "MainCamera.h"
#include "Export_Function.h"
COgreStartDoor::COgreStartDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	,m_bMoveStart(false)
	,m_fSpeed(10.f)
	,m_pTrigger(nullptr), m_bCreateTrigger(false)
{
}

COgreStartDoor::~COgreStartDoor()
{
}

HRESULT COgreStartDoor::Ready_Object(void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	//스케일 및 포스
	//tTemp.vScale = { 0.016f, 0.010f, 0.012f };
	//tTemp.vPos = { 219.f, 4.f, 169.5f };
	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	return S_OK;
}

_int COgreStartDoor::Update_Object(const _float & fTimeDelta)
{
	if (!m_bCreateTrigger)
	{
		/*TriggerCreate();*/
	}

	if (Key_Down(DIK_I))
	{
		m_bMoveStart = true;
	}

	if(m_bMoveStart)
		Move(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int COgreStartDoor::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void COgreStartDoor::Render_Object(const _int & iIndex)
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

HRESULT COgreStartDoor::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	//m_pTransformCom->Set_Scale(tTemp.vScale * 0.7f);
	//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(35.f));
	//m_pTransformCom->Set_Pos(tTemp.vPos);

	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"OgreStartDoor"));
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

	m_pTransformCom->Set_Scale(tTemp.vScale * 0.7f);
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(35.f));
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
	pBody->setName("OgreStartDoor");
	PxTransform Transform = pBody->getGlobalPose();

	PxQuat pxQ = Transform.q;

	pBody->attachShape(*pShape);
	return S_OK;
}

HRESULT COgreStartDoor::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj; 

	//_matrix			matTempWorld;
	//D3DXMatrixIdentity(&matTempWorld);
	//D3DXMatrixRotationY(&matTempWorld, D3DXToRadian(90.f));


	//m_pTransformCom->Get_WorldMatrix(&matWorld);
	//matWorld = matWorld* matTempWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));

	return S_OK;
}

void COgreStartDoor::Move(const _float & fTimeDelta)
{
	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();

	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	pxTransform.p.x = vPos.x;
	pxTransform.p.y = vPos.y;
	pxTransform.p.z = vPos.z;
	m_pRigidActor->setGlobalPose(pxTransform);

	m_pTransformCom->Move_Pos(&_vec3(0.f, 1.f, 0.f), -m_fSpeed,fTimeDelta);
	if (vPos.y <= 0.f)
	{
		m_bMoveStart = false;
		m_pTransformCom->Set_Pos(vPos.x, 0.f, vPos.z);

		if (!m_bSound[BGM])
		{
			StopAll();
			PlayBGM(L"ChessOgre_BGM.wav");
			m_bSound[BGM] = true;

			StopSound(CSoundMgr::CHANNELID::Crusher);
			PlaySoundW(L"CF_OgreStart.ogg", CSoundMgr::CHANNELID::CrusherHitPoint, 0.3f);
		}
	}
	
}

void COgreStartDoor::TriggerCreate()
{
	m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_pTransformCom->Get_Position(), 0.5f, TRIGGER_CHESS_OGRE_DOOR, FilterGroup::eButton);

	m_bCreateTrigger = true;
}

COgreStartDoor * COgreStartDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	COgreStartDoor*	pInstance = new COgreStartDoor(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void COgreStartDoor::Free(void)
{
	Engine::CGameObject::Free();
}
