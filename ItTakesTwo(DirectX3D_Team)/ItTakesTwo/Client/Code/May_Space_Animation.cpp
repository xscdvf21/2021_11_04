#include "stdafx.h"
#include "May_Space.h"

#include "Export_Function.h"


void CMay_Space::ChangeAnimation(CMay_Space* pMay, CMay_Space::PLAYER_STATE eCurState, CMay_Space::PLAYER_STATE* ePreState, Engine::CDynamicMesh* pMesh)
{
	switch (eCurState)
	{
	case CMay_Space::PL_IDLE:
		PlayIdle(pMesh, ePreState);
		break;
	case CMay_Space::PL_MOVE:
		PlayWalk(pMesh, ePreState);
		break;
	case CMay_Space::PL_JUMP:
		PlayJump(pMay, pMesh);
		break;
	case CMay_Space::PL_SIT:
		PlaySit(pMesh);
		break;
	case CMay_Space::PL_DASH:
		PlayDash(pMay, pMesh);
		break;
	case CMay_Space::PL_RUN:
		PlayRun(pMesh);
		break;
	case CMay_Space::PL_HANG:
		PlayHang(pMay, pMesh, ePreState);
		break;
	case CMay_Space::PL_SLIDE:
		PlaySlide(pMay, pMesh, ePreState);
		break;
	case CMay_Space::PL_CLIMBING:
		PlayClimbing(pMay, pMesh, ePreState);
		break;
	case CMay_Space::PL_PICKUP:
		PlayPickUp(pMay, pMesh, ePreState);
		break;
	case CMay_Space::PL_RIDE:
		PlayRide(pMay, pMesh, ePreState);
		break;
	case CMay_Space::PL_LEVER:
		PlayLever(pMay, pMesh, ePreState);
		break;
	case CMay_Space::PL_FLOAT:
		PlayFloat(pMay, pMesh, ePreState);
		break;
	case CMay_Space::PL_FALL:
		PlayFall(pMay, pMesh, ePreState);
		break;
	case CMay_Space::PL_HANGSWITCH:
		PlayHangSwitch(pMay, pMesh, ePreState);
		break;
	case CMay_Space::PL_FREEGRAVITY:
		PlayGravityFly();
		break;
	case CMay_Space::PL_PUSH:
		PlayPush(pMay, pMesh, ePreState);
		break;
	case CMay_Space::PL_MISSILE:
		PlayMissile();
		break;
	case CMay_Space::PL_HIT:
		PlayHit();
		break;
	}
}

void CMay_Space::PlayAnimation(CMay_Space * pCody, Engine::CDynamicMesh * pMesh, const _float & fTimeDelta, CMay_Space::PLAYER_STATE eCurState, CMay_Space::PLAYER_STATE * pPreState)
{
	ChangeAnimation(pCody, eCurState, pPreState, pMesh);
	if (m_bPlayAnmation)
		pMesh->Play_Animationset(fTimeDelta);
}

void CMay_Space::PlayIdle(Engine::CDynamicMesh * pMesh, CMay_Space::PLAYER_STATE * pPreState)
{
	if (*pPreState != CMay_Space::PL_IDLE)
	{
		switch (*pPreState)
		{
		case CMay_Space::PL_JUMP:
		{
			m_b2ndJump = false;
			m_b2ndJumpStart = false;
			m_bJumpDash = false;
			m_bJumpDashStart = false;
			m_bGroundPoundStop = false;
			m_eMayAnim = May_Jump_Land;
			pMesh->Set_Animationset(m_eMayAnim);
			*pPreState = CMay_Space::PL_IDLE;
			break;
		}
		case CMay_Space::PL_SIT:
		{
			m_eMayAnim = May_Crouch_End;
			pMesh->Set_Animationset(m_eMayAnim, 0.5f);
			*pPreState = CMay_Space::PL_IDLE;
			break;
		}
		case CMay_Space::PL_DASH:
		{
			if (pMesh->Is_AnimationSetEnd(0.15f))
			{
				m_eMayAnim = May_Idle;
				pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
			}
			*pPreState = CMay_Space::PL_IDLE;
			break;
		}
		case CMay_Space::PL_HANGSWITCH:
		{
			m_eMayAnim = May_Idle;
			pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.01f);
			break;
		}
		case CMay_Space::PL_HANG:
		{
			m_eMayAnim = May_Idle;
			pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.01f);
			break;
		}
		default:
			m_eMayAnim = May_Idle;
			pMesh->Set_Animationset(May_Idle);

			// Dash상태 때문에 번거롭게 모든곳에 넣어줌
			*pPreState = CMay_Space::PL_IDLE;
			break;
		}

		return;
	}

	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		m_eMayAnim = May_Idle;
		pMesh->Set_Animationset(May_Idle, 1.0f, 0.1f);
	}
}

