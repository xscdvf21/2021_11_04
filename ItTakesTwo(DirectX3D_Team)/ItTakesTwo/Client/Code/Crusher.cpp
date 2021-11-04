#include "stdafx.h"
#include "Crusher.h"
#include "MainCamera.h"
#include "ChessDoorSwitch.h"
#include "CrusherBreakWall.h"
#include "Export_Function.h"
#include "CrusherBridge.h"
#include "Cody_Wizard.h"
#include "May_Castle.h"

CCrusher::CCrusher(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_bMove(false)
	, m_bDeadAni(false)
	, m_iAniIndex(0)
	, m_bMoveStart(false)
	, m_bMoveReady(false)
	, m_iHitPointCount(0)
	, m_bDeadMove(false)
	, m_fCurTime(0.f)
{
}

CCrusher::~CCrusher()
{
}

HRESULT CCrusher::Ready_Object(void * pArg)
{
	FAILED_CHECK_RETURN(Add_Component(pArg), E_FAIL);

	OBJINFO tTemp;
	ZeroMemory(&tTemp, sizeof(OBJINFO));

	if (nullptr != pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}

	if (!m_bSound[BGM])
	{
		StopAll();
		PlayBGM(L"CrusherBGM.wav");
		m_bSound[BGM] = true;
	}

	m_pMeshCom->Set_Animationset(animID::ToyCrusher_MH);


	D3DLIGHT9				tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));


	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);


	tLightInfo.Diffuse = D3DXCOLOR(1.5f * 1.5f, 0.5f * 1.5f, 0.3f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(3.95f, 7.f, -17.99f) * 0.7f;
	tLightInfo.Range = 4.f;
	m_pLight[0] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 25);

	tLightInfo.Diffuse = D3DXCOLOR(1.5f * 1.5f, 0.5f * 1.5f, 0.3f * 1.5f, 1.f);
	tLightInfo.Position = _vec3(3.95f, 7.f, -17.99f) * 0.7f;
	tLightInfo.Range = 4.f;
	m_pLight[1] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 26);

	tLightInfo.Diffuse = D3DXCOLOR(1.f , 0.7f, 0.2f, 1.f);
	tLightInfo.Position = _vec3(3.95f, 7.f, -17.99f) * 0.7f;
	tLightInfo.Range = 5.f;
	m_pLight[2] = Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 27);


	return S_OK;
}

_int CCrusher::Update_Object(const _float & fTimeDelta)
{
	m_iAniIndex = m_pMeshCom->Get_AniIndex();
	if (m_bDead)
	{
		CCrusherBreakWall* pWall = (CCrusherBreakWall*)Engine::Get_GameObject(L"GameObject", L"CrusherBreakWall5");
		if (pWall)
			pWall->Set_Dead();

		pWall = (CCrusherBreakWall*)Engine::Get_GameObject(L"GameObject", L"CrusherBreakWall6");
		if (pWall)
			pWall->Set_Dead();

		StopAll();
		PlayBGM(L"ChessField_BGM.wav");



		CCodyWizard*		pCody = (CCodyWizard*)Engine::Get_GameObject(L"GameObject", L"Cody");
		CMayCastle*			pMay = (CMayCastle*)Engine::Get_GameObject(L"GameObject", L"May");

		pCody->Set_CutScene(false);
		pCody->Set_Stencil(true);

		pMay->Set_CutScene(false);
		pMay->Set_Stencil(true);

		return OBJ_DEAD;
	}
	if (m_iHitPointCount == 4)
	{
		Dead(fTimeDelta);
	}

	if (m_bMove && !m_bDeadAni)
	{
		Move(fTimeDelta);
	}


	m_pMeshCom->Play_Animationset(fTimeDelta);

	Engine::CGameObject::Update_Object(fTimeDelta);
	Engine::Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	_vec3 vPos;

	vPos = m_pTransformCom->Get_Position();

	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	pxTransform.p.x = vPos.x;
	pxTransform.p.y = vPos.y;
	pxTransform.p.z = vPos.z;
	m_pRigidActor->setGlobalPose(pxTransform);

	m_pLight[0]->Set_LightPos(m_pTransformCom->Get_Position() + _vec3(2.5f, 2.f, -1.f));
	m_pLight[1]->Set_LightPos(m_pTransformCom->Get_Position() + _vec3(1.f, 2.f, -2.5f));
	m_pLight[2]->Set_LightPos(m_pTransformCom->Get_Position() + _vec3(2.f, 3.f, -2.f));

	return OBJ_NOEVENT;
}

_int CCrusher::LateUpdate_Object(const _float & fTimeDelta)
{
	PxTransform pxTransform = m_pRigidActor->getGlobalPose();
	m_pTransformCom->Set_Pos(pxTransform);

	return _int();
}

void CCrusher::Render_Object(const _int & iIndex)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect, iIndex), );

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
										//pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes_VTF(pEffect, 0);

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CCrusher::Add_Component(void * pArg)
{
	Engine::CComponent*		pComponent = nullptr;

	OBJINFO tTemp;
	if (pArg)
	{
		memcpy(&tTemp, pArg, sizeof(OBJINFO));
	}


	pComponent = m_pMeshCom = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(tTemp.eSceneID, L"ToyCrusher"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);
	// Transform
	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = static_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, m_fColDis, _vec4(50.f, 244.f, 234.f, 0.f));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_VTF"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);


	m_pTransformCom->Set_Scale(tTemp.vScale);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(tTemp.vAngle.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(tTemp.vAngle.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(tTemp.vAngle.z));
	m_pTransformCom->Set_Pos(tTemp.vPos);


	auto* pPhysics = Engine::Get_Physics();
	_vec3 vPos = m_pTransformCom->Get_Position();
	PxTransform tTransform(vPos.x, vPos.y, vPos.z);

	PxMaterial* pMaterial = pPhysics->createMaterial(0.5f, 0.5f, 0.0f);

	PxShape* pShape = pPhysics->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *pMaterial, true);
	//밑 두개중 하나는 false 둘다 true안?
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	m_pRigidActor = PxCreateDynamic(*pPhysics, tTransform, *pShape, 1);

	auto* pBody = static_cast<PxRigidDynamic*>(m_pRigidActor);

	// 트랜스폼은 가지고있지만 움직이지 않는애들
	pBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	// 그래비티를 끈다.

	pBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);

	// 질량설정
	pBody->setMass(5);
	pBody->setName("Crusher");
	pBody->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
	//이동 제동력
	pBody->setLinearDamping(0.05f);


	return S_OK;
}

HRESULT CCrusher::SetUp_ConstantTable(LPD3DXEFFECT & pEffect, const _int & iIndex)
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
	pEffect->CommitChanges();

	return S_OK;
}

