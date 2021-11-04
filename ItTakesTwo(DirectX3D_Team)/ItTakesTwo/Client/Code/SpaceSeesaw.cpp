#include "stdafx.h"
#include "SpaceSeesaw.h"
#include "Export_Function.h"
#include "May_Space.h"
#include "Cody_Space.h"



CSpaceSeesaw::CSpaceSeesaw(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CSpaceSeesaw::~CSpaceSeesaw()
{
}

HRESULT CSpaceSeesaw::Ready_Object(void * pArg)
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

_int CSpaceSeesaw::Update_Object(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		_vec3 vPos, vCurPos;
		vPos = vCurPos= m_pTransformCom->Get_Position();
		_vec3 vAngle = m_pTransformCom->Get_Angle();

		PxTransform Transform;
		Transform.p = { vCurPos.x,vCurPos.y,vCurPos.z };

		_mat matRot;
		D3DXMatrixRotationY(&matRot, m_pTransformCom->Get_Angle().y);
		D3DXVec3TransformCoord(&vPos, &_vec3{ 0.f,0.f,12.f }, &matRot);

		CTrigger::CreateBoxTrigger(m_pScene, this, m_pTransformCom->Get_Position() + vPos, 3.f, TriggerType::TRIGGER_BOUNCE_SWITCH);
		m_bStart = false;

		D3DXVec3Normalize(&vPos, &vPos);
		//D3DXVec3TransformNormal(&vPos, &vPos,&matRot);
		D3DXVec3Cross(&m_vAxis, &vUP, &vPos);
		Transform.q = PxQuat(vAngle.x, { m_vAxis.x,m_vAxis.y,m_vAxis.z})*PxQuat(vAngle.y, { 0.f,1.f,0.f });
		m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(Transform);
	}
	m_fResetTime += fTimeDelta;

	Move(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CSpaceSeesaw::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CSpaceSeesaw::Render_Object(const _int & iIndex)
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
	_matrix			matWorld;
	m_pTransformBaseCom->Get_WorldMatrix(&matWorld);
	pEffect->SetMatrix("g_matWorld", &matWorld);

	m_pMeshBaseCom->Render_Meshes(pEffect, 8);

	pEffect->End();

	Safe_Release(pEffect);
}

void* CSpaceSeesaw::On_Active(void* pCaller)
{
	if (m_fResetTime < 2.f)
		return nullptr;
	_int iSize =static_cast<CGameObject*>(pCaller)->Get_Size();
	switch (iSize)
	{
	case 0: 
		m_fPushWeight = 0.f;
		return nullptr;
		break;
	case 1:
		m_fTargetValue = -0.2f;
		m_fPushWeight = 1.5f;
		break;
	case 2:
		m_fTargetValue = -0.4f;
		m_fPushWeight = 2.5f;
		break;
	default:
		break;
	}
	m_fResetTime = 0.f;
	m_bMove = true;
	Push();
	return nullptr;
}

void CSpaceSeesaw::Push()
{
	Engine::StopNPlaySound(L"LaunchBoard.wav", CSoundMgr::CHANNELID::Space_LaunchBoard, 0.5f);

	Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay_Space>()->Set_EventJump(m_fPushWeight);
	Engine::Get_GameObject(L"GameObject", L"Cody")->Is<CCody_Space>()->Set_EventJump(m_fPushWeight);
}

HRESULT CSpaceSeesaw::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	
	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"Seesaw"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pMeshBaseCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"SeesawBase"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MeshBase", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTransformBaseCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform2", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformCom->Set_Pos(tTemp.vPos);

	m_pTransformBaseCom->Set_Scale(tTemp.vScale);
	m_pTransformBaseCom->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	m_pTransformBaseCom->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	m_pTransformBaseCom->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformBaseCom->Set_Pos(tTemp.vPos + _vec3{0.f,-0.9f,0.f});

	m_fValue = 0.11f;
	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();
	_vec3 vAngle = m_pTransformCom->Get_Angle();

	PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(D3DXToRadian(tTemp.vAngle.y), { 0.f,1.f,0.f }));
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

	m_pTransformCom->Set_AngleX(m_fValue);

	return S_OK;
}

HRESULT CSpaceSeesaw::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CSpaceSeesaw::Move(const _float& fTimeDelta)
{
	if (!m_bMove)
		return;
	if (m_fValue < m_fTargetValue)
	{
		m_bResetStart = true;

	}
	if (m_bResetStart)
	{
		m_fValue += fTimeDelta*m_fSpeed;

		if (m_fValue > 0.11f)
		{

			m_fValue = 0.11f;
			m_bResetStart = false;
			m_bMove = false;
		}
	}
	else
	{
		m_fValue -= fTimeDelta*m_fSpeed;
	}

	m_pTransformCom->Set_AngleX(m_fValue);
	_vec3 vPos =m_pTransformCom->Get_Position();
	_vec3 vAngle =m_pTransformCom->Get_Angle();

	PxTransform Transform;
	Transform.p = { vPos.x,vPos.y,vPos.z };
	
	Transform.q = PxQuat(vAngle.x, { m_vAxis.x,m_vAxis.y,m_vAxis.z })*PxQuat(vAngle.y, { 0.f,1.f,0.f });
	m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(Transform);
}

CSpaceSeesaw * CSpaceSeesaw::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CSpaceSeesaw*	pInstance = new CSpaceSeesaw(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpaceSeesaw::Free(void)
{
	Engine::CGameObject::Free();
}
