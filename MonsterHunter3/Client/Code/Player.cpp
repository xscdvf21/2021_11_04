#include "stdafx.h"
#include "Player.h"
#include "NaviMgr.h"
#include "SoundMgr.h"
#include "KeyMgr.h"
#include "HP_Potion.h"
#include "SP_Potion.h"
#include "Export_Fucntion.h"
#include "Iru.h"
#include "HPUI.h"
#include "Weapon.h"
#include "Wind.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CPlayer::~CPlayer(void)
{

}


HRESULT CPlayer::Ready_Object(void)
{


	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	ZeroMemory(&m_tINFOPLAYER, sizeof(m_tINFOPLAYER));


	m_pMeshCom->Set_Animationset(16);
	m_pTransformCom->m_vScale = _vec3(0.02f, 0.02f,0.02f);
	
	//시작점
	m_pNaviMeshCom->Set_CellIndex(0);
	m_pTransformCom->Set_Pos(_vec3(30.f, 2.f, 220.f));

	//테스트용
	//m_pTransformCom->Set_Pos(_vec3(0.f, 1.f, 0.f));


	for (_uint i = 0; i < PLAYER_END; ++i)
	{
		m_pParentBoneMatrix_Sphere[i] = nullptr;
	}

	for (_uint i = 0; i < PLAYER_END; ++i)
	{
		D3DXMatrixIdentity(&m_pFinalMatrix_Sphere[i]);
	}

	exMapSee = true;

	m_tINFOPLAYER.fSp = 100;
	m_tINFOPLAYER.iMaxHP = 100;
	m_tINFOPLAYER.iHP = 100;
	//m_tINFOPLAYER.fSpMax = 100;

	return S_OK;
}

Engine::_int CPlayer::Update_Object(const _float& pfTimeDelta)
{

	_float fTimeDelta = pfTimeDelta;
	CManagement*			pManagement = CManagement::GetInstance();


	CWeapon*				pWeapon = dynamic_cast<CWeapon*>(pManagement->Get_GameObject(L"GameLogic", L"Player_Weapon"));
	m_bTimeStop = pWeapon->Get_TimeStop();

	if (m_bTimeStop)
	{
		fTimeDelta = 0.f;
	}

	
	Engine::CGameObject::Update_Object(fTimeDelta);



	if (Engine::Get_DIKeyState(DIK_F9) & 0x8000)
	{
		exColisionSee = false;

	}
	else if (Engine::Get_DIKeyState(DIK_F8) & 0x8000)
	{
		exColisionSee = true;

	}

	//
	Default(); //카메라와 플레이어 기본연산.


	if (!m_bFallDown) 
	{
		
		Dash(fTimeDelta);
		if (!m_bDash)
			Attack(fTimeDelta);

		Turn(iKeySelect);
		Key_Input(fTimeDelta);
	}
	else if (m_bFallDown)
	{
		_vec3 vDown = { 0.f, -1.f, 0.f };

		if (!m_bFallDownOne)
		{
			m_pMeshCom->Set_Animationset(11);
			m_bFallDownOne = true;
		}
		m_pTransformCom->Move_Pos(&vDown, 0.2f, fTimeDelta);
		if (true == m_pMeshCom->Is_AnimationSetEnd())
		{
			m_pMeshCom->Set_Animationset(16);
			m_bFallDown = false;

			_vec3 vPos;
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			vPos.y = 2.f;
			m_pTransformCom->Set_Pos(vPos);
		}
	}


	SetUp_Sphere();
	Inventory();
	//Turn(iKeySelect);
	//SetUp_OnTerrain();
	

	PlayAni(fTimeDelta);


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CPlayer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	if(exColisionSee)
		m_pNaviMeshCom->Render_NaviMesh();

	m_pMeshCom->Render_Meshes();


	if (exColisionSee)
	{
		for (_uint i = 0; i < PLAYER_END; ++i)
		{
			m_pSphere->Render_Buffer(&m_pFinalMatrix_Sphere[i], _vec3(0.7f, 0.7f, 0.7f)/*, _vec3(-1.f,1.f,0.f) */);
		}
	}

	//_matrix PlayerWorld;
	//PlayerWorld = *(m_pTransformCom->Get_WorldMatrix());


	//Engine::Render_Font3D(L"Font_Jinji", L"12",&_vec2(0.f,0.f), PlayerWorld, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

}

_bool CPlayer::Get_AttackCheck()
{
	return m_bAttackCheck;
}

_uint CPlayer::Get_AttackStack()
{
	return m_iAttackStack;
}

