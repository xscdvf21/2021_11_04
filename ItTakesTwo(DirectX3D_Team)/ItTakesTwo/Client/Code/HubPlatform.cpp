#include "stdafx.h"
#include "HubPlatform.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "PhysXMesh.h"



CHubPlatform::CHubPlatform(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_fDir(0.f), m_fAngle(180.f), m_fTurnSpeed(D3DXToRadian(10)), m_fCurrentAngle(0.f), m_fCurTime(0.f),m_fPlatformHeight(3.f)
{
}

CHubPlatform::~CHubPlatform(void)
{
}

HRESULT CHubPlatform::Ready_Object(_vec3 vStartPos, _vec3 vEndPos, void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);
	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;


	m_vMoveStartPos = vStartPos;
	m_vMoveEndPos = vEndPos;

	
	

	m_pMeshHubCom->Set_Animationset(0);
	return S_OK;
}

_int CHubPlatform::Update_Object(const _float & TimeDelta)
{
	Move(TimeDelta);

	m_pMeshHubCom->Play_Animationset(TimeDelta);

	Engine::CGameObject::Update_Object(TimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CHubPlatform::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CHubPlatform::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshHubCom->Render_Meshes_VTF(pEffect, 0);

	_matrix matWorld;
	m_pPlatformTransformCom->Get_WorldMatrix(&matWorld);
	pEffect->SetMatrix("g_matWorld", &matWorld);
	m_pMeshPlatformCom->Render_Meshes(pEffect,4);


	//m_pHolderMeshCom->Render_Meshes(pEffect);

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CHubPlatform::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	pComponent = m_pMeshPlatformCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"HubPlatform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_PlatformMesh", pComponent);

	// Transform
	pComponent = m_pPlatformTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_PlatformTransform", pComponent);

	m_pPlatformTransformCom->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	m_pPlatformTransformCom->Set_Pos(_vec3(tTemp.vPos.x, tTemp.vPos.y + 3.5f, tTemp.vPos.z));


	pComponent = m_pMeshHubCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"MiniVacuumHub"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_HubMesh", pComponent);
	// Transform
	pComponent = m_pHubTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_HubTransform", pComponent);

	m_pHubTransformCom->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	m_pHubTransformCom->Rotation(ROT_Y, D3DXToRadian(0.f));
	m_pHubTransformCom->Set_Pos(tTemp.vPos);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_VTF"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pPlatformTransformCom->Get_Position();


	PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(m_pPlatformTransformCom->Get_Angle().y, { 0.f,1.f,0.f }));
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxTriangleMeshGeometry pMeshGeo;
	pMeshGeo.triangleMesh = m_pMeshPlatformCom->Get_PxMesh();//m_pPhysMesh->Get_PxMesh();
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

HRESULT CHubPlatform::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pHubTransformCom->Get_WorldMatrix(&matWorld);
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

void CHubPlatform::Move(const _float & fTimeDelta)
{

	_vec3 vPos;
	vPos = m_pPlatformTransformCom->Get_Position();

	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	pxTransform.p.x = vPos.x;
	pxTransform.p.y = vPos.y;
	pxTransform.p.z = vPos.z;
	m_pRigidActor->setGlobalPose(pxTransform);



	m_fCurrentAngle += D3DXToDegree(m_fTurnSpeed);
	if (m_fAngle >= m_fCurrentAngle)
	{
		m_pHubTransformCom->Rotation(ROT_Y, -m_fTurnSpeed);
	}
	else if (m_fCurrentAngle >= m_fAngle)
	{
		if (!m_bChangeStart_Pos)
			ChangePos(fTimeDelta, 5.f, m_vMoveStartPos, m_vMoveEndPos);
		else if (m_bChangeStart_Pos)
			ChangePos(fTimeDelta, 5.f, m_vMoveEndPos, m_vMoveStartPos);

		//_vec3 vLook;
		//m_pHubTransformCom->Get_Info(INFO_LOOK, &vLook);
		//D3DXVec3Normalize(&vLook, &vLook);
		//m_pHubTransformCom->Move_Pos(&vLook, 2.f, fTimeDelta);
		//m_pPlatformTransformCom->Move_Pos(&vLook, 2.f, fTimeDelta);
	}
	Engine::PlaySoundW(L"Water.wav", Engine::CSoundMgr::CHANNELID::Vacuum_Hub,0.3f);

}

void CHubPlatform::ChangePos(const _float & fTimeDelta, const _float & fMoveTime, _vec3 vStartPos, _vec3 vEndPos)
{
	m_fMoveTime = fMoveTime;
	m_vStartPos = vStartPos;
	m_vEndPos = vEndPos;


	m_fCurTime += fTimeDelta;
	m_pHubTransformCom->Set_Pos
	(
		(((m_vStartPos.x + ((((m_vEndPos.x) - (m_vStartPos.x)) / m_fMoveTime)*m_fCurTime)))),
		(((m_vStartPos.y + ((((m_vEndPos.y) - (m_vStartPos.y)) / m_fMoveTime)*m_fCurTime)))),
		(((m_vStartPos.z + ((((m_vEndPos.z) - (m_vStartPos.z)) / m_fMoveTime)*m_fCurTime))))
	);

	m_pPlatformTransformCom->Set_Pos
	(
		(((m_vStartPos.x + ((((m_vEndPos.x) - (m_vStartPos.x)) / m_fMoveTime)*m_fCurTime)))),
		(((m_vStartPos.y + ((((m_vEndPos.y) - (m_vStartPos.y)) / m_fMoveTime)*m_fCurTime)))) + m_fPlatformHeight,
		(((m_vStartPos.z + ((((m_vEndPos.z) - (m_vStartPos.z)) / m_fMoveTime)*m_fCurTime))))
	);


	if (m_fMoveTime < m_fCurTime)
	{
		m_bChangeStart_Pos = !m_bChangeStart_Pos;
		m_fCurrentAngle = 0.f;
		m_fCurTime = 0.f;
	}
}

CHubPlatform * CHubPlatform::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vStartPos, _vec3 vEndPos, void * pArg)
{
	CHubPlatform*	pInstance = new CHubPlatform(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vStartPos, vEndPos,pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CHubPlatform::Free(void)
{
	Engine::CGameObject::Free();
}
