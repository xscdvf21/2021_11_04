#include "stdafx.h"
#include "Cody_Space.h"

#include "Export_Function.h"

void CCody_Space::ChangeAnimation(CCody_Space* pCody, CCody_Space::PLAYER_STATE eCurState, CCody_Space::PLAYER_STATE* ePreState, Engine::CDynamicMesh* pMesh)
{
	switch (eCurState)
	{
	case CCody_Space::PL_IDLE:
		PlayIdle(pMesh, ePreState);
		break;
	case CCody_Space::PL_MOVE:
		PlayWalk(pMesh, ePreState);
		break;
	case CCody_Space::PL_JUMP:
		PlayJump(pCody, pMesh);
		break;
	case CCody_Space::PL_SIT:
		PlaySit(pMesh);
		break;
	case CCody_Space::PL_DASH:
		PlayDash(pCody, pMesh);
		break;
	case CCody_Space::PL_RUN:
		PlayRun(pMesh);
		break;
	case CCody_Space::PL_HANG:
		PlayHang(pCody, pMesh, ePreState);
		break;
	case CCody_Space::PL_SLIDE:
		PlaySlide(pCody, pMesh, ePreState);
		break;
	case CCody_Space::PL_CLIMBING:
		PlayClimbing(pCody, pMesh, ePreState);
		break;
	case CCody_Space::PL_PICKUP:
		PlayPickUp(pCody, pMesh, ePreState);
		break;
	case CCody_Space::PL_RIDE:
		PlayRide(pCody, pMesh, ePreState);
		break;
	case CCody_Space::PL_LEVER:
		PlayLever(pCody, pMesh, ePreState);
		break;
	case CCody_Space::PL_FLOAT:
		PlayFloat(pCody, pMesh, ePreState);
		break;
	case CCody_Space::PL_FALL:
		PlayFall(pCody, pMesh, ePreState);
		break;
	case CCody_Space::PL_HANGSWITCH:
		PlayHangSwitch(pCody, pMesh, ePreState);
		break;
	case CCody_Space::PL_PUSH:
		PlayPush(pCody, pMesh, ePreState);
		break;
	case CCody_Space::PL_HANDLE:
		PlayHandle();
		break;
	case CCody_Space::PL_FREEGRAVITY:
		PlayGravityFly();
		break;
	case CCody_Space::PL_MISSILE:
		PlayMissile();
		break;
	case CCody_Space::PL_HIT:
		PlayHit();
		break;
	}
}

void CCody_Space::PlayAnimation(CCody_Space* pCody, CDynamicMesh* pMesh, const _float& fTimeDelta, CCody_Space::PLAYER_STATE eCurState, CCody_Space::PLAYER_STATE* pPreState)
{
	ChangeAnimation(pCody, eCurState, pPreState, pMesh);
	if (m_bPlayAnmation)
	{
		if (m_iSize == 2)
			pMesh->Play_Animationset(fTimeDelta * 0.8f);
		else
			pMesh->Play_Animationset(fTimeDelta);
	}
}