void CMay_Space::PlayJump(CMay_Space * pCody, Engine::CDynamicMesh * pMesh)
{
	switch (m_eMayAnim)
	{
	case May_Jump:
		Jump(pMesh);
		break;
	case May_2ndJump:
		SecondJump(pMesh);
		break;
	case May_Jump_Fall:
		Jump_Fall(pMesh);
		break;
	case May_Jump_Dash:
		Jump_Dash(pCody, pMesh);
		break;
	case May_GroundPound_Start:
		GroundPound_Start(pCody, pMesh);
		break;
	case May_GroundPound_Fall:
		GroundPound_Fall(pCody, pMesh);
		break;
	case May_GroundPound_Land:
		GroundPound_Land(pMesh);
		break;
	case May_GroundPound_End:
		GroundPound_End(pCody, pMesh);
		break;
	}
}

void CMay_Space::PlayWalk(Engine::CDynamicMesh * pMesh, CMay_Space::PLAYER_STATE * pPreState)
{
	if (*pPreState != CMay_Space::PL_MOVE)
	{
		switch (*pPreState)
		{
		case CMay_Space::PL_SIT:
			m_eMayAnim = May_Crouch_Walk_End;
			pMesh->Set_Animationset(m_eMayAnim);

			*pPreState = CMay_Space::PL_MOVE;
			break;
		case CMay_Space::PL_JUMP:
		{
			m_b2ndJump = false;
			m_b2ndJumpStart = false;
			m_bJumpDash = false;
			m_bJumpDashStart = false;
			m_bGroundPoundStop = false;
			m_eMayAnim = May_Jump_Land;

			*pPreState = CMay_Space::PL_MOVE;
			pMesh->Set_Animationset(m_eMayAnim);
			break;
		}
		case CMay_Space::PL_DASH:
		{
			if (pMesh->Is_AnimationSetEnd(0.1f))
			{
				m_eMayAnim = May_Walk;
				pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.05f);
			}
			break;
		}
		default:
			m_eMayAnim = May_Walk;
			pMesh->Set_Animationset(m_eMayAnim);

			// 대쉬 때문에 모든곳에 넣어줌
			*pPreState = CMay_Space::PL_MOVE;
			break;
		}

		return;
	}

	if (pMesh->Is_AnimationSetEnd(0.3f))
	{
		m_eMayAnim = May_Walk;
		pMesh->Set_Animationset(m_eMayAnim);
	}
}

void CMay_Space::PlayDash(CMay_Space * pMay, Engine::CDynamicMesh * pMesh)
{
	switch (m_eMayAnim)
	{
	case May_Dash_Start:
	{
		if (false == m_bPlaySound[PLAY_DASH])
		{
			CSoundMgr::GetInstance()->StopNPlaySound(L"May_Dash_Start.wav", CSoundMgr::CHANNELID::May_Dash, m_fEffectvolume);
			m_bPlaySound[PLAY_DASH] = true;

		}
		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			m_eMayAnim = May_Dash_End;
			pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		}
		break;
	}
	case May_Dash_End:
	{
		if (false == m_bPlaySound[PLAY_DASH_END])
		{
			CSoundMgr::GetInstance()->StopNPlaySound(L"May_Dash_End.wav", CSoundMgr::CHANNELID::May_Dash, m_fEffectvolume);
			m_bPlaySound[PLAY_DASH_END] = true;

		}
		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			pMay->Clear(CPlayerValue::PL_DASH);
		}
		break;
	}
	default:
		pMay->Set_Dash(CMay_Space::PL_1STDASH);
		m_eMayAnim = May_Dash_Start;
		pMesh->Set_Animationset(m_eMayAnim);
		break;
	}
}