_bool CPlayer::Get_Dash()
{
	return m_bDash;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*	pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CPlayer::Add_Component(void)
{

	Engine::CComponent*		pComponent = nullptr;

	/*pCamera->*/
	pComponent = m_pSphere = dynamic_cast<Engine::CSphere*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Proto_Buffer_Sphere"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Sphere", pComponent);


	// CDynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Player"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);


	pComponent = m_pNaviMeshCom = CNaviMgr::GetInstance()->Get_NaviMesh(RESOURCE_STAGE, NAVI_NORMAL);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Navi", pComponent);


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

	return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	CManagement*		pManagemenet = CManagement::GetInstance();

	if (nullptr == pManagemenet)
		return;


	if (m_tINFOPLAYER.fSp >= 100.f)
	{
		m_tINFOPLAYER.fSp = 100.f;
	}
	if (m_tINFOPLAYER.iHP >= 100.f)
	{
		m_tINFOPLAYER.iHP = 100.f;
	}
	

	_uint iIndex = m_pMeshCom->Get_AniIndex();

	_vec3 vUp = { 0.f, 1.f, 0.f };
	_vec3 vPlayerTemp = { 0.f, 0.f, 0.f };

	_vec3 vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	vAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vCameraLook, &vPlayerLook)));


	if (Engine::Get_DIKeyState(DIK_SPACE) & 0x8000 && m_tINFOPLAYER.fSp >= 20.f && !m_bDash)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::PLAYER_EFFECT);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Attack_Long_Voice_0.wav", CSoundMgr::CHANNELID::PLAYER_EFFECT);
		m_bDash = true;
		m_tINFOPLAYER.fSp -= 20.f;

	}
	if (!m_bDash)
	{
		m_tINFOPLAYER.fSp += 0.01f;
	}


	if (!m_bAttackCheck) //공격중이 아닐때에만. 이동가능.
	{

		if (Engine::Key_Down(DIK_R) && !m_bDash)
		{
			CHPUI*		pHPUI = dynamic_cast<CHPUI*>(pManagemenet->Get_GameObject(L"UI", L"CHPUI"));
			_uint		iTemp; //사용되는 아이템의 갯수를 -1 해주기 위해
			UIID		eTemp; //사용되는 아이템의 타입을 알기 위해.


			eTemp = pHPUI->Get_ItemType();

			if (eTemp == UIID::UIID_HP_POTION)
			{
				CGameObject*		pHPObj = dynamic_cast<CHP_Potion*>(pManagemenet->Get_GameObject(L"GameLogic", L"HP_Potion"));

				_uint		iTemp; //사용되는 아이템의 갯수를 -1 해주기 위해
				iTemp = pHPObj->Get_PotionCount();

				if (iTemp <= 0 || m_tINFOPLAYER.iHP >= 100)
				{
					return;
				}
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::PLAYER);
				CSoundMgr::Get_Instance()->PlaySoundW(L"Drink_Potion.mp3", CSoundMgr::CHANNELID::PLAYER);
				iTemp--;
				pHPObj->Set_PotionCount(iTemp);

				m_tINFOPLAYER.iHP += 20.f;
			}
			else if (eTemp == UIID::UIID_SP_POTION)
			{
				CGameObject*		pSPObj = dynamic_cast<CSP_Potion*>(pManagemenet->Get_GameObject(L"GameLogic", L"SP_Potion"));

				iTemp = pSPObj->Get_PotionCount();

				if (iTemp <= 0 || m_tINFOPLAYER.fSp >= 100.f)
				{
					return;
				}
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::CHANNELID::PLAYER);
				CSoundMgr::Get_Instance()->PlaySoundW(L"Drink_Potion.mp3", CSoundMgr::CHANNELID::PLAYER);
				iTemp--;
				pSPObj->Set_PotionCount(iTemp);

				m_tINFOPLAYER.fSp += 20.f;
			}


			//if (eTemp == UIID::UIID_HP_POTION && m_tINFOPLAYER.iHP < 100.f)
			//{
			//	iTemp = pHPUI->Get_ItemCount();
			//	iTemp--;
			//	pHPUI->Set_ItemCount(iTemp);

			//	m_tINFOPLAYER.iHP += 5.f;
			//}
			//else
			//{

			//}




		}

		if (Engine::Get_DIKeyState(DIK_W) & 0x8000)
		{
			if(true == m_bBuffOn)
				WindEffectCreate();
		/*	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);*/
			CSoundMgr::Get_Instance()->PlaySoundW(L"Step.mp3",CSoundMgr::PLAYER);
			iKeySelect = DIK_W;
	
			if (!m_bDash)
			{
				m_pMeshCom->Set_Animationset(18);
			}

		
			if (Engine::Get_DIKeyState(DIK_D) & 0x8000)
			{

				iKeySelect = DIK_W + DIK_D;
				_vec3 vRightMove = vRightLook;
				vRightMove.y = 0.f;
				m_pTransformCom->Move_Pos(&vRightMove, m_fMoveSpeed, fTimeDelta);

				vPos = m_pNaviMeshCom->Move_OnNaviMesh(&vPos, &(vRightMove * fTimeDelta * m_fMoveSpeed));
				m_pTransformCom->Set_Pos(vPos);
			}
			else if (Engine::Get_DIKeyState(DIK_A) & 0x8000)
			{
				iKeySelect = DIK_W + DIK_A;

				_vec3 LeftMove = vLeftLook;
				LeftMove.y = 0.f;
				m_pTransformCom->Move_Pos(&LeftMove, m_fMoveSpeed, fTimeDelta);

				vPos = m_pNaviMeshCom->Move_OnNaviMesh(&vPos, &(LeftMove * fTimeDelta * m_fMoveSpeed));
				m_pTransformCom->Set_Pos(vPos);
			}
			else
			{
				_vec3 vCameraLookMove = vCameraLook;
				vCameraLookMove.y = 0.f;
				/*D3DXVec3Normalize(&vCameraLook, &vCameraLook);*/
				m_pTransformCom->Move_Pos(&vCameraLookMove, m_fMoveSpeed, fTimeDelta);

				vPos = m_pNaviMeshCom->Move_OnNaviMesh(&vPos, &(vCameraLookMove * fTimeDelta * m_fMoveSpeed));
				m_pTransformCom->Set_Pos(vPos);
			}

		}
		else if (Engine::Get_DIKeyState(DIK_S) & 0x8000)
		{

			iKeySelect = DIK_S;
			if (true == m_bBuffOn)
				WindEffectCreate();
			CSoundMgr::Get_Instance()->PlaySoundW(L"Step.mp3", CSoundMgr::PLAYER);

			if (!m_bDash)
			{
				m_pMeshCom->Set_Animationset(18);
			}
			if (Engine::Get_DIKeyState(DIK_D) & 0x8000)
			{
				iKeySelect = DIK_S + DIK_D;
				_vec3 RightBack = vRightBack;
				RightBack.y = 0.f;
				m_pTransformCom->Move_Pos(&RightBack, m_fMoveSpeed, fTimeDelta);

				vPos = m_pNaviMeshCom->Move_OnNaviMesh(&vPos, &(RightBack * fTimeDelta * m_fMoveSpeed));
				m_pTransformCom->Set_Pos(vPos);
			}
			else if (Engine::Get_DIKeyState(DIK_A) & 0x8000)
			{
				iKeySelect = DIK_S + DIK_A;
				_vec3 LeftBack = vLeftBack;
				LeftBack.y = 0.f;
				m_pTransformCom->Move_Pos(&LeftBack, m_fMoveSpeed, fTimeDelta);


				vPos = m_pNaviMeshCom->Move_OnNaviMesh(&vPos, &(LeftBack * fTimeDelta * m_fMoveSpeed));
				m_pTransformCom->Set_Pos(vPos);
			}
			else
			{
				D3DXVec3Normalize(&vCameraLook, &vCameraLook);
				m_pTransformCom->Move_Pos(&vPlayerLook, m_fMoveSpeed, fTimeDelta);


				vPos = m_pNaviMeshCom->Move_OnNaviMesh(&vPos, &(vPlayerLook * fTimeDelta * m_fMoveSpeed));
				m_pTransformCom->Set_Pos(vPos);
			}


		}
		else if (Engine::Get_DIKeyState(DIK_A) & 0x8000)
		{
		
			iKeySelect = DIK_A;
			if (true == m_bBuffOn)
				WindEffectCreate();
			CSoundMgr::Get_Instance()->PlaySoundW(L"Step.mp3", CSoundMgr::PLAYER);
			if (!m_bDash)
			{
				m_pMeshCom->Set_Animationset(18);
			}
			m_pTransformCom->Move_Pos(&vCameraRight, m_fMoveSpeed, fTimeDelta);

			vPos = m_pNaviMeshCom->Move_OnNaviMesh(&vPos, &(vCameraRight * fTimeDelta * m_fMoveSpeed));
			m_pTransformCom->Set_Pos(vPos);
		}

		else if (Engine::Get_DIKeyState(DIK_D) & 0x8000)
		{

			iKeySelect = DIK_D;
			if (true == m_bBuffOn)
				WindEffectCreate();
			CSoundMgr::Get_Instance()->PlaySoundW(L"Step.mp3", CSoundMgr::PLAYER);
			if (!m_bDash)
			{
				m_pMeshCom->Set_Animationset(18);
			}

			if (Engine::Get_DIKeyState(DIK_W) & 0x8000)
			{

				/*iKeySelect = DIK_W + DIK_D;*/
				_vec3 vLightLook;
				vLightLook = vCameraRight + vPlayerLook;
				D3DXVec3Normalize(&vLightLook, &vLightLook);
				m_pTransformCom->Move_Pos(&vLightLook, m_fMoveSpeed, fTimeDelta);

				vPos = m_pNaviMeshCom->Move_OnNaviMesh(&vPos, &(vLightLook * fTimeDelta * m_fMoveSpeed));
				m_pTransformCom->Set_Pos(vPos);
			}
			else if (Engine::Get_DIKeyState(DIK_S) & 0x8000)
			{
				/*iKeySelect = DIK_S + DIK_D;*/

				_vec3 vLightBack;
				vLightBack = vCameraRight + vPlayerLook;
				D3DXVec3Normalize(&vLightBack, &vLightBack);
				m_pTransformCom->Move_Pos(&vLightBack, m_fMoveSpeed, fTimeDelta);

				vPos = m_pNaviMeshCom->Move_OnNaviMesh(&vPos, &(vLightBack * fTimeDelta * m_fMoveSpeed));
				m_pTransformCom->Set_Pos(vPos);
			}
			else
			{
				D3DXVec3Normalize(&vCameraLook, &vCameraLook);
				m_pTransformCom->Move_Pos(&vCameraRight, m_fMoveSpeed, fTimeDelta);

				vPos = m_pNaviMeshCom->Move_OnNaviMesh(&vPos, &(vCameraRight * fTimeDelta * m_fMoveSpeed));
				m_pTransformCom->Set_Pos(vPos);
			}

		}
		else 
		{
			if(!m_bDash)
				m_pMeshCom->Set_Animationset(16);

		}
	

	}

	if (true == m_pMeshCom->Is_AnimationSetEnd())
	{
		/*m_pMeshCom->Set_Animationset(16);*/
		m_bAttackCheck = false;
		m_iAniSet = 0;
		m_iAttackStack = 0;
		m_iTimeDelta = 0;
		m_iChargeAttackStack = 0;

	}
}