void CCody_Space::PlayIdle(Engine::CDynamicMesh* pMesh, CCody_Space::PLAYER_STATE* pPreState)
{
	if (*pPreState != CCody_Space::PL_IDLE)
	{
		switch (*pPreState)
		{
		case CCody_Space::PL_JUMP:
		{
			m_b2ndJump = false;
			m_b2ndJumpStart = false;
			m_bJumpDash = false;
			m_bJumpDashStart = false;
			m_bGroundPoundStop = false;
			m_eCodyAnim = Cody_Space_Jump_Land;
			pMesh->Set_Animationset(m_eCodyAnim);
			*pPreState = CCody_Space::PL_IDLE;
			break;
		}
		case CCody_Space::PL_SIT:
		{
			m_eCodyAnim = Cody_Space_Crouch_End;
			pMesh->Set_Animationset(m_eCodyAnim, 0.5f);
			*pPreState = CCody_Space::PL_IDLE;
			break;
		}
		case CCody_Space::PL_DASH:
		{
			if (pMesh->Is_AnimationSetEnd(0.15f))
			{
				m_eCodyAnim = Cody_Space_Idle;
				pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
			}
			*pPreState = CCody_Space::PL_IDLE;
			break;
		}
		case CCody_Space::PL_HANGSWITCH:
		{
			m_eCodyAnim = Cody_Space_Idle;
			pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.0f);
			break;
		}
		case CCody_Space::PL_HANG:
		{
			m_eCodyAnim = Cody_Space_Idle;
			pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.01f);
			break;
		}
		default:
			m_eCodyAnim = Cody_Space_Idle;
			pMesh->Set_Animationset(Cody_Space_Idle);

			// Dash상태 때문에 번거롭게 모든곳에 넣어줌
			*pPreState = CCody_Space::PL_IDLE;
			break;
		}

		return;
	}

	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		m_eCodyAnim = Cody_Space_Idle;
		pMesh->Set_Animationset(Cody_Space_Idle, 1.0f, 0.1f);
	}
}

void CCody_Space::PlayJump(CCody_Space* pCody, Engine::CDynamicMesh* pMesh)
{
	switch (m_eCodyAnim)
	{
	case Cody_Space_Jump:
		Jump(pMesh);
		break;
	case Cody_Space_2ndJump:
		SecondJump(pMesh);
		break;
	case Cody_Space_Jump_Fall:
		Jump_Fall(pMesh);
		break;
	case Cody_Space_Jump_Dash:
		Jump_Dash(pCody, pMesh);
		break;
	case Cody_Space_GroundPound_Start:
		GroundPound_Start(pCody, pMesh);
		break;
	case Cody_Space_GroundPound_Fall:
		GroundPound_Fall(pCody, pMesh);
		break;
	case Cody_Space_GroundPound_Land:
		GroundPound_Land(pMesh);
		break;
	case Cody_Space_GroundPound_End:
		GroundPound_End(pCody, pMesh);
		break;
	}
}

void CCody_Space::PlayWalk(Engine::CDynamicMesh* pMesh, CCody_Space::PLAYER_STATE* pPreState)
{
	if (*pPreState != CCody_Space::PL_MOVE)
	{
		switch (*pPreState)
		{
		case CCody_Space::PL_SIT:
			m_eCodyAnim = Cody_Space_Crouch_Walk_End;
			pMesh->Set_Animationset(m_eCodyAnim);

			*pPreState = CCody_Space::PL_MOVE;
			break;
		case CCody_Space::PL_JUMP:
		{
			m_b2ndJump = false;
			m_b2ndJumpStart = false;
			m_bJumpDash = false;
			m_bJumpDashStart = false;
			m_bGroundPoundStop = false;
			m_eCodyAnim = Cody_Space_Jump_Land;

			*pPreState = CCody_Space::PL_MOVE;
			pMesh->Set_Animationset(m_eCodyAnim);
			break;
		}
		case CCody_Space::PL_DASH:
		{
			if (pMesh->Is_AnimationSetEnd(0.1f))
			{
				m_eCodyAnim = Cody_Space_Walk;
				pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.05f);
			}
			break;
		}
		default:
			m_eCodyAnim = Cody_Space_Walk;
			pMesh->Set_Animationset(m_eCodyAnim);

			// 대쉬 때문에 모든곳에 넣어줌
			*pPreState = CCody_Space::PL_MOVE;
			break;
		}

		return;
	}

	if (pMesh->Is_AnimationSetEnd(0.3f))
	{
		m_eCodyAnim = Cody_Space_Walk;
		pMesh->Set_Animationset(m_eCodyAnim);
	}

}