void CMay_Space::PlaySit(Engine::CDynamicMesh * pMesh)
{
	switch (m_eMayAnim)
	{
	case May_Crouch_Start:
		Crouch_Start(pMesh);
		break;
	case May_Crouch_Walk_Start:
		Crouch_Walk_Start(pMesh);
		break;
	case May_Crouch_Idle:
		Crouch_Idle(pMesh);
		break;
	case May_Crouch_Walk:
		Crouch_Walk(pMesh);
		break;
	case May_Walk:
		m_eMayAnim = May_Crouch_Walk_Start;
		pMesh->Set_Animationset(m_eMayAnim, 0.5f);
		break;
	default:
		m_eMayAnim = May_Crouch_Start;
		pMesh->Set_Animationset(m_eMayAnim, 0.5f);
		break;
	}
}

void CMay_Space::PlayRun(Engine::CDynamicMesh * pMesh)
{
	m_eMayAnim = May_Run;
	pMesh->Set_Animationset(m_eMayAnim);
}

void CMay_Space::PlayClimbing(CMay_Space* pMay, Engine::CDynamicMesh * pMesh, CMay_Space::PLAYER_STATE* pPreState)
{
	if (*pPreState != CMay_Space::PL_CLIMBING)
	{
		switch (*pPreState)
		{
		case CMay_Space::PL_JUMP:
			m_b2ndJump = false;
			m_b2ndJumpStart = false;
			m_bJumpDash = false;
			m_bJumpDashStart = false;
			m_bGroundPoundStop = false;
			*pPreState = CMay_Space::PL_CLIMBING;
			break;
		}
	}

	switch (m_eMayAnim)
	{
	case May_Wall_Idle:
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		if (m_bClimbingJump)
		{
			m_eMayAnim = May_Wall_Jump;
		}
		break;
	case May_Wall_Jump:
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);

		if (m_bClimbingJumpEnd)
		{
			m_bClimbingJumpEnd = false;
			m_eMayAnim = May_Wall_Idle;
		}
		else if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			m_eMayAnim = May_Jump_Fall;
		}
		break;
	case May_Jump_Fall:
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.01f);
		if (m_bClimbingJumpEnd)
		{
			m_bClimbingJumpEnd = false;
			m_eMayAnim = May_Wall_Idle;
		}
		break;
	default:
		m_eMayAnim = May_Wall_Idle;
		break;
	}
}

void CMay_Space::PlayHang(CMay_Space * pCody, Engine::CDynamicMesh * pMesh, CMay_Space::PLAYER_STATE * pPreState)
{
	switch (m_eHangType)
	{
	case CPlayerValue::LEDGE:
		LedgeAnimation();
		break;
	case CPlayerValue::HANG:
		HangAnimation();
		break;
	}


}

void CMay_Space::PlaySlide(CMay_Space* pMay, Engine::CDynamicMesh* pMesh, CMay_Space::PLAYER_STATE* ePreState)
{

}

void CMay_Space::PlayRide(CMay_Space * pMay, Engine::CDynamicMesh * pMesh, CMay_Space::PLAYER_STATE * ePreState)
{

}

void CMay_Space::PlayLever(CMay_Space * pMay, Engine::CDynamicMesh * pMesh, CMay_Space::PLAYER_STATE * ePreState)
{
	if (m_bSlap)
		PlayLeverSlap();
	else
		PlayLeverGrap();


}

void CMay_Space::PlayHangSwitch(CMay_Space* pMay, Engine::CDynamicMesh* pMesh, CMay_Space::PLAYER_STATE* ePreState)
{
	switch (m_eMayAnim)
	{
	case May_PowerSwitch_Start:
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.01f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
			m_eMayAnim = May_PowerSwitch_Idle;
		break;

	case May_Jump:
		pMesh->Set_Animationset(m_eMayAnim, 1.5f, 0.1f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
			m_eMayAnim = May_PowerSwitch_Start;
		break;
	default:
		m_eMayAnim = May_Jump;
		break;
	}
}

void CMay_Space::PlayFloat(CMay_Space * pCody, Engine::CDynamicMesh * pMesh, CMay_Space::PLAYER_STATE * ePreState)
{
	m_eMayAnim = May_Zerogravity_Idle;
	pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
}

void CMay_Space::PlayFall(CMay_Space * pMay, Engine::CDynamicMesh * pMesh, CMay_Space::PLAYER_STATE * ePreState)
{
	m_eMayAnim = May_Jump_Fall;
	pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
}

void CMay_Space::PlayPush(CMay_Space * pMay, Engine::CDynamicMesh * pMesh, CMay_Space::PLAYER_STATE * ePreState)
{
	if (m_bGrap)
	{
		switch (m_eMayAnim)
		{
		case SPACE::May_CubeMove_Start:
			m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
			if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
				m_eMayAnim = May_CubeMove_Idle;
			break;
		case SPACE::May_CubeMove_End:
			break;
		case SPACE::May_CubeMove_Idle:
		{
			m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
			switch (m_eKEYDIR)
			{
			case CPlayerValue::DIR_LEFT:
				m_eMayAnim = May_CubeMove_Bwd;
				break;
			case CPlayerValue::DIR_LEFT_UP:
				m_eMayAnim = May_CubeMove_Bwd_Left;
				break;
			case CPlayerValue::DIR_UP:
				m_eMayAnim = May_CubeMove_Left;
				break;
			case CPlayerValue::DIR_RIGHT_UP:
				m_eMayAnim = May_CubeMove_Fwd_Left;
				break;
			case CPlayerValue::DIR_RIGHT:
				m_eMayAnim = May_CubeMove_Fwd;
				break;
			case CPlayerValue::DIR_RIGHT_DOWN:
				m_eMayAnim = May_CubeMove_Fwd_Right;
				break;
			case CPlayerValue::DIR_DOWN:
				m_eMayAnim = May_CubeMove_Right;
				break;
			case CPlayerValue::DIR_LEFT_DOWN:
				m_eMayAnim = May_CubeMove_Bwd_Right;
				break;
			}
		}
			break;
		case SPACE::May_CubeMove_Fwd:
		{
			m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
			switch (m_eKEYDIR)
			{
			case CPlayerValue::DIR_LEFT:
				m_eMayAnim = May_CubeMove_Bwd;
				break;
			case CPlayerValue::DIR_LEFT_UP:
				m_eMayAnim = May_CubeMove_Bwd_Left;
				break;
			case CPlayerValue::DIR_UP:
				m_eMayAnim = May_CubeMove_Left;
				break;
			case CPlayerValue::DIR_RIGHT_UP:
				m_eMayAnim = May_CubeMove_Fwd_Left;
				break;
			case CPlayerValue::DIR_RIGHT_DOWN:
				m_eMayAnim = May_CubeMove_Fwd_Right;
				break;
			case CPlayerValue::DIR_DOWN:
				m_eMayAnim = May_CubeMove_Right;
				break;
			case CPlayerValue::DIR_LEFT_DOWN:
				m_eMayAnim = May_CubeMove_Bwd_Right;
				break;
			case CPlayerValue::DIR_STOP:
				m_eMayAnim = May_CubeMove_Idle;
				break;
			}
		}
			break;
		case SPACE::May_CubeMove_Fwd_Right:
			m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
			switch (m_eKEYDIR)
			{
			case CPlayerValue::DIR_LEFT:
				m_eMayAnim = May_CubeMove_Bwd;
				break;
			case CPlayerValue::DIR_LEFT_UP:
				m_eMayAnim = May_CubeMove_Bwd_Left;
				break;
			case CPlayerValue::DIR_UP:
				m_eMayAnim = May_CubeMove_Left;
				break;
			case CPlayerValue::DIR_RIGHT_UP:
				m_eMayAnim = May_CubeMove_Fwd_Left;
				break;
			case CPlayerValue::DIR_RIGHT:
				m_eMayAnim = May_CubeMove_Fwd;
				break;
			case CPlayerValue::DIR_DOWN:
				m_eMayAnim = May_CubeMove_Right;
				break;
			case CPlayerValue::DIR_LEFT_DOWN:
				m_eMayAnim = May_CubeMove_Bwd_Right;
				break;
			case CPlayerValue::DIR_STOP:
				m_eMayAnim = May_CubeMove_Idle;
				break;
			}
			break;
		case SPACE::May_CubeMove_Right:
			m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
			switch (m_eKEYDIR)
			{
			case CPlayerValue::DIR_LEFT:
				m_eMayAnim = May_CubeMove_Bwd;
				break;
			case CPlayerValue::DIR_LEFT_UP:
				m_eMayAnim = May_CubeMove_Bwd_Left;
				break;
			case CPlayerValue::DIR_UP:
				m_eMayAnim = May_CubeMove_Left;
				break;
			case CPlayerValue::DIR_RIGHT_UP:
				m_eMayAnim = May_CubeMove_Fwd_Left;
				break;
			case CPlayerValue::DIR_RIGHT:
				m_eMayAnim = May_CubeMove_Fwd;
				break;
			case CPlayerValue::DIR_RIGHT_DOWN:
				m_eMayAnim = May_CubeMove_Fwd_Right;
				break;
			case CPlayerValue::DIR_LEFT_DOWN:
				m_eMayAnim = May_CubeMove_Bwd_Right;
				break;
			case CPlayerValue::DIR_STOP:
				m_eMayAnim = May_CubeMove_Idle;
				break;
			}
			break;
		case SPACE::May_CubeMove_Bwd_Right:
			m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
			switch (m_eKEYDIR)
			{
			case CPlayerValue::DIR_LEFT:
				m_eMayAnim = May_CubeMove_Bwd;
				break;
			case CPlayerValue::DIR_LEFT_UP:
				m_eMayAnim = May_CubeMove_Bwd_Left;
				break;
			case CPlayerValue::DIR_UP:
				m_eMayAnim = May_CubeMove_Left;
				break;
			case CPlayerValue::DIR_RIGHT_UP:
				m_eMayAnim = May_CubeMove_Fwd_Left;
				break;
			case CPlayerValue::DIR_RIGHT:
				m_eMayAnim = May_CubeMove_Fwd;
				break;
			case CPlayerValue::DIR_RIGHT_DOWN:
				m_eMayAnim = May_CubeMove_Fwd_Right;
				break;
			case CPlayerValue::DIR_DOWN:
				m_eMayAnim = May_CubeMove_Right;
				break;
			case CPlayerValue::DIR_STOP:
				m_eMayAnim = May_CubeMove_Idle;
				break;
			}
			break;
		case SPACE::May_CubeMove_Bwd:
			m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
			switch (m_eKEYDIR)
			{
			case CPlayerValue::DIR_LEFT_UP:
				m_eMayAnim = May_CubeMove_Bwd_Left;
				break;
			case CPlayerValue::DIR_UP:
				m_eMayAnim = May_CubeMove_Left;
				break;
			case CPlayerValue::DIR_RIGHT_UP:
				m_eMayAnim = May_CubeMove_Fwd_Left;
				break;
			case CPlayerValue::DIR_RIGHT:
				m_eMayAnim = May_CubeMove_Fwd;
				break;
			case CPlayerValue::DIR_RIGHT_DOWN:
				m_eMayAnim = May_CubeMove_Fwd_Right;
				break;
			case CPlayerValue::DIR_DOWN:
				m_eMayAnim = May_CubeMove_Right;
				break;
			case CPlayerValue::DIR_LEFT_DOWN:
				m_eMayAnim = May_CubeMove_Bwd_Right;
				break;
			case CPlayerValue::DIR_STOP:
				m_eMayAnim = May_CubeMove_Idle;
				break;
			}
			break;
		case SPACE::May_CubeMove_Bwd_Left:
			m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
			switch (m_eKEYDIR)
			{
			case CPlayerValue::DIR_LEFT:
				m_eMayAnim = May_CubeMove_Bwd;
				break;
			case CPlayerValue::DIR_UP:
				m_eMayAnim = May_CubeMove_Left;
				break;
			case CPlayerValue::DIR_RIGHT_UP:
				m_eMayAnim = May_CubeMove_Fwd_Left;
				break;
			case CPlayerValue::DIR_RIGHT:
				m_eMayAnim = May_CubeMove_Fwd;
				break;
			case CPlayerValue::DIR_RIGHT_DOWN:
				m_eMayAnim = May_CubeMove_Fwd_Right;
				break;
			case CPlayerValue::DIR_DOWN:
				m_eMayAnim = May_CubeMove_Right;
				break;
			case CPlayerValue::DIR_LEFT_DOWN:
				m_eMayAnim = May_CubeMove_Bwd_Right;
				break;
			case CPlayerValue::DIR_STOP:
				m_eMayAnim = May_CubeMove_Idle;
				break;
			}
			break;
		case SPACE::May_CubeMove_Left:
			m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
			switch (m_eKEYDIR)
			{
			case CPlayerValue::DIR_LEFT:
				m_eMayAnim = May_CubeMove_Bwd;
				break;
			case CPlayerValue::DIR_LEFT_UP:
				m_eMayAnim = May_CubeMove_Bwd_Left;
				break;
			case CPlayerValue::DIR_RIGHT_UP:
				m_eMayAnim = May_CubeMove_Fwd_Left;
				break;
			case CPlayerValue::DIR_RIGHT:
				m_eMayAnim = May_CubeMove_Fwd;
				break;
			case CPlayerValue::DIR_RIGHT_DOWN:
				m_eMayAnim = May_CubeMove_Fwd_Right;
				break;
			case CPlayerValue::DIR_DOWN:
				m_eMayAnim = May_CubeMove_Right;
				break;
			case CPlayerValue::DIR_LEFT_DOWN:
				m_eMayAnim = May_CubeMove_Bwd_Right;
				break;
			case CPlayerValue::DIR_STOP:
				m_eMayAnim = May_CubeMove_Idle;
				break;
			}
			break;
		case SPACE::May_CubeMove_Fwd_Left:
			m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
			switch (m_eKEYDIR)
			{
			case CPlayerValue::DIR_LEFT:
				m_eMayAnim = May_CubeMove_Bwd;
				break;
			case CPlayerValue::DIR_LEFT_UP:
				m_eMayAnim = May_CubeMove_Bwd_Left;
				break;
			case CPlayerValue::DIR_UP:
				m_eMayAnim = May_CubeMove_Left;
				break;
			case CPlayerValue::DIR_RIGHT:
				m_eMayAnim = May_CubeMove_Fwd;
				break;
			case CPlayerValue::DIR_RIGHT_DOWN:
				m_eMayAnim = May_CubeMove_Fwd_Right;
				break;
			case CPlayerValue::DIR_DOWN:
				m_eMayAnim = May_CubeMove_Right;
				break;
			case CPlayerValue::DIR_LEFT_DOWN:
				m_eMayAnim = May_CubeMove_Bwd_Right;
				break;
			case CPlayerValue::DIR_STOP:
				m_eMayAnim = May_CubeMove_Idle;
				break;
			}
			break;
		}
	}
	else
	{
		switch (m_eMayAnim)
		{
		case May_Push:
			pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
			switch (m_ePickUpType)
			{
			case CPlayerValue::GENERATOR_BATTERY:
				if (!pMay->Click_Up())
					m_eMayAnim = May_Push_Idle;
				break;
			case CPlayerValue::SPACE_WALL:
				if (!pMay->Click_Right())
					m_eMayAnim = May_Push_Idle;
				break;
			}
			break;
		case May_Push_Idle:
			pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
			switch (m_ePickUpType)
			{
			case CPlayerValue::GENERATOR_BATTERY:
				if (pMay->Click_Up())
					m_eMayAnim = May_Push;
				break;
			case CPlayerValue::SPACE_WALL:
				if (pMay->Click_Right())
					m_eMayAnim = May_Push;
				break;
			}
			break;
		default:
			m_eMayAnim = May_Push_Idle;
			break;
		}
	}
}

void CMay_Space::PlayAttach()
{
	switch (m_eMayAnim)
	{
	case May_Wall_Idle:
		m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		if (m_bClimbingJump)
		{
			m_eMayAnim = May_Wall_Jump;
		}
		break;
	case May_Wall_Jump:
		m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);

		if (m_bClimbingJumpEnd)
		{
			m_bClimbingJumpEnd = false;
			m_eMayAnim = May_Wall_Idle;
		}
		else if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{
			m_eMayAnim = May_Jump_Fall;
		}
		break;
	case May_Jump_Fall:
		m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.01f);
		if (m_bClimbingJumpEnd)
		{
			m_bClimbingJumpEnd = false;
			m_eMayAnim = May_Wall_Idle;
		}
		break;
	default:
		m_eMayAnim = May_Wall_Idle;
		break;
	}
}

