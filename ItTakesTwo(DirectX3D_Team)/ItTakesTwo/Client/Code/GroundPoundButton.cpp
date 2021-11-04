#include "stdafx.h"
#include "GroundPoundButton.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "PhysXMesh.h"
#include "Blow.h"


CGroundPoundButton::CGroundPoundButton(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CGroundPoundButton::~CGroundPoundButton(void)
{
}

void CGroundPoundButton::Move(_vec3 vDir, _float fTime, const _float& fTimeDelta, _bool bMoveAll)
{
	m_pTransformCom->Set_Pos(m_pTransformCom->Get_Position() + vDir * fTimeDelta / fTime);
	_vec3 vPos = m_pTransformCom->Get_Position();
	if (m_bMoveAll)
	{
		m_vMoveDir = vDir * fTimeDelta / fTime;
		((CGameObject*)m_pInteractActor)->On_Active(this);
	}

	PxTransform tTransform = m_pRigidActor->getGlobalPose();
	tTransform.p.x = vPos.x;
	tTransform.p.y = vPos.y;
	tTransform.p.z = vPos.z;

	m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(tTransform);
}

void CGroundPoundButton::MoveAll(const _float & fTimeDelta, _float fMoveTime)
{
	m_vMoveDir = { 0.0f, 0.0f, 0.0f };
	m_fMoveTime += fTimeDelta;

	if (false == m_bMoveAll)
	{
		m_bMoveAll = true;
		m_vButtonPos = m_pTransformCom->Get_Position();
	}
	else if (true == m_bMoveAll && m_fMoveTime <= fMoveTime)
	{
		Move(m_vAllDir, fMoveTime, fTimeDelta, true);
	}
	else
	{
		m_bMoveAllEnd = true;
		m_vMoveDir = m_vButtonPos + m_vAllDir - m_pTransformCom->Get_Position();
		m_pTransformCom->Set_Pos(m_vButtonPos + m_vAllDir);
		_vec3 vPos = m_pTransformCom->Get_Position();
		((CGameObject*)m_pInteractActor)->On_Active(this);

		PxTransform tTransform = m_pRigidActor->getGlobalPose();
		tTransform.p.x = vPos.x;
		tTransform.p.y = vPos.y;
		tTransform.p.z = vPos.z;

		m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(tTransform);
	}
}

void CGroundPoundButton::MoveButton(const _float & fTimeDelta, _float fMoveTime)
{
	m_vMoveDir = { 0.0f, 0.0f, 0.0f };
	m_fMoveTime += fTimeDelta;

	if (false == m_bButtonMove)
	{
		m_bButtonMove = true;
		m_vButtonPos = m_pTransformCom->Get_Position();
	}
	else if (true == m_bButtonMove && m_fMoveTime <= fMoveTime)
	{
		Move(m_vButtonDir, fMoveTime, fTimeDelta);
	}
	else
	{
		m_bButtonMoveEnd = true;
		m_pTransformCom->Set_Pos(m_vButtonPos + m_vButtonDir);
		_vec3 vPos = m_pTransformCom->Get_Position();

		PxTransform tTransform = m_pRigidActor->getGlobalPose();
		tTransform.p.x = vPos.x;
		tTransform.p.y = vPos.y;
		tTransform.p.z = vPos.z;

		m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(tTransform);
	}
}

HRESULT CGroundPoundButton::Ready_Object(void* pArg)
{

	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;

	m_vButtonDir = { -0.25f, 0.0f, 0.0f };
	m_vAllDir = { -4.0f, 0.0f, 0.0f };

	return S_OK;
}

Engine::_int CGroundPoundButton::Update_Object(const _float& fTimeDelta)
{


	if (m_bDead)
		return OBJ_DEAD;
	if (m_bStart)
	{
		m_bStart = false;
	}

	if (m_bButton_Click)
	{
		if (false == m_bPlaySound)
		{

			StopNPlaySound(L"GroundPoundBox.wav", Engine::CSoundMgr::CHANNELID::Space_GroundPoundBox, 0.4f);
			m_bPlaySound = true;
		}


		if (!m_bEnd)
		{
			if (false == m_bButtonMoveEnd)
				MoveButton(fTimeDelta, 0.05f);
			else if (false == m_bMoveAllEnd)
				MoveAll(fTimeDelta, 0.2f);
			else
			{
				m_bButtonMoveEnd = false;
				m_bMoveAllEnd = false;
				m_bButtonMove = false;
				m_bMoveAll = false;
				m_bEnd = true;
				m_bActive = false;
				m_fMoveTime = 0.0f;
				m_fTime = 0.0f;
				m_vButtonDir *= -1.0f;
				m_vAllDir *= -1.0f;
			}
		}
		else
		{
			m_fTime += fTimeDelta;
			if (m_fTime >= 5.0f)
			{
				if (false == m_bButtonMoveEnd)
					MoveButton(fTimeDelta, 0.1f);
				else if (false == m_bMoveAllEnd)
					MoveAll(fTimeDelta, 2.0f);
				else
				{
					m_bButtonMoveEnd = false;
					m_bMoveAllEnd = false;
					m_bButtonMove = false;
					m_bMoveAll = false;
					m_bEnd = false;
					m_bButton_Click = false;
					m_bPlaySound = false;
					m_bActive = false;
					m_fTime = 0.0f;
					m_fMoveTime = 0.0f;
					m_vButtonDir *= -1.0f;
					m_vAllDir *= -1.0f;
				}
			}	
		}
	}
	//Move(fTimeDelta);


	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CGroundPoundButton::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CGroundPoundButton::Render_Object(const _int& iIndex)
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

void* CGroundPoundButton::On_Active(void * pCaller)
{
	if (m_bActive)
		return nullptr;
	m_fMoveTime = 0.f;
	m_bActive = true;
	m_bButton_Click = true;
	//for (auto* pObj : m_vecInteractActor)
	//	static_cast<CGameObject*>(pObj)->On_Active(this);

	return nullptr;
}

CGroundPoundButton* CGroundPoundButton::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg, _bool bBase)
{
	CGroundPoundButton*	pInstance = new CGroundPoundButton(pGraphicDev);

	pInstance->Set_Base(bBase);
	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CGroundPoundButton::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CGroundPoundButton::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CStaticMesh
	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"GroundPound_BoxButton"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Scale(_float3(0.02f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(tTemp.vPos);
	m_pTransformCom->Set_AngleY(D3DXToRadian(180.0f));

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();

	PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(m_pTransformCom->Get_Angle().y, { 0.f,1.f,0.f }));
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);
	m_vStartPos = tTransform.p;
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

HRESULT CGroundPoundButton::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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
