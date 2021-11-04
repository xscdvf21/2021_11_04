#include "stdafx.h"
#include "Boss.h"
#include "Bosstail.h"
#include "SoundMgr.h"
#include "Player.h"
#include "Export_Fucntion.h"
#include "Blood.h"
#include "FlyWind.h"
#include "DynamicCamera.h"


CBoss::CBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CBoss::~CBoss()
{
}

HRESULT CBoss::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.02f, 0.02f, 0.02f));
	m_pTransformCom->Set_Pos(_vec3(45.f, 6.f, 80.f));


	ZeroMemory(&m_tINFOBOSS, sizeof(INFOPLAYER));

	_vec3 Look;
	_vec3 Right;
	_vec3 Up;

	m_pMeshCom->Set_Animationset(50);

	for (_uint i = 0; i < SPHERE_END; ++i)
	{
		m_pParentBoneMatrix[i] = nullptr;
	}

	for (_uint i = 0; i < SPHERE_END; ++i)
	{
		D3DXMatrixIdentity(&m_pFinalMatrix[i]);
	}
	

	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &Look);
	m_pTransformCom->Get_Info(INFO::INFO_RIGHT, &Right);
	m_pTransformCom->Get_Info(INFO::INFO_UP, &Up);


	m_tINFOBOSS.iMaxHP = 1000;
	m_tINFOBOSS.iHP = 1000;
	m_tINFOBOSS.iAttack = 6;
	
	



	return S_OK;
}

_int CBoss::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	//10번 애니 1번애니 죽으면.
	if (m_tINFOBOSS.iHP <= 0)
	{
		m_bDead = true;
	}

	if (!m_bDead) 
	{
		m_fDelTime += fTimeDelta;
		if (!m_bPlayerCheck)
			m_bPattern = PlayerCheck(); //플레이어 체크, 시야 양쪽 70도에, 거리 10으로 줬음.

		//플레이어 체크 시 아닐 시
		if (!m_bPattern)
		{
			IdleBoss_Move(fTimeDelta);

		}
		else if (m_bPattern)
		{

			Boss_Move(fTimeDelta);
		}

		m_fCollTime += fTimeDelta;

		if (m_bAttack)
		{
			CManagement*		pManagement = CManagement::GetInstance();
			NULL_CHECK_RETURN(pManagement, );

			CPlayer*			pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObject(L"GameLogic", L"Player"));


			if (nullptr == pPlayer)
				return;

			_bool		bPlayerDash = pPlayer->Get_Dash();

			m_bColl = Collision_ToObject(L"GameLogic", L"Player", fTimeDelta);

		}


		if (true == m_bColl && 4.f < m_fCollTime)
		{
			CManagement*		pManagement = CManagement::GetInstance();
			NULL_CHECK_RETURN(pManagement, );

			CPlayer*			pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObject(L"GameLogic", L"Player"));

			if (nullptr == pPlayer)
				return;

			_bool		bPlayerDash = pPlayer->Get_Dash();

			if (!bPlayerDash)
			{
				INFOPLAYER Temp = pPlayer->Get_INFOPLAYER();
				Temp.iHP -= m_tINFOBOSS.iAttack;

				pPlayer->Set_INFOPLAYER(Temp);
				OutputDebugStringW(L"보스가 플레이어 공격했음 \n");

				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::MONSTER_EFFECT);
				CSoundMgr::Get_Instance()->PlaySoundW(L"Hit_Player_Blood.wav", CSoundMgr::CHANNELID::MONSTER_EFFECT);

				Engine::CLayer*			pLayer = nullptr;

				pLayer = pManagement->Get_Layer(L"GameLogic");
				NULL_CHECK_RETURN(pLayer, E_FAIL);

				// 오브젝트 추가
				Engine::CGameObject*		pGameObject = nullptr;

				_vec3 vPos = { m_pFinalMatrix[SPHERE_HEAD]._41, m_pFinalMatrix[SPHERE_HEAD]._42, m_pFinalMatrix[SPHERE_HEAD]._43 };

				pGameObject = CBlood::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Blood", pGameObject), E_FAIL);


				CDynamicCamera*		pCamera = dynamic_cast<CDynamicCamera*>(pManagement->Get_GameObject(L"Environment", L"DynamicCamera"));

				if (nullptr == pCamera)
					return;

				pCamera->Set_Shake(true);

			}

			//충돌해서 피 까였으면 시간 초기화해서, 4초동안 공격중이여도 충돌판정을 안함.
			
			m_fCollTime = 0.f;
			m_bColl = false;
		}

		SetUp_Sphere(); //충돌체 생성.
		SetUp_AttackSphere(); //공격 충돌체 생성.

	}

	if (m_bDead)
	{
		_uint iIndex = m_pMeshCom->Get_AniIndex();
		if (iIndex != 10 && m_iAniNum != 10)
		{

			m_pMeshCom->Set_Animationset(10);
			m_iAniNum = 10;

		}
		if (m_iAniNum == 10 && true == m_pMeshCom->Is_AnimationSetEnd())
		{
			m_pMeshCom->Set_Animationset(1);
			m_iAniNum = 10;

			if (false == m_bBossDieBGM)
			{
				CSoundMgr::Get_Instance()->StopAll();
				CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Win.mp3");
				m_bVictory = true;
				m_bBossDieBGM = true;
			}
		}

	

		_vec3 vDown = { 0.f, -1.f, 0.f };
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		if (vPos.y >= 3.f)
		{
			m_pTransformCom->Move_Pos(&vDown, 1.f, fTimeDelta);

			if (false == m_bBossDieSound)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::MONSTER);
				CSoundMgr::Get_Instance()->PlaySoundW(L"Liea_Dying.wav", CSoundMgr::CHANNELID::MONSTER);
				m_bBossDieSound = true;
			}
		}

	}

	_uint x = m_pMeshCom->Get_AniIndex();
	if (x == 61)
	{
		_float TimeTemp = fTimeDelta;
		TimeTemp = TimeTemp * 2;
		m_pMeshCom->Play_AnimationsetBoss(TimeTemp);
	}
	else if (x == 10)
	{
		_float TimeTemp = fTimeDelta;
		TimeTemp = TimeTemp * 2.5f;
		m_pMeshCom->Play_AnimationsetBoss(TimeTemp);
	}
	else 
		m_pMeshCom->Play_AnimationsetBoss(fTimeDelta);
	

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_MAP, this);


	return 0;
}

void CBoss::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pMeshCom->Render_Meshes();


	if (exColisionSee)
	{
		for (_uint i = 0; i < SPHERE_END; ++i)
		{
			m_pSphere->Render_Buffer(&m_pFinalMatrix[i], _vec3(1.f, 1.f, 1.f), _vec3(0.f, 0.f, 0.f), 0);

		}

		if (true == m_bAttack)
		{
			for (_uint i = 0; i < SPHERE_END; ++i)
			{
				m_pAttackSphere->Render_Buffer(&m_pFinalMatrix[i], _vec3(1.f, 1.f, 1.f), _vec3(0.f, 0.f, 0.f), 1);

			}
		}
	}


	
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);




	//_matrix		matWorld;
	//m_pTransformCom->Get_WorldMatrix(&matWorld);

	//m_pColliderCom->Render_Collider(Engine::COLTYPE(0), &matWorld);

}

HRESULT CBoss::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	//Sphere
	pComponent = m_pSphere = dynamic_cast<Engine::CSphere*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Proto_Buffer_Sphere"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Sphere", pComponent);


	pComponent = m_pAttackSphere = dynamic_cast<Engine::CSphere*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Proto_Buffer_Sphere"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_AttackSphere", pComponent);

	// CDynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Boss"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// NaviMesh
	//pComponent = m_pNaviMeshCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Navi"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Navi", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(Engine::Clone_Prototype(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	//// Collider
	//pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev,
	//	m_pMeshCom->Get_VtxPos(),
	//	m_pMeshCom->Get_VtxCnt(),
	//	m_pMeshCom->Get_Stride());
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	

	return S_OK;
}

_bool CBoss::PlayerCheck()
{
	CManagement*		pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, );

	CTransform*			pPlayerTransform = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));

	_vec3 vPlayerPos;
	pPlayerTransform->Get_Info(INFO::INFO_POS, &vPlayerPos);

	_vec3 vDis;
	vDis = vPlayerPos - m_pTransformCom->m_vInfo[INFO::INFO_POS];
	_float fDis = D3DXVec3Length(&vDis); //플레이어와 거리계산할 단순 float 

	_vec3 vPlayerDis = vDis; //플레이어와 보스의 방향벡터. 
	D3DXVec3Normalize(&vPlayerDis, &vPlayerDis); //몬스터의 시야각을 위한 플레이어와의 방향벡터.

	_vec3 vBossLook; //보스의 룩 방향벡터.
	m_pTransformCom->Get_Info(INFO_LOOK, &vBossLook);
	D3DXVec3Normalize(&vBossLook, &vBossLook);

	
	_float fAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vPlayerDis, &vBossLook)));



	if (fAngle <= 80 && fDis <= 30.f)
	{ 
		m_bPlayerCheck = true;
		return true;
	}


	return false;
}