void CMay_Space::PlayGravityFly()
{
	m_eMayAnim = May_Zerogravity_Idle;
	m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
}

void CMay_Space::PlayLeverSlap()
{
	if (Get_LeverStart())
	{
		switch (m_eMayAnim)
		{
		case May_LeverLeft:
			m_pMeshCom->Set_Animationset(m_eMayAnim);
			if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
			{
				Set_Lever(true);
				m_pMeshCom->Set_Animationset(May_Idle, 1.0f, 0.1f);
			}
			break;
		case May_LeverRight:
			m_pMeshCom->Set_Animationset(m_eMayAnim);
			if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
			{
				Set_Lever(true);
				m_pMeshCom->Set_Animationset(May_Idle, 1.0f, 0.09f);
			}
			break;
		}
	}
}

void CMay_Space::PlayLeverGrap()
{
	switch (m_eMayAnim)
	{
	case May_Lever_LeftToRight:
		if (m_pMeshCom->Is_AnimationSetEnd(0.35f))
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
	case May_Lever_RightToLeft:
		if (m_pMeshCom->Is_AnimationSetEnd(0.35f))
		{
			Set_Lever(true);
			m_bPlayAnmation = false;
			if (false == m_bLeverCom)
			{
				m_pPickUpTarget->On_Active(nullptr);
				m_bLeverCom = true;
			}
			//m_pMeshCom->Set_Animationset(Cody_Space_Idle, 1.0f, 0.09f);
		}
		break;
	}
}

