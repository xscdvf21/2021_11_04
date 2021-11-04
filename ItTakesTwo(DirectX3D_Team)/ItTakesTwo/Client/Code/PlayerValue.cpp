#include "stdafx.h"
#include "PlayerValue.h"


CPlayerValue::CPlayerValue(LPDIRECT3DDEVICE9 pDevice)
	:CGameObject(pDevice)
{
}


CPlayerValue::~CPlayerValue()
{
}

void CPlayerValue::Clear_Jump()
{
	ZeroMemory(&m_bJump, sizeof(m_bJump));
	memset(m_bPlaySound + PLAYER_SOUND::PLAY_JUMP, 0, sizeof(_bool) * (PLAY_JUMP_END + 1 - PLAY_JUMP));

	m_bJumpDashStart = false;
	m_bJumpDash = false; // 점프대쉬중엔 방향키 이동을 못하게 막는변수
	m_fJumpTime = 0.0f;
	m_bStopGravity = false;
	m_bDashStart = false;
	m_bEventJumpStart = false;
	//m_fJumpPower = 1.3f;
}

void CPlayerValue::Clear_Ride()
{
	m_bRideJumpMax = false;
	m_bTakeOff = false;
	m_bRideStart = false;
	m_bRideEnd = false;
	m_bForceRideOff = false;
}

void CPlayerValue::Clear_Sit()
{
	memset(m_bPlaySound + PLAYER_SOUND::PLAY_SIT, 0, sizeof(_bool) * (PLAY_SIT_END + 1 - PLAY_SIT));
}

void CPlayerValue::Clear_PickUp()
{
	m_bPickUpEnd = false;
	m_bPickUpStart = false;
	m_fCurMoveTime = 0.0f;
	m_bPickUp = false;
	m_bInsert = false;
	m_bInteractable = false;
	m_bInitInsertFuse = false;
	m_bInsert_Fuse = false;
}

void CPlayerValue::Clear_Lever()
{
	m_bLever = false;
	m_bLeverStart = false;
	m_bLeverMove = false;
	m_bLeverRot = false;
	m_bLeverArriveRot = false;
}

void CPlayerValue::Clear_Float()
{
	m_bFloat = false;
	m_bFloatUp = false;
	m_bFloatStart = false;
	m_fFloatHeight = 0.0f;
	m_fOrgHeight = 1000.0f;
}

void CPlayerValue::Clear_Dead()
{
	m_fGravityTime = 0.0f;
	m_bPhys = true;
	m_bStopGravity = false;
	m_bRespawn = false;
	m_bSlideEnd = false;
	m_bPlayerDead = false;
	m_bDeadStart = false;
	m_bClimbingJump = false;
	m_bRideGround = false;
	m_fCurDeadTime = 0.0f;
	Clear_Jump();
	Clear_Ride();
	Clear_PickUp();
	Clear_Lever();
	Clear_Float();
	Clear_Move();
	Clear_Fall();
	Clear_Hang();
	Clear_Dash();
	Clear_SawShoot();
}

void CPlayerValue::Clear_Move()
{
	m_fWalkSoundTime = 0.0f;
	m_fCurMoveTime = 0.0f;
	memset(m_bPlaySound + PLAYER_SOUND::PLAY_MOVE, 0, sizeof(_bool) * (PLAY_MOVE_END + 1 - PLAY_MOVE));
}

void CPlayerValue::Clear_Fall()
{
	m_bFallStart = false;
}

void CPlayerValue::Clear_Hang()
{
	m_bHang = false;
	m_bEndRise = false;
}

void CPlayerValue::Clear_Dash()
{
	ZeroMemory(&m_bDash, sizeof(m_bDash));
	memset(m_bPlaySound + PLAYER_SOUND::PLAY_DASH, 0, sizeof(_bool) * (PLAY_DASH_END + 1 - PLAY_DASH));
	m_bDashStart = false;
}

void CPlayerValue::Clear_Climbing()
{
	m_bClimbingFall = false;
	m_bAttach = false;
	m_bStopGravity = false;
	m_bClimbingJump = false;
	m_bClimbingStart = false;
	m_bHang = false;
	m_bEventJumpStart = false;
}

void CPlayerValue::Clear_SawShoot()
{
	m_bFly = false;
	m_bFlyStart = false;
	m_bFlyMove = false;
	m_bLanch = false;
}

