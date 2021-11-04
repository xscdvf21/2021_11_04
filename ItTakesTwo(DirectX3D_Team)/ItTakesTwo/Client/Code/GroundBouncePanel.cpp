#include "stdafx.h"
#include "GroundBouncePanel.h"
#include "Export_Function.h"
#include "Trigger.h"



CGroundBouncePanel::CGroundBouncePanel(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CGroundBouncePanel::~CGroundBouncePanel()
{
}

void CGroundBouncePanel::Move(const _float & fTimeDelta)
{
	if (!m_bMove)
		return;
	_vec3 vPos =m_pTransformCom->Get_Position();
	_vec3 vRot =m_pTransformCom->Get_Angle();
	if (vRot.x >= D3DXToRadian(180.f))
	{
		m_bMove = false;
		vRot.x = D3DXToRadian(180.f);
		m_pTransformCom->Set_Angle(vRot);
		m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(PxTransform(vPos.x, vPos.y, vPos.z, PxQuat(D3DXToRadian(180.f), { 1.f,0.f,0.f })));
		return;
	}

	m_pTransformCom->Set_AddAngleX(fTimeDelta/m_fTotalTime);
	m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(PxTransform(vPos.x, vPos.y, vPos.z, PxQuat(vRot.x, {1.f,0.f,0.f})));
}

HRESULT CGroundBouncePanel::Ready_Object(void * pArg)
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

_int CGroundBouncePanel::Update_Object(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		_vec3 vPos =m_pTransformCom->Get_Position();
		vPos.x -= 3.f;
		vPos.y += 2.f;

		CTrigger::CreateBoxTrigger(m_pScene, this, vPos, 2.f, 0.5f, 2.f, TriggerType::TRIGGER_BOUNCE_SWITCH);
		vPos.x += 5.f;
		vPos.y -= 4.f;

		CTrigger::CreateBoxTrigger(m_pScene, this, vPos, 2.f, 0.5f, 2.f, TriggerType::TRIGGER_BOUNCE_SWITCH);
		m_bStart = false;
	}
	if (m_iCount > 0)
	{
		m_fTime += fTimeDelta;
		if (m_fTime > 1.f)
		{
			--m_iCount;
			m_fTime = 0.f;
		}
	}
	Move(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CGroundBouncePanel::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CGroundBouncePanel::Render_Object(const _int & iIndex)
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

	Safe_Release(pEffect);
}


void* CGroundBouncePanel::On_Trigger_Active(void* pTrigger, void* pCaller)
{
	++m_iCount;
	m_fTime = 0;
	if (m_iCount >= 2)
	{
		m_bMove = true;
	}
	return nullptr;
}

void* CGroundBouncePanel::On_Active(void* pCaller)
{
	++m_iCount;
	m_fTime = 0;
	if (m_iCount >= 2)
	{
		CSoundMgr::GetInstance()->PlaySoundW(L"GroundPoundBox.wav", CSoundMgr::CHANNELID::Space_Panel, g_fEffecctSound);
		m_bMove = true;
	}
	return nullptr;
}

HRESULT CGroundBouncePanel::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}



	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"GroundBounce_Panel"));
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

	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();
	_vec3 vDir = m_pTransformCom->Get_Angle();

	m_vStartPos = { vPos.x,vPos.y,vPos.z };
	PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(D3DXToRadian(tTemp.vAngle.y), { 0.f,1.f,0.f }));
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxTriangleMeshGeometry pMeshGeo;
	pMeshGeo.triangleMesh = m_pMeshCom->Get_PxMesh();//m_pPhysMesh->Get_PxMesh();
	pMeshGeo.scale = PxMeshScale(PxVec3(tTemp.vScale.x, tTemp.vScale.y, tTemp.vScale.z)/*, PxQuat(D3DXToRadian(tTemp.vAngle.y), { 0.f,1.f,0.f })*/);

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

HRESULT CGroundBouncePanel::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

CGroundBouncePanel * CGroundBouncePanel::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CGroundBouncePanel*	pInstance = new CGroundBouncePanel(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGroundBouncePanel::Free(void)
{
	Engine::CGameObject::Free();
}