void CMay_Space::PlayMissile()
{
	switch (m_eMayAnim)
	{
	case May_Jump:
		m_pMeshCom->Set_Animationset(m_eMayAnim);
		if (m_bRideEnd)
			m_eMayAnim = May_Hose_Start;
		else if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
			m_eMayAnim = May_Jump_Fall;
		break;
	case May_Jump_Fall:
		m_pMeshCom->Set_Animationset(m_eMayAnim);
		if (m_bRideEnd)
			m_eMayAnim = May_Hose_Start;
		break;
	case May_Hose_Start:
		m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
			m_eMayAnim = May_Hose_Idle;
		break;
	case May_Hose_Idle:
		m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.09f);
		break;
	}
}

void CMay_Space::PlayHit()
{
	m_pMeshCom->Set_Animationset(May_Hit, 1.0f, 0.01f);
	if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_bHitEnd = true;
	}
}

void CMay_Space::LedgeAnimation()
{
	switch (m_eMayAnim)
	{
	case May_Ledge_Start:
		m_pMeshCom->Set_Animationset(m_eMayAnim);
		if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
			m_eMayAnim = May_Ledge_Idle;
		break;
	case May_Ledge_Idle:
		m_pMeshCom->Set_Animationset(m_eMayAnim);
		if (m_bRise)
		{
			m_bRise = false;
			m_eMayAnim = May_Ledge_ClimbUp;
		}
		break;
	case May_Ledge_ClimbUp:
		m_pMeshCom->Set_Animationset(m_eMayAnim);
		if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{
			m_bEndRise = true;
		}
		break;
	default:
		m_eMayAnim = May_Ledge_Start;
		break;
	}
}

