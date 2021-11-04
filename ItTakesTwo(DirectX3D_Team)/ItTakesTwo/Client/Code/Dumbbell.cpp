#include "stdafx.h"
#include "Dumbbell.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "PhysXMesh.h"
#include "MiniVacuum.h"


CDumbbell::CDumbbell(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CDumbbell::~CDumbbell(void)
{
}

HRESULT CDumbbell::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;

	_vec3 vPos = m_pTransformCom->Get_Position();
	m_vTargetPos1 = _vec3(vPos.x - 4.0f, -3.304f, vPos.z - 2.85f);
	m_vTargetPos2 = _vec3(vPos.x + 4.0f, -3.304f, vPos.z - 2.85f);
	m_vTriggerPos1 = _vec3(vPos.x - 4.0f, -3.304f, vPos.z - m_tObjInfo.vTriggerScale.z);
	m_vTriggerPos2 = _vec3(vPos.x + 4.0f, -3.304f, vPos.z - m_tObjInfo.vTriggerScale.z);

	return S_OK;
}

Engine::_int CDumbbell::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bStart)
	{
		m_pTrigger1 = CTrigger::CreateBoxTrigger(m_pScene, this, m_vTriggerPos1, m_tObjInfo.vTriggerScale.x, m_tObjInfo.vTriggerScale.y, m_tObjInfo.vTriggerScale.z, TRIGGERTYPE::TRIGGER_VACUUM_DUMBBELL_CODY);
		m_pTrigger2 = CTrigger::CreateBoxTrigger(m_pScene, this, m_vTriggerPos2, m_tObjInfo.vTriggerScale.x, m_tObjInfo.vTriggerScale.y, m_tObjInfo.vTriggerScale.z, TRIGGERTYPE::TRIGGER_VACUUM_DUMBBELL_MAY);
		m_bStart = false;
	}
	//_vec3 vDir = _vec3(0.0f, 0.0f, 1.0f) * fTimeDelta  * 0.2f;
	//Move(vDir, fTimeDelta);

	//if (Engine::Key_Pressing(DIK_5))
	//{
	//	m_pTransformCom->Move_Pos(&_vec3(0.0f, 0.0f, 1.0f), 1.0f, fTimeDelta);
	//}
	//else if (Engine::Key_Pressing(DIK_6))
	//{
	//	m_pTransformCom->Move_Pos(&_vec3(0.0f, 0.0f, 1.0f), -1.0f, fTimeDelta);
	//}
	//else if (Engine::Key_Pressing(DIK_7))
	//{
	//	m_pTransformCom->Move_Pos(&_vec3(1.0f, 0.0f, 0.0f), 1.0f, fTimeDelta);
	//}
	//else if (Engine::Key_Pressing(DIK_8))
	//{
	//	m_pTransformCom->Move_Pos(&_vec3(1.0f, 0.0f, 0.0f), -1.0f, fTimeDelta);
	//}
	//else if (Engine::Key_Pressing(DIK_9))
	//{
	//	m_pTransformCom->Move_Pos(&_vec3(0.0f, 1.0f, 0.0f), 1.0f, fTimeDelta);
	//}
	//else if (Engine::Key_Pressing(DIK_0))
	//{
	//	m_pTransformCom->Move_Pos(&_vec3(0.0f, 1.0f, 0.0f), -1.0f, fTimeDelta);
	//}

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CDumbbell::LateUpdate_Object(const _float & fTimeDelta)
{
	PxTransform cTrans = m_pRigidActor->getGlobalPose();

	_vec3 vRot = m_pTransformCom->Get_Angle();

	auto* pBody = m_pRigidActor->is<PxRigidDynamic>();
	cTrans.q = PxQuat(vRot.y, { 0.f,1.f,0.f });
	pBody->setKinematicTarget(cTrans);

	m_bMoveCody = false;
	m_bMoveMay = false;
	return _int();
}

void CDumbbell::Render_Object(const _int& iIndex)
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

void CDumbbell::Move(_float fTimeDelta, DUMBBELLPLAYER ePlayer)
{
	if (m_pTransformCom->Get_Position().z >= 35.5f)
	{
		m_bMove = false;
		Engine::Get_GameObject(L"GameObject", L"SceneChange")->On_Active(this);
		return;
	}

	switch (ePlayer)
	{
	case CDumbbell::DUMBBELL_MAY:
		m_bMoveMay = true;
		break;
	case CDumbbell::DUMBBELL_CODY:
		m_bMoveCody = true;
		break;
	}
	
	// 둘다 밀었을 경우 true 아니면 false
	if (true == m_bMoveCody && true == m_bMoveMay)
	{
		m_vDir = _vec3(0.0f, 0.0f, 1.0f) * fTimeDelta * 2.0f;

		_vec3 vPos = m_pTransformCom->Get_Position();
		vPos += m_vDir;
		m_pTransformCom->Set_Pos(vPos);

		PxTransform Transfrom = m_pRigidActor->getGlobalPose();
		Transfrom.p.x = vPos.x;
		Transfrom.p.y = vPos.y;
		Transfrom.p.z = vPos.z;
		m_pRigidActor->setGlobalPose(Transfrom);

		vPos = m_pTrigger1->Get_Pos();
		m_pTrigger1->Set_Pos(vPos + m_vDir);

		vPos = m_pTrigger2->Get_Pos();
		m_pTrigger2->Set_Pos(vPos + m_vDir);

		m_vTargetPos1 += m_vDir;
		m_vTargetPos2 += m_vDir;

		m_pTransformCom->Update_Component(fTimeDelta);
		m_bMove = true;
	}
	else
		m_bMove = false;
}

CDumbbell* CDumbbell::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CDumbbell*	pInstance = new CDumbbell(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CDumbbell::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CDumbbell::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CStaticMesh

	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"Dumbbell"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(tTemp.vPos);
	//m_pTransformCom->Set_AddAngleX(D3DXToRadian(-90.0f));
	//m_pTransformCom->Set_AddAngleY(D3DXToRadian(90.0f));

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();


	PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(m_pTransformCom->Get_Angle().y, { 0.f,1.f,0.f }));
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxTriangleMeshGeometry pMeshGeo;
	pMeshGeo.triangleMesh = m_pMeshCom->Get_PxMesh();//m_pPhysMesh->Get_PxMesh();
	pMeshGeo.scale = PxMeshScale(PxVec3(0.01f, 0.01f, 0.01f));

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

HRESULT CDumbbell::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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
	pEffect->SetFloat("g_fMaterialIndex", 1.f);
	pEffect->CommitChanges();

	return S_OK;
}