void CPlayer::MouseMove(const _float & fTimeDelta)
{
	CManagement*		pManagement = CManagement::GetInstance();

	CCamera*		pCamera = dynamic_cast<CCamera*>(pManagement->Get_GameObject(L"Environment", L"DynamicCamera"));

}

void CPlayer::Attack(const _float & fTimeDelta)
{
	m_iCurrentAttack = m_iAttackStack;
	m_iCurrentCharge = m_iChargeAttackStack;

	_uint AniIndex = m_pMeshCom->Get_AniIndex();

	if (AniIndex != 32)
	{
		m_bAttackSound32 = false;
	}

	if (AniIndex != 31)
	{
		m_bAttackSound31 = false;
	}

	if (AniIndex != 30)
	{
		m_bAttackSound30 = false;
	}


	if (m_bAttackCheck)
	{
		m_iTimeDelta += fTimeDelta;
	}
	
	if (AniIndex == 32 && false == m_bAttackSound32)
	{

		m_fTimeDeltaSound += fTimeDelta;

		if (m_fTimeDeltaSound >= 1.7f)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
			CSoundMgr::Get_Instance()->PlaySoundW(L"Attack_Voice_0.wav", CSoundMgr::PLAYER);
			m_fTimeDeltaSound = 0.f;
			m_bAttackSound32 = true;
		}
	}
	else if (AniIndex == 31 && false == m_bAttackSound31)
	{
		m_fTimeDeltaSound += fTimeDelta;

		if (m_fTimeDeltaSound >= 1.7f)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
			CSoundMgr::Get_Instance()->PlaySoundW(L"Attack_Voice_1.wav", CSoundMgr::PLAYER);
			m_fTimeDeltaSound = 0.f;
			m_bAttackSound31 = true;
		}
	}
	else if (AniIndex == 30 && false == m_bAttackSound30)
	{
		m_fTimeDeltaSound += fTimeDelta;

		if (m_fTimeDeltaSound >= 0.5f)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
			CSoundMgr::Get_Instance()->PlaySoundW(L"Attack_Long_Voice_3.wav", CSoundMgr::PLAYER);
			m_fTimeDeltaSound = 0.f;
			m_bAttackSound30 = true;
		}

	}

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_bAttackCheck = true;
		
		switch (m_iAttackStack)
		{
		case 0:
			m_pMeshCom->Set_Animationset(32);
			break;
		case 1:
			m_pMeshCom->Set_Animationset(31);
			break;
		case 2:
			m_pMeshCom->Set_Animationset(30);
			break;
		}

		if (m_iTimeDelta >= 1.7f)
		{

			m_iAttackStack++;
			if (m_iCurrentAttack < m_iAttackStack)
			{
				m_iTimeDelta = 0.f;
			}
		}
	}



}

