#include "stdafx.h"
#include "SpacePowerButton.h"
#include "Export_Function.h"
#include "MainCamera.h"
#include "PhysXMesh.h"
#include "Blow.h"


CSpacePowerButton::CSpacePowerButton(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CSpacePowerButton::~CSpacePowerButton(void)
{
}

void CSpacePowerButton::Move(const _float& fTimeDelta)
{
	if (!m_bButton_Click)
		return;
	m_fMoveTime += fTimeDelta;
	if (0.9f < m_fMoveTime)
	{
		m_bActive = false;
		m_bButton_Click = false;
		m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(PxTransform(m_vStartPos));
		m_pBodyTransformCom->Set_Pos(PxTransform(m_vStartPos));

		return;
	}
	if (m_fMoveTime < 0.3f)
	{

		m_pBodyTransformCom->Move_Pos(&_vec3{ 0.f,-1.f,0.f }, 3, fTimeDelta);
		_vec3 vPos = m_pBodyTransformCom->Get_Position();
		m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(PxTransform(PxVec3(vPos.x, vPos.y, vPos.z)));
	}
	if (0.6f < m_fMoveTime)
	{
		m_pBodyTransformCom->Move_Pos(&_vec3{ 0.f,1.f,0.f }, 2, fTimeDelta);
		_vec3 vPos = m_pBodyTransformCom->Get_Position();
		m_pRigidActor->is<PxRigidDynamic>()->setKinematicTarget(PxTransform(PxVec3(vPos.x, vPos.y, vPos.z)));
	}
}

HRESULT CSpacePowerButton::Ready_Object(void* pArg, const _int& iInteractableSize)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO Temp;
	if (pArg)
	{
		memcpy(&Temp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = Temp;
	m_iInteractableSize = iInteractableSize;

	return S_OK;
}

HRESULT CSpacePowerButton::Ready_Object(void* pArg /*= nullptr*/)
{
	return S_OK;
}

Engine::_int CSpacePowerButton::Update_Object(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	if (m_bStart)
	{
		m_bStart = false;
	}
	Move(fTimeDelta);


	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CSpacePowerButton::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CSpacePowerButton::Render_Object(const _int& iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1���� : ���� ���̴� ������ ���� �ִ� pass�� �ִ� ����, 2���� : �����ϴ� ���(default)
										//pEffect->BeginPass(0);
	if (true == m_bBase)
	{
		m_pMeshCom->Render_Meshes(pEffect, 8);
	}

	_matrix matWorld;
	m_pBodyTransformCom->Get_WorldMatrix(&matWorld);
	pEffect->SetMatrix("g_matWorld", &matWorld);
	m_pBodyMeshCom->Render_Meshes(pEffect);

	pEffect->End();

	Safe_Release(pEffect);
}

void* CSpacePowerButton::On_Active(void * pCaller)
{
	if (m_bActive)
		return nullptr;
	if (m_iInteractableSize == static_cast<CGameObject*>(pCaller)->Get_Size())
	{
		m_fMoveTime = 0.f;
		m_bActive = true;
		m_bButton_Click = true;
		if (m_pInteractActor)
			static_cast<CGameObject*>(m_pInteractActor)->On_Active(this);
		Engine::PlaySoundW(L"Play_Inter_Shared_GroundPoundButton_Press.bnk_1.wav", Engine::CSoundMgr::CHANNELID::Vacuum_PowerButton_Active);
	}

	return nullptr;
}

CSpacePowerButton* CSpacePowerButton::Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg /*= nullptr*/, const _int& iInteractableSize /*=1 */, _bool bBase /*= true*/)
{
	CSpacePowerButton*	pInstance = new CSpacePowerButton(pGraphicDev);

	pInstance->Set_Base(bBase);
	if (FAILED(pInstance->Ready_Object(pArg, iInteractableSize)))
		Safe_Release(pInstance);

	return pInstance;
}

void CSpacePowerButton::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CSpacePowerButton::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	// CStaticMesh
	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"BigCody_ButtonBase"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_HeadMesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	m_pTransformCom->Set_Pos(tTemp.vPos);

	// BodyCStaticMesh
	pComponent = m_pBodyMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"BigCody_Button"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_BodyMesh", pComponent);

	// Transform
	pComponent = m_pBodyTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_BodyTransform", pComponent);
	m_pBodyTransformCom->Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	tTemp.vPos.y += 1.f;
	m_pBodyTransformCom->Set_Pos(tTemp.vPos);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pBodyTransformCom->Get_Position();

	PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(m_pBodyTransformCom->Get_Angle().y, { 0.f,1.f,0.f }));
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);
	m_vStartPos = tTransform.p;
	PxTriangleMeshGeometry pMeshGeo;
	pMeshGeo.triangleMesh = m_pBodyMeshCom->Get_PxMesh();
	_vec3 vScale = m_pBodyTransformCom->Get_Scale();
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

HRESULT CSpacePowerButton::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));

	return S_OK;
}