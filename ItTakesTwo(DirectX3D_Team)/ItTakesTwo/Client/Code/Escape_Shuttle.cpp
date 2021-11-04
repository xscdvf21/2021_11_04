#include "stdafx.h"
#include "Escape_Shuttle.h"
#include "MainCamera.h"
#include "Cody_Wizard.h"
#include "May_Castle.h"
#include "Logo.h"
#include "Export_Function.h"

CEscape_Shuttle::CEscape_Shuttle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CEscape_Shuttle::~CEscape_Shuttle()
{
}

HRESULT CEscape_Shuttle::Ready_Object(void * pArg, _bool bState)
{
	//업데이트
	m_bState = bState;
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}



	D3DLIGHT9				tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));


	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 0.8f, 0.6f, 1.f);
	tLightInfo.Position = _vec3(0.f, 5.f, 0.f);
	tLightInfo.Range = 7.f;
	m_pLight[0] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 16);

	tLightInfo.Diffuse = D3DXCOLOR(0.25f , 0.55f, 0.9f , 1.f);
	tLightInfo.Position = _vec3(0.f, 5.f, 0.f);
	tLightInfo.Range = 7.f;
	m_pLight[1] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 17);




	return S_OK;
}

_int CEscape_Shuttle::Update_Object(const _float & fTimeDelta)
{
	if (m_bStart)
	{
		_vec3 vTriggerPos;
		vTriggerPos = m_pTransformCom[MESHID::Shuttle]->Get_Position();

		m_pTrigger = CTrigger::CreateSphereTirgger(m_pScene, this, _vec3(vTriggerPos.x, vTriggerPos.y + 2.f, vTriggerPos.z), 2.f, TRIGGER_CHESS_SHUTTLE, FilterGroup::eDefaultTrigger);
		m_bStart = false;
	}
	if (m_iShuttleStack == 2)
	{
		m_bMove[SHUTTLE::ShuttleDoorClose] = true;
	}
	DoorOpen(fTimeDelta);
	DoorClose(fTimeDelta);
	CutScene(fTimeDelta);
	int iEvent = ShuttleUpMove(fTimeDelta);

	m_pLight[0]->Set_LightPos(m_pTransformCom[MESHID::Shuttle]->Get_Position() + _vec3(3.f, -1.f, 0.f));
	m_pLight[1]->Set_LightPos(m_pTransformCom[MESHID::Shuttle]->Get_Position() + _vec3(3.f, 5.f, 0.f));


	if (iEvent == SCENE_CHANGE)
		return iEvent;

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return OBJ_NOEVENT;
}

_int CEscape_Shuttle::LateUpdate_Object(const _float & fTimeDelta)
{
	return _int();
}

void CEscape_Shuttle::Render_Object(const _int & iIndex)
{

	for (_uint i = 0; i < MESHID::Shuttle_end; ++i)
	{
		LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);
		pEffect->AddRef();

		FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex, i), );

		_uint	iPassMax = 0;

		pEffect->Begin(&iPassMax, 8);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
											//pEffect->BeginPass(0);
		m_pMeshCom[i]->Render_Meshes(pEffect);

		pEffect->End();

		Safe_Release(pEffect);
	}
}

Engine::_mat CEscape_Shuttle::Get_EventPos()
{
	return *m_pTransformCom[Shuttle]->Get_WorldMatrix();
}

