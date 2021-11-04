#include "stdafx.h"
#include "SpaceValve.h"
#include "Export_Function.h"
#include "May_Space.h"


CSpaceValve::CSpaceValve(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CSpaceValve::~CSpaceValve()
{
}

HRESULT CSpaceValve::Ready_Object(_vec3 vTargetPos, _vec3 vNormal, wstring ObjTag, void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	m_vTargetPos = vTargetPos;
	m_vNormal = vNormal;
	m_ObjTag = ObjTag;

	return S_OK;
}

_int CSpaceValve::Update_Object(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		CTrigger::CreateBoxTrigger(m_pScene, this, m_tObjInfo.vTriggerPos, m_tObjInfo.vTriggerScale.x, m_tObjInfo.vTriggerScale.y, m_tObjInfo.vTriggerScale.z, TRIGGERTYPE::TRIGGER_HANDLE);
		m_pLayer = Engine::Get_Layer(L"Valve");

		m_bStart = false;
	}
	
	if (m_bResetAngle)
		ResetAngle(fTimeDelta);
	else
		Move(fTimeDelta);

	

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CSpaceValve::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CSpaceValve::Render_Object(const _int & iIndex)
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
void CSpaceValve::Move(const _float & fTimeDelta)
{
	_vec3 vAngle = m_pTransformCom->Get_Angle();
	m_pTransformCom->Set_AngleX(vAngle.x);
}
void CSpaceValve::Rotate(const _float& fValue)
{
	m_fValue += fValue;
	if (m_fValue >= 10.f)
		m_fValue = 10.f;
	for (auto& rPair : m_pLayer->Get_GameObjectMap())
	{
		rPair.second->Set_Value(m_fValue);
	}

	CSoundMgr::GetInstance()->PlaySoundW(L"SF2ValveSpring.wav", CSoundMgr::CHANNELID::Space_Valve, g_fEffecctSound);
}

void CSpaceValve::Release_Valve()
{
	Engine::Get_GameObject(L"GameObject", L"May")->Is<CMay_Space>()->Set_EventJump(1.3f + 0.15f * m_fValue);
	m_fValue = 0.f;
	for (auto& rPair : m_pLayer->Get_GameObjectMap())
	{
		rPair.second->Set_Value(m_fValue);
	}

	m_bResetAngle = true;
}

void CSpaceValve::Set_AddAngle(_float fRad)
{
	if (m_fValue >= 10.0f)
		return;

	m_pTransformCom->Set_AddAngleX(fRad);
}

HRESULT CSpaceValve::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	
	m_tObjInfo = tTemp;

	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"Space_Valve"));
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



	return S_OK;
}

HRESULT CSpaceValve::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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

void CSpaceValve::ResetAngle(const _float & fTimeDelta)
{
	if (false == m_bResetStart)
	{
		m_bResetStart = true;
		m_fTargetAngle = m_pTransformCom->Get_Angle().x;
		m_fTime = m_fResetTime;
	}

	m_fTime -= fTimeDelta;
	if (m_fTime <= 0.0f)
	{
		m_fTime = m_fResetTime;
		m_pTransformCom->Set_AngleX(m_fTargetAngle);
		m_bResetAngle = false;
		m_bResetStart = false;
		return;
	}
	m_pTransformCom->Set_AngleX(m_fTargetAngle * cosf(D3DXToRadian(90.0f * 1.0f - m_fTime)));
}

CSpaceValve * CSpaceValve::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vTargetPos, _vec3 vNormal, wstring ObjTag, void * pArg)
{
	CSpaceValve*	pInstance = new CSpaceValve(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vTargetPos, vNormal, ObjTag, pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpaceValve::Free(void)
{
	Engine::CGameObject::Free();
}