HRESULT CBoss::Boss_Move(const _float& fTimeDelta)
{
	CManagement*		pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, );

	CTransform*			pPlayerTransform = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));

	_vec3 vPlayerPos;
	pPlayerTransform->Get_Info(INFO::INFO_POS, &vPlayerPos);

	//srand(time(NULL));


	//소리 지르기 
	if (!m_bBossScream)
	{
		m_fScream += fTimeDelta;
		m_fScreamShake += fTimeDelta;
		//카메라쉐이킹 시간 새로해야함.

		if (m_fScreamShake > 2.f)
		{
			CDynamicCamera*		pCamera = dynamic_cast<CDynamicCamera*>(pManagement->Get_GameObject(L"Environment", L"DynamicCamera"));

			if (nullptr == pCamera)
				return;

			pCamera->Set_Shake(true);
		}


		if (m_fScream > 2.f)
		{
			
			if (!m_bBossScreamSound)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER);
				CSoundMgr::Get_Instance()->PlaySoundW(L"Liea_Dying.wav", CSoundMgr::MONSTER);
				m_bBossScreamSound = true;
			}
			CPlayer*		pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObject(L"GameLogic", L"Player"));
			pPlayer->Set_FallDown(true);

			m_fScream = 0.f;
		}
		CManagement*		pManagement = CManagement::GetInstance();
		NULL_CHECK_RETURN(pManagement, );

		//CGameObject*	pPlayer = dynamic_cast<CGameObject*>(pManagement->Get_GameObject(L"GameLogic", L"Player"));



		CTransform*			pPlayerTransform = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));
		_vec3 vPlayerPos;

		if (nullptr == pPlayerTransform)
			return;

		pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
		//도착지에서 출발지를 빼야 우리가원하는게나옴
		_vec3 vDir;
		vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];
		vDir.y = 0.f;
		D3DXVec3Normalize(&vDir, &vDir);

		_vec3 vLook;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		D3DXVec3Normalize(&vLook, &vLook);
		vAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vLook, &vDir))); //각도 구했고.

		_vec3 vRight;
		_vec3 vLeft;

		m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

		D3DXVec3Normalize(&vRight, &vRight);
		vLeft = vRight * -1.f;
		/*D3DXVec3Normalize(&vLeft, &vLeft);*/


		_float fRightAngle = D3DXVec3Dot(&vDir, &vRight);
		_float fLeftAngle = D3DXVec3Dot(&vDir, &vLeft);


		if (vAngle >= D3DXToDegree(fTurnSpeed))
		{
			if (fRightAngle < 0 && fLeftAngle > 0)
			{
				m_pTransformCom->Rotation(ROT_Y, -fTurnSpeed);
			}
			else if (fRightAngle > 0 && fLeftAngle < 0)
				m_pTransformCom->Rotation(ROT_Y, fTurnSpeed);
		}



		m_pMeshCom->Set_Animationset(61);
		if (true == m_pMeshCom->Is_AnimationSetEnd()) //소리 지르는 애니메이션이 끝나면 다시는 소리안지르게;
		{
			m_iAniNum = 61;
			m_bBossScream = true;
		}
		
	}



	//소리 지르고 나면 패턴 시작.
	if (m_bBossScream)
	{
		m_fPatternTime += fTimeDelta; //시간 누적.

		//체력이 70이하가되면 땅 패턴 시작.
		if (m_bDownPattern && m_tINFOBOSS.iHP <= 700)
		{
			m_bColl = false;
			m_bAttack = false;

			CManagement*		pManagement = CManagement::GetInstance();
			NULL_CHECK_RETURN(pManagement, );

			CTransform*			pPlayerTransform = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));

			if (nullptr == pPlayerTransform)
				return;

			CBosstail*			pBossTail = dynamic_cast<CBosstail*>(pManagement->Get_GameObject(L"GameLogic", L"Boss_tail"));
			pBossTail->Set_TailDestory(true);

			_vec3 vPlayerPos;
			_vec3 vPos;
			_vec3 vDown = { 0.f, -1.f, 0.f };
			pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
			m_pTransformCom->Get_Info(INFO_POS, &vPos);

			m_bFlyPattern = false;

			if (vPos.y >= 2.f)
			{
				m_pMeshCom->Set_Animationset(15);
				m_pTransformCom->Move_Pos(&vDown, 5.f, fTimeDelta);

				if (false == m_bBossFlyDown)
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::MONSTER_EFFECT);
					CSoundMgr::Get_Instance()->PlaySoundW(L"Liea_Hurt3.wav", CSoundMgr::CHANNELID::MONSTER_EFFECT);
					m_bBossFlyDown = true;
				}
			}

			else
			{

				if (false == m_bBossDownSound)
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::MONSTER_EFFECT);
					CSoundMgr::Get_Instance()->PlaySoundW(L"Liea_Hurt4.wav", CSoundMgr::CHANNELID::MONSTER_EFFECT);
					m_bBossDownSound = true;
				}

				m_pMeshCom->Set_Animationset(14);
				m_iAniNum = 14;
				if (m_iAniNum == 14 && m_pMeshCom->Is_AnimationSetEnd())
				{
					m_bGroundPattern = true;
					m_bDownPattern = false;
				}
			}
		
		}

		 //공중에서 떨어지고 일어나는 패턴 구현 해야할듯.  15 / 14번 애니메이션.

		//처음 시작시 공중 패턴.
		if (m_bFlyPattern)
		{
		
			//공중으로 올라감
			if (!m_bFlyStart)
			{
				CSoundMgr::Get_Instance()->PlaySoundW(L"Fly_05.wav", CSoundMgr::MONSTER); //공중 패턴일때는 계속 나야하므로, 계속틀어줌.
				if (m_iAniNum == 61)
				{
					m_pMeshCom->Set_Animationset(38);
					if (m_iAniNum == 61 && true == m_pMeshCom->Is_AnimationSetEnd())
					{
						m_iAniNum = 38;
					}
				}

				if (m_iAniNum == 38)
				{
					m_pMeshCom->Set_Animationset(36);
					_vec3 vUp = { 0.f, 1.f, 0.f };
					m_pTransformCom->Move_Pos(&vUp, 6.f, fTimeDelta);
					

					if (m_iAniNum == 38 && true == m_pMeshCom->Is_AnimationSetEnd())
					{
						m_iAniNum = 36;
						
					}
				}

				if (m_iAniNum == 36)
				{
					m_pMeshCom->Set_Animationset(44);

					Engine::CLayer*			pLayer = nullptr;

					pLayer = pManagement->Get_Layer(L"GameLogic");
					NULL_CHECK_RETURN(pLayer, E_FAIL);

					// 오브젝트 추가
					Engine::CGameObject*		pGameObject = nullptr;
					//날개짓 바람///////////////////////////////////////////////////////////////////////////
					_vec3 vPos = { m_pFinalMatrix[SPHERE_WL01]._41, 0.f , m_pFinalMatrix[SPHERE_WL01]._43 };
					_vec3 vPosWR = { m_pFinalMatrix[SPHERE_WR01]._41, 0.f , m_pFinalMatrix[SPHERE_WR01]._43 };

					_vec3 vRight;
					_vec3 vLeft;

					_vec3 vRightHalf;
					_vec3 vLeftHalf;
					m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
					D3DXVec3Normalize(&vRight, &vRight);

					vRight.y = 0.f;
					vLeft = vRight * -1.f;
					//vRight = vRight * 10.f;
					//vLeft = vLeft * 10.f;


					vRightHalf = vRight * 3.f;
					vLeftHalf = vLeft * 3.f;

					vRight = vRight * 10.f;
					vLeft = vLeft * 10.f;

					_vec3 vPosLeft;
					_vec3 vPosRight;
					_vec3 vPosLeftHalf;
					_vec3 vPosRightHalf;

					vPosLeftHalf = vPos + vLeftHalf;
					vPosRightHalf = vPosWR + vRightHalf;

					vPosRight = vPosWR + vRight;
					vPosLeft = vPos + vLeft;



					pGameObject = CFlyWind::Create(m_pGraphicDev, vPosRightHalf);
					NULL_CHECK_RETURN(pGameObject, E_FAIL);
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FlyWindRight", pGameObject), E_FAIL);

					pGameObject = CFlyWind::Create(m_pGraphicDev, vPosRight);
					NULL_CHECK_RETURN(pGameObject, E_FAIL);
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FlyWindRight", pGameObject), E_FAIL);



					pGameObject = CFlyWind::Create(m_pGraphicDev, vPosLeftHalf);
					NULL_CHECK_RETURN(pGameObject, E_FAIL);
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FlyWindLeft", pGameObject), E_FAIL);

					pGameObject = CFlyWind::Create(m_pGraphicDev, vPosLeft);
					NULL_CHECK_RETURN(pGameObject, E_FAIL);
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FlyWindLeft", pGameObject), E_FAIL);

					if (m_iAniNum == 36 && true == m_pMeshCom->Is_AnimationSetEnd())
					{
						m_iAniNum = 44;
						iSelect = rand() % 2 + 2;
						m_pTransformCom->Get_Info(INFO_POS, &m_vCurrentPos);
						m_bFlyStart = true;
					}
				}
			}

			if (m_bFlyStart) //공중으로 다날았으면 패턴 시작.
			{
				CManagement*		pManagement = CManagement::GetInstance();
				NULL_CHECK_RETURN(pManagement, );

				_uint AniIndex = m_pMeshCom->Get_AniIndex();
				if (AniIndex == 44)
				{
					CSoundMgr::Get_Instance()->PlaySoundW(L"Fly_05.wav", CSoundMgr::MONSTER); //공중 패턴일때는 계속 나야하므로, 계속틀어줌.

					Engine::CLayer*			pLayer = nullptr;

					pLayer = pManagement->Get_Layer(L"GameLogic");
					NULL_CHECK_RETURN(pLayer, E_FAIL);

					// 오브젝트 추가
					Engine::CGameObject*		pGameObject = nullptr;
					//날개짓 바람///////////////////////////////////////////////////////////////////////////
					_vec3 vPos = { m_pFinalMatrix[SPHERE_WL01]._41, 0.f , m_pFinalMatrix[SPHERE_WL01]._43 };
					_vec3 vPosWR = { m_pFinalMatrix[SPHERE_WR01]._41, 0.f , m_pFinalMatrix[SPHERE_WR01]._43 };

					_vec3 vRight;
					_vec3 vLeft;

					_vec3 vRightHalf;
					_vec3 vLeftHalf;
					m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
					D3DXVec3Normalize(&vRight, &vRight);

					vRight.y = 0.f;
					vLeft = vRight * -1.f;
					//vRight = vRight * 10.f;
					//vLeft = vLeft * 10.f;


					vRightHalf = vRight * 3.f;
					vLeftHalf = vLeft * 3.f;

					vRight = vRight * 10.f;
					vLeft = vLeft * 10.f;

					_vec3 vPosLeft;
					_vec3 vPosRight;
					_vec3 vPosLeftHalf;
					_vec3 vPosRightHalf;

					vPosLeftHalf = vPos + vLeftHalf;
					vPosRightHalf = vPosWR + vRightHalf;

					vPosRight = vPosWR + vRight;
					vPosLeft = vPos + vLeft;



					pGameObject = CFlyWind::Create(m_pGraphicDev, vPosRightHalf);
					NULL_CHECK_RETURN(pGameObject, E_FAIL);
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FlyWindRight", pGameObject), E_FAIL);

					pGameObject = CFlyWind::Create(m_pGraphicDev, vPosRight);
					NULL_CHECK_RETURN(pGameObject, E_FAIL);
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FlyWindRight", pGameObject), E_FAIL);



					pGameObject = CFlyWind::Create(m_pGraphicDev, vPosLeftHalf);
					NULL_CHECK_RETURN(pGameObject, E_FAIL);
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FlyWindLeft", pGameObject), E_FAIL);

					pGameObject = CFlyWind::Create(m_pGraphicDev, vPosLeft);
					NULL_CHECK_RETURN(pGameObject, E_FAIL);
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FlyWindLeft", pGameObject), E_FAIL);

					////////////////////////////////////////////////////////////////////////////////////
				}

		
				CTransform*			pPlayerTransform = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));

				if (nullptr == pPlayerTransform)
					return;

				_vec3 vPlayerPos;
				pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);

				_vec3 vPos;
				m_pTransformCom->Get_Info(INFO_POS, &vPos);

				_vec3 Temp = vPlayerPos - vPos;
				_float fDir = D3DXVec3Length(&Temp);



				if (fDir > 40.f)
				{
					iSelect = 1;
					iBulletMax = 0;
					m_bAttack = false;
				}

				//불뿜는 패턴.
				if (2 == iSelect && 3.f <= m_fPatternTime)
				{

					m_pMeshCom->Set_Animationset(43);
					m_fFireTime += fTimeDelta;

					CManagement*		pManagement = CManagement::GetInstance();
					NULL_CHECK_RETURN(pManagement, );

					CTransform*			pPlayerTransform = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));
					_vec3 vPlayerPos;

					if (nullptr == pPlayerTransform)
						return;

					pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
					//도착지에서 출발지를 빼야 우리가원하는게나옴
					_vec3 vDir;
					vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];
					vDir.y = 0.f;
					D3DXVec3Normalize(&vDir, &vDir);

					_vec3 vLook;
					m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
					D3DXVec3Normalize(&vLook, &vLook);
					vAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vLook, &vDir))); //각도 구했고.

					_vec3 vRight;
					_vec3 vLeft;

					m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

					D3DXVec3Normalize(&vRight, &vRight);
					vLeft = vRight * -1.f;
					/*D3DXVec3Normalize(&vLeft, &vLeft);*/


					_float fRightAngle = D3DXVec3Dot(&vDir, &vRight);
					_float fLeftAngle = D3DXVec3Dot(&vDir, &vLeft);


					if (vAngle >= D3DXToDegree(fTurnSpeed))
					{
						if (fRightAngle < 0 && fLeftAngle > 0)
						{
							m_pTransformCom->Rotation(ROT_Y, -fTurnSpeed);
						}
						else if (fRightAngle > 0 && fLeftAngle < 0)
							m_pTransformCom->Rotation(ROT_Y, fTurnSpeed);
					}


					if (m_fFireTime >= 2.3f)
					{
						if (false == m_bFireSound && iBulletMax < 3)
						{
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::MONSTER);
							CSoundMgr::Get_Instance()->PlaySoundW(L"Liea_Bite.wav", CSoundMgr::CHANNELID::MONSTER);
							m_bFireSound = true;
						}
						if (iBulletMax < 3)
						{

							Engine::CLayer*			pLayer = nullptr;

							pLayer = pManagement->Get_Layer(L"GameLogic");
							NULL_CHECK_RETURN(pLayer, E_FAIL);

							// 오브젝트 추가
							Engine::CGameObject*		pGameObject = nullptr;

							_vec3 vPos = { m_pFinalMatrix[SPHERE_HEAD]._41, m_pFinalMatrix[SPHERE_HEAD]._42, m_pFinalMatrix[SPHERE_HEAD]._43 };

							pGameObject = CBossBullet::Create(m_pGraphicDev, vPos);
							NULL_CHECK_RETURN(pGameObject, E_FAIL);
							FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss_Bullet", pGameObject), E_FAIL);

							iBulletMax++;
							m_fFireTime = 0.f;
							m_bBossAttack0 = false;
							m_bFireSound = false;
							return;
						}


					}

					if (true == m_pMeshCom->Is_AnimationSetEnd())
					{
						iBulletMax = 0;
						m_pMeshCom->Set_Animationset(44);
						m_fPatternTime = 0.f;
						m_fFireTime = 0.f;
						iSelect = rand() % 2 + 2;
						m_iAniNum = 44;
						m_bBossAttack0 = false;
						return;
					}
				}

				//앞으로 돌진하는 패턴. 36, 42, 37 애니메이션 순서.
				else if (1 == iSelect && 3.f <= m_fPatternTime)
				{

					//애니메이션 순서대로 넘기는것.
					if (m_iAniNum == 44)
					{

						_vec3 vPlayerPos;
						pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);

						_vec3 vPos;
						m_pTransformCom->Get_Info(INFO_POS, &vPos);

						_vec3 vDown = { 0.f, -1.f, 0.f };
						if (vPos.y - 1.f > vPlayerPos.y)
							m_pTransformCom->Move_Pos(&vDown, 10.f, fTimeDelta);


						if (!m_bBossAttack1)
						{

							pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
							//도착지에서 출발지를 빼야 우리가원하는게나옴
							_vec3 vDir;
							vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];
							vDir.y = 0.f;
							D3DXVec3Normalize(&vDir, &vDir);

							_vec3 vLook;
							m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
							D3DXVec3Normalize(&vLook, &vLook);
							vAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vLook, &vDir))); //각도 구했고.

							_vec3 vRight;
							_vec3 vLeft;

							m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

							D3DXVec3Normalize(&vRight, &vRight);
							vLeft = vRight * -1.f;
							/*D3DXVec3Normalize(&vLeft, &vLeft);*/


							_float fRightAngle = D3DXVec3Dot(&vDir, &vRight);
							_float fLeftAngle = D3DXVec3Dot(&vDir, &vLeft);


							if (fRightAngle < 0 && fLeftAngle > 0)
							{
								m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-vAngle));
							}
							else if (fRightAngle > 0 && fLeftAngle < 0)
								m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(vAngle));


							m_bBossAttack1 = true;

						}

						m_pMeshCom->Set_Animationset(36);

						if (m_iAniNum == 44 && true == m_pMeshCom->Is_AnimationSetEnd())
						{
							m_iAniNum = 36;
							m_bBossAttack1 = false;
							_vec3 temp = vPos - vPlayerPos;
							m_vPlayerDis = D3DXVec3Length(&temp);
							
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER);
							CSoundMgr::Get_Instance()->PlaySoundW(L"Liea_Fly_Rush.wav", CSoundMgr::MONSTER);
						}

					}

					if (m_iAniNum == 36)
					{
						m_pMeshCom->Set_Animationset(42);

						_vec3 vLook;
						m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
						D3DXVec3Normalize(&vLook, &vLook);
						m_pTransformCom->Move_Pos(&vLook, 30.f, fTimeDelta);
						m_fMoveDis += 30 * fTimeDelta;

						m_bAttack = true;

						if (m_vPlayerDis < m_fMoveDis)
						{
							m_iAniNum = 42;
							m_fMoveDis = 0.f;
							m_bAttack = false;
							m_bColl = false;
						}

						if (m_iAniNum == 36 && true == m_pMeshCom->Is_AnimationSetEnd())
						{
							m_bColl = false;
							m_bAttack = false;
							m_iAniNum = 42;
						}

					}

					if (m_iAniNum == 42)
					{
						m_pMeshCom->Set_Animationset(37);
						m_bAttack = false;
						_vec3 vPos;
						m_pTransformCom->Get_Info(INFO_POS, &vPos);

						_vec3 vUp = { 0.f, 1.f, 0.f };
						if (vPos.y < m_vCurrentPos.y)
							m_pTransformCom->Move_Pos(&vUp, 5.f, fTimeDelta);

						if (m_iAniNum == 42 && true == m_pMeshCom->Is_AnimationSetEnd())
						{
							m_bAttack = false;
							m_iAniNum = 37;
						}
					}

					//패턴 끝 다시 공중 아이들.
					if (m_iAniNum == 37)
					{
						m_iAniNum = 44;
						m_pMeshCom->Set_Animationset(44);
						m_fPatternTime = 0.f;
						m_fFireTime = 0.f;
						iSelect = rand() % 2 + 2;
						m_iAniNum = 44;


						return;
					}

				}


				//꼬리치기
				else if (3 == iSelect && 3.f <= m_fPatternTime)
				{

					CManagement*		pManagement = CManagement::GetInstance();
					NULL_CHECK_RETURN(pManagement, );

					CTransform*			pPlayerTransform = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));
					_vec3 vPlayerPos;

					if (nullptr == pPlayerTransform)
						return;

					pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
					//도착지에서 출발지를 빼야 우리가원하는게나옴
					_vec3 vDir;
					vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];
					_float fDir = D3DXVec3Length(&vDir);

					vDir.y = 0.f;
					D3DXVec3Normalize(&vDir, &vDir);

					if (fDir >= 20.f)
					{
						iSelect = rand() % 2 + 2;
						m_bAttack = false;

						m_pMeshCom->Set_Animationset(44);
						m_iAniNum = 44;
						return ;
					}

					m_pMeshCom->Set_Animationset(5);

					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::MONSTER);
					CSoundMgr::Get_Instance()->PlaySoundW(L"Liea_BackJump.wav", CSoundMgr::CHANNELID::MONSTER);
			
					m_bAttack = true;
					
					_vec3 vLook;
					m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
					D3DXVec3Normalize(&vLook, &vLook);
					vAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vLook, &vDir))); //각도 구했고.

					_vec3 vRight;
					_vec3 vLeft;

					m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

					D3DXVec3Normalize(&vRight, &vRight);
					vLeft = vRight * -1.f;
					/*D3DXVec3Normalize(&vLeft, &vLeft);*/


					_float fRightAngle = D3DXVec3Dot(&vDir, &vRight);
					_float fLeftAngle = D3DXVec3Dot(&vDir, &vLeft);


					if (vAngle >= D3DXToDegree(fTurnSpeed))
					{
						if (fRightAngle < 0 && fLeftAngle > 0)
						{
							m_pTransformCom->Rotation(ROT_Y, -fTurnSpeed);
						}
						else if (fRightAngle > 0 && fLeftAngle < 0)
							m_pTransformCom->Rotation(ROT_Y, fTurnSpeed);
					}

					if (true == m_pMeshCom->Is_AnimationSetEnd())
					{
						m_pMeshCom->Set_Animationset(44);
						m_fPatternTime = 0.f;
						iSelect = rand() % 2 + 2;
						m_iAniNum = 44;
						m_bAttack = false;
						return;
					}
				}

			}
		}

		if (m_bGroundPattern)
		{
			
			//쓰러지고나서 일어나는 패턴.
			if (m_iAniNum == 14)
			{
				m_bAttack = false;
				m_bColl = false;

				m_pMeshCom->Set_Animationset(16);

				_vec3 vUp = { 0.f, 1.f, 0.f };
				_vec3 vPos;
				m_pTransformCom->Get_Info(INFO_POS, &vPos);

				m_pTransformCom->Move_Pos(&vUp, 1.6f, fTimeDelta);
			
				if (m_iAniNum == 14 && true == m_pMeshCom->Is_AnimationSetEnd())
				{
					m_pMeshCom->Set_Animationset(63);
					m_fGroundTime = 0.f;
					m_iAniNum = 63;
				}
			}

		
			//일어난 후에 기본적으로 플레이어 보면서 따라감.
			if (m_iAniNum == 63)
			{
				m_fGroundTime += fTimeDelta;

				//플레이어와 거리계산후, 

				_vec3 vPlayerPos2;
				pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos2);

				_vec3 vPos2;
				m_pTransformCom->Get_Info(INFO_POS, &vPos2);

				_vec3 Temp2 = vPlayerPos2 - vPos2;
				_float fDir2 = D3DXVec3Length(&Temp2);

				_vec3 vMove;
				m_pTransformCom->Get_Info(INFO_LOOK, &vMove);
				D3DXVec3Normalize(&vMove, &vMove);

				//거리가 멀면 천천히 따라옴.
				if (fDir2 >= 20.f)
				{

					CManagement*		pManagement = CManagement::GetInstance();
					NULL_CHECK_RETURN(pManagement, );

					//CGameObject*	pPlayer = dynamic_cast<CGameObject*>(pManagement->Get_GameObject(L"GameLogic", L"Player"));

					CTransform*			pPlayerTransform = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));
					_vec3 vPlayerPos;

					if (nullptr == pPlayerTransform)
						return;

					pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
					//도착지에서 출발지를 빼야 우리가원하는게나옴
					_vec3 vDir;
					vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];
					vDir.y = 0.f;
					D3DXVec3Normalize(&vDir, &vDir);

					_vec3 vLook;
					m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
					D3DXVec3Normalize(&vLook, &vLook);
					vAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vLook, &vDir))); //각도 구했고.

					_vec3 vRight;
					_vec3 vLeft;

					m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

					D3DXVec3Normalize(&vRight, &vRight);
					vLeft = vRight * -1.f;
					/*D3DXVec3Normalize(&vLeft, &vLeft);*/


					_float fRightAngle = D3DXVec3Dot(&vDir, &vRight);
					_float fLeftAngle = D3DXVec3Dot(&vDir, &vLeft);


					if (vAngle >= D3DXToDegree(fTurnSpeed))
					{
						if (fRightAngle < 0 && fLeftAngle > 0)
						{
							m_pTransformCom->Rotation(ROT_Y, -fTurnSpeed);
						}
						else if (fRightAngle > 0 && fLeftAngle < 0)
							m_pTransformCom->Rotation(ROT_Y, fTurnSpeed);
					}
					m_pMeshCom->Set_Animationset(60);
					m_pTransformCom->Move_Pos(&vMove, 2.f, fTimeDelta);
				}
				else
				{
					m_pMeshCom->Set_Animationset(51);
				}


				if (m_fGroundTime >= 4.f)
				{
					m_iGroundSelect = rand() % 3 + 1;
					m_fGroundTime = 0.f;
					m_iAniNum = 60; //이게 패턴의 시작.
				}

			}


			//플레이어를 향해 달리기. 58번 애니, 57번 애니.
			if (m_iGroundSelect == 1)
			{

				if (!m_bBossAttack2)
				{
					CManagement*		pManagement = CManagement::GetInstance();
					NULL_CHECK_RETURN(pManagement, );

					//CGameObject*	pPlayer = dynamic_cast<CGameObject*>(pManagement->Get_GameObject(L"GameLogic", L"Player"));

					CTransform*			pPlayerTransform = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));
					_vec3 vPlayerPos;

					pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
					//도착지에서 출발지를 빼야 우리가원하는게나옴
					_vec3 vDir;
					vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];
					D3DXVec3Normalize(&vDir, &vDir);

					_vec3 vLook;
					m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
					D3DXVec3Normalize(&vLook, &vLook);
					vAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vLook, &vDir))); //각도 구했고.

					_vec3 vRight;
					_vec3 vLeft;

					m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

					D3DXVec3Normalize(&vRight, &vRight);
					vLeft = vRight * -1.f;
					/*D3DXVec3Normalize(&vLeft, &vLeft);*/


					_float fRightAngle = D3DXVec3Dot(&vDir, &vRight);
					_float fLeftAngle = D3DXVec3Dot(&vDir, &vLeft);


					if (fRightAngle < 0 && fLeftAngle > 0)
					{
						m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-vAngle));
					}
					else if (fRightAngle > 0 && fLeftAngle < 0)
						m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(vAngle));


					_vec3 vPos;
					m_pTransformCom->Get_Info(INFO_POS, &vPos);
					_vec3 temp = vPos - vPlayerPos;
					m_vPlayerDis = D3DXVec3Length(&temp);

					m_bBossAttack2 = true;

				}
				//애니메이션 시작.
				if (m_iAniNum == 60)
				{
					_vec3 vLook;
					m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
					D3DXVec3Normalize(&vLook, &vLook);
					m_pMeshCom->Set_Animationset(58);
					m_pTransformCom->Move_Pos(&vLook, 20.f, fTimeDelta);
					m_bAttack = true;
					m_fMoveDis += 20 * fTimeDelta;


					CDynamicCamera*		pCamera = dynamic_cast<CDynamicCamera*>(pManagement->Get_GameObject(L"Environment", L"DynamicCamera"));

					if (nullptr == pCamera)
						return;

					pCamera->Set_Shake(true);

					CSoundMgr::Get_Instance()->PlaySoundW(L"Liea_Rush.wav", CSoundMgr::CHANNELID::MONSTER);


					if ((m_vPlayerDis + 20.f) < m_fMoveDis)
					{
						m_iAniNum = 58;
						m_fMoveDis = 0.f;
						m_bAttack = false;
					}
				
				}
				else if (m_iAniNum == 58)
				{
					m_pMeshCom->Set_Animationset(57);	
				}

				if (m_iAniNum == 60 && true == m_pMeshCom->Is_AnimationSetEnd())
				{
					m_bAttack = false;
					m_iAniNum = 58;
				}
				else if (m_iAniNum == 58 && true == m_pMeshCom->Is_AnimationSetEnd())
				{
					//m_pMeshCom->Set_Animationset(60);
					m_iAniNum = 63;
					m_bBossAttack2 = false;
					m_fRunTime = 0.f;
				}

			}

			// 불 뿜는 애니메이션 4번.
			if (m_iGroundSelect == 2)
			{

				m_fFireTime2 += fTimeDelta;

				//애니메이션 시작.
				if (m_iAniNum == 60)
				{

					m_pMeshCom->Set_Animationset(31);

					

					if (iTimeBulletMax < 1)
						{
							CManagement*		pManagement = CManagement::GetInstance();
							NULL_CHECK_RETURN(pManagement, );

							//CGameObject*	pPlayer = dynamic_cast<CGameObject*>(pManagement->Get_GameObject(L"GameLogic", L"Player"));

							CTransform*			pPlayerTransform = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));
							_vec3 vPlayerPos;

							pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
							//도착지에서 출발지를 빼야 우리가원하는게나옴
							_vec3 vDir;
							vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];
							D3DXVec3Normalize(&vDir, &vDir);

							_vec3 vLook;
							m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
							D3DXVec3Normalize(&vLook, &vLook);
							vAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vLook, &vDir))); //각도 구했고.

							_vec3 vRight;
							_vec3 vLeft;

							m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

							D3DXVec3Normalize(&vRight, &vRight);
							vLeft = vRight * -1.f;
							/*D3DXVec3Normalize(&vLeft, &vLeft);*/


							_float fRightAngle = D3DXVec3Dot(&vDir, &vRight);
							_float fLeftAngle = D3DXVec3Dot(&vDir, &vLeft);


							if (fRightAngle < 0 && fLeftAngle > 0)
							{
								m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-vAngle));
							}
							else if (fRightAngle > 0 && fLeftAngle < 0)
								m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(vAngle));


							Engine::CLayer*			pLayer = nullptr;

							pLayer = pManagement->Get_Layer(L"GameLogic");
							NULL_CHECK_RETURN(pLayer, E_FAIL);

							// 오브젝트 추가
							Engine::CGameObject*		pGameObject = nullptr;

							_vec3 vPos = { m_pFinalMatrix[SPHERE_WL01]._41, m_pFinalMatrix[SPHERE_WL01]._42 + 4.f, m_pFinalMatrix[SPHERE_WL01]._43 };

							pGameObject = CBossTimeBullet::Create(m_pGraphicDev, vPos);
							NULL_CHECK_RETURN(pGameObject, E_FAIL);
							FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Mesh_BossTimebullet", pGameObject), E_FAIL);


							vPos = { m_pFinalMatrix[SPHERE_WR01]._41, m_pFinalMatrix[SPHERE_WR01]._42 + 4.f, m_pFinalMatrix[SPHERE_WR01]._43 };

							pGameObject = CBossTimeBullet::Create(m_pGraphicDev, vPos);
							NULL_CHECK_RETURN(pGameObject, E_FAIL);
							FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Mesh_BossTimebullet", pGameObject), E_FAIL);


							vPos = { m_pFinalMatrix[SPHERE_AL02]._41, m_pFinalMatrix[SPHERE_AL02]._42 + 4.f, m_pFinalMatrix[SPHERE_AL02]._43 };

							pGameObject = CBossTimeBullet::Create(m_pGraphicDev, vPos);
							NULL_CHECK_RETURN(pGameObject, E_FAIL);
							FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Mesh_BossTimebullet", pGameObject), E_FAIL);

							vPos = { m_pFinalMatrix[SPHERE_AR02]._41, m_pFinalMatrix[SPHERE_AR02]._42 + 4.f, m_pFinalMatrix[SPHERE_AR02]._43 };

							pGameObject = CBossTimeBullet::Create(m_pGraphicDev, vPos);
							NULL_CHECK_RETURN(pGameObject, E_FAIL);
							FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Mesh_BossTimebullet", pGameObject), E_FAIL);


							vPos = { m_pFinalMatrix[SPHERE_HEAD]._41, m_pFinalMatrix[SPHERE_HEAD]._42 + 4.f, m_pFinalMatrix[SPHERE_HEAD]._43 };

							pGameObject = CBossTimeBullet::Create(m_pGraphicDev, vPos);
							NULL_CHECK_RETURN(pGameObject, E_FAIL);
							FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Mesh_BossTimebullet", pGameObject), E_FAIL);

							iTimeBulletMax++;
						}

				}

				if (m_iAniNum == 60 && true == m_pMeshCom->Is_AnimationSetEnd())
				{
					m_iAniNum = 63;
					m_bBossAttack2 = false;
					m_fFireTime2 = 0.f;
					iTimeBulletMax = 0;
					iBulltMax2 = 0;
				}

			}

			//양쪽으로 날개 휘두르기. 33번애니, 32번 애니.
			if (m_iGroundSelect == 3)
			{

				_vec3 vPlayerPos2;
				pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos2);

				_vec3 vPos2;
				m_pTransformCom->Get_Info(INFO_POS, &vPos2);

				_vec3 Temp2 = vPlayerPos2 - vPos2;
				_float fDir2 = D3DXVec3Length(&Temp2);

				_vec3 vMove;
				m_pTransformCom->Get_Info(INFO_LOOK, &vMove);
				D3DXVec3Normalize(&vMove, &vMove);

				//거리가 멀면 천천히 따라옴.
				if (fDir2 >= 15.f)
				{
					m_iGroundSelect = rand() % 3 + 1;
					m_iAniNum = 60;
					m_bAttack = false;
					return;
				}

				if (!m_bBossAttack3)
				{

					CManagement*		pManagement = CManagement::GetInstance();
					NULL_CHECK_RETURN(pManagement, );

					//CGameObject*	pPlayer = dynamic_cast<CGameObject*>(pManagement->Get_GameObject(L"GameLogic", L"Player"));

					CTransform*			pPlayerTransform = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Player", L"Com_Transform", COMPONENTID::ID_DYNAMIC));
					_vec3 vPlayerPos;

					pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
					//도착지에서 출발지를 빼야 우리가원하는게나옴
					_vec3 vDir;
					vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];
					D3DXVec3Normalize(&vDir, &vDir);

					_vec3 vLook;
					m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
					D3DXVec3Normalize(&vLook, &vLook);
					vAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vLook, &vDir))); //각도 구했고.

					_vec3 vRight;
					_vec3 vLeft;

					m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

					D3DXVec3Normalize(&vRight, &vRight);
					vLeft = vRight * -1.f;
					/*D3DXVec3Normalize(&vLeft, &vLeft);*/


					_float fRightAngle = D3DXVec3Dot(&vDir, &vRight);
					_float fLeftAngle = D3DXVec3Dot(&vDir, &vLeft);


					if (fRightAngle < 0 && fLeftAngle > 0)
					{
						m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-vAngle));
					}
					else if (fRightAngle > 0 && fLeftAngle < 0)
						m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(vAngle));

					m_bBossAttack3 = true;
				}

				//애니메이션 시작.
				if (m_iAniNum == 60)
				{
					if (false == m_bSwingSound)
					{
						//CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::MONSTER);
						CSoundMgr::Get_Instance()->PlaySoundW(L"Swing.wav", CSoundMgr::CHANNELID::MONSTER);
						m_bSwingSound = true;
					}

					m_pMeshCom->Set_Animationset(33);
					m_bAttack = true;
					if (m_iAniNum == 60 && true == m_pMeshCom->Is_AnimationSetEnd())
					{
						m_bColl = false;
						m_bAttack = false;
						m_bBossAttack3 = false;
						m_bSwingSound = false;
						m_iAniNum = 33;
					}
			
				}

				if (m_iAniNum == 33)
				{

					if (false == m_bSwingSound)
					{
						//CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::MONSTER_EFFECT);
						CSoundMgr::Get_Instance()->PlaySoundW(L"Swing.wav", CSoundMgr::CHANNELID::MONSTER);
						m_bSwingSound = true;
					}

					m_pMeshCom->Set_Animationset(32);
					m_bAttack = true;
					if (m_iAniNum == 33 && true == m_pMeshCom->Is_AnimationSetEnd())
					{
						m_iAniNum = 63;
						m_bColl = false;
						m_bAttack = false;
						m_bBossAttack3 = false;
						m_bSwingSound = false;

					}
				}
			}

		}
	}

	

	return S_OK;
}