HRESULT CEscape_Shuttle::Add_Component(void * pArg)
{

	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	pComponent = m_pMeshCom[MESHID::Shuttle] = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"ChessEscape_Shuttle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MeshShuttler", pComponent);

	pComponent = m_pMeshCom[MESHID::ShuttleDoor] = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"ChessEscape_ShuttleDoor"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_MeshShuttleDoorr", pComponent);

	// Transform
	pComponent = m_pTransformCom[Shuttle] = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_TransformShuttle", pComponent);

	pComponent = m_pTransformCom[ShuttleDoor] = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_TransformShuttleDoor", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_All"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);



	//for (_uint i = 0; i < MESHID::Shuttle_end; ++i)
	//{
	//	m_pTransformCom[i]->Set_Scale(tTemp.vScale);
	//	m_pTransformCom[i]->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	//	m_pTransformCom[i]->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	//	m_pTransformCom[i]->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	//	m_pTransformCom[i]->Set_Pos(tTemp.vPos);
	//}

	m_pTransformCom[MESHID::Shuttle]->Set_Scale(tTemp.vScale);
	m_pTransformCom[MESHID::Shuttle]->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	m_pTransformCom[MESHID::Shuttle]->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	m_pTransformCom[MESHID::Shuttle]->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformCom[MESHID::Shuttle]->Set_Pos(tTemp.vPos);
	if (!m_bState)
	{
		_vec3 vPos = tTemp.vPos;
		m_pTransformCom[MESHID::ShuttleDoor]->Set_Scale(tTemp.vScale);
		m_pTransformCom[MESHID::ShuttleDoor]->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
		m_pTransformCom[MESHID::ShuttleDoor]->Rotation(ROT_Y, D3DXToRadian(0.f));
		m_pTransformCom[MESHID::ShuttleDoor]->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
		m_pTransformCom[MESHID::ShuttleDoor]->Set_Pos(tTemp.vPos.x + 1.7f, tTemp.vPos.y + 1.5f, tTemp.vPos.z - 1.5f);
	}
	else if (m_bState)
	{
		_vec3 vPos = tTemp.vPos;
		m_pTransformCom[MESHID::ShuttleDoor]->Set_Scale(tTemp.vScale);
		m_pTransformCom[MESHID::ShuttleDoor]->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
		m_pTransformCom[MESHID::ShuttleDoor]->Rotation(ROT_Y, D3DXToRadian(-135.f));
		m_pTransformCom[MESHID::ShuttleDoor]->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
		m_pTransformCom[MESHID::ShuttleDoor]->Set_Pos(tTemp.vPos.x +2.25f , tTemp.vPos.y + 1.5f, tTemp.vPos.z + 0.05f);
	}

	for (_uint i = 0; i < MESHID::ShuttleDoor; ++i)
	{
		auto* pPhysics = Engine::Get_Physics();
		_vec3 vPos = m_pTransformCom[i]->Get_Position();

		PxTransform tTransform(vPos.x, vPos.y, vPos.z, PxQuat(m_pTransformCom[i]->Get_Angle().y, { 0.f,1.f,0.f }));
		PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

		PxTriangleMeshGeometry pMeshGeo;
		pMeshGeo.triangleMesh = m_pMeshCom[i]->Get_PxMesh();//m_pPhysMesh->Get_PxMesh();
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


	}

	return S_OK;

}

HRESULT CEscape_Shuttle::SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex, const _int& iMeshNum)
{
	_matrix			matWorld, matView, matProj;


	m_pTransformCom[iMeshNum]->Get_WorldMatrix(&matWorld);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);


	pEffect->SetVector("g_vColor", &_vec4(255.f, 255.f, 255.f, 255.f));

	return S_OK;
}

void CEscape_Shuttle::Move(const _float & fTimeDelta)
{
	if (m_iShuttleStack == 2)
	{
		m_bMove[SHUTTLE::ShuttleDoorClose] = true;
	}
	DoorOpen(fTimeDelta);
	DoorClose(fTimeDelta);
	ShuttleUpMove(fTimeDelta);
}

void CEscape_Shuttle::DoorOpen(const _float & fTimeDelta)
{
	if (true == m_bState)
	{


		m_fOpenDoorTime += fTimeDelta;
		if (m_fOpenDoorTime > 1.f)
		{
			PlaySoundOnce(L"SpaceShip_Open.wav", CSoundMgr::CHANNELID::Space_Ship_Open);
		}

		if (m_fOpenDoorTime > 2.f)
		{

			m_fCurrentAngle += D3DXToDegree(m_fOpenSpeed);
			if (m_fAngle >= m_fCurrentAngle)
			{
				m_pTransformCom[MESHID::ShuttleDoor]->Rotation(ROT_Y, m_fOpenSpeed);
			}
		}
	}
}