void CCody_Space::PlayDash(CCody_Space* pCody, Engine::CDynamicMesh* pMesh)
{
	switch (m_eCodyAnim)
	{
	case Cody_Space_Dash_Start:
	{
		if (false == m_bPlaySound[PLAY_DASH])
		{
			CSoundMgr::GetInstance()->StopNPlaySound(L"Cody_Dash_Start.wav", CSoundMgr::CHANNELID::Cody_Dash, m_fEffectvolume);
			m_bPlaySound[PLAY_DASH] = true;
		}

		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			m_eCodyAnim = Cody_Space_Dash_End;
			pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		}
		break;
	}
	case Cody_Space_Dash_End:
	{
		if (false == m_bPlaySound[PLAY_DASH_END])
		{
			CSoundMgr::GetInstance()->StopNPlaySound(L"Cody_Dash_End.wav", CSoundMgr::CHANNELID::Cody_Dash, m_fEffectvolume);
			m_bPlaySound[PLAY_DASH_END] = true;
		}

		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			pCody->Clear(CPlayerValue::PL_DASH);
		}
		break;
	}
	default:
		pCody->Set_Dash(CCody_Space::PL_1STDASH);
		m_eCodyAnim = Cody_Space_Dash_Start;
		pMesh->Set_Animationset(m_eCodyAnim);
		break;
	}
}

void CCody_Space::PlaySit(Engine::CDynamicMesh* pMesh)
{
	switch (m_eCodyAnim)
	{
	case Cody_Space_Crouch_Start:
		Crouch_Start(pMesh);
		break;
	case Cody_Space_Crouch_Walk_Start:
		Crouch_Walk_Start(pMesh);
		break;
	case Cody_Space_Crouch_Idle:
		Crouch_Idle(pMesh);
		break;
	case Cody_Space_Crouch_Walk:
		Crouch_Walk(pMesh);
		break;
	case Cody_Space_Walk:
		m_eCodyAnim = Cody_Space_Crouch_Walk_Start;
		pMesh->Set_Animationset(m_eCodyAnim, 0.5f);
		break;
	default:
		m_eCodyAnim = Cody_Space_Crouch_Start;
		pMesh->Set_Animationset(m_eCodyAnim, 0.5f);
		break;
	}
}

void CCody_Space::PlayRun(Engine::CDynamicMesh * pMesh)
{
	m_eCodyAnim = Cody_Space_Run;
	pMesh->Set_Animationset(m_eCodyAnim);
}

void CCody_Space::PlayClimbing(CCody_Space* pCody, Engine::CDynamicMesh * pMesh, CCody_Space::PLAYER_STATE* pPreState)
{
	if (*pPreState != CCody_Space::PL_CLIMBING)
	{
		switch (*pPreState)
		{
		case CCody_Space::PL_JUMP:
			m_b2ndJump = false;
			m_b2ndJumpStart = false;
			m_bJumpDash = false;
			m_bJumpDashStart = false;
			m_bGroundPoundStop = false;
			*pPreState = CCody_Space::PL_CLIMBING;
			break;
		}
	}

	switch (m_eCodyAnim)
	{
	case Cody_Space_Wall_Idle:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		if (m_bClimbingJump)
		{
			m_eCodyAnim = Cody_Space_Wall_Jump;
		}
		break;
	case Cody_Space_Wall_Jump:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);

		if (m_bClimbingJumpEnd)
		{
			m_bClimbingJumpEnd = false;
			m_eCodyAnim = Cody_Space_Wall_Idle;
		}
		else if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			m_eCodyAnim = Cody_Space_Jump_Fall;
		}
		break;
	case Cody_Space_Jump_Fall:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.01f);
		if (m_bClimbingJumpEnd)
		{
			m_bClimbingJumpEnd = false;
			m_eCodyAnim = Cody_Space_Wall_Idle;
		}
		break;
	default:
		m_eCodyAnim = Cody_Space_Wall_Idle;
		break;
	}
}

