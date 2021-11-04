#include "stdafx.h"
#include "WallSlideCylinder.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "PhysXMesh.h"


CWallSlideCylinder::CWallSlideCylinder(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CWallSlideCylinder::~CWallSlideCylinder(void)
{
}

HRESULT CWallSlideCylinder::Ready_Object(void* pArg)
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

Engine::_int CWallSlideCylinder::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bStart)
	{
		m_pCodyTransform = Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", ID_DYNAMIC)->Is<CTransform>();
		m_pMayTransform = Engine::Get_Component(L"GameObject", L"Cody", L"Com_Transform", ID_DYNAMIC)->Is<CTransform>();
		m_bStart = false;
	}

	_float3 vDis1 = m_pCodyTransform->Get_Position() - m_pTransformCom->Get_Position();
	_float3 vDis2 = m_pMayTransform->Get_Position() - m_pTransformCom->Get_Position();

	_float fDis1 = D3DXVec3Length(&vDis1);
	_float fDis2 = D3DXVec3Length(&vDis2);

	if (false == m_bStopRot)
	{
		if (!m_bChange)
		{
			if (false == m_bPlaySound && (fDis2 <= 25.0f || fDis1 <= 25.0f))
			{
				CSoundMgr::GetInstance()->StopNPlaySound(L"BandSaw.wav", CSoundMgr::CHANNELID::Vacuum_BandSaw, g_fEffecctSound);
				m_bPlaySound = true;
			}

			m_fTime += fTimeDelta * 2.0f;
			
			_float fRad = cosf(D3DXToRadian(90.0f - 90.0f * m_fTime));
			m_pTransformCom->Set_AngleY(D3DXToRadian(90.0f + 180.0f * fRad));
			if (m_fTime >= 1.0f)
			{
				m_bStopRot = true;
				m_fTime = 0.0f;
				m_pTransformCom->Set_AngleY(D3DXToRadian(270.0f));
				m_bChange = !m_bChange;
				m_bPlaySound = false;
			}
		}
		else
		{
			if (false == m_bPlaySound && (fDis2 <= 25.0f || fDis1 <= 25.0f))
			{
				CSoundMgr::GetInstance()->StopNPlaySound(L"BandSaw.wav", CSoundMgr::CHANNELID::Vacuum_BandSaw, g_fEffecctSound);
				m_bPlaySound = true;
			}

			m_fTime += fTimeDelta * 2.0f;
			_float fRad = cosf(D3DXToRadian(90.0f * m_fTime));
			m_pTransformCom->Set_AngleY(D3DXToRadian(90.0f + 180.0f * fRad));
			if (m_fTime >= 1.0f)
			{
				m_bStopRot = true;
				m_fTime = 0.0f;
				m_pTransformCom->Set_AngleY(D3DXToRadian(90.0f));
				m_bChange = !m_bChange;
				m_bPlaySound = false;
			}
		}
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

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CWallSlideCylinder::LateUpdate_Object(const _float & fTimeDelta)
{
	PxTransform cTrans = m_pRigidActor->getGlobalPose();
	_vec3 vRot = m_pTransformCom->Get_Angle();

	auto* pBody = m_pRigidActor->is<PxRigidDynamic>();
	cTrans.q = PxQuat(vRot.y, { 0.f,1.f,0.f });
	pBody->setKinematicTarget(cTrans);
	return _int();
}

void CWallSlideCylinder::Render_Object(const _int& iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1���� : ���� ���̴� ������ ���� �ִ� pass�� �ִ� ����, 2���� : �����ϴ� ���(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect, 8);

	pEffect->End();

	Safe_Release(pEffect);
}

CWallSlideCylinder* CWallSlideCylinder::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg)
{
	CWallSlideCylinder*	pInstance = new CWallSlideCylinder(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CWallSlideCylinder::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CWallSlideCylinder::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CStaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"WallSlideCylinder"));
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

	// Ʈ�������� ������������ �������� �ʴ¾ֵ�
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	//pBody->setKinematicTarget()
	// �׷���Ƽ�� ����.
	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	// ��������
	pBody->setMass(0);
	//�̵� ������
	pBody->setName((char*)this);

	return S_OK;
}

HRESULT CWallSlideCylinder::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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