void CMay_Space::HangAnimation()
{
	switch (m_eMayAnim)
	{
	case May_PowerSwitch_Start:
		m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
			m_eMayAnim = May_PowerSwitch_Idle;
		break;
	case May_PowerSwitch_Idle:
		m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.01f);
		Set_SwitchReady();
		//if (m_bHangSwitchSuccess)
		//	m_eMayAnim = May_PowerSwitch_Success;
		break;
	case May_Jump:
		m_pMeshCom->Set_Animationset(m_eMayAnim, 1.5f, 0.1f);
		if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
			m_eMayAnim = May_PowerSwitch_Start;
		break;
	default:
		m_eMayAnim = May_Jump;
		break;
	}
}

void CMay_Space::PlayPickUp(CMay_Space * pMay, Engine::CDynamicMesh * pMesh, CMay_Space::PLAYER_STATE * pPreState)
{

}

#pragma region Crouch
void CMay_Space::Crouch_Idle(Engine::CDynamicMesh * pMesh)
{
	if (m_bCrouchMove)
	{
		m_eMayAnim = May_Crouch_Walk;
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
	}
	else
	{
		m_eMayAnim = May_Crouch_Idle;
		pMesh->Set_Animationset(m_eMayAnim);
	}
}

void CMay_Space::Crouch_Walk_Start(Engine::CDynamicMesh * pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		if (m_bCrouchMove)
		{
			m_eMayAnim = May_Crouch_Walk;
			pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		}
		else
		{
			m_eMayAnim = May_Crouch_Idle;
			pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		}
	}
}

void CMay_Space::Crouch_Walk(Engine::CDynamicMesh * pMesh)
{
	if (!m_bCrouchMove)
	{
		m_eMayAnim = May_Crouch_Idle;
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
	}
}

void CMay_Space::Crouch_Start(Engine::CDynamicMesh * pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		m_eMayAnim = May_Crouch_Idle;
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
	}
}

#pragma endregion Crouch
#pragma region Jump