HRESULT CBoss::IdleBoss_Move(const _float& fTimeDelta)
{

	_vec3 vMove;

	if (m_fDelTime <= 5.0f)
	{
		m_pMeshCom->Set_Animationset(60);
		m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vMove);
		D3DXVec3Normalize(&vMove, &vMove);


		if (m_fAngle <= D3DXToRadian(90.f))
		{
			m_fAngle += D3DXToRadian(90.f) * fTimeDelta;
			m_pTransformCom->Add_Rotation(EAxis::Y, D3DXToRadian(90.f), fTimeDelta);
		}
		m_pTransformCom->Move_Pos(&vMove, 2.f, fTimeDelta);
	}
	else
	{
		m_pMeshCom->Set_Animationset(50);

		if (true == m_pMeshCom->Is_AnimationSetEnd())
		{
			/*m_pTransformCom->Add_Rotation(EAxis::X, D3DXToRadian(90.f), fTimeDelta);*/
			m_fAngle = 0.f;
			m_fDelTime = 0.f;
			
		}

	}



	return S_OK;
}

_bool CBoss::Collision_ToObject(const _tchar * pLayerTag, const _tchar * pObjTag, const _float & fTimeDelta)
{

	

	CManagement*	pManagement = CManagement::GetInstance();

	CGameObject*	pPlayer = pManagement->Get_GameObject(pLayerTag, pObjTag);

	_bool PlayerAttackCheck = dynamic_cast<CPlayer*>(pManagement->Get_GameObject(pLayerTag, pObjTag))->Get_AttackCheck();


	if (m_pCalculatorCom->Collision_Sphere(this, pPlayer))
	{
		return true;
	}

	return false;


}

