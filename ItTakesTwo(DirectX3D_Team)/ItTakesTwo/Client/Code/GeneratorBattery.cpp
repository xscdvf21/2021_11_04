#include "stdafx.h"
#include "GeneratorBattery.h"
#include "Export_Function.h"
#include "BoundingBox.h"



CGeneratorBattery::CGeneratorBattery(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CGeneratorBattery::~CGeneratorBattery()
{
}

HRESULT CGeneratorBattery::Ready_Object(void * pArg, _vec3 TriggerPos, _vec3 vStartPos, _vec3 MoveDir)
{
	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	m_vTriggerScale = tTemp.vTriggerScale;
	m_vPushStartPos = vStartPos;
	m_vTriggerPos = TriggerPos;
	m_vMoveDir = MoveDir;
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	return S_OK;
}

_int CGeneratorBattery::Update_Object(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		m_pTrigger = CTrigger::CreateBoxTrigger(m_pScene, this, m_vTriggerPos, m_vTriggerScale.x, m_vTriggerScale.y, m_vTriggerScale.z, TRIGGERTYPE::TRIGGER_SPACE_GENERATOR_BATTERY);
		m_bStart = false;
	}

	_vec3 vPos;
	vPos = m_pTransformCom->Get_Position();

	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	pxTransform.p.x = vPos.x;
	pxTransform.p.y = vPos.y;
	pxTransform.p.z = vPos.z;
	_vec3 vRot = m_pTransformCom->Get_Angle();
	pxTransform.q = PxQuat(vRot.x, { 1.f,0.f,0.f })*PxQuat(vRot.y, { 0.f,1.f,0.f })*PxQuat(vRot.z, { 0.f,0.f,1.f });
	m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(pxTransform);

	Move(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CGeneratorBattery::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CGeneratorBattery::Render_Object(const _int & iIndex)
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

	m_pBox->Render_Collider(g_bRenderBox, pEffect);

	Safe_Release(pEffect);
}

void* CGeneratorBattery::On_Active(void* pCaller)
{
	return m_bMoveEnd ? &m_bMoveEnd : nullptr;
}


void CGeneratorBattery::Active()
{
	for (auto* pObj : m_vecInteractActor)
		static_cast<CGameObject*>(pObj)->On_Active(this);
}

HRESULT CGeneratorBattery::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}



	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"GeneratorBattery"));
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

	// Collider
	pComponent = m_pBox = CBoundingBox::Create(m_pGraphicDev, m_vTriggerScale * 2.0f, _vec4(0.0f, 244.f, 234.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);
	m_pBox->Set_Pos(m_vTriggerPos);


	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();

	PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(m_pTransformCom->Get_Angle().y, { 0.f,1.f,0.f }));
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

	return S_OK;
}

HRESULT CGeneratorBattery::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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



void CGeneratorBattery::Move(const _float & fTimeDelta)
{
	if (!m_bMove)
		return;


	if (m_fAngle >= 0.f)
	{
		m_pTransformCom->Rotation(ROT_Y, -D3DXToRadian(0.5f));
		m_fAngle -= D3DXToRadian(0.5f);
	}
	else
	{

		if (!m_bSoundStart)
		{
			StopSound(CSoundMgr::CHANNELID::Effect);
			PlaySoundW(L"Generator_Battery.wav", CSoundMgr::CHANNELID::Effect, 0.5f);

			m_bSoundStart = true;
			Active();
		}

		m_bMoveEnd = true;
	}

	m_bMove = false;
}

CGeneratorBattery * CGeneratorBattery::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, _vec3 TriggerPos, _vec3 StartPos, _vec3 MoveDir)
{
	CGeneratorBattery*	pInstance = new CGeneratorBattery(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg, TriggerPos, StartPos, MoveDir)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGeneratorBattery::Free(void)
{
	Engine::CGameObject::Free();
}