void CCody_Space::PlayHang(CCody_Space * pCody, Engine::CDynamicMesh * pMesh, CCody_Space::PLAYER_STATE * pPreState)
{
	switch (m_eCodyAnim)
	{
	case Cody_Space_Ledge_Start:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
			m_eCodyAnim = Cody_Space_Ledge_Idle;
		break;
	case Cody_Space_Ledge_Idle:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.01f);
		if (m_bRise)
		{
			m_bRise = false;
			m_eCodyAnim = Cody_Space_Ledge_ClimbUp;
		}
		break;
	case Cody_Space_Ledge_Drop:
		break;
	case Cody_Space_Ledge_ClimbUp:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			m_bEndRise = true;
		}
		break;
	case Cody_Space_Ledge_ClimbLeft:
		break;
	case Cody_Space_Ledge_ClimbRight:
		break;
	default:
		m_eCodyAnim = Cody_Space_Ledge_Start;
		break;
	}
}
void CCody_Space::PlaySlide(CCody_Space* pCody, Engine::CDynamicMesh* pMesh, CCody_Space::PLAYER_STATE* ePreState)
{

}

void CCody_Space::PlayRide(CCody_Space * pCody, Engine::CDynamicMesh * pMesh, CCody_Space::PLAYER_STATE * ePreState)
{

}

void CCody_Space::PlayLever(CCody_Space * pCody, Engine::CDynamicMesh * pMesh, CCody_Space::PLAYER_STATE * ePreState)
{
	if (m_bSlap)
		PlayLeverSlap();
	else
		PlayLeverGrap();
}
void CCody_Space::PlayHangSwitch(CCody_Space* pCody, Engine::CDynamicMesh* pMesh, CCody_Space::PLAYER_STATE* ePreState)
{

}

void CCody_Space::PlayFloat(CCody_Space * pCody, Engine::CDynamicMesh * pMesh, CCody_Space::PLAYER_STATE * ePreState)
{
	m_eCodyAnim = Cody_Space_Zerogravity_Idle;
	pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
}

void CCody_Space::PlayFall(CCody_Space * pCody, Engine::CDynamicMesh * pMesh, CCody_Space::PLAYER_STATE * ePreState)
{
	m_eCodyAnim = Cody_Space_Jump_Fall;
	pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
}

void CCody_Space::PlayPush(CCody_Space * pCody, Engine::CDynamicMesh * pMesh, CCody_Space::PLAYER_STATE * ePreState)
{
	switch (m_eCodyAnim)
	{
	case Cody_Space_Push:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		switch (m_ePickUpType)
		{
		case CPlayerValue::GENERATOR_BATTERY:
			if (!pCody->Click_Up())
				m_eCodyAnim = Cody_Space_Push_Idle;
			break;
		case CPlayerValue::SPACE_WALL:
			if (!pCody->Click_Right())
				m_eCodyAnim = Cody_Space_Push_Idle;
			break;
		}
		break;
	case Cody_Space_Push_Idle:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		switch (m_ePickUpType)
		{
		case CPlayerValue::GENERATOR_BATTERY:
			if (pCody->Click_Up())
				m_eCodyAnim = Cody_Space_Push;
			break;
		case CPlayerValue::SPACE_WALL:
			if (pCody->Click_Right())
				m_eCodyAnim = Cody_Space_Push;
			break;
		}
		break;
	default:
		m_eCodyAnim = Cody_Space_Push_Idle;
		break;
	}
}

void CCody_Space::PlayAttach()
{
	switch (m_eCodyAnim)
	{
	case Cody_Space_Wall_Idle:
		m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		if (m_bClimbingJump)
		{
			m_eCodyAnim = Cody_Space_Wall_Jump;
		}
		break;
	case Cody_Space_Wall_Jump:
		m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);

		if (m_bClimbingJumpEnd)
		{
			m_bClimbingJumpEnd = false;
			m_eCodyAnim = Cody_Space_Wall_Idle;
		}
		else if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{
			m_eCodyAnim = Cody_Space_Jump_Fall;
		}
		break;
	case Cody_Space_Jump_Fall:
		m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.01f);
		if (m_bClimbingJumpEnd)
		{
			m_bClimbingJumpEnd = false;
			m_eCodyAnim = Cody_Space_Wall_Idle;
		}
		break;
	default:
		m_eCodyAnim = Cody_Space_Wall_Idle;
		break;
	}
}