void CMay_Space::Jump(Engine::CDynamicMesh * pMesh)
{
	pMesh->Set_Animationset(m_eMayAnim);
	if (m_bJump[PL_GROUNDPOUND_START])
	{
		m_eMayAnim = May_GroundPound_Start;
		pMesh->Set_Animationset(m_eMayAnim);
		return;
	}
	else if (m_bJumpDash && !m_bJumpDashStart)
	{
		m_bJumpDashStart = true;
		m_eMayAnim = May_Jump_Dash;
		pMesh->Set_Animationset(m_eMayAnim, 2.0f, 0.1f);
		return;
	}

	else if (!m_b2ndJumpStart && m_b2ndJump)
	{
		m_b2ndJumpStart = true;
		m_eMayAnim = May_2ndJump;
		pMesh->Set_Animationset(m_eMayAnim);
		return;
	}

	if (pMesh->Is_AnimationSetEnd(0.3f))
		m_eMayAnim = May_Jump_Fall;
}

void CMay_Space::Jump_Fall(Engine::CDynamicMesh * pMesh)
{
	if (m_bJump[PL_GROUNDPOUND_START])
	{
		m_eMayAnim = May_GroundPound_Start;
		pMesh->Set_Animationset(m_eMayAnim);
		return;
	}
	else if (m_bJumpDash && !m_bJumpDashStart)
	{
		m_bJumpDashStart = true;
		m_eMayAnim = May_Jump_Dash;
		pMesh->Set_Animationset(m_eMayAnim, 2.0f, 0.1f);
		return;
	}
	else if (!m_b2ndJumpStart && m_b2ndJump)
	{
		m_b2ndJumpStart = true;
		m_eMayAnim = May_2ndJump;
		pMesh->Set_Animationset(m_eMayAnim);
		return;
	}

	m_eMayAnim = May_Jump_Fall;
	pMesh->Set_Animationset(m_eMayAnim);
}

void CMay_Space::Jump_Dash(CMay_Space* pCody, Engine::CDynamicMesh * pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		pCody->Set_JumpDashStop();
		m_eMayAnim = May_Jump_Fall;
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.08f);
	}
}

void CMay_Space::SecondJump(Engine::CDynamicMesh * pMesh)
{
	if (m_bJump[PL_GROUNDPOUND_START])
	{
		m_eMayAnim = May_GroundPound_Start;
		pMesh->Set_Animationset(m_eMayAnim);
		return;
	}
	else if (m_bJumpDash && !m_bJumpDashStart)
	{
		m_bJumpDashStart = true;
		m_eMayAnim = May_Jump_Dash;
		pMesh->Set_Animationset(m_eMayAnim, 2.0f, 0.1f);
		return;
	}
	else if (pMesh->Is_AnimationSetEnd(0.1f))
		m_eMayAnim = May_Jump_Fall;
}

void CMay_Space::GroundPound_Start(CMay_Space * pCody, Engine::CDynamicMesh * pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		pCody->Set_Jump(CMay_Space::PL_GROUNDPOUND, true);
		m_eMayAnim = May_GroundPound_Fall;
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
	}
}

void CMay_Space::GroundPound_Fall(CMay_Space* pMay, Engine::CDynamicMesh * pMesh)
{
	if (m_bGroundPoundStop)
	{
		m_eMayAnim = May_GroundPound_Land;
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
	}
}

void CMay_Space::GroundPound_Land(Engine::CDynamicMesh * pMesh)
{
	if (false == m_bPlaySound[PLAY_GROUNDPOUND_IMPACT])
	{
		CSoundMgr::GetInstance()->StopNPlaySound(L"May_GroundPound_Impact.wav", CSoundMgr::CHANNELID::May_Jump, m_fEffectvolume);
		m_bPlaySound[PLAY_GROUNDPOUND_IMPACT] = true;
	}

	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		m_eMayAnim = May_GroundPound_End;
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
	}

}

void CMay_Space::GroundPound_End(CMay_Space * pMay, Engine::CDynamicMesh * pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		m_eMayAnim = May_Idle;
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.05f);
		pMay->Set_Jump(CMay_Space::PL_GROUNDPOUND_STOP);

		m_b2ndJump = false;
		m_b2ndJumpStart = false;
		m_bJumpDash = false;
		m_bJumpDashStart = false;
	}
}
#pragma endregion Jump

void CMay_Space::Set_Insert_Fuse()
{
	m_bInsert_Fuse = true;
}

void CMay_Space::Set_ClearJump()
{
	m_b2ndJumpStart = false;
	m_b2ndJump = false;
	m_bJumpDash = false;
	m_bJumpDashStart = false;
}