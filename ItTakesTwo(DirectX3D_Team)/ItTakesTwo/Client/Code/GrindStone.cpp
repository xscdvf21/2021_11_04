#include "stdafx.h"
#include "GrindStone.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "PhysXMesh.h"
#include "PlayerValue.h"
#include "BoundingBox.h"
#include "Cody.h"
#include "May.h"


CGrindStone::CGrindStone(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CGrindStone::~CGrindStone(void)
{
}

HRESULT CGrindStone::Ready_Object(void* pArg)
{
	m_vecCogsList.reserve(2);
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;

	return S_OK;
}

Engine::_int CGrindStone::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fMoveAngle = 0.0f;

	if(true == m_bButton1)
		m_fDuration += fTimeDelta;

	if (true == m_bWork)
	{
		if (false == m_bPlaySound)
		{
			m_bPlaySound = true;
			CSoundMgr::GetInstance()->PlaySoundW(L"GrindStone_Start.wav", CSoundMgr::CHANNELID::Vacuum_GrindStone, g_fEffecctSound);
		}

		for (auto iter : m_vecCogsList)
			Engine::Get_GameObject(L"GameObject", iter.c_str())->On_Active(nullptr);

		m_vecCogsList.clear();
		m_pTransformCom->Set_AddAngleZ(D3DXToRadian(60.0f) * fTimeDelta);
		m_fMoveAngle = D3DXToRadian(60.0f) * fTimeDelta;
	}
	



	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CGrindStone::LateUpdate_Object(const _float & fTimeDelta)
{
	if (true == m_bButton1 && true == m_bButton2)
		m_bWork = true;

	if (m_fDuration >= 1.0f)
	{
		m_bButton1 = false;
		m_bButton2 = false;
	}
	
	PxTransform cTrans = m_pRigidActor->getGlobalPose();
	_vec3 vRot = m_pTransformCom->Get_Angle();

	auto* pBody = m_pRigidActor->is<PxRigidDynamic>();
	cTrans.q = PxQuat(vRot.y, { 0.f,1.f,0.f });
	pBody->setKinematicTarget(cTrans);
	return _int();
}

void CGrindStone::Render_Object(const _int& iIndex)
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

void* CGrindStone::On_Active(void * pCaller)
{
	if (false == m_bButton1)
	{
		m_fDuration = 0.0f;
		m_bButton1 = true;
		return nullptr;
	}
	else if (false == m_bButton2)
		m_bButton2 = true;

	return nullptr;
}

void * CGrindStone::On_Attach(void * pCaller)
{
	CGameObject* pPlayer = (CGameObject*)pCaller;
	CTransform* pTransform = (CTransform*)pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_vec3 vPlayerPos = pTransform->Get_Position();
	_vec3 vPos = m_pTransformCom->Get_Position();
	// 그라인더의 위치를 원점으로 맞춰줌
	vPlayerPos -= vPos;
	vPlayerPos.x = 0.0f;

	_matrix matRotZ;
	D3DXMatrixRotationX(&matRotZ, m_fMoveAngle);

	_vec3 vResultPos;
	D3DXVec3TransformNormal(&vResultPos, &vPlayerPos, &matRotZ);

	_vec3 vResultDir = vResultPos - vPlayerPos;
	_vec3 vGravitation{};
	if (vPos.x - vPlayerPos.x <= 0.0f)
		vGravitation.x = -0.01f;
	else
		vGravitation.x = 0.01f;

	if (typeid(CCody) == typeid(*pPlayer))
		pPlayer->Is<CCody>()->Set_CheckGround();
	else
		pPlayer->Is<CMay>()->Set_CheckGround();

	m_vResult = vResultDir;
	return &m_vResult;
}

CGrindStone* CGrindStone::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CGrindStone*	pInstance = new CGrindStone(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CGrindStone::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CGrindStone::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"GrindStone"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(tTemp.vPos);
	//m_pTransformCom->Set_AddAngleX(D3DXToRadian(-90.0f));
	m_pTransformCom->Set_AddAngleY(D3DXToRadian(90.0f));

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	

	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();


	PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(m_pTransformCom->Get_Angle().y, { 0.f,1.f,0.f }));
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);
	PxTriangleMeshGeometry pMeshGeo;
	pMeshGeo.triangleMesh = m_pMeshCom->Get_PxMesh();
	_vec3 vScale = m_pTransformCom->Get_Scale();
	pMeshGeo.scale = PxMeshScale(PxVec3(vScale.x, vScale.y, vScale.z));

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

HRESULT CGrindStone::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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
