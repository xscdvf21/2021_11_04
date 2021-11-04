#include "stdafx.h"
#include "VacuumBigFan.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "PhysXMesh.h"
#include "Cody.h"
#include "May.h"


CVacuumBigFan::CVacuumBigFan(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CVacuumBigFan::~CVacuumBigFan(void)
{
}

HRESULT CVacuumBigFan::Ready_Object(void* pArg)
{
	ZeroMemory(&m_pSylinderMeshCom, sizeof(m_pSylinderMeshCom));
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;

	return S_OK;
}

Engine::_int CVacuumBigFan::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bStart)
	{
		m_pCodyTransform = Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", ID_DYNAMIC)->Is<CTransform>();
		m_pMayTransform = Engine::Get_Component(L"GameObject", L"May", L"Com_Transform", ID_DYNAMIC)->Is<CTransform>();
		m_bStart = false;
	}

	if (m_bSoundFan)
	{

		_float3 vDis1 = m_pCodyTransform->Get_Position() - m_pTransformCom->Get_Position();
		_float3 vDis2 = m_pMayTransform->Get_Position() - m_pTransformCom->Get_Position();

		_float fDis1 = D3DXVec3Length(&vDis1);
		_float fDis2 = D3DXVec3Length(&vDis2);

		if (fDis2 <= 95.0f || fDis1 <= 95.0f)
		{
			if (fDis1 < fDis2)
				m_fVolume = fDis1;
			else
				m_fVolume = fDis2;

			_float fSound = g_fEffecctSound * (1 - m_fVolume / 95.0f);
			CSoundMgr::GetInstance()->PlaySoundW(L"BigFan.wav", CSoundMgr::CHANNELID::Vaccum_BigFan, fSound);
			CSoundMgr::GetInstance()->Set_Volum(fSound, CSoundMgr::CHANNELID::Vaccum_BigFan);
			//CSoundMgr::GetInstance()->PlaySoundW(L"BigFan.wav", CSoundMgr::CHANNELID::Vaccum_BigFan, g_fEffecctSound);
		}
		else
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNELID::Vaccum_BigFan);
	}


	//if (m_fVolume >= 105.0f)
	//	m_fVolume = 105.0f;

	//

	m_fMoveAngle = 0.0f;

	m_pTransformCom->Set_AddAngleY(D3DXToRadian(45.0f * fTimeDelta));
	m_fMoveAngle = D3DXToRadian(45.0f * fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CVacuumBigFan::LateUpdate_Object(const _float & fTimeDelta)
{
	PxTransform cTrans = m_pRigidActor->getGlobalPose();
	_vec3 vRot = m_pTransformCom->Get_Angle();

	auto* pBody = m_pRigidActor->is<PxRigidDynamic>();
	cTrans.q = PxQuat(vRot.y, { 0.f,1.f,0.f });
	pBody->setKinematicTarget(cTrans);
	return _int();
}

void CVacuumBigFan::Render_Object(const _int& iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 8);

	_matrix matWorld;
	for (_uint i = 0; i < 4; ++i)
	{
		m_pTransformCom->Get_WorldMatrix(&matWorld);
		matWorld._42 += i * 10.0f;
		pEffect->SetMatrix("g_matWorld", &matWorld);
		m_pSylinderMeshCom[i]->Render_Meshes(pEffect, 8);
	}

	pEffect->End();

	Safe_Release(pEffect);
}

void * CVacuumBigFan::On_Attach(void * pCaller)
{
	CGameObject* pPlayer = (CGameObject*)pCaller;
	CTransform* pTransform = (CTransform*)pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_vec3 vPlayerPos = pTransform->Get_Position();
	_vec3 vPos = m_pTransformCom->Get_Position();
	// 그라인더의 위치를 원점으로 맞춰줌
	vPlayerPos -= vPos;
	vPlayerPos.y = 0.0f;

	_matrix matRotZ;
	D3DXMatrixRotationY(&matRotZ, m_fMoveAngle);

	_vec3 vResultPos;
	D3DXVec3TransformNormal(&vResultPos, &vPlayerPos, &matRotZ);

	_vec3 vResultDir = vResultPos - vPlayerPos;
	if (typeid(CCody) == typeid(*pPlayer))
	{
		pPlayer->Is<CCody>()->Set_AttachAngle(m_fMoveAngle);
		pPlayer->Is<CCody>()->Set_Rot(true);
	}
	else
	{
		pPlayer->Is<CMay>()->Set_AttachAngle(m_fMoveAngle);
		pPlayer->Is<CMay>()->Set_Rot(true);
	}

	m_vResult = vResultDir;
	return &m_vResult;
}

CVacuumBigFan* CVacuumBigFan::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CVacuumBigFan*	pInstance = new CVacuumBigFan(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CVacuumBigFan::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CVacuumBigFan::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"BigFanBlade"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_HolderMesh", pComponent);

	for (_uint i = 0; i < 4; ++i)
	{
		pComponent = m_pSylinderMeshCom[i] = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"BigFanSylinder"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		TCHAR szbuf[MAX_PATH] = L"";
		wsprintf(szbuf, L"Com_BigFanSylinder%d", i);
		m_mapComponent[Engine::ID_STATIC].emplace(szbuf, pComponent);
	}

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


	/*auto* pPhys = Engine::Get_Physics();

	PxTransform cTransform = PxTransform(tTemp.vPos.x, tTemp.vPos.y, tTemp.vPos.z);


	m_pRigidActor =  pPhys->createRigidDynamic(cTransform);
	m_pRigidBody= pPhys->createShape(PxTriangleMeshGeometry(m_pMeshCom->Get_PxMesh(), PxMeshScale(0.01f)),* pPhys->createMaterial(0.5f, 0.5f, 0.f), true);
	m_pRigidActor->attachShape(*m_pRigidBody);

	auto* pBody =m_pRigidActor->is<PxRigidDynamic>();
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);*/

	//////////////////////////////////////////////////////////////////////////
	//PxTransform cTrans = m_pRigidActor->getGlobalPose();
	//_vec3 vRot = m_pTransformCom->Get_Angle();

	//cTrans.q = PxQuat(vRot.x, { 1.f,0.f,0.f })*PxQuat(vRot.y, { 0.f,1.f,0.f })*PxQuat(vRot.z, { 0.f,0.f,1.f });
	//pBody->setKinematicTarget(cTrans);

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

HRESULT CVacuumBigFan::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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
