#include "stdafx.h"
#include "PowerSwitch.h"

#include "Export_Function.h"
#include "FuseStatic.h"
#include "Cody.h"
#include "May.h"
#include "FuseNPC.h"
#include "MayCamera.h"

CPowerSwitch::CPowerSwitch(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)

{
	ZeroMemoryArray<void*>(m_pTrigger, 2);
}

CPowerSwitch::~CPowerSwitch()
{
}

void CPowerSwitch::Move(const _float & fTimeDelta)
{
	if (m_bMoveEnd)
		return;

	_float fAngle = m_pTransformCom->Get_Angle().z;


	if (m_bMayOn && m_bCodyOn && m_bAllReady)
	{
		if (fAngle > D3DXToRadian(60))
		{
			ZeroMemory(&m_bSocket, sizeof(m_bSocket));
			m_bAllReady = false;
			if (false == m_bLaunchFuse)
			{
				// #레버 내리고 퓨즈 발사
				m_pTransformCom->Set_AngleZ(D3DXToRadian(60.0f));
				m_bLaunchFuse = true;
				CFuseNPC* pFuse = Engine::Get_GameObject(L"GameObject", L"FuseNPC")->Is<CFuseNPC>();
				if (pFuse)
					pFuse->Set_Work();
				pFuse = Engine::Get_GameObject(L"GameObject", L"FuseNPC1")->Is<CFuseNPC>();
				if (pFuse)
					pFuse->Set_Work();
				pFuse = Engine::Get_GameObject(L"GameObject", L"FuseNPC2")->Is<CFuseNPC>();
				if (pFuse)
					pFuse->Set_Work();

				m_bShoot = true;
				Engine::End_Split();
				CMayCamera* pCamera= Engine::Get_GameObject(L"GameObject", L"MayCamera")->Is<CMayCamera>();
				pCamera->Set_Mode(CAMERAMODE::CAMERA_FIX);
				pCamera->Set_Fix({ 48.8749733f, 0.0343255997f ,-14.2252769f }, { 52.4530029f , -2.80048752f , -21.1139965f }, true);
				pCamera->Set_ResetTimer(3.f,false);
				
				StopNPlaySound(L"VF_PowerSwitch_1.ogg", CSoundMgr::CHANNELID::CrusherHitPoint, 0.3f);

			}
			else
			{
				// #마지막에 레버 내리는곳
				Engine::Get_GameObject(L"GameObject", L"SceneChangeLine")->On_Active(nullptr);
				Engine::Get_GameObject(L"Trigger", L"SceneChange")->On_Active(nullptr);
				
				StopNPlaySound(L"VF_Clear.ogg", CSoundMgr::CHANNELID::CrusherHitPoint, 0.3f);
			}
			return;

		}
		m_pTransformCom->Set_AddAngleZ(/*D3DXToRadian()*/fTimeDelta * 2.0f);
	}
	else if (true == m_bLaunchFuse && false == m_bRev)
	{
		m_fRevTime += fTimeDelta;
		if (m_fRevTime <= 1.0f)
			return;
		if (fAngle < D3DXToRadian(-15))
		{
			m_pTransformCom->Set_AngleZ(D3DXToRadian(-15.0f));
			m_bRev = true;
		}
		m_pTransformCom->Set_AddAngleZ(/*D3DXToRadian()*/fTimeDelta * -2.0f);
	}
}

HRESULT CPowerSwitch::Ready_Object(void * pArg, _bool bRun)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);
	ZeroMemory(&m_bSocket, sizeof(m_bSocket));
	memset(&m_bSocket, true, sizeof(_bool) * 3);

	return S_OK;
}