void CCody_Space::PlayHandle()
{
	switch (m_eCodyAnim)
	{
	case Cody_Space_Valve_Idle:
		m_pMeshCom->Set_Animationset(m_eCodyAnim);
		if (true == m_bHandleRot)
			m_eCodyAnim = Cody_Space_Valve_RotateRight;
		break;
	case Cody_Space_Valve_RotateRight:
		m_pMeshCom->Set_Animationset(m_eCodyAnim);
		if (false == m_bHandleRot)
			m_eCodyAnim = Cody_Space_Valve_Idle;
		break;
	}
}

void CCody_Space::PlayGravityFly()
{
	m_eCodyAnim = Cody_Space_Zerogravity_Idle;
	m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
}

void CCody_Space::PlayMissile()
{
	switch (m_eCodyAnim)
	{
	case Cody_Space_Jump:
		m_pMeshCom->Set_Animationset(m_eCodyAnim);
		if (m_bRideEnd)
			m_eCodyAnim = Cody_Space_Hose_Start;
		else if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
			m_eCodyAnim = Cody_Space_Jump_Fall;
		break;
	case Cody_Space_Jump_Fall:
		m_pMeshCom->Set_Animationset(m_eCodyAnim);
		if (m_bRideEnd)
			m_eCodyAnim = Cody_Space_Hose_Start;
		break;
	case Cody_Space_Hose_Start:
		m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
			m_eCodyAnim = Cody_Space_Hose_Idle;
		break;
	case Cody_Space_Hose_Idle:
		m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.09f);
		if (m_bTakeOff)
			m_eCodyAnim = Cody_Space_Hose_End;
		//else if (Click_Right())
		//	m_eCodyAnim = Cody_Space_Hose_Left;
		//else if (Click_Left())
		//	m_eCodyAnim = Cody_Space_Hose_Right;
		break;
	case Cody_Space_Hose_Up:
		m_pMeshCom->Set_Animationset(m_eCodyAnim);
		if (m_bTakeOff)
			m_eCodyAnim = Cody_Space_Hose_End;
		else if (!Click_Down())
			m_eCodyAnim = Cody_Space_Hose_Idle;
		break;
	case Cody_Space_Hose_Left:
		m_pMeshCom->Set_Animationset(m_eCodyAnim);
		if (m_bTakeOff)
			m_eCodyAnim = Cody_Space_Hose_End;
		else if (!Click_Left())
			m_eCodyAnim = Cody_Space_Hose_Idle;
		break;
	case Cody_Space_Hose_Right:
		m_pMeshCom->Set_Animationset(m_eCodyAnim);
		if (m_bTakeOff)
			m_eCodyAnim = Cody_Space_Hose_End;
		else if (!Click_Right())
			m_eCodyAnim = Cody_Space_Hose_Idle;
		break;
	case Cody_Space_Hose_End:
		m_pMeshCom->Set_Animationset(m_eCodyAnim);
		if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{
			Set_RideJumpMax();
			m_eCodyAnim = Cody_Space_Jump_Fall;
		}
		break;
	}
}

void CCody_Space::PlayHit()
{
	m_pMeshCom->Set_Animationset(Cody_Space_Hit, 1.0f, 0.01f);
	if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_bHitEnd = true;
	}
}

void CCody_Space::PlayLeverSlap()
{
	if (Get_LeverStart())
	{
		switch (m_eLever)
		{
		case CCody_Space::LEFT_TO_RIGHT:
			m_eCodyAnim = Cody_Space_LeverLeft;
			m_pMeshCom->Set_Animationset(m_eCodyAnim);
			if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
			{
				Set_Lever(true);
				m_pMeshCom->Set_Animationset(Cody_Space_Idle, 1.0f, 0.1f);
			}
			break;
		case CCody_Space::RIGHT_TO_LEFT:
			m_eCodyAnim = Cody_Space_LeverRight;
			m_pMeshCom->Set_Animationset(m_eCodyAnim);
			if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
			{
				Set_Lever(true);
				m_pMeshCom->Set_Animationset(Cody_Space_Idle, 1.0f, 0.09f);
			}
			break;
		}
	}
	else
	{
		m_pMeshCom->Set_Animationset(Cody_Space_Walk);
	}
}