void CCrusher::Move(const _float & fTimeDelta)
{

	_vec3 vLook;
	if (!m_bMoveStart)
	{
		_vec3 vCameraPos;
		vCameraPos = m_pTransformCom->Get_Position();


		CMainCamera*	pCamera = static_cast<CMainCamera*>(Engine::Get_GameObject(L"GameObject", L"MainCamera"));
		pCamera->Set_Mode(CAMERA_FIX);
		pCamera->Set_Fix(m_pTransformCom->Get_Position(), _vec3(vCameraPos.x + 8.f, vCameraPos.y + 10.f, vCameraPos.z - 10.f));


		CCodyWizard*		pCody = (CCodyWizard*)Engine::Get_GameObject(L"GameObject", L"Cody");
		CMayCastle*			pMay = (CMayCastle*)Engine::Get_GameObject(L"GameObject", L"May");

		pCody->Set_CutScene(true);
		pCody->Set_Stencil(false);

		pMay->Set_CutScene(true);
		pMay->Set_Stencil(false);


		CCrusherBreakWall* pWall = (CCrusherBreakWall*)Engine::Get_GameObject(L"GameObject", L"CrusherBreakWall1");
		if(pWall)
			pWall->Set_Dead();

		pWall = (CCrusherBreakWall*)Engine::Get_GameObject(L"GameObject", L"CrusherBreakWall2");
		if (pWall)
			pWall->Set_Dead();

		m_pMeshCom->Set_Animationset(animID::PlayRoom_Castle_Dungeon_CrusherIntro_ToyCrusher);
	}
	if (m_iAniIndex == animID::PlayRoom_Castle_Dungeon_CrusherIntro_ToyCrusher && m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		_vec3 vEyeDir;
		_vec3 vDir = { -3.0f, 15.0f, -10.0f };
		vEyeDir = -vDir;
		D3DXVec3Normalize(&vEyeDir, &vEyeDir);

		CMainCamera*	pCamera = static_cast<CMainCamera*>(Engine::Get_GameObject(L"GameObject", L"MainCamera"));
		pCamera->Set_Mode(CAMERA_DEFAULT);
		pCamera->Set_EyeDir(vEyeDir);


		CCodyWizard*		pCody = (CCodyWizard*)Engine::Get_GameObject(L"GameObject", L"Cody");
		CMayCastle*			pMay = (CMayCastle*)Engine::Get_GameObject(L"GameObject", L"May");

		pCody->Set_CutScene(false);
		pCody->Set_Stencil(true);

		pMay->Set_CutScene(false);
		pMay->Set_Stencil(true);


		CCrusherBreakWall* pWall = (CCrusherBreakWall*)Engine::Get_GameObject(L"GameObject", L"CrusherBreakWall3");
		if (pWall)
			pWall->Set_Hit(true);

		pWall = (CCrusherBreakWall*)Engine::Get_GameObject(L"GameObject", L"CrusherBreakWall4");
		if (pWall)
			pWall->Set_Hit(true);

		m_bMoveStart = true;
	}
	else if (m_iAniIndex == animID::PlayRoom_Castle_Dungeon_CrusherIntro_ToyCrusher && m_pMeshCom->Is_AnimationSetEnd(2.f))
	{
		if (!m_bSound[StartCry])
		{
			StopSound(CSoundMgr::CHANNELID::Crusher);
			PlaySoundW(L"CF_Crusher_Start.ogg", CSoundMgr::CHANNELID::Space_Boss_Mirror_Move, 0.3f);

			m_bSound[StartCry] = true;
		}
	}

	if (m_bMoveStart)
	{
		//_vec3 vCameraPos = m_pTransformCom->Get_Position();
		//_vec3 vLook;
		//m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		//D3DXVec3Normalize(&vLook, &vLook);
		//vLook = vLook * 5.f;

		//auto* pCamera = (CMainCamera*)Engine::Get_GameObject(L"GameObject", L"MainCamera");
		//pCamera->Set_Mode(CAMERA_STALKING);
		//pCamera->Set_Fix(vCameraPos, _vec3(vCameraPos.x + 15.f, vCameraPos.y + 15.f, vCameraPos.z));

		m_pMeshCom->Set_Animationset(animID::ToyCrusher_Smash);

		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);
		m_pTransformCom->Move_Pos(&vLook, 2.f, fTimeDelta);

		if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{
			//CMainCamera*	pCamera = static_cast<CMainCamera*>(Engine::Get_GameObject(L"GameObject", L"MainCamera"));
			//pCamera->Set_Shake();
		}
	}
}

