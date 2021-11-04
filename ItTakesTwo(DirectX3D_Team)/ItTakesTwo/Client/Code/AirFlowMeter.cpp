#include "stdafx.h"
#include "AirFlowMeter.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "PhysXMesh.h"
#include "MiniVacuum.h"


CAirFlowMeter::CAirFlowMeter(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CAirFlowMeter::~CAirFlowMeter(void)
{
}

HRESULT CAirFlowMeter::Ready_Object(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;

	return S_OK;
}

Engine::_int CAirFlowMeter::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	CMiniVacuum* pMiniVacuum = (CMiniVacuum*)Engine::Get_GameObject(L"GameObject", m_tObjInfo.tagMesh);
	_matrix matVacuum = pMiniVacuum->Get_EventPos();

	_vec3 vLook = { matVacuum._31,matVacuum._32, matVacuum._33};
	//CTransform* pTransform = (CTransform*)Engine::Get_Component(L"GameObject", m_tObjInfo.tagMesh, L"Com_Transform", ID_DYNAMIC);
	//pTransform->Get_Info(INFO_LOOK, &vLook);
	
	vLook.y = 0.0f;
	D3DXVec3Normalize(&vLook, &vLook);

	_float fCosTh = D3DXVec3Dot(&vLook, &_vec3(0.0f, 0.0f, 1.0f));
	_float fRad = acosf(fCosTh);
	
	_float fDir = D3DXVec3Dot(&_vec3(1.0f, 0.0f, 0.0f), &vLook);
	if (D3DXToDegree(fRad) >= 15.0f && D3DXToDegree(fRad) <= 50.0f && fDir <= 0.0f)
	{
		CSoundMgr::GetInstance()->PlaySoundW(L"Weather.wav", CSoundMgr::CHANNELID::Vaccum_AirFlowMeter, g_fEffecctSound * 0.5f);
		m_fRotSpeed += fTimeDelta * 10.0f;
		if (m_fRotSpeed >= 45.0f)
			m_fRotSpeed = 45.0f;
	}
	else
	{
		m_fRotSpeed += -fTimeDelta * 10.0f;
		if (m_fRotSpeed <= 0.0f)
		{
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNELID::Vaccum_AirFlowMeter);
			m_fRotSpeed = 0.0f;
		}
	}

	m_pTransformCom->Set_AddAngleY(D3DXToRadian(m_fRotSpeed * fTimeDelta));

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CAirFlowMeter::LateUpdate_Object(const _float & fTimeDelta)
{
	PxTransform cTrans = m_pRigidActor->getGlobalPose();
	_vec3 vRot = m_pTransformCom->Get_Angle();

	auto* pBody = m_pRigidActor->is<PxRigidDynamic>();
	cTrans.q = PxQuat(vRot.y, { 0.f,1.f,0.f });
	pBody->setKinematicTarget(cTrans);
	return _int();
}

void CAirFlowMeter::Render_Object(const _int& iIndex)
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

CAirFlowMeter* CAirFlowMeter::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CAirFlowMeter*	pInstance = new CAirFlowMeter(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CAirFlowMeter::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CAirFlowMeter::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"AirFlowMeter"));
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
	pBody->setName((char*) this);

	return S_OK;
}

HRESULT CAirFlowMeter::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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