void CCody_Space::PlayLeverGrap()
{
	//if (false == m_bLeverStart)
	//	return;



	switch (m_eCodyAnim)
	{
	case Cody_Space_LeverLeftToRight:
		if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{
			Set_Lever(true);
			m_bPlayAnmation = false;
			if (false == m_bLeverCom)
			{
				m_pPickUpTarget->On_Active(nullptr);
				m_bLeverCom = true;
			}
			//m_pMeshCom->Set_Animationset(Cody_Space_Idle, 1.0f, 0.1f);
		}
		break;
	case Cody_Space_LeverRightToLeft:
		if (m_pMeshCom->Is_AnimationSetEnd(0.4f))
		{
			if (false == m_bLeverCom)
			{
				m_pPickUpTarget->On_Active(nullptr);
				m_bLeverCom = true;
			}
			Set_Lever(true);
			m_bPlayAnmation = false;
			//m_pMeshCom->Set_Animationset(Cody_Space_Idle, 1.0f, 0.09f);
		}
		break;
	}
}

void CCody_Space::PlayPickUp(CCody_Space * pCody, Engine::CDynamicMesh * pMesh, CCody_Space::PLAYER_STATE * pPreState)
{

}


#pragma region Crouch

void CCody_Space::Crouch_Idle(Engine::CDynamicMesh * pMesh)
{
	if (m_bCrouchMove)
	{
		m_eCodyAnim = Cody_Space_Crouch_Walk;
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
	}
	else
	{
		m_eCodyAnim = Cody_Space_Crouch_Idle;
		pMesh->Set_Animationset(m_eCodyAnim);
	}
}


void CCody_Space::Crouch_Walk_Start(Engine::CDynamicMesh * pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		if (m_bCrouchMove)
		{
			m_eCodyAnim = Cody_Space_Crouch_Walk;
			pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		}
		else
		{
			m_eCodyAnim = Cody_Space_Crouch_Idle;
			pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		}
	}
}

void CCody_Space::Crouch_Walk(Engine::CDynamicMesh * pMesh)
{
	if (!m_bCrouchMove)
	{
		m_eCodyAnim = Cody_Space_Crouch_Idle;
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
	}
}

void CCody_Space::Crouch_Start(Engine::CDynamicMesh * pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		m_eCodyAnim = Cody_Space_Crouch_Idle;
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
	}
}

#pragma endregion Crouch
#pragma region Jump
void CCody_Space::Jump(Engine::CDynamicMesh * pMesh)
{
	pMesh->Set_Animationset(m_eCodyAnim);
	if (m_bJump[PL_GROUNDPOUND_START])
	{
		m_eCodyAnim = Cody_Space_GroundPound_Start;
		pMesh->Set_Animationset(m_eCodyAnim);
		return;
	}
	else if (m_bJumpDash && !m_bJumpDashStart)
	{
		m_bJumpDashStart = true;
		m_eCodyAnim = Cody_Space_Jump_Dash;
		pMesh->Set_Animationset(m_eCodyAnim, 2.0f, 0.1f);
		return;
	}

	else if (!m_b2ndJumpStart && m_b2ndJump)
	{
		m_b2ndJumpStart = true;
		m_eCodyAnim = Cody_Space_2ndJump;
		pMesh->Set_Animationset(m_eCodyAnim);
		return;
	}

	if (pMesh->Is_AnimationSetEnd(0.3f))
		m_eCodyAnim = Cody_Space_Jump_Fall;
}

