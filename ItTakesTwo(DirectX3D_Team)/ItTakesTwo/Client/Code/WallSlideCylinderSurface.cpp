#include "stdafx.h"
#include "WallSlideCylinderSurface.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "PhysXMesh.h"
#include "PlayerValue.h"
#include "Cody.h"
#include "May.h"

CWallSlideCylinderSurface::CWallSlideCylinderSurface(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
	
}

CWallSlideCylinderSurface::~CWallSlideCylinderSurface(void)
{
}

HRESULT CWallSlideCylinderSurface::Ready_Object(void* pArg)
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

Engine::_int CWallSlideCylinderSurface::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fMoveAngle = 0.0f;

	if (false == m_bStopRot)
	{
		m_fPreTime = m_fTime;
		m_fTime += fTimeDelta * 2.0f;
		m_fPreAngle = m_pTransformCom->Get_RevAngle().y;
		if (!m_bChange)
		{
			_float fRad = cosf(D3DXToRadian(90.0f - 90.0f * m_fTime));
			m_pTransformCom->Set_RevAngleY(D3DXToRadian(180.0f * fRad));

			if (m_fTime >= 1.0f)
			{
				m_bStopRot = true;
				m_fTime = 0.0f;
				m_pTransformCom->Set_RevAngleY(D3DXToRadian(180.0f));
				m_bChange = !m_bChange;
			}
		}
		else
		{
			_float fRad = cosf(D3DXToRadian(90.0f * m_fTime));
			m_pTransformCom->Set_RevAngleY(D3DXToRadian(180.0f * fRad));
			if (m_fTime >= 1.0f)
			{
				m_bStopRot = true;
				m_fTime = 0.0f;
				m_pTransformCom->Set_RevAngleY(D3DXToRadian(0.0f));
				m_bChange = !m_bChange;
			}
		}

		m_fMoveAngle = m_pTransformCom->Get_RevAngle().y - m_fPreAngle;
	}
	else
	{
		m_fStopTime += fTimeDelta;
		if (m_fStopTime >= 1.0f)
		{
			m_fStopTime = 0.0f;
			m_bStopRot = false;
		}
	}

	//if (false == m_bStopRot)
	//{
	//	m_fPreTime = m_fTime;
	//	m_fTime += fTimeDelta * 2.5f;
	//	if (m_fTime >= 1.0f)
	//	{
	//		m_bStopRot = true;
	//		m_fTime = 0.0f;
	//		m_fAddAngle = 3.14f - m_fAddAngle;
	//		m_pTransformCom->Set_RevAddAngleY(m_fAddAngle);
	//		m_fMoveAngle = m_fAddAngle;
	//		m_fAddAngle = 0.0f;
	//	}
	//	else
	//	{
	//		m_fAddAngle += 3.14f * (cosf(m_fPreTime * 3.14f * 0.5f) - cosf(m_fTime * 3.14f * 0.5f));
	//		_float fAngle = 3.14f * (cosf(m_fPreTime * 3.14f * 0.5f) - cosf(m_fTime * 3.14f * 0.5f));
	//		m_pTransformCom->Set_RevAddAngleY(fAngle);
	//		m_fMoveAngle = fAngle;
	//	}
	//}
	//else
	//{
	//	m_fMoveAngle = 0.0f;
	//	m_fStopTime += fTimeDelta;
	//	if (m_fStopTime >= 1.0f)
	//	{
	//		m_fStopTime = 0.0f;
	//		m_bStopRot = false;
	//	}
	//}

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	_matrix matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	PxTransform cTrans = m_pRigidActor->getGlobalPose();
	cTrans.p.x = matWorld._41;
	cTrans.p.y = matWorld._42;
	cTrans.p.z = matWorld._43;

	_vec3 vRot = m_pTransformCom->Get_RevAngle() + m_pTransformCom->Get_Angle();

	auto* pBody = m_pRigidActor->is<PxRigidDynamic>();
	cTrans.q = PxQuat(vRot.y, { 0.f,1.f,0.f });
	pBody->setKinematicTarget(cTrans);
	return OBJ_NOEVENT;
}

_int CWallSlideCylinderSurface::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CWallSlideCylinderSurface::Render_Object(const _int& iIndex)
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

CWallSlideCylinderSurface* CWallSlideCylinderSurface::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CWallSlideCylinderSurface*	pInstance = new CWallSlideCylinderSurface(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CWallSlideCylinderSurface::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CWallSlideCylinderSurface::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"WallSlideCylinderSurface"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Parent(tTemp.vPos);
	m_pTransformCom->Set_Pos(_vec3(0.0f, 0.0f, 1.8f));
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

HRESULT CWallSlideCylinderSurface::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void * CWallSlideCylinderSurface::On_Attach(void * pCaller)
{
	_matrix matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	CGameObject* pPlayer = (CGameObject*)pCaller;
	CTransform* pTransform = (CTransform*)pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_vec3 vPlayerPos = pTransform->Get_Position();
	_vec3 vPos = _vec3(matWorld._41, matWorld._42, matWorld._43);
	_vec3 vAttachDir = pPlayer->Is<CCody>()->Get_AttachDir();
	// 그라인더의 위치를 원점으로 맞춰줌
	//vPlayerPos -= vPos;
	//vPlayerPos.y = 0.0f;

	_matrix matRotZ;
	D3DXMatrixRotationY(&matRotZ, m_fMoveAngle);

	_vec3 vResultPos;
	//D3DXVec3TransformNormal(&vResultPos, &vPlayerPos, &matRotZ);
	D3DXVec3TransformNormal(&vAttachDir, &vAttachDir, &matRotZ);
	pPlayer->Is<CCody>()->Set_AttachDir(vAttachDir);

	vResultPos = vPos + vAttachDir;

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
