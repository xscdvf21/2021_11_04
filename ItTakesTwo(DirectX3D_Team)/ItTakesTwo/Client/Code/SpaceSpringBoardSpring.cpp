#include "stdafx.h"
#include "SpaceSpringBoardSpring.h"
#include "Export_Function.h"



CSpaceSpringBoardSpring::CSpaceSpringBoardSpring(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CSpaceSpringBoardSpring::~CSpaceSpringBoardSpring()
{
}

HRESULT CSpaceSpringBoardSpring::Ready_Object(void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	m_fValue = 1.f;

	return S_OK;
}

_int CSpaceSpringBoardSpring::Update_Object(const _float & fTimeDelta)
{
	m_vPower = { 0.f,0.f,0.f };
	m_iWeight = m_iCodyWeight + m_iMayWeight;
	if (m_bStart)
	{
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_pTransformBoardCom->Get_Position(), 12.f, 3.f, 5.f, TriggerType::TRIGGER_SPRING);
		m_bStart = false;
	}

	Move(fTimeDelta);


	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CSpaceSpringBoardSpring::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CSpaceSpringBoardSpring::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);
	_matrix			matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	pEffect->SetMatrix("g_matWorld", &matWorld);

	m_pMeshBaseCom->Render_Meshes(pEffect, 8);

	m_pTransformSpringCom->Get_WorldMatrix(&matWorld);
	pEffect->SetMatrix("g_matWorld", &matWorld);

	m_pMeshCom->Render_Meshes(pEffect, 8);

	m_pTransformBoardCom->Get_WorldMatrix(&matWorld);
	pEffect->SetMatrix("g_matWorld", &matWorld);

	m_pMeshBoardCom->Render_Meshes(pEffect, 8);


	pEffect->End();

	Safe_Release(pEffect);
}
void CSpaceSpringBoardSpring::Set_Weight(const _bool& IsCody, const _int& iWeight)
{
	if (IsCody)
		m_iCodyWeight = iWeight;
	else
		m_iMayWeight = iWeight;

}
void* CSpaceSpringBoardSpring::On_Active(void* pCaller)
{
	CGameObject* pGameObject = nullptr;
	pGameObject = static_cast<CGameObject*>(pCaller);

	if (pGameObject->Get_ObjTag() == L"Cody")
		m_iCodyWeight = pGameObject->Get_Size();
	else
		m_iMayWeight = pGameObject->Get_Size();



	return nullptr;
}

void* CSpaceSpringBoardSpring::On_Attach(void* pCaller)
{
	if (m_vPower.y > 0.f)
	{
		m_vPos = m_pTransformBoardCom->Get_Position();
		return &m_vPos;
	}
	else
		return nullptr;
}

Engine::_mat CSpaceSpringBoardSpring::Get_EventPos()
{
	return *m_pTransformBoardCom->Get_WorldMatrix();
}

HRESULT CSpaceSpringBoardSpring::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}



	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"SpaceWeight_Spring"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pMeshBaseCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"SpaceWeight_SpringBase"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh2", pComponent);

	pComponent = m_pMeshBoardCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"SpaceWeight_Platform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh3", pComponent);

	// Transform
	pComponent = m_pTransformSpringCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform2", pComponent);

	pComponent = m_pTransformBoardCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform3", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);
	_vec3 vScale = tTemp.vScale;
	vScale *= 0.9f;
	m_pTransformCom->Set_Scale(vScale);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformCom->Set_Pos(tTemp.vPos + _vec3{ 0.f,0.f,0.f });

	m_pTransformBoardCom->Set_Scale(vScale);
	m_pTransformBoardCom->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	m_pTransformBoardCom->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	m_pTransformBoardCom->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformBoardCom->Set_Pos(tTemp.vPos + _vec3{ 0.f,17.5f,0.f });
	//vScale.y *= 0.5f;

	m_pTransformSpringCom->Set_Scale(vScale);
	m_pTransformSpringCom->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	m_pTransformSpringCom->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	m_pTransformSpringCom->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformSpringCom->Set_Pos(tTemp.vPos + _vec3{ 0.f,0.f,0.f });
	//tTemp.vPos += _vec3{ 0.f,17.5f,0.f };
	m_tObjInfo = tTemp;
	auto* pPhysics = Engine::Get_Physics();

	_vec3 vPos = tTemp.vPos;

	PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(D3DXToRadian(tTemp.vAngle.y), { 0.f,1.f,0.f }));
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxTriangleMeshGeometry pMeshGeo;
	pMeshGeo.triangleMesh = m_pMeshBoardCom->Get_PxMesh();//m_pPhysMesh->Get_PxMesh();
	pMeshGeo.scale = PxMeshScale(PxVec3(vScale.x, vScale.y, vScale.z)/*, PxQuat(D3DXToRadian(tTemp.vAngle.y), { 0.f,1.f,0.f })*/);

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

HRESULT CSpaceSpringBoardSpring::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CSpaceSpringBoardSpring::Move(const _float& fTimeDelta)
{
	_float fValue = 0.9f;

	if (m_iWeight == 0)
	{
		fValue = 0.9f;
	}
	else if (m_iWeight == 1)
	{
		fValue = 0.8f;
	}
	else if (m_iWeight == 2)
	{
		if (m_iMayWeight == 0)
		{
			fValue = 0.1f;
		}
		else
		{
			fValue = 0.5f;
		}
	}
	else if (m_iWeight >= 3)
	{
		fValue = 0.1f;
	}

	if (fabs(fValue - m_fValue) < 0.001f)
		m_fValue = fValue;
	else
	{

		m_fValue += (fValue - m_fValue)*fTimeDelta;
		m_vPower = { 0.f,(fValue - m_fValue)*fTimeDelta,0.f };
	}
	if (m_iWeight)
		PlaySoundW(L"SF2ValveSpring.wav", Engine::CSoundMgr::CHANNELID::Space_Spring, 0.4f);

	_vec3 vScale = m_pTransformSpringCom->Get_Scale();
	//_float fMin = 2.f;
	//if (m_fValue <= fMin)
	//	vScale.y = 0.01f;
	//else
	vScale.y = m_fValue*0.01f;
	m_pTransformSpringCom->Set_Scale(vScale);

	_vec3 vPos = m_tObjInfo.vPos;
	vPos.y += m_fValue*19.444f;

	m_pTransformBoardCom->Set_Pos(vPos);

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);
	PxTransform tTransform = pBody->getGlobalPose();
	tTransform.p.x = vPos.x;
	tTransform.p.y = vPos.y;
	tTransform.p.z = vPos.z;
	m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(tTransform);
	m_pTrigger->Set_Pos(vPos);
}

CSpaceSpringBoardSpring * CSpaceSpringBoardSpring::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CSpaceSpringBoardSpring*	pInstance = new CSpaceSpringBoardSpring(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpaceSpringBoardSpring::Free(void)
{
	Engine::CGameObject::Free();
}