void CPlayer::Dash(const _float & fTimeDelta)
{
	if (m_bDash)
	{
		m_bAttackCheck = false; //공격중이면 공격체크 풀기
		m_iAttackStack = 0;
		m_iTimeDelta = 0.f;
		m_pMeshCom->Set_Animationset(22);
		if (m_pMeshCom->Is_AnimationSetEnd())
		{
			m_bDash = false;
		}
	}

	/*if(GetKeyt)*/
}

void CPlayer::Turn(_uint iKey)
{

	CManagement*		pManagement = CManagement::GetInstance();
	CCamera*		pCamera = dynamic_cast<CCamera*>(pManagement->Get_GameObject(L"Environment", L"DynamicCamera"));

	if (iKey == DIK_W) 
	{

		
		vCameraLook.y = 0.f;
		D3DXVec3Normalize(&vCameraLook, &vCameraLook);
		
		vAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vCameraLook, &vPlayerLook)));

		if (vAngle <= D3DXToDegree(fTurnSpeed))
		{
			iKeySelect = 0;

			return;
		}

		m_fRAngle = D3DXVec3Dot(&vCameraLook, &vPlayerRight);
		m_fLAngle = D3DXVec3Dot(&vCameraLook, &vPlayerLeft);

		if (m_fRAngle > 0 && m_fLAngle < 0.f)
		{
			m_bTurnAngle = true; //
		}
		else if (m_fRAngle < 0 && m_fLAngle > 0.f)
		{
			m_bTurnAngle = false;
		}

		//S키 플레이어와 카메라 룩방향의 역방향과 같음.
		if (m_bTurnAngle)
		{
			m_pTransformCom->Rotation(Engine::ROT_Y, fTurnSpeed);
		}
		else if (!m_bTurnAngle)
		{
			m_pTransformCom->Rotation(Engine::ROT_Y, -fTurnSpeed);
		}

	}
	///////////////////////////////////////////////////////
	if (iKey == DIK_S)
	{
		vCameraLook.y = 0.f;
		D3DXVec3Normalize(&vCameraLook, &vCameraLook);

		vCameraLook = vCameraLook * - 1.f;
		vAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vCameraLook, &vPlayerLook)));

		if (vAngle <= D3DXToDegree(fTurnSpeed))
		{
			iKeySelect = 0;
			return;
		}

		m_fRAngle = D3DXVec3Dot(&vCameraLook, &vPlayerRight);
		m_fLAngle = D3DXVec3Dot(&vCameraLook, &vPlayerLeft);

		if (m_fRAngle > 0 && m_fLAngle < 0.f)
		{
			m_bTurnAngle = true; //
		}
		else if (m_fRAngle < 0 && m_fLAngle > 0.f)
		{
			m_bTurnAngle = false;
		}

		//W키 플레이어와 카메라 룩방향이 같음.
		if (m_bTurnAngle)
		{
			m_pTransformCom->Rotation(Engine::ROT_Y, fTurnSpeed);
		}
		else if (!m_bTurnAngle)
		{
			m_pTransformCom->Rotation(Engine::ROT_Y, -fTurnSpeed);
		}

	}

	if (iKey == DIK_D)
	{
		vCameraRight.y = 0.f;
		D3DXVec3Normalize(&vCameraRight, &vCameraRight);

		vAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vPlayerLook, &vCameraRight)));

		if (vAngle <= D3DXToDegree(fTurnSpeed))
		{
			iKeySelect = 0;
			return;
		}


		m_fRAngle = D3DXVec3Dot(&vCameraLook, &vPlayerLook);
		m_fLAngle = D3DXVec3Dot(&vCameraLook, &vPlayerBack);

		if (m_fRAngle > 0 && m_fLAngle < 0.f)
		{
			m_bTurnAngle = true; //
		}
		else if (m_fRAngle < 0 && m_fLAngle > 0.f)
		{
			m_bTurnAngle = false;
		}
		else if (m_fRAngle == 0 && m_fLAngle == 0)
		{
			m_bTurnAngle = false;
		}

		//D키 카메라의 라이트벡터방향으로 가면됨.
		if (m_bTurnAngle)
		{
			m_pTransformCom->Rotation(Engine::ROT_Y, fTurnSpeed);
		}
		else if (!m_bTurnAngle)
		{
			m_pTransformCom->Rotation(Engine::ROT_Y, -fTurnSpeed);
		}

	}

	if (iKey == DIK_A)
	{

		vCameraRight.y = 0.f;
		D3DXVec3Normalize(&vCameraRight, &vCameraRight);
		//_vec3 vUp = { 0.f, 1.f, 0.f };
		//_vec3 vRight = { 0.f, 1.f, 0.f };
		//D3DXVec3Cross(&vRight, &vCameraLook, &vUp);
		/*vCameraRight = vCameraRight * 1.f;*/
		vCameraRight = vCameraRight * -1.f;
		vAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vPlayerLook, &vCameraRight)));

		if (vAngle <= D3DXToDegree(fTurnSpeed))
		{
			iKeySelect = 0;
			return;
		}


		m_fRAngle = D3DXVec3Dot(&vCameraLook, &vPlayerLook);
		m_fLAngle = D3DXVec3Dot(&vCameraLook, &vPlayerBack);

		if (m_fRAngle > 0 && m_fLAngle < 0.f)
		{
			m_bTurnAngle = false; //
		}
		else if (m_fRAngle < 0 && m_fLAngle > 0.f)
		{
			m_bTurnAngle = true;
		}
		else if (m_fRAngle == 0 && m_fLAngle == 0)
		{
			m_bTurnAngle = false;
		}

		//D키 카메라의 라이트벡터방향으로 가면됨.
		if (m_bTurnAngle)
		{
			m_pTransformCom->Rotation(Engine::ROT_Y, fTurnSpeed);
		}
		else if (!m_bTurnAngle)
		{
			m_pTransformCom->Rotation(Engine::ROT_Y, -fTurnSpeed);
		}

	}

	if (iKey == DIK_W + DIK_A)
	{

		vLeftLook.y = 0.f;
		D3DXVec3Normalize(&vLeftLook, &vLeftLook);

		vAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vPlayerLook, &vLeftLook)));
		vAngle = vAngle * -1.f;
	
		if (vAngle >= D3DXToDegree(fTurnSpeed * -1.f))
		{
			iKeySelect = 0;
			return;
		}

		m_fRAngle = D3DXVec3Dot(&vLeftLook, &vPlayerRight);
		m_fLAngle = D3DXVec3Dot(&vLeftLook, &vPlayerLeft);

		if (m_fRAngle < 0 && m_fLAngle > 0.f)
		{
			m_bTurnAngle = true; //
		}
		else if (m_fRAngle > 0 && m_fLAngle < 0.f)
		{
			m_bTurnAngle = false;
		}

		//S키 플레이어와 카메라 룩방향의 역방향과 같음.
		if (m_bTurnAngle)
		{
			m_pTransformCom->Rotation(Engine::ROT_Y, -fTurnSpeed);
		}
		else if (!m_bTurnAngle)
		{
			m_pTransformCom->Rotation(Engine::ROT_Y, fTurnSpeed);
		}
		
	}

	if (iKey == DIK_W + DIK_D)
	{
		vRightLook.y = 0.f;
		D3DXVec3Normalize(&vRightLook, &vRightLook);

		vAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vPlayerLook, &vRightLook)));

		if (vAngle <= D3DXToDegree(fTurnSpeed))
		{
			iKeySelect = 0;
			return;
		}

		m_fRAngle = D3DXVec3Dot(&vRightLook, &vPlayerRight);
		m_fLAngle = D3DXVec3Dot(&vRightLook, &vPlayerLeft);

		if (m_fRAngle > 0 && m_fLAngle < 0.f)
		{
			m_bTurnAngle = true; //
		}
		else if (m_fRAngle < 0 && m_fLAngle > 0.f)
		{
			m_bTurnAngle = false;
		}

		//S키 플레이어와 카메라 룩방향의 역방향과 같음.
		if (m_bTurnAngle)
		{
			m_pTransformCom->Rotation(Engine::ROT_Y, fTurnSpeed);
		}
		else if (!m_bTurnAngle)
		{
			m_pTransformCom->Rotation(Engine::ROT_Y, -fTurnSpeed);
		}
	}

	if (iKey == DIK_S + DIK_D)
	{
		vRightBack.y = 0.f;
		D3DXVec3Normalize(&vRightBack, &vRightBack);

		vAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vPlayerLook, &vRightBack)));

		if (vAngle <= D3DXToDegree(fTurnSpeed))
		{
			iKeySelect = 0;
			return;
		}

		m_fRAngle = D3DXVec3Dot(&vRightBack, &vPlayerRight);
		m_fLAngle = D3DXVec3Dot(&vRightBack, &vPlayerLeft);

		if (m_fRAngle > 0 && m_fLAngle < 0.f)
		{
			m_bTurnAngle = true; //
		}
		else if (m_fRAngle < 0 && m_fLAngle > 0.f)
		{
			m_bTurnAngle = false;
		}

		//S키 플레이어와 카메라 룩방향의 역방향과 같음.
		if (m_bTurnAngle)
		{
			m_pTransformCom->Rotation(Engine::ROT_Y, fTurnSpeed);
		}
		else if (!m_bTurnAngle)
		{
			m_pTransformCom->Rotation(Engine::ROT_Y, -fTurnSpeed);
		}
	}

	if (iKey == DIK_S + DIK_A)
	{
		vLeftBack.y = 0.f;
		D3DXVec3Normalize(&vLeftBack, &vLeftBack);

		vAngle = D3DXToDegree(acosf(D3DXVec3Dot(&vPlayerLook, &vLeftBack)));
		vAngle * -1.f;
		if (vAngle <= D3DXToDegree(fTurnSpeed))
		{
			iKeySelect = 0;
			return;
		}

		
		m_fRAngle = D3DXVec3Dot(&vLeftBack, &vPlayerRight);
		m_fLAngle = D3DXVec3Dot(&vLeftBack, &vPlayerLeft);

		if (m_fRAngle > 0 && m_fLAngle < 0.f)
		{
			m_bTurnAngle = true; //
		}
		else if (m_fRAngle < 0 && m_fLAngle > 0.f)
		{
			m_bTurnAngle = false;
		}

		//S키 플레이어와 카메라 룩방향의 역방향과 같음.
		if (m_bTurnAngle)
		{
			m_pTransformCom->Rotation(Engine::ROT_Y, fTurnSpeed);
		}
		else if (!m_bTurnAngle)
		{
			m_pTransformCom->Rotation(Engine::ROT_Y, -fTurnSpeed);
		}
	}
}