CBoss * CBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss*	pInstance = new CBoss(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CBoss::Free(void)
{
	Engine::CGameObject::Free();
}

void CBoss::SetUp_Sphere(void)
{

	//머리
	if (nullptr == m_pParentBoneMatrix[SPHERE_HEAD])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("HD-101");

		m_pParentBoneMatrix[SPHERE_HEAD] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}

	m_pFinalMatrix[SPHERE_HEAD] = *m_pParentBoneMatrix[SPHERE_HEAD] * *m_pParentWorldMatrix;




#pragma region 목 콜라이더
	//목
	if (nullptr == m_pParentBoneMatrix[SPHERE_NK00])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("NK-00");

		m_pParentBoneMatrix[SPHERE_NK00] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}

	m_pFinalMatrix[SPHERE_NK00] = *m_pParentBoneMatrix[SPHERE_NK00] * *m_pParentWorldMatrix;

	if (nullptr == m_pParentBoneMatrix[SPHERE_NK01])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("NK-01");

		m_pParentBoneMatrix[SPHERE_NK01] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}

	m_pFinalMatrix[SPHERE_NK01] = *m_pParentBoneMatrix[SPHERE_NK01] * *m_pParentWorldMatrix;


	if (nullptr == m_pParentBoneMatrix[SPHERE_NK02])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("NK-02");

		m_pParentBoneMatrix[SPHERE_NK02] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}

	m_pFinalMatrix[SPHERE_NK02] = *m_pParentBoneMatrix[SPHERE_NK02] * *m_pParentWorldMatrix;