void CEscape_Shuttle::DoorClose(const _float & fTimeDelta)
{
	if(!m_bMove[SHUTTLE::ShuttleDoorClose])
		return;

	m_fCurrentAngle += D3DXToDegree(m_fOpenSpeed);
	if (m_fAngle >= m_fCurrentAngle)
	{
		m_pTransformCom[MESHID::ShuttleDoor]->Rotation(ROT_Y, -m_fOpenSpeed);
	}
	else
	{
		//_vec3 vPos, vCameraPos;
		//vPos = m_pTransformCom[MESHID::Shuttle]->Get_Position();
		//CMainCamera*		pMainCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
		//vCameraPos = pMainCamera->Get_CameraPos();
		//pMainCamera->Set_Mode(CAMERA_FIX);
		//pMainCamera->Set_Fix(vPos, _vec3(vCameraPos.x + 2.f, vCameraPos.y, vCameraPos.z));


		m_bMove[SHUTTLE::ShuttleDoorClose] = false;
		m_bMove[SHUTTLE::ShuttleMove] = true;
	}
}

_int CEscape_Shuttle::ShuttleUpMove(const _float & fTimeDelta)
{
	if (!m_bMove[SHUTTLE::ShuttleMove])
		return 0;

	m_bCutScene = false;





	CCodyWizard*		pCody = (CCodyWizard*)Engine::Get_GameObject(L"GameObject", L"Cody");
	CMayCastle*			pMay = (CMayCastle*)Engine::Get_GameObject(L"GameObject", L"May");

	pCody->Set_CutScene(true);
	pCody->Set_Stencil(false);

	pMay->Set_CutScene(true);
	pMay->Set_Stencil(false);

	m_fShuttleTime += fTimeDelta;

	_vec3 vPos;

	CMainCamera*		pMainCamera = (CMainCamera*) Engine::Get_GameObject(L"GameObject", L"MainCamera");
	vPos = m_pTransformCom[MESHID::Shuttle]->Get_Position();
	pMainCamera->Set_Mode(CAMERA_TRACING, this);
	pMainCamera->Set_Shake();

	if (m_fShuttleTime > 2.f)
	{
		if (!m_bSound)
		{	
			StopAll();

			PlaySoundW(L"Rocket_MoveStart.wav", CSoundMgr::CHANNELID::Space_Missile_Start, 0.2f);
			m_bSound = true;
		}
		if (Engine::Is_Play_End(CSoundMgr::CHANNELID::Space_Missile_Start))
		{
			PlaySoundW(L"Rocket_Moving.wav", CSoundMgr::CHANNELID::Space_Missile_Move);
		}
		if (m_fSpeed > 5.f)
		{
			m_fSpeed = 5.f;
		}
		for (_uint i = 0; i < MESHID::Shuttle_end; ++i)
		{
			m_pTransformCom[i]->Move_Pos(&_vec3(0.f, 1.f, 0.f), m_fSpeed, fTimeDelta);
		}
		m_fSpeed += 0.03f;
	}


	if (vPos.y > 40.f)
	{
		CHANGE_SCENE_POS(m_pGraphicDev, SCENE_SF, _vec3(0.f, 3.f, -3.5f), OBJ_NOEVENT);
		return SCENE_CHANGE;
	}
	return 0;
}

void CEscape_Shuttle::CutScene(const _float & fTimeDelta)
{
	if (!m_bCutScene)
		return;

	_vec3 vCameraPos;
	CMainCamera*		pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
	
	vCameraPos = m_pTransformCom[Shuttle]->Get_Position();
	
	pCamera->Set_Mode(CAMERA_FIX);
	pCamera->Set_Fix(_vec3(vCameraPos.x, vCameraPos.y + 2.f, vCameraPos.z), _vec3(-5.f, 3.f, 0.f));

}

CEscape_Shuttle * CEscape_Shuttle::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, _bool bState)
{
	CEscape_Shuttle*	pInstance = new CEscape_Shuttle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg, bState)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEscape_Shuttle::Free(void)
{
	Engine::CGameObject::Free();
}