void CPlayer::Default()
{
	CManagement*		pManagement = CManagement::GetInstance();
	CCamera*		pCamera = dynamic_cast<CCamera*>(pManagement->Get_GameObject(L"Environment", L"DynamicCamera"));
	_matrix			pCameraWorld;

	pCamera->Get_View(&pCameraWorld);
	D3DXMatrixInverse(&pCameraWorld, NULL, &pCameraWorld);

	memcpy(&vCameraRight, &pCameraWorld.m[0][0], sizeof(_vec3));
	memcpy(&vCameraUp, &pCameraWorld.m[1][0], sizeof(_vec3));

	D3DXVec3Normalize(&vCameraRight, &vCameraRight);
	D3DXVec3Normalize(&vCameraUp, &vCameraUp);

	pCamera->Get_EyeAtUp(&vCameraEye, &vCameraAt, &vCameraUp);

	

	vCameraLook = vCameraAt - vCameraEye;
	vCameraPos = vCameraEye;
	m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vPlayerPos);

	vDir = vCameraPos - vPlayerPos;


	_vec3 vUp = { 0.f, 1.f, 0.f };
	_vec3 vPlayerTemp = { 0.f, 0.f, 0.f };

	m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vPlayerLook);
	m_pTransformCom->Get_Info(Engine::INFO_RIGHT, &vPlayerRight);

	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
	D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);
	D3DXVec3Normalize(&vDir, &vDir);

	vPlayerLeft = vPlayerRight * -1.f;
	vCameraLeft = vCameraRight * -1.f;
	vPlayerBack = vPlayerLook * -1.f;
	vCameraBack = vCameraLook * -1.f;

	
	D3DXVec3Normalize(&vCameraBack, &vCameraBack);
	D3DXVec3Normalize(&vCameraLook, &vCameraLook);
	D3DXVec3Normalize(&vCameraLeft, &vCameraLeft);
	vRightLook = vCameraRight + vCameraLook;
	D3DXVec3Normalize(&vRightLook, &vRightLook);

	vLeftLook = vCameraLeft + vCameraLook;
	D3DXVec3Normalize(&vLeftLook, &vLeftLook);

	vRightBack = vCameraRight + vCameraBack;
	D3DXVec3Normalize(&vRightBack, &vRightBack);

	vLeftBack = vCameraLeft + vCameraBack;
	D3DXVec3Normalize(&vLeftBack, &vLeftBack);

	//vLeftLook = vPlayerLeft + vPlayerLook;
	//D3DXVec3Normalize(&vLeftLook, &vLeftLook);

	Engine::CLayer*			pLayer = nullptr;

	pLayer = pManagement->Get_Layer(L"GameLogic");
	NULL_CHECK_RETURN(pLayer,);

	CGameObject* pObj = pLayer->Get_GameObject(L"Aura_Buff");

	if (nullptr == pObj)
	{
		m_fMoveSpeed = 5.f;
		m_bBuffOn = false;
	}

	if (nullptr != pObj)
	{
		m_tINFOPLAYER.iHP += 0.001f;
		m_fMoveSpeed = 10.f;
		m_bBuffOn = true;
	}

	
}