#pragma endregion


#pragma region 날개
	//오른쪽 날개
	if (nullptr == m_pParentBoneMatrix[SPHERE_WR00])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("WR-00");

		m_pParentBoneMatrix[SPHERE_WR00] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}

	m_pFinalMatrix[SPHERE_WR00] = *m_pParentBoneMatrix[SPHERE_WR00] * *m_pParentWorldMatrix;

	if (nullptr == m_pParentBoneMatrix[SPHERE_WR01])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("WR-01");

		m_pParentBoneMatrix[SPHERE_WR01] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix[SPHERE_WR01] = *m_pParentBoneMatrix[SPHERE_WR01] * *m_pParentWorldMatrix;


	if (nullptr == m_pParentBoneMatrix[SPHERE_WR02])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("WR-02");

		m_pParentBoneMatrix[SPHERE_WR02] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix[SPHERE_WR02] = *m_pParentBoneMatrix[SPHERE_WR02] * *m_pParentWorldMatrix;

	//왼쪽 날개

	if (nullptr == m_pParentBoneMatrix[SPHERE_WL00])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("WL-00");

		m_pParentBoneMatrix[SPHERE_WL00] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}

	m_pFinalMatrix[SPHERE_WL00] = *m_pParentBoneMatrix[SPHERE_WL00] * *m_pParentWorldMatrix;

	if (nullptr == m_pParentBoneMatrix[SPHERE_WL01])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("WL-01");

		m_pParentBoneMatrix[SPHERE_WL01] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix[SPHERE_WL01] = *m_pParentBoneMatrix[SPHERE_WL01] * *m_pParentWorldMatrix;


	if (nullptr == m_pParentBoneMatrix[SPHERE_WL02])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("WL-02");

		m_pParentBoneMatrix[SPHERE_WL02] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix[SPHERE_WL02] = *m_pParentBoneMatrix[SPHERE_WL02] * *m_pParentWorldMatrix;


