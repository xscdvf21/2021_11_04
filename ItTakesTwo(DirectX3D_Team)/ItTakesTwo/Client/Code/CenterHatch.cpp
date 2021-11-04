#include "stdafx.h"
#include "CenterHatch.h"
#include "Ogre.h"

#include "Export_Function.h"

CCenterHatch::CCenterHatch(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_fAngle(0.f), m_fOpenSpeed(D3DXToRadian(5)), m_bDir(false), m_bMove(false), m_bHatchOpen(false)
{
}

CCenterHatch::~CCenterHatch()
{
}

HRESULT CCenterHatch::Ready_Object(_bool _bDir, void * pArg)
{
	m_bDir = _bDir;

	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}




	return S_OK;
}

_int CCenterHatch::Update_Object(const _float & fTimeDelta)
{
	//여기에 Ogre가 가지고있는, 석상과 충돌 카운드 횟수가 4면.
	COgre*		pOgre = (COgre*)Engine::Get_GameObject(L"GameObject", L"ToyOgre");
	
	if(nullptr != pOgre)
		m_iHatchOpen = pOgre->Get_StoneCol();

	if (m_iHatchOpen == 4)
	{
		m_bMove = true;
	}


	//테스트용 나중에 지움.
	if (Key_Pressing(DIK_I))
	{
		m_bMove = true;
	}

	if (m_bDead)
		return OBJ_DEAD;

	if(m_bMove)
		Move(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CCenterHatch::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CCenterHatch::Render_Object(const _int & iIndex)
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

HRESULT CCenterHatch::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"OgreCenterHatch"));
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


	m_pTransformCom->Set_Scale(0.007f, 0.007f, 0.007f);
	if (!m_bDir)
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(35.f));

	if (m_bDir)
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(215.f));

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
	pBody->setName("OgreHatch");
	PxTransform Transform = pBody->getGlobalPose();

	PxQuat pxQ = Transform.q;

	pBody->attachShape(*pShape);

	return S_OK;
}

HRESULT CCenterHatch::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CCenterHatch::Move(const _float & fTimeDelta)
{
	//자기 Down 벡터와, 라이트 벡터 내적해서. 내리면될듯?


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

	_vec3 vRight;
	_vec3 vDown = { 0.f, -1.f, 0.f };

	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vRight, &vRight);

	if(!m_bDir)
		m_fAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vRight, &vDown)));

	if(m_bDir)
		m_fAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vDown, &vRight)));

	if (m_fAngle >= D3DXToDegree(m_fOpenSpeed))
	{
		if (!m_bDir)
		{
			m_pTransformCom->Rotation(ROT_Z, -m_fOpenSpeed);
		}
		if (m_bDir)
		{
			m_pTransformCom->Rotation(ROT_Z, -m_fOpenSpeed);
		}
	}
	else if (m_fAngle <= D3DXToDegree(m_fOpenSpeed))
	{
		m_bHatchOpen = true;
	}

}




CCenterHatch * CCenterHatch::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _bDir, void * pArg)
{
	CCenterHatch*	pInstance = new CCenterHatch(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_bDir, pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}
void CCenterHatch::Free(void)
{
	Engine::CGameObject::Free();
}