void CPlayer::SetUp_Sphere(void)
{

	if (nullptr == m_pParentBoneMatrix_Sphere[PLAYER_00])
	{
		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("BD2D00");

		m_pParentBoneMatrix_Sphere[PLAYER_00] = &pFrame->CombinedTranformationMatrix;
		m_pParentWorldMatrix_Sphere = m_pTransformCom->Get_WorldMatrix();
	}
	m_pFinalMatrix_Sphere[PLAYER_00] = *m_pParentBoneMatrix_Sphere[PLAYER_00] * *m_pParentWorldMatrix_Sphere;


	if (!m_listCollision.empty())
	{
		m_listCollision.clear();
	}
	_vec3 vSpherePos;
	vSpherePos = { m_pFinalMatrix_Sphere[PLAYER_00]._41, m_pFinalMatrix_Sphere[PLAYER_00]._42, m_pFinalMatrix_Sphere[PLAYER_00]._43 };

	/*_float vRadius = 1.5f;*/
	_float vRadius = 3.f;
	m_listCollision.push_back(make_pair(vSpherePos, vRadius));
}

void CPlayer::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	Engine::CTerrainTex*	pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>
		(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Buffer", Engine::ID_STATIC));
		
	_float fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos,
																pTerrainBufferCom->Get_VtxPos(),
																pTerrainBufferCom->Get_VtxCntX(),
																pTerrainBufferCom->Get_VtxCntZ(), 1);

	m_pTransformCom->Set_Pos(vPos.x, fHeight,vPos.z);
}