#pragma endregion

#pragma region 다리

	//오른쪽 다리.
	if (nullptr == m_pParentBoneMatrix[SPHERE_LR00])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LR-00");

		m_pParentBoneMatrix[SPHERE_LR00] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}

	m_pFinalMatrix[SPHERE_LR00] = *m_pParentBoneMatrix[SPHERE_LR00] * *m_pParentWorldMatrix;

	if (nullptr == m_pParentBoneMatrix[SPHERE_LR01])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LR-01");

		m_pParentBoneMatrix[SPHERE_LR01] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix[SPHERE_LR01] = *m_pParentBoneMatrix[SPHERE_LR01] * *m_pParentWorldMatrix;


	if (nullptr == m_pParentBoneMatrix[SPHERE_LR02])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LR-02");

		m_pParentBoneMatrix[SPHERE_LR02] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix[SPHERE_LR02] = *m_pParentBoneMatrix[SPHERE_LR02] * *m_pParentWorldMatrix;

	if (nullptr == m_pParentBoneMatrix[SPHERE_LR03])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LR-03");

		m_pParentBoneMatrix[SPHERE_LR03] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix[SPHERE_LR03] = *m_pParentBoneMatrix[SPHERE_LR03] * *m_pParentWorldMatrix;

	//왼쪽 다리
	if (nullptr == m_pParentBoneMatrix[SPHERE_LL00])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LL-00");

		m_pParentBoneMatrix[SPHERE_LL00] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}

	m_pFinalMatrix[SPHERE_LL00] = *m_pParentBoneMatrix[SPHERE_LL00] * *m_pParentWorldMatrix;


	if (nullptr == m_pParentBoneMatrix[SPHERE_LL01])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LL-01");

		m_pParentBoneMatrix[SPHERE_LL01] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix[SPHERE_LL01] = *m_pParentBoneMatrix[SPHERE_LL01] * *m_pParentWorldMatrix;


	if (nullptr == m_pParentBoneMatrix[SPHERE_LL02])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LL-02");

		m_pParentBoneMatrix[SPHERE_LL02] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix[SPHERE_LL02] = *m_pParentBoneMatrix[SPHERE_LL02] * *m_pParentWorldMatrix;


	if (nullptr == m_pParentBoneMatrix[SPHERE_LL03])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LL-03");

		m_pParentBoneMatrix[SPHERE_LL03] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix[SPHERE_LL03] = *m_pParentBoneMatrix[SPHERE_LL03] * *m_pParentWorldMatrix;