void CCrusher::Dead(const _float & fTimeDelta)
{

	if (!m_bDeadMove)
	{
		m_vMoveStartPos = m_pTransformCom->Get_Position();
		m_vMoveEndPos = { 68.f, 0.f, -141.8f };
		m_bDeadMove = true;
	}


	if(!m_bDeadAni)
		DeadMove(fTimeDelta, 2.f, m_vMoveStartPos, m_vMoveEndPos);
	else if (m_bDeadAni)
	{

		if (!m_bSound[DeadSound])
		{
			StopSound(CSoundMgr::CHANNELID::Crusher);
			PlaySoundW(L"CrusherDead.wav", CSoundMgr::CHANNELID::Crusher, 0.8f);

			m_bSound[DeadSound] = true;
		}

		m_pMeshCom->Set_Animationset(animID::PlayRoom_Castle_Dungeon_BridgeCollapse_ToyCrusher);

		CCrusherBridge*		pBridge = (CCrusherBridge*)Engine::Get_GameObject(L"GameObject", L"CrusherBridge");
		pBridge->Set_Move();
		if (m_iAniIndex == animID::PlayRoom_Castle_Dungeon_BridgeCollapse_ToyCrusher&& m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{
			_vec3 vEyeDir;
			_vec3 vDir = { -8.0f, 15.0f, -5.0f };
			vEyeDir = -vDir;
			D3DXVec3Normalize(&vEyeDir, &vEyeDir);

			CMainCamera*	pCamera = static_cast<CMainCamera*>(Engine::Get_GameObject(L"GameObject", L"MainCamera"));
			pCamera->Set_Mode(CAMERA_DEFAULT);
			pCamera->Set_EyeDir(vEyeDir);



			m_bDead = true;
		}
		else if (m_iAniIndex == animID::PlayRoom_Castle_Dungeon_BridgeCollapse_ToyCrusher&& m_pMeshCom->Is_AnimationSetEnd(4.f))
		{
			//if (!m_bCameraFinger)
			//{
				const D3DXFRAME_DERIVED* pRightBone = m_pMeshCom->Get_FrameByName("RightFingers1");
				_matrix matRightBoneMatrix = pRightBone->CombinedTranformationMatrix;
				_matrix matRightFingerWorld = matRightBoneMatrix * *m_pTransformCom->Get_WorldMatrix();

				_vec3 vCameraPos = { matRightFingerWorld._41, matRightFingerWorld._42, matRightFingerWorld._43 };

				CMainCamera*	pCamera = static_cast<CMainCamera*>(Engine::Get_GameObject(L"GameObject", L"MainCamera"));
				pCamera->Set_Mode(CAMERA_FIX);
				pCamera->Set_Fix(vCameraPos, _vec3(vCameraPos.x + 2.f, vCameraPos.y/* + 2.f*/, vCameraPos.z + 2.f), true);


				CCodyWizard*		pCody = (CCodyWizard*)Engine::Get_GameObject(L"GameObject", L"Cody");
				CMayCastle*			pMay = (CMayCastle*)Engine::Get_GameObject(L"GameObject", L"May");

				pCody->Set_CutScene(true);
				pCody->Set_Stencil(false);

				pMay->Set_CutScene(true);
				pMay->Set_Stencil(false);

		}


	}
}

void CCrusher::DeadMove(const _float & fTimeDelta, const _float & fMoveTime, _vec3 vStartPos, _vec3 vEndPos)
{
	m_fMoveTime = fMoveTime;
	m_vStartPos = vStartPos;
	m_vEndPos = vEndPos;


	m_fCurTime += fTimeDelta;
	m_pTransformCom->Set_Pos
	(
		(((m_vStartPos.x + ((((m_vEndPos.x) - (m_vStartPos.x)) / m_fMoveTime)*m_fCurTime)))),
		(((m_vStartPos.y + ((((m_vEndPos.y) - (m_vStartPos.y)) / m_fMoveTime)*m_fCurTime)))),
		(((m_vStartPos.z + ((((m_vEndPos.z) - (m_vStartPos.z)) / m_fMoveTime)*m_fCurTime))))
	);


	if (m_fMoveTime < m_fCurTime)
	{
		m_fCurTime = 0.f;
		m_bDeadAni = true;
	}
}

CCrusher * CCrusher::Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg)
{
	CCrusher*	pInstance = new CCrusher(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCrusher::Free(void)
{
	Engine::CGameObject::Free();
}