_int CPowerSwitch::Update_Object(const _float & fTimeDelta)
{
	if (Engine::Key_Down(DIK_F2))
	{
		Engine::Get_GameObject(L"GameObject", L"SceneChangeLine")->On_Active(nullptr);
		Engine::Get_GameObject(L"Trigger", L"SceneChange")->On_Active(nullptr);
	}

	m_fDeltaTime += fTimeDelta;
	Engine::CGameObject::Update_Object(fTimeDelta);
	if (m_bStart)
	{
		for (_int i = 0; i < 2; ++i)
		{
			m_pTrigger[i] = CTrigger::CreateBoxTrigger(m_pScene, this, m_tObjInfo.vPos, 3.0f, 7.0f, 3.0f, TRIGGERTYPE::TRIGGER_POWERSWITCH);
			m_pTrigger[i]->Set_Index(i);
		}

		PxTransform tTransform = m_pRigidActor->getGlobalPose();
		//tTransform.q = PxQuat(D3DXToRadian(90), PxVec3(0.f, 1.f, 0.f));
		m_pRigidActor->setGlobalPose(tTransform);
		m_bStart = false;
	}
	Check_AllReady();
	Move(fTimeDelta);

	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CPowerSwitch::LateUpdate_Object(const _float & fTimeDelta)
{

	return _int();
}

void CPowerSwitch::Render_Object(const _int & iIndex)
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

void* CPowerSwitch::On_Trigger_Active(void* pTrigger, void* pCaller)
{
	CTrigger* pTrig = ToTrigger(pTrigger);
	_int iIndex = pTrig->Get_Index();
	_vec3 vPos = pTrig->Get_Pos();
	m_tObjInfo.vPos = vPos + _vec3{ -0.9f,-0.2f,0.f };
	m_tObjInfo.vAngle = { 0.f,D3DXToRadian(90.f),0.f };
	m_tObjInfo.vScale = { 0.01f,0.01f ,0.01f };
	//m_pFuse[iIndex] = CFuseStatic::Create(m_pGraphicDev, &m_tObjInfo);
	//m_pFuseLight[iIndex]->Set_Light(true);
	return nullptr;
}

HRESULT CPowerSwitch::Add_Component(void* pArg)
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
	pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"PowerSwitch"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	m_pTransformCom->Set_Pos(tTemp.vPos);
	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Set_Angle(m_tObjInfo.vAngle);
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
	PxTransform tTransform(tTemp.vPos.x, tTemp.vPos.y, tTemp.vPos.z, PxQuat(m_tObjInfo.vAngle.x, PxVec3(1.f, 0.f, 0.f))*PxQuat(m_tObjInfo.vAngle.y, PxVec3(0.f, 1.f, 0.f))*PxQuat(m_tObjInfo.vAngle.z, PxVec3(0.f, 0.f, 1.f)));
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

HRESULT CPowerSwitch::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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


Engine::_bool CPowerSwitch::Check_AllReady()
{
	//Engine::Get_GameObject(L"")
	if (m_bAllReady)
		return true;
	m_bAllReady = true;
	_int j = 0;
	for (_uint i = 0; i < 3; ++i)
	{
		if (m_bSocket[i] == false)
		{
			m_bAllReady = false;
			if (j == 2 && m_bSound)
			{

				StopNPlaySound(L"VF_WhereLastFuse.ogg", CSoundMgr::CHANNELID::CrusherHitPoint, 0.3f);
				m_bSound = false;
			}
		}
		++j;
	}
	if (!m_bAllReady)
		return false;


	if(m_bShoot)
		Engine::StopNPlaySound(L"Play_World_Shed_Awakening_Interactable_FuseSocket_Generator_Start.bnk_1.wav", CSoundMgr::CHANNELID::Vacuum_Fuse_Ready, 1.f);

	return true;
}

void CPowerSwitch::Set_Socket(_uint iIdx, _bool bState)
{
	m_bSocket[iIdx] = bState;
}

CPowerSwitch * CPowerSwitch::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, _bool bRun)
{
	CPowerSwitch*	pInstance = new CPowerSwitch(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg, bRun)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPowerSwitch::Free(void)
{

	Engine::CGameObject::Free();
}

void CPowerSwitch::Set_Switch(bool bState)
{
	m_bAllReady = true;
}