#pragma endregion

#pragma region 팔

	//오른쪽
	if (nullptr == m_pParentBoneMatrix[SPHERE_AR00])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("AR-00");

		m_pParentBoneMatrix[SPHERE_AR00] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix[SPHERE_AR00] = *m_pParentBoneMatrix[SPHERE_AR00] * *m_pParentWorldMatrix;


	if (nullptr == m_pParentBoneMatrix[SPHERE_AR01])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("AR-01");

		m_pParentBoneMatrix[SPHERE_AR01] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix[SPHERE_AR01] = *m_pParentBoneMatrix[SPHERE_AR01] * *m_pParentWorldMatrix;

	//이걸 조정해서 해야할듯. 팔꿈치.
	if (nullptr == m_pParentBoneMatrix[SPHERE_AR02])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("AR-02");

		m_pParentBoneMatrix[SPHERE_AR02] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix[SPHERE_AR02] = *m_pParentBoneMatrix[SPHERE_AR02] * *m_pParentWorldMatrix;

	if (nullptr == m_pParentBoneMatrix[SPHERE_AR03])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("AR-03");

		m_pParentBoneMatrix[SPHERE_AR03] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix[SPHERE_AR03] = *m_pParentBoneMatrix[SPHERE_AR03] * *m_pParentWorldMatrix;

	//왼쪽 팔
	if (nullptr == m_pParentBoneMatrix[SPHERE_AL00])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("AL-00");

		m_pParentBoneMatrix[SPHERE_AL00] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix[SPHERE_AL00] = *m_pParentBoneMatrix[SPHERE_AL00] * *m_pParentWorldMatrix;


	if (nullptr == m_pParentBoneMatrix[SPHERE_AL01])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("AL-01");

		m_pParentBoneMatrix[SPHERE_AL01] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix[SPHERE_AL01] = *m_pParentBoneMatrix[SPHERE_AL01] * *m_pParentWorldMatrix;


	if (nullptr == m_pParentBoneMatrix[SPHERE_AL02])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("AL-02");

		m_pParentBoneMatrix[SPHERE_AL02] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix[SPHERE_AL02] = *m_pParentBoneMatrix[SPHERE_AL02] * *m_pParentWorldMatrix;

	if (nullptr == m_pParentBoneMatrix[SPHERE_AL03])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("AL-03");

		m_pParentBoneMatrix[SPHERE_AL03] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix[SPHERE_AL03] = *m_pParentBoneMatrix[SPHERE_AL03] * *m_pParentWorldMatrix;
#pragma endregion

#pragma region 발톱
	if (nullptr == m_pParentBoneMatrix[SPHERE_FL00])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("FL-00");

		m_pParentBoneMatrix[SPHERE_FL00] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}

	m_pFinalMatrix[SPHERE_FL00] = *m_pParentBoneMatrix[SPHERE_FL00] * *m_pParentWorldMatrix;

	if (nullptr == m_pParentBoneMatrix[SPHERE_FR00])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("FR-00");

		m_pParentBoneMatrix[SPHERE_FR00] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}

	m_pFinalMatrix[SPHERE_FR00] = *m_pParentBoneMatrix[SPHERE_FR00] * *m_pParentWorldMatrix;
#pragma endregion



#pragma region 꼬리
	//꼬리
	if (nullptr == m_pParentBoneMatrix[SPHERE_TL00])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("TL-00");

		m_pParentBoneMatrix[SPHERE_TL00] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}

	m_pFinalMatrix[SPHERE_TL00] = *m_pParentBoneMatrix[SPHERE_TL00] * *m_pParentWorldMatrix;

	if (nullptr == m_pParentBoneMatrix[SPHERE_TL01])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("TL-01");

		m_pParentBoneMatrix[SPHERE_TL01] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}

	m_pFinalMatrix[SPHERE_TL01] = *m_pParentBoneMatrix[SPHERE_TL01] * *m_pParentWorldMatrix;

	if (nullptr == m_pParentBoneMatrix[SPHERE_TL02])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("TL-02");

		m_pParentBoneMatrix[SPHERE_TL02] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}

	m_pFinalMatrix[SPHERE_TL02] = *m_pParentBoneMatrix[SPHERE_TL02] * *m_pParentWorldMatrix;

	if (nullptr == m_pParentBoneMatrix[SPHERE_TL03])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("TL-03");

		m_pParentBoneMatrix[SPHERE_TL03] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}

	m_pFinalMatrix[SPHERE_TL03] = *m_pParentBoneMatrix[SPHERE_TL03] * *m_pParentWorldMatrix;

	if (nullptr == m_pParentBoneMatrix[SPHERE_TL04])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("TL-04");

		m_pParentBoneMatrix[SPHERE_TL04] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}

	m_pFinalMatrix[SPHERE_TL04] = *m_pParentBoneMatrix[SPHERE_TL04] * *m_pParentWorldMatrix;

	if (nullptr == m_pParentBoneMatrix[SPHERE_TL05])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("TL-05");

		m_pParentBoneMatrix[SPHERE_TL05] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	}

	m_pFinalMatrix[SPHERE_TL05] = *m_pParentBoneMatrix[SPHERE_TL05] * *m_pParentWorldMatrix;


#pragma endregion
	if (!m_listCollision.empty())
	{
		m_listCollision.clear();
	}


	for (_uint i = 0; i < SPHERE_END; ++i)
	{
		_vec3 vSpherePos;
		vSpherePos = { m_pFinalMatrix[i]._41, m_pFinalMatrix[i]._42, m_pFinalMatrix[i]._43 };

	/*	_float vRadius = 1.f;*/
		_float vRadius = 2.f;
		m_listCollision.push_back(make_pair(vSpherePos, vRadius));
	}

}

void CBoss::SetUp_AttackSphere(void)
{

	{

		//머리
		if (nullptr == m_pParentBoneMatrix[SPHERE_HEAD])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("HD-101");

			m_pParentBoneMatrix[SPHERE_HEAD] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}

		m_pFinalMatrix[SPHERE_HEAD] = *m_pParentBoneMatrix[SPHERE_HEAD] * *m_pParentWorldMatrix;




#pragma region 목 콜라이더
		//목
		if (nullptr == m_pParentBoneMatrix[SPHERE_NK00])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("NK-00");

			m_pParentBoneMatrix[SPHERE_NK00] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}

		m_pFinalMatrix[SPHERE_NK00] = *m_pParentBoneMatrix[SPHERE_NK00] * *m_pParentWorldMatrix;

		if (nullptr == m_pParentBoneMatrix[SPHERE_NK01])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("NK-01");

			m_pParentBoneMatrix[SPHERE_NK01] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}

		m_pFinalMatrix[SPHERE_NK01] = *m_pParentBoneMatrix[SPHERE_NK01] * *m_pParentWorldMatrix;


		if (nullptr == m_pParentBoneMatrix[SPHERE_NK02])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("NK-02");

			m_pParentBoneMatrix[SPHERE_NK02] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}

		m_pFinalMatrix[SPHERE_NK02] = *m_pParentBoneMatrix[SPHERE_NK02] * *m_pParentWorldMatrix;

#pragma endregion


#pragma region 날개
		//오른쪽 날개
		if (nullptr == m_pParentBoneMatrix[SPHERE_WR00])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("WR-00");

			m_pParentBoneMatrix[SPHERE_WR00] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}

		m_pFinalMatrix[SPHERE_WR00] = *m_pParentBoneMatrix[SPHERE_WR00] * *m_pParentWorldMatrix;

		if (nullptr == m_pParentBoneMatrix[SPHERE_WR01])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("WR-01");

			m_pParentBoneMatrix[SPHERE_WR01] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}
		m_pFinalMatrix[SPHERE_WR01] = *m_pParentBoneMatrix[SPHERE_WR01] * *m_pParentWorldMatrix;


		if (nullptr == m_pParentBoneMatrix[SPHERE_WR02])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("WR-02");

			m_pParentBoneMatrix[SPHERE_WR02] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}
		m_pFinalMatrix[SPHERE_WR02] = *m_pParentBoneMatrix[SPHERE_WR02] * *m_pParentWorldMatrix;

		//왼쪽 날개

		if (nullptr == m_pParentBoneMatrix[SPHERE_WL00])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("WL-00");

			m_pParentBoneMatrix[SPHERE_WL00] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}

		m_pFinalMatrix[SPHERE_WL00] = *m_pParentBoneMatrix[SPHERE_WL00] * *m_pParentWorldMatrix;

		if (nullptr == m_pParentBoneMatrix[SPHERE_WL01])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("WL-01");

			m_pParentBoneMatrix[SPHERE_WL01] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}
		m_pFinalMatrix[SPHERE_WL01] = *m_pParentBoneMatrix[SPHERE_WL01] * *m_pParentWorldMatrix;


		if (nullptr == m_pParentBoneMatrix[SPHERE_WL02])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("WL-02");

			m_pParentBoneMatrix[SPHERE_WL02] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}
		m_pFinalMatrix[SPHERE_WL02] = *m_pParentBoneMatrix[SPHERE_WL02] * *m_pParentWorldMatrix;