void CCody_Space::Jump_Fall(Engine::CDynamicMesh * pMesh)
{
	if (m_bJump[PL_GROUNDPOUND_START])
	{
		m_eCodyAnim = Cody_Space_GroundPound_Start;
		pMesh->Set_Animationset(m_eCodyAnim);
		return;
	}
	else if (m_bJumpDash && !m_bJumpDashStart)
	{
		m_bJumpDashStart = true;
		m_eCodyAnim = Cody_Space_Jump_Dash;
		pMesh->Set_Animationset(m_eCodyAnim, 2.0f, 0.1f);
		return;
	}
	else if (!m_b2ndJumpStart && m_b2ndJump)
	{
		m_b2ndJumpStart = true;
		m_eCodyAnim = Cody_Space_2ndJump;
		pMesh->Set_Animationset(m_eCodyAnim);
		return;
	}

	m_eCodyAnim = Cody_Space_Jump_Fall;
	pMesh->Set_Animationset(m_eCodyAnim);
}

void CCody_Space::Jump_Dash(CCody_Space* pCody, Engine::CDynamicMesh * pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		pCody->Set_JumpDashStop();
		m_eCodyAnim = Cody_Space_Jump_Fall;
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.08f);
	}
}

void CCody_Space::SecondJump(Engine::CDynamicMesh * pMesh)
{
	if (m_bJump[PL_GROUNDPOUND_START])
	{
		m_eCodyAnim = Cody_Space_GroundPound_Start;
		pMesh->Set_Animationset(m_eCodyAnim);
		return;
	}
	else if (m_bJumpDash && !m_bJumpDashStart)
	{
		m_bJumpDashStart = true;
		m_eCodyAnim = Cody_Space_Jump_Dash;
		pMesh->Set_Animationset(m_eCodyAnim, 2.0f, 0.1f);
		return;
	}
	else if (pMesh->Is_AnimationSetEnd(0.1f))
		m_eCodyAnim = Cody_Space_Jump_Fall;
}

void CCody_Space::GroundPound_Start(CCody_Space* pCody, Engine::CDynamicMesh * pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		pCody->Set_Jump(CCody_Space::PL_GROUNDPOUND, true);
		m_eCodyAnim = Cody_Space_GroundPound_Fall;
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
	}
}

void CCody_Space::GroundPound_Fall(CCody_Space* pCody, Engine::CDynamicMesh * pMesh)
{
	if (m_bGroundPoundStop)
	{
		m_eCodyAnim = Cody_Space_GroundPound_Land;
		pMesh->Set_Animationset(m_eCodyAnim);
	}
}

void CCody_Space::GroundPound_Land(Engine::CDynamicMesh * pMesh)
{
	if (false == m_bPlaySound[PLAY_GROUNDPOUND_IMPACT])
	{
		CSoundMgr::GetInstance()->StopNPlaySound(L"Cody_GroundPound_Impact.wav", CSoundMgr::CHANNELID::Cody_Jump, m_fEffectvolume);
		m_bPlaySound[PLAY_GROUNDPOUND_IMPACT] = true;
	}

	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		m_eCodyAnim = Cody_Space_GroundPound_End;
		pMesh->Set_Animationset(m_eCodyAnim);
	}
}

void CCody_Space::GroundPound_End(CCody_Space* pCody, Engine::CDynamicMesh * pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		m_eCodyAnim = Cody_Space_GroundPound_End;
		pMesh->Set_Animationset(m_eCodyAnim);
		pCody->Set_Jump(CCody_Space::PL_GROUNDPOUND_STOP);

		m_b2ndJump = false;
		m_b2ndJumpStart = false;
		m_bJumpDash = false;
		m_bJumpDashStart = false;
		m_bGroundPoundStop = false;
	}
}


#pragma endregion Jump

void CCody_Space::Set_Insert_Fuse()
{
	m_bInsert_Fuse = true;
}

void CCody_Space::Set_ClearJump()
{
	m_b2ndJumpStart = false;
	m_b2ndJump = false;
	m_bJumpDash = false;
	m_bJumpDashStart = false;
}