void CPlayerValue::Clear_SawSliding()
{
	m_bSlidingStart = false;
	m_bSawJumpStart = false;
	m_bSawJumpTrigger = false;
	m_fSawPosZ = 0.0f;
	m_bStopGravity = false;
}

void CPlayerValue::Clear_Missile()
{
	m_bRideEnd = false;
	m_bMoveTo = false;
	m_fFlyTime = 0.0f;
	m_bStopGravity = false;
	m_bRideStart = false;
	m_fCurJumpToTime = 0.0f;
	m_bRide = false;

}

void CPlayerValue::Clear(PLAYER_STATE eState)
{
	switch (eState)
	{
	case CPlayerValue::PL_IDLE:
		break;
	case CPlayerValue::PL_MOVE:
		Clear_Move();
		break;
	case CPlayerValue::PL_JUMP:
		Clear_Jump();
		break;
	case CPlayerValue::PL_SIT:
		Clear_Sit();
		break;
	case CPlayerValue::PL_DASH:
		Clear_Dash();
		break;
	case CPlayerValue::PL_RUN:
		break;
	case CPlayerValue::PL_CLIMBING:
		Clear_Climbing();
		break;
	case CPlayerValue::PL_HANG:
		Clear_Hang();
		break;
	case CPlayerValue::PL_RIDE:
		Clear_Ride();
		break;
	case CPlayerValue::PL_SLIDE:
		break;
	case CPlayerValue::PL_PICKUP:
		Clear_PickUp();
		break;
	case CPlayerValue::PL_DEAD:
		Clear_Dead();
		break;
	case CPlayerValue::PL_LEVER:
		Clear_Lever();
		break;
	case CPlayerValue::PL_HANGSWITCH:
		break;
	case CPlayerValue::PL_FLOAT:
		Clear_Float();
		break;
	case CPlayerValue::PL_FALL:
		Clear_Fall();
		break;
	case CPlayerValue::PL_SAW_SHOOT:
		Clear_SawShoot();
		break;
	case CPlayerValue::PL_SAW_SLIDING:
		Clear_SawSliding();
		break;
	case CPlayerValue::PL_MISSILE:
		Clear_Missile();
		break;
	}
}

void CPlayerValue::Set_Ride(_bool bState, CGameObject * pObj)
{
	m_bRide = bState;
	m_pPickUpTarget = pObj;
}

void CPlayerValue::Set_Ride(_bool bState)
{
	m_bRide = bState;
}

void CPlayerValue::Set_EventJump(_float fEventJumpPower)
{
	m_bEventJump = true;
	m_fEventJumpPower = fEventJumpPower;
}

void CPlayerValue::Set_JumpDashStop()
{
	m_bJump[PL_JUMPDASHSTOP] = true;
	m_bStopGravity = false;
	m_bJumpDash = false;
}

void CPlayerValue::Set_Hang(HANGTYPE eType, _bool bHang, _vec3 vGoalPos, _vec3 vNormal)
{
	m_eHangType = eType;
	m_bHangDoor = bHang;
	m_vHangGoalPos = vGoalPos;
	m_vHangNormal = vNormal;
}

void CPlayerValue::Set_PickUp(_bool bPickUp, PICKUP_TYPE ePickUpType, void * pArg)
{
	if (bPickUp)
	{
		switch (ePickUpType)
		{
		case CPlayerValue::FUSE:
			m_pPickUpTarget = (CGameObject*)pArg;
			m_ePickUpType = FUSE;
			m_bPickUp = bPickUp;
			break;
		case CPlayerValue::HOSE:
			m_pPickUpTarget = (CGameObject*)pArg;
			m_ePickUpType = HOSE;
			m_bRide = bPickUp;
			break;
		case CPlayerValue::LEVER:
			m_pPickUpTarget = (CGameObject*)pArg;
			m_ePickUpType = LEVER;
			m_bColLever = bPickUp;
			break;
		}
	}
	else
	{
		switch (ePickUpType)
		{
		case CPlayerValue::FUSE:
			m_ePickUpType = FUSE;
			m_bPickUp = bPickUp;
			//m_pPickUpTarget = nullptr;
			break;
		case CPlayerValue::HOSE:
			m_ePickUpType = HOSE;
			m_bRide = bPickUp;
			// 여기선 nullptr을 넣으면 안?
			// m_pPickUpTarget = nullptr;
			break;
		case CPlayerValue::LEVER:
			m_ePickUpType = LEVER;
			m_bColLever = bPickUp;
			//m_pPickUpTarget = nullptr;
			break;
		}
	}
}