#pragma endregion

#pragma region 다리

		//오른쪽 다리.
		if (nullptr == m_pParentBoneMatrix[SPHERE_LR00])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LR-00");

			m_pParentBoneMatrix[SPHERE_LR00] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}

		m_pFinalMatrix[SPHERE_LR00] = *m_pParentBoneMatrix[SPHERE_LR00] * *m_pParentWorldMatrix;

		if (nullptr == m_pParentBoneMatrix[SPHERE_LR01])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LR-01");

			m_pParentBoneMatrix[SPHERE_LR01] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}
		m_pFinalMatrix[SPHERE_LR01] = *m_pParentBoneMatrix[SPHERE_LR01] * *m_pParentWorldMatrix;


		if (nullptr == m_pParentBoneMatrix[SPHERE_LR02])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LR-02");

			m_pParentBoneMatrix[SPHERE_LR02] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}
		m_pFinalMatrix[SPHERE_LR02] = *m_pParentBoneMatrix[SPHERE_LR02] * *m_pParentWorldMatrix;

		if (nullptr == m_pParentBoneMatrix[SPHERE_LR03])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LR-03");

			m_pParentBoneMatrix[SPHERE_LR03] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}
		m_pFinalMatrix[SPHERE_LR03] = *m_pParentBoneMatrix[SPHERE_LR03] * *m_pParentWorldMatrix;

		//왼쪽 다리
		if (nullptr == m_pParentBoneMatrix[SPHERE_LL00])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LL-00");

			m_pParentBoneMatrix[SPHERE_LL00] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}

		m_pFinalMatrix[SPHERE_LL00] = *m_pParentBoneMatrix[SPHERE_LL00] * *m_pParentWorldMatrix;


		if (nullptr == m_pParentBoneMatrix[SPHERE_LL01])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LL-01");

			m_pParentBoneMatrix[SPHERE_LL01] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}
		m_pFinalMatrix[SPHERE_LL01] = *m_pParentBoneMatrix[SPHERE_LL01] * *m_pParentWorldMatrix;


		if (nullptr == m_pParentBoneMatrix[SPHERE_LL02])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LL-02");

			m_pParentBoneMatrix[SPHERE_LL02] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}
		m_pFinalMatrix[SPHERE_LL02] = *m_pParentBoneMatrix[SPHERE_LL02] * *m_pParentWorldMatrix;


		if (nullptr == m_pParentBoneMatrix[SPHERE_LL03])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("LL-03");

			m_pParentBoneMatrix[SPHERE_LL03] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}
		m_pFinalMatrix[SPHERE_LL03] = *m_pParentBoneMatrix[SPHERE_LL03] * *m_pParentWorldMatrix;


#pragma endregion

#pragma region 팔

		//오른쪽
		if (nullptr == m_pParentBoneMatrix[SPHERE_AR00])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("AR-00");

			m_pParentBoneMatrix[SPHERE_AR00] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}
		m_pFinalMatrix[SPHERE_AR00] = *m_pParentBoneMatrix[SPHERE_AR00] * *m_pParentWorldMatrix;


		if (nullptr == m_pParentBoneMatrix[SPHERE_AR01])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("AR-01");

			m_pParentBoneMatrix[SPHERE_AR01] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}
		m_pFinalMatrix[SPHERE_AR01] = *m_pParentBoneMatrix[SPHERE_AR01] * *m_pParentWorldMatrix;

		//이걸 조정해서 해야할듯. 팔꿈치.
		if (nullptr == m_pParentBoneMatrix[SPHERE_AR02])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("AR-02");

			m_pParentBoneMatrix[SPHERE_AR02] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}
		m_pFinalMatrix[SPHERE_AR02] = *m_pParentBoneMatrix[SPHERE_AR02] * *m_pParentWorldMatrix;

		if (nullptr == m_pParentBoneMatrix[SPHERE_AR03])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("AR-03");

			m_pParentBoneMatrix[SPHERE_AR03] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}
		m_pFinalMatrix[SPHERE_AR03] = *m_pParentBoneMatrix[SPHERE_AR03] * *m_pParentWorldMatrix;

		//왼쪽 팔
		if (nullptr == m_pParentBoneMatrix[SPHERE_AL00])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("AL-00");

			m_pParentBoneMatrix[SPHERE_AL00] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}
		m_pFinalMatrix[SPHERE_AL00] = *m_pParentBoneMatrix[SPHERE_AL00] * *m_pParentWorldMatrix;


		if (nullptr == m_pParentBoneMatrix[SPHERE_AL01])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("AL-01");

			m_pParentBoneMatrix[SPHERE_AL01] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}
		m_pFinalMatrix[SPHERE_AL01] = *m_pParentBoneMatrix[SPHERE_AL01] * *m_pParentWorldMatrix;


		if (nullptr == m_pParentBoneMatrix[SPHERE_AL02])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("AL-02");

			m_pParentBoneMatrix[SPHERE_AL02] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}
		m_pFinalMatrix[SPHERE_AL02] = *m_pParentBoneMatrix[SPHERE_AL02] * *m_pParentWorldMatrix;

		if (nullptr == m_pParentBoneMatrix[SPHERE_AL03])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("AL-03");

			m_pParentBoneMatrix[SPHERE_AL03] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}
		m_pFinalMatrix[SPHERE_AL03] = *m_pParentBoneMatrix[SPHERE_AL03] * *m_pParentWorldMatrix;
#pragma endregion

#pragma region 발톱
		if (nullptr == m_pParentBoneMatrix[SPHERE_FL00])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("FL-00");

			m_pParentBoneMatrix[SPHERE_FL00] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}

		m_pFinalMatrix[SPHERE_FL00] = *m_pParentBoneMatrix[SPHERE_FL00] * *m_pParentWorldMatrix;

		if (nullptr == m_pParentBoneMatrix[SPHERE_FR00])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("FR-00");

			m_pParentBoneMatrix[SPHERE_FR00] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}

		m_pFinalMatrix[SPHERE_FR00] = *m_pParentBoneMatrix[SPHERE_FR00] * *m_pParentWorldMatrix;
#pragma endregion

#pragma region 꼬리
		//꼬리
		if (nullptr == m_pParentBoneMatrix[SPHERE_TL00])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("TL-00");

			m_pParentBoneMatrix[SPHERE_TL00] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}

		m_pFinalMatrix[SPHERE_TL00] = *m_pParentBoneMatrix[SPHERE_TL00] * *m_pParentWorldMatrix;

		if (nullptr == m_pParentBoneMatrix[SPHERE_TL01])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("TL-01");

			m_pParentBoneMatrix[SPHERE_TL01] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}

		m_pFinalMatrix[SPHERE_TL01] = *m_pParentBoneMatrix[SPHERE_TL01] * *m_pParentWorldMatrix;

		if (nullptr == m_pParentBoneMatrix[SPHERE_TL02])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("TL-02");

			m_pParentBoneMatrix[SPHERE_TL02] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}

		m_pFinalMatrix[SPHERE_TL02] = *m_pParentBoneMatrix[SPHERE_TL02] * *m_pParentWorldMatrix;

		if (nullptr == m_pParentBoneMatrix[SPHERE_TL03])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("TL-03");

			m_pParentBoneMatrix[SPHERE_TL03] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}

		m_pFinalMatrix[SPHERE_TL03] = *m_pParentBoneMatrix[SPHERE_TL03] * *m_pParentWorldMatrix;

		if (nullptr == m_pParentBoneMatrix[SPHERE_TL04])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("TL-04");

			m_pParentBoneMatrix[SPHERE_TL04] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}

		m_pFinalMatrix[SPHERE_TL04] = *m_pParentBoneMatrix[SPHERE_TL04] * *m_pParentWorldMatrix;

		if (nullptr == m_pParentBoneMatrix[SPHERE_TL05])
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("TL-05");

			m_pParentBoneMatrix[SPHERE_TL05] = &pFrame->CombinedTranformationMatrix;
			m_pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		}

		m_pFinalMatrix[SPHERE_TL05] = *m_pParentBoneMatrix[SPHERE_TL05] * *m_pParentWorldMatrix;


#pragma endregion

		if (!m_listAttackCollision.empty())
		{
			m_listAttackCollision.clear();
		}


		for (_uint i = 0; i < SPHERE_END; ++i)
		{
			_vec3 vSpherePos;
			vSpherePos = { m_pFinalMatrix[i]._41, m_pFinalMatrix[i]._42, m_pFinalMatrix[i]._43 };

			/*	_float vRadius = 1.f;*/
			_float vRadius = 2.f;
			m_listAttackCollision.push_back(make_pair(vSpherePos, vRadius));
		}

	}
}
