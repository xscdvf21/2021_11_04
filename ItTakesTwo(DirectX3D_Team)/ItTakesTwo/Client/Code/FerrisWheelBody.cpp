#include "stdafx.h"
#include "FerrisWheelBody.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "PhysXMesh.h"
#include "PlayerValue.h"
#include "BoundingBox.h"
#include "SmallFan.h"
#include "FerrisWheelModule.h"
#include "Cody.h"
#include "May.h"


CFerriswheelBody::CFerriswheelBody(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CFerriswheelBody::~CFerriswheelBody(void)
{
}

HRESULT CFerriswheelBody::Ready_Object(_vec3 vDis, _vec3 vRot, void* pArg)
{
	_matrix matRotX;
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(vRot.x));
	D3DXVec3TransformNormal(&m_vAddDir, &vDis, &matRotX);

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

Engine::_int CFerriswheelBody::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fAddAngle = 0.0f;
	switch (m_eDir)
	{
	case CFerriswheelBody::Right:
		m_fRotAngle -= fTimeDelta * 360.0f;
		m_fAddAngle = -D3DXToRadian(fTimeDelta * 360.0f);
		if (m_fRotAngle <= -90.0f)
		{
			m_fAddAngle = 0.0f;
			m_fRotAngle = -90.0f;
		}
		break;
	case CFerriswheelBody::Left:
		m_fRotAngle += fTimeDelta * 360.0f;
		m_fAddAngle = D3DXToRadian(fTimeDelta * 360.0f);
		if (m_fRotAngle >= 90.0f)
		{
			m_fAddAngle = 0.0f;
			m_fRotAngle = 90.0f;
		}
		break;
	}
	m_pTransformCom->Set_AngleY(D3DXToRadian(m_fRotAngle));
	
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CFerriswheelBody::LateUpdate_Object(const _float & fTimeDelta)
{
	_matrix matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	PxTransform cTrans = m_pRigidActor->getGlobalPose();
	cTrans.p.x = matWorld._41;
	cTrans.p.y = matWorld._42;
	cTrans.p.z = matWorld._43;

	//PxTransform cTrans = m_pRigidActor->getGlobalPose();
	_vec3 vRot = m_pTransformCom->Get_Angle();

	auto* pBody = m_pRigidActor->is<PxRigidDynamic>();
	cTrans.q = PxQuat(vRot.y, { 0.f,1.f,0.f });
	pBody->setKinematicTarget(cTrans);
	return _int();
}

void CFerriswheelBody::Render_Object(const _int& iIndex)
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

void * CFerriswheelBody::On_Attach(void * pCaller)
{
	CGameObject* pPlayer = (CGameObject*)pCaller;
	CTransform* pTransform = (CTransform*)pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC);
	_vec3 vResultDir;
	_vec3 vResultPos;
	_vec3 vPos = m_pTransformCom->Get_Position();
	_vec3 vPlayerPos = pTransform->Get_Position();


	if (typeid(CCody) == typeid(*pPlayer))
	{
		_vec3 vAttachDir = pPlayer->Is<CCody>()->Get_AttachDir();

		_matrix matRotX, matRotY;
		D3DXMatrixRotationY(&matRotY, m_fAddAngle);

		D3DXVec3TransformNormal(&vAttachDir, &vAttachDir, &matRotY);
		pPlayer->Is<CCody>()->Set_AttachDir(vAttachDir);

		vResultPos = vPos + vAttachDir;
		vResultDir = vResultPos - vPlayerPos;

		pPlayer->Is<CCody>()->Set_AttachAngle(m_fAddAngle);
		pPlayer->Is<CCody>()->Set_Rot(true);
	}
	else
	{
		_vec3 vAttachDir = pPlayer->Is<CMay>()->Get_AttachDir();

		_matrix matRotX, matRotY;
		D3DXMatrixRotationY(&matRotY, m_fAddAngle);

		D3DXVec3TransformNormal(&vAttachDir, &vAttachDir, &matRotY);
		pPlayer->Is<CMay>()->Set_AttachDir(vAttachDir);

		vResultPos = vPos + vAttachDir;
		vResultDir = vResultPos - vPlayerPos;

		pPlayer->Is<CMay>()->Set_AttachAngle(m_fAddAngle);
		pPlayer->Is<CMay>()->Set_Rot(true);
	}

	m_vResult = vResultDir;
	return &m_vResult;
}

CFerriswheelBody* CFerriswheelBody::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vDis, _vec3 vRot, void* pArg)
{
	CFerriswheelBody*	pInstance = new CFerriswheelBody(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vDis, vRot, pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CFerriswheelBody::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CFerriswheelBody::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"FerrisBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(tTemp.vPos + m_vAddDir);
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

HRESULT CFerriswheelBody::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CFerriswheelBody::Update_Position(_vec3 vMovePos, _vec3 vCenterPos, _float fRevAngle, _vec3 vRevDir, const _float& fTimeDelta)
{
	m_fAddRevAngle = fRevAngle;
	m_vCenterPos = vCenterPos;
	m_vRevDir = vRevDir;

	m_pTransformCom->Set_Pos(vMovePos + _vec3(-4.07f, -0.45f, -0.06f));
	_vec3 vDir = m_pTransformCom->Get_Position() - vCenterPos;
	D3DXVec3Normalize(&vDir, &vDir);
	//_vec3 vUp = { 0.0f, 1.0f, 0.0f };
	_vec3 vLook = { 0.0f, 0.0f, 1.0f };

	vDir.x = 0.0f;

	_float fCosTh = D3DXVec3Dot(&vDir, &vLook);
	//_float fCosTh = D3DXVec3Dot(&vDir, &vUp);

	if (fCosTh >= 0.0f)
	{
		m_eDir = Left;
	}
	else
	{
		m_eDir = Right;
	}

	Engine::CGameObject::Update_Object(fTimeDelta);
}