void CPlayerValue::Set_Slide(_bool bHit, const _float & fPower, const _float & fShootDuration, vector<vector<_vec3>*> vecActor,const _float& fSpeed)
{
	m_ePre_State = m_ePL_STATE;
	m_ePL_STATE = PL_SLIDE;

	m_vecSlidePos = vecActor;
	m_bSlideHit = bHit;
	m_bSlideStart = true;
	m_iSlideIndex = 0;

	m_fShootPower = fPower;
	m_fShootDuration = fShootDuration;
	m_fSlideSpeed = fSpeed;

}

void CPlayerValue::Set_Shoot(const _vec3 & vDir, const _float & fPower, const _float & fDuration)
{
	m_bShootState = true;
	m_vShootDir = vDir;
	m_fShootPower = fPower;
	m_fShootTime = 0.f;
	m_bShoot = true;
	Engine::StopNPlaySound(L"Play_Gameplay_Vehicles_VacuumHose.bnk_1.wav", Engine::CSoundMgr::CHANNELID::Vacuum_Out);
}

void CPlayerValue::Set_Float(_bool bFloat, _bool bUp, void * pArg, _float3 vPos,_float fHeight)
{
	m_bFloat = bFloat;
	m_bFloatUp = bUp;
	m_pPickUpTarget = (CGameObject*)pArg;
	m_vFanPos = vPos;
	m_fFlyHeight = fHeight;
	if (false == m_bFloat)
		m_fOrgHeight = 1000.0f;
}

void CPlayerValue::Set_HangSwitch(const _bool & bState, CGameObject* pObj)
{
	m_bHangSwitch = bState;
	m_pPickUpTarget = pObj;
	if (m_bHangSwitch && bState == false)
	{
		m_bJumpToEnd = false;
		m_bPhys = true;
		return;
	}
}

void CPlayerValue::Set_RideTag(const char * tagBoneName, const _tchar* tagMesh, const _tchar* tagTrans, _vec3 vRideAngle)
{
	strcpy_s(m_tagBone, tagBoneName);
	lstrcpy(m_tagMash, tagMesh);
	lstrcpy(m_tagTrans, tagTrans);
	m_vRideAngle = vRideAngle;
}

void CPlayerValue::Set_Push(_bool bPush, CGameObject * pObj, _vec3 vTargetPos, PICKUP_TYPE eType, _vec3 vPushDir, _bool bGrap)
{
	m_bPush = bPush;
	m_vTargetPos = vTargetPos;
	m_pPickUpTarget = pObj;
	m_ePickUpType = eType;
	m_vPushDIr = vPushDir;
	m_bGrap = bGrap;
}

void CPlayerValue::Set_Push(_bool bPush)
{
	m_bPush = bPush;
	m_bGrap = false;
	m_pPickUpTarget = nullptr;
}

void CPlayerValue::Set_Handle(_bool bHandle, wstring tagHandleName)
{
	m_bHandle = bHandle;
	m_tagHandleName = tagHandleName;
}

void CPlayerValue::Set_FanButton(_bool bButton, wstring tagButton)
{
	m_bFanButton = bButton;
	m_tagButton = tagButton;
}

void CPlayerValue::Set_Fly(_bool bFly, wstring tagFlyName)
{
	m_bFly = bFly;
	m_tagHandleName = tagFlyName;
}

void CPlayerValue::Set_FreeGravity(const _bool& bState /*= true*/)
{
	if (bState)
	{
		m_iFreeGravity++;
		m_bFreeGravity = bState;
		m_bStopGravity = true;
	}
	else
	{
		m_iFreeGravity--;
		if (m_iFreeGravity <= 0)
		{
    			m_bFreeGravity = false;
		}
	}

}

void CPlayerValue::Set_OnPlatform(const _bool& bState /*= true*/)
{
	m_bOnPlatform = bState;
}

void CPlayerValue::Fix_ZMove(const _float& fDepth)
{
	m_bFixDepth = true;
	m_fDepth = fDepth;
}

void CPlayerValue::Set_ReadySceneChange()
{
	m_bKeyOff = true;
	m_bRenderOff = true;
	m_bFadeOut = true;
}