Engine::_vec3 CPlayer::PickUp_OnTerrain(void)
{
	Engine::CTerrainTex*	pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Buffer", Engine::ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3(0.f, 0.f, 0.f));

	Engine::CTransform*		pTerrainTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransformCom, _vec3(0.f, 0.f, 0.f));

	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);

}

void CPlayer::Inventory()
{
	CManagement*	pManagement = CManagement::GetInstance();

	CTransform*		pIruTransform = dynamic_cast<CTransform*>(pManagement->Get_Component(L"GameLogic", L"Iru", L"Com_Transform",COMPONENTID::ID_DYNAMIC));

	_vec3 vDir;
	_vec3 vPlayerPos;
	_vec3 vIruPos;

	_float fDir;

	m_pTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	pIruTransform->Get_Info(INFO_POS, &vIruPos);
	vDir = vPlayerPos - vIruPos;
	fDir = D3DXVec3Length(&vDir);


	if (false == m_bInventory && fDir <= 2.f && Engine::Key_Down(DIK_F))
	{

		m_bInventory = true;
	
	}
	else if (true == m_bInventory && fDir <= 2.f && Engine::Key_Down(DIK_F))
	{
		m_bInventory = false;
	}
	else if (fDir > 2.f)
	{
		m_bInventory = false;
	}

	
}

HRESULT CPlayer::WindEffectCreate()
{

	CManagement*			pManagement = CManagement::GetInstance();

	Engine::CLayer*			pLayer = nullptr;

	pLayer = pManagement->Get_Layer(L"GameLogic");
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pObj = pLayer->Get_GameObject(L"PlayerWind");

	if (nullptr == pObj)
	{
		Engine::CGameObject*		pGameObject = nullptr;

		pGameObject = CWind::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlayerWind", pGameObject), E_FAIL);
	}

}

void CPlayer::FallDown(const _float & fTimeDelta)
{
	m_pMeshCom->Set_Animationset(11);
	
}

void CPlayer::PlayAni(const _float & fTimeDelta)
{


	_uint iAniindex = m_pMeshCom->Get_AniIndex();

	if (iAniindex == 22)//22이면 대쉬
	{
		m_pMeshCom->Play_Animationset(fTimeDelta * 2);
	}
	else if (m_bFallDown && iAniindex == 11) // FallDown
	{
		m_pMeshCom->Play_Animationset(fTimeDelta / 2);
	}
	else
	{
		m_pMeshCom->Play_Animationset(fTimeDelta);
	}
}

