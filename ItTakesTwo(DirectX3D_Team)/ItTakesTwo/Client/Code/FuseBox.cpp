#include "stdafx.h"
#include "FuseBox.h"

#include "Export_Function.h"
#include "FuseStatic.h"

CFuseBox::CFuseBox(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)

{
	ZeroMemory(m_pFuse, sizeof(void*)*g_iFuseCount);
	//ZeroMemory(m_pFuseLight, sizeof(void*)*g_iFuseCount);
}

CFuseBox::~CFuseBox()
{
}

HRESULT CFuseBox::Ready_Object(void * pArg, _bool bRun)
{

	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);


	return S_OK;
}

_int CFuseBox::Update_Object(const _float & fTimeDelta)
{
	m_fDeltaTime += fTimeDelta;
	Engine::CGameObject::Update_Object(fTimeDelta);
	if (m_bStart)
	{
		for (_int i = 0; i < g_iFuseCount; ++i)
		{
			m_pSlotTrigger[i] = CTrigger::CreateBoxTrigger(m_pScene, this, m_pTransformCom->Get_Position() + _vec3{ -1.f,0.f,-0.8f + i*1.6f }, 0.5f, 3.f, 3.f, TRIGGERTYPE::TRIGGER_FUSE_SLOT);
			m_pSlotTrigger[i]->Set_Rotation(m_pTransformCom->Get_Angle());
			m_pSlotTrigger[i]->Set_Index(i);
			m_tObjInfo.vPos = m_pTransformCom->Get_Position() + _vec3{ -1.8f,1.f,-0.8f + i*1.6f };
			m_tObjInfo.vAngle = { 0.f,D3DXToRadian(180.f),0.f };
			m_pFuseLight[i] = CFuseLight::Create(m_pGraphicDev, &m_tObjInfo);
		}
		PxTransform tTransform = m_pRigidActor->getGlobalPose();
		tTransform.q = PxQuat(D3DXToRadian(90), PxVec3(0.f, 1.f, 0.f));
		m_pRigidActor->setGlobalPose(tTransform);
		m_bStart = false;
	}

	for (_int i = 0; i < g_iFuseCount; ++i)
	{
		if (m_pFuse[i])
			m_pFuse[i]->Update_Object(fTimeDelta);
		if (m_pFuseLight[i])
			m_pFuseLight[i]->Update_Object(fTimeDelta);
	}

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CFuseBox::LateUpdate_Object(const _float & fTimeDelta)
{

	return _int();
}

void CFuseBox::Render_Object(const _int & iIndex)
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
void* CFuseBox::On_Trigger_Active(void* pTrigger, void* pCaller)
{
	CTrigger* pTrig = ToTrigger(pTrigger);
	_int iIndex = pTrig->Get_Index();
	_vec3 vPos = pTrig->Get_Pos();
	m_tObjInfo.vPos = vPos + _vec3{ -0.9f,-0.2f,0.f };
	m_tObjInfo.vAngle = { 0.f,D3DXToRadian(90.f),0.f };
	m_tObjInfo.vScale = { 0.01f,0.01f ,0.01f };
	m_pFuse[iIndex] = CFuseStatic::Create(m_pGraphicDev, &m_tObjInfo);
	m_pFuseLight[iIndex]->Set_Light(true);
	Engine::StopNPlaySound(L"Play_World_Shed_Awakening_Interactable_FuseSocket_Insert.bnk_1.wav", CSoundMgr::CHANNELID::Vacuum_Fuse_Insert,0.5f);

	return nullptr;
}

HRESULT CFuseBox::Add_Component(void* pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));
	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}
	m_tObjInfo = tTemp;
	// CStaticMesh
	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"FuseBox"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Pos(tTemp.vPos);
	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Set_AngleY(D3DXToRadian(90.f));
	// Calculator
	pComponent = m_pCalculatorCom = static_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_fColDis, _vec4(50.f, 244.f, 234.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();
	PxTransform tTransform(tTemp.vPos.x, tTemp.vPos.y, tTemp.vPos.z);
	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0);
	PxTriangleMeshGeometry pMeshGeo;
	pMeshGeo.triangleMesh = m_pMeshCom->Get_PxMesh();//m_pPhysMesh->Get_PxMesh();
	pMeshGeo.scale = PxMeshScale(PxVec3(tTemp.vScale.x, tTemp.vScale.y, tTemp.vScale.z));
	PxShape* pShape = pPhysics->createShape(pMeshGeo, *pMaterial, true);

	if (pShape)
	{

		m_pRigidActor = PxCreateStatic(*pPhysics, tTransform, *pShape);

		m_pRigidActor->setName((char*)this);
	}

	return S_OK;
}

HRESULT CFuseBox::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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


void CFuseBox::Insert_Fuse(const _uint& iIndex)
{
	if (iIndex >= g_iFuseCount || iIndex < 0)
		return;
	if (m_bSlot[iIndex])
		return;

	m_bSlot[iIndex] = true;
	Check_AllReady();

}

Engine::_bool CFuseBox::Check_AllReady()
{
	if (m_bAllReady != false)
		return true;
	for (int i = 0; i < g_iFuseCount; ++i)
		if (!m_bSlot[i])
			return false;
	Engine::StopNPlaySound(L"Play_World_Shed_Awakening_Interactable_FuseSocket_Generator_Start.bnk_1.wav", CSoundMgr::CHANNELID::Vacuum_Fuse_Ready, 1.f);

	m_bAllReady = true;
	return true;
}

CFuseBox * CFuseBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, _bool bRun)
{
	CFuseBox*	pInstance = new CFuseBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg, bRun)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFuseBox::Free(void)
{
	for (_int i = 0; i < g_iFuseCount; ++i)
	{
		if (m_pFuse[i])
			Safe_Release(m_pFuse[i]);
		if (m_pFuseLight[i])
			Safe_Release(m_pFuseLight[i]);
	}
	Engine::CGameObject::Free();
}
