#include "stdafx.h"
#include "May.h"

#include "Export_Function.h"
#include "FuseNPC.h"

void CMay::ChangeAnimation(CMay* pMay, CMay::PLAYER_STATE eCurState, CMay::PLAYER_STATE* ePreState, Engine::CDynamicMesh* pMesh)
{
	switch (eCurState)
	{
	case CMay::PL_IDLE:
		PlayIdle(pMesh, ePreState);
		break;
	case CMay::PL_MOVE:
		PlayWalk(pMesh, ePreState);
		break;
	case CMay::PL_JUMP:
		PlayJump(pMay, pMesh);
		break;
	case CMay::PL_SIT:
		PlaySit(pMesh);
		break;
	case CMay::PL_DASH:
		PlayDash(pMay, pMesh);
		break;
	case CMay::PL_RUN:
		PlayRun(pMesh);
		break;
	case CMay::PL_HANG:
		PlayHang(pMay, pMesh, ePreState);
		break;
	case CMay::PL_SLIDE:
		PlaySlide(pMay, pMesh, ePreState);
		break;
	case CMay::PL_CLIMBING:
		PlayClimbing(pMay, pMesh, ePreState);
		break;
	case CMay::PL_PICKUP:
		PlayPickUp(pMay, pMesh, ePreState);
		break;
	case CMay::PL_RIDE:
		PlayRide(pMay, pMesh, ePreState);
		break;
	case CMay::PL_LEVER:
		PlayLever(pMay, pMesh, ePreState);
		break;
	case CMay::PL_FLOAT:
		PlayFloat(pMay, pMesh, ePreState);
		break;
	case CMay::PL_FALL:
		PlayFall(pMay, pMesh, ePreState);
		break;
	case CMay::PL_HANGSWITCH:
		PlayHangSwitch(pMay, pMesh, ePreState);
		break;
	case CMay::PL_PUSH:
		PlayPush(pMay, pMesh, ePreState);
		break;
	case CMay::PL_HANDLE:
		PlayHandle();
		break;
	case CMay::PL_BUTTON:
		PlayButton();
		break;
	case CMay::PL_SAW_SHOOT:
		PlaySawShoot();
		break;
	case CMay::PL_SAW_SLIDING:
		PlaySawSliding();
		break;
	case CMay::PL_SHOOT:
		PlaySlide(pMay, pMesh, ePreState);
		break;
	case CMay::PL_HIT:
		PlayHit();
		break;
	}
}

void CMay::PlayAnimation(CMay * pCody, Engine::CDynamicMesh * pMesh, const _float & fTimeDelta, CMay::PLAYER_STATE eCurState, CMay::PLAYER_STATE * pPreState)
{
	ChangeAnimation(pCody, eCurState, pPreState, pMesh);
	pMesh->Play_Animationset(fTimeDelta);
}

void CMay::PlayIdle(Engine::CDynamicMesh * pMesh, CMay::PLAYER_STATE * pPreState)
{
	if (*pPreState != CMay::PL_IDLE)
	{
		switch (*pPreState)
		{
		case CMay::PL_JUMP:
		{
			m_b2ndJump = false;
			m_b2ndJumpStart = false;
			m_bJumpDash = false;
			m_bJumpDashStart = false;
			m_bGroundPoundStop = false;
			m_eMayAnim = May_Jump_Land;
			pMesh->Set_Animationset(m_eMayAnim);
			*pPreState = CMay::PL_IDLE;
			break;
		}
		case CMay::PL_SIT:
		{
			m_eMayAnim = May_Crouch_End;
			pMesh->Set_Animationset(m_eMayAnim, 0.5f);
			*pPreState = CMay::PL_IDLE;
			break;
		}
		case CMay::PL_DASH:
		{
			if (pMesh->Is_AnimationSetEnd(0.15f))
			{
				m_eMayAnim = May_Idle;
				pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
			}
			*pPreState = CMay::PL_IDLE;
			break;
		}
		case CMay::PL_HANGSWITCH:
		{
			m_eMayAnim = May_Idle;
			pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.01f);
			break;
		}
		case CMay::PL_HANG:
		{
			m_eMayAnim = May_Idle;
			pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.01f);
			break;
		}
		default:
			m_eMayAnim = May_Idle;
			pMesh->Set_Animationset(May_Idle);

			// Dash상태 때문에 번거롭게 모든곳에 넣어줌
			*pPreState = CMay::PL_IDLE;
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

void CMay::PlayJump(CMay * pCody, Engine::CDynamicMesh * pMesh)
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

void CMay::PlayWalk(Engine::CDynamicMesh * pMesh, CMay::PLAYER_STATE * pPreState)
{
	if (*pPreState != CMay::PL_MOVE)
	{
		switch (*pPreState)
		{
		case CMay::PL_SIT:
			m_eMayAnim = May_Crouch_Walk_End;
			pMesh->Set_Animationset(m_eMayAnim);

			*pPreState = CMay::PL_MOVE;
			break;
		case CMay::PL_JUMP:
		{
			m_b2ndJump = false;
			m_b2ndJumpStart = false;
			m_bJumpDash = false;
			m_bJumpDashStart = false;
			m_bGroundPoundStop = false;
			m_eMayAnim = May_Jump_Land;

			*pPreState = CMay::PL_MOVE;
			pMesh->Set_Animationset(m_eMayAnim);
			break;
		}
		case CMay::PL_DASH:
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
			*pPreState = CMay::PL_MOVE;
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

void CMay::PlayDash(CMay * pMay, Engine::CDynamicMesh * pMesh)
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
		pMay->Set_Dash(CMay::PL_1STDASH);
		m_eMayAnim = May_Dash_Start;
		pMesh->Set_Animationset(m_eMayAnim);
		break;
	}
}

void CMay::PlaySit(Engine::CDynamicMesh * pMesh)
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

void CMay::PlayRun(Engine::CDynamicMesh * pMesh)
{
	m_eMayAnim = May_Run;
	pMesh->Set_Animationset(m_eMayAnim);
}

void CMay::PlayClimbing(CMay* pMay, Engine::CDynamicMesh * pMesh, CMay::PLAYER_STATE* pPreState)
{
	if (*pPreState != CMay::PL_CLIMBING)
	{
		switch (*pPreState)
		{
		case CMay::PL_JUMP:
			m_b2ndJump = false;
			m_b2ndJumpStart = false;
			m_bJumpDash = false;
			m_bJumpDashStart = false;
			m_bGroundPoundStop = false;
			*pPreState = CMay::PL_CLIMBING;
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

void CMay::PlayHang(CMay * pCody, Engine::CDynamicMesh * pMesh, CMay::PLAYER_STATE * pPreState)
{
	switch (m_eMayAnim)
	{
	case May_Ledge_Start:
		pMesh->Set_Animationset(m_eMayAnim);
		if (pMesh->Is_AnimationSetEnd(0.1f))
			m_eMayAnim = May_Ledge_Idle;
		break;
	case May_Ledge_Idle:
		pMesh->Set_Animationset(m_eMayAnim);
		if (m_bRise)
		{
			m_bRise = false;
			m_eMayAnim = May_Ledge_ClimbUp;
		}
		break;
	case May_Ledge_ClimbUp:
		pMesh->Set_Animationset(m_eMayAnim);
		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			m_bEndRise = true;
		}
		break;
	default:
		m_eMayAnim = May_Ledge_Start;
		break;
	}
}

void CMay::PlaySlide(CMay* pMay, Engine::CDynamicMesh* pMesh, CMay::PLAYER_STATE* ePreState)
{
	switch (m_eMayAnim)
	{
	case May_Hose_Inside:
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		if (pMay->Get_SlideEnd())
			m_eMayAnim = May_Hose_BlowOut;
		break;
	case May_Hose_BlowOut:
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
			m_eMayAnim = May_VacuumLaunch_Idle;
		break;
	case May_VacuumLaunch_Idle:
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
			m_eMayAnim = May_VacuumLaunch_IdleEnd;
		break;
	case May_VacuumLaunch_IdleEnd:
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		break;
	default:
		m_eMayAnim = May_Hose_Inside;
		break;
	}
}

void CMay::PlayRide(CMay * pMay, Engine::CDynamicMesh * pMesh, CMay::PLAYER_STATE * ePreState)
{
	switch (m_eMayAnim)
	{
	case May_Jump:
		pMesh->Set_Animationset(m_eMayAnim);
		if (pMay->Get_RideEnd())
		{
			m_eMayAnim = May_Hose_Start;
			m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.01f);
		}
		else if (pMesh->Is_AnimationSetEnd(0.1f))
			m_eMayAnim = May_Jump_Fall;
		break;
	case May_Jump_Fall:
		pMesh->Set_Animationset(m_eMayAnim);
		if (pMay->Get_RideEnd() && !pMay->Get_TakeOff())
		{
			m_eMayAnim = May_Hose_Start;
			m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.01f);
		}
		break;
	case May_Hose_Start:
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			m_eMayAnim = May_Hose_Idle;
			m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.01f);
		}
		break;
	case May_Hose_Idle:
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.09f);
		if (m_bTakeOff)
			m_eMayAnim = May_Hose_End;
		else if (pMay->Click_Left())
			m_eMayAnim = May_Hose_Left;
		else if (pMay->Click_Right())
			m_eMayAnim = May_Hose_Right;
		break;
	case May_Hose_Up:
		pMesh->Set_Animationset(m_eMayAnim);
		if (m_bTakeOff)
			m_eMayAnim = May_Hose_End;
		else if (!pMay->Click_Down())
			m_eMayAnim = May_Hose_Idle;
		break;
	case May_Hose_Left:
		pMesh->Set_Animationset(m_eMayAnim);
		if (m_bTakeOff)
			m_eMayAnim = May_Hose_End;
		else if (!pMay->Click_Left())
			m_eMayAnim = May_Hose_Idle;
		break;
	case May_Hose_Right:
		pMesh->Set_Animationset(m_eMayAnim);
		if (m_bTakeOff)
			m_eMayAnim = May_Hose_End;
		else if (!pMay->Click_Right())
			m_eMayAnim = May_Hose_Idle;
		break;
	case May_Hose_End:
		pMesh->Set_Animationset(m_eMayAnim);
		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			pMay->Set_RideJumpMax();
			m_eMayAnim = May_Jump_Fall;
		}
		break;
	case May_Hose_SuckedIn:
		break;
	case May_Hose_Inside:
		break;
	case May_Hose_BlowOut:
		break;
	default:
		m_eMayAnim = May_Jump;
		break;
	}

	if (true == m_bTakeOff)
		m_bTakeOff = false;
}

void CMay::PlayLever(CMay * pMay, Engine::CDynamicMesh * pMesh, CMay::PLAYER_STATE * ePreState)
{
	if (pMay->Get_LeverStart())
	{
		switch (m_eLever)
		{
		case CMay::LEFT_TO_RIGHT:
			m_eMayAnim = May_LeverLeft;
			pMesh->Set_Animationset(m_eMayAnim);
			if (pMesh->Is_AnimationSetEnd(0.1f))
			{
				pMay->Set_Lever(true);
				pMesh->Set_Animationset(May_Idle, 1.0f, 0.1f);
			}
			break;
		case CMay::RIGHT_TO_LEFT:
			m_eMayAnim = May_LeverRight;
			pMesh->Set_Animationset(m_eMayAnim);
			if (pMesh->Is_AnimationSetEnd(0.1f))
			{
				pMay->Set_Lever(true);
				pMesh->Set_Animationset(May_Idle, 1.0f, 0.09f);
			}
			break;
		}
	}
	else
	{
		pMesh->Set_Animationset(May_Walk);
	}
}

void CMay::PlayHangSwitch(CMay* pMay, Engine::CDynamicMesh* pMesh, CMay::PLAYER_STATE* ePreState)
{
	switch (m_eMayAnim)
	{
	case May_PowerSwitch_Start:
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.01f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
			m_eMayAnim = May_PowerSwitch_Idle;
		break;
	case May_PowerSwitch_Idle:
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.01f);
		pMay->Set_SwitchReady();
		if (m_bHangSwitchSuccess)
			m_eMayAnim = May_PowerSwitch_Success;
		break;
	case May_PowerSwitch_Success:
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.01f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			pMay->Set_Success();

			const D3DXFRAME_DERIVED* pBone = pMesh->Get_FrameByName("Align");
			_matrix matBone = pBone->CombinedTranformationMatrix;
			_matrix matPlayer;
			pMay->Get_Component(L"Com_Transform", ID_DYNAMIC)->Is<CTransform>()->Get_WorldMatrix(&matPlayer);

			_matrix matCombined = matBone * matPlayer;
			pMay->Set_SwitchEndPos(_vec3(matCombined._41, matCombined._42, matCombined._43));
		}
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

void CMay::PlayFloat(CMay * pCody, Engine::CDynamicMesh * pMesh, CMay::PLAYER_STATE * ePreState)
{
	m_eMayAnim = May_Zerogravity_Idle;
	pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
}

void CMay::PlayFall(CMay * pMay, Engine::CDynamicMesh * pMesh, CMay::PLAYER_STATE * ePreState)
{
	m_eMayAnim = May_Jump_Fall;
	pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
}

void CMay::PlayPush(CMay * pMay, Engine::CDynamicMesh * pMesh, CMay::PLAYER_STATE * ePreState)
{
	switch (m_eMayAnim)
	{
	case May_Push:
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		if (!pMay->Click_Up())
			m_eMayAnim = May_Push_Idle;
		break;
	case May_Push_Idle:
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		if (pMay->Click_Up())
			m_eMayAnim = May_Push;
		break;
	default:
		m_eMayAnim = May_Push_Idle;
		break;
	}
}

void CMay::PlayAttach()
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

void CMay::PlayHandle()
{
	switch (m_eMayAnim)
	{
	case May_Valve_Idle:
		m_pMeshCom->Set_Animationset(m_eMayAnim);
		if (true == m_bHandleRot)
			m_eMayAnim = May_Valve_RotateRight;
		break;
	case May_Valve_RotateRight:
		m_pMeshCom->Set_Animationset(m_eMayAnim);
		if (false == m_bHandleRot)
			m_eMayAnim = May_Valve_Idle;
		break;
	}
}

void CMay::PlayButton()
{
	m_pMeshCom->Set_Animationset(m_eMayAnim);
	if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_bButtonEnd = true;
	}
}

void CMay::PlaySawShoot()
{
	switch (m_eMayAnim)
	{
	case May_Grind_Start:
		m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{
			m_eMayAnim = May_Grind_Idle;
		}
		break;
	case May_Grind_Idle:
		m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		break;
	case May_VacuumLaunch_Idle:
		m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		break;
	}
}

void CMay::PlaySawSliding()
{
	switch (m_eMayAnim)
	{
	case May_Slide_Start:
		m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
			m_eMayAnim = May_Slide;
		break;
	case May_Slide:
		m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		break;
	case May_Slide_Left:
		m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		break;
	case May_Slide_Right:
		m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		break;
	case May_VacuumLaunch_Idle:
		m_pMeshCom->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		break;
	}
}

void CMay::PlayHit()
{
	m_pMeshCom->Set_Animationset(May_Hit, 1.0f, 0.01f);
	if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_bHitEnd = true;
	}
}

void CMay::PlayPickUp(CMay * pMay, Engine::CDynamicMesh * pMesh, CMay::PLAYER_STATE * pPreState)
{
	switch (m_eMayAnim)
	{
	case May_Pickup:
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			m_ePreMayAnim = May_Pickup;
			m_eMayAnim = May_Pickup_Idle;
		}
		break;
	case May_Pickup_Idle:
		if (m_bInsert_Fuse)
		{
			pMesh->Set_Animationset(May_Pickup_PutinSocket);
			m_eMayAnim = May_Pickup_PutinSocket;
			m_bInsert_Fuse = false;
			return;
		}
		if (m_ePreMayAnim == May_Pickup)
			pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.01f);
		else
			pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);

		if (m_bPickUpDrop)
		{
			m_bPickUpDrop = false;
			m_eMayAnim = May_Pickup_PutDown;
		}
		else if (pMay->Get_PickUpJump())
			m_eMayAnim = May_Pickup_Jump_Start;
		else if (m_bPickUpMove)
			m_eMayAnim = May_Pickup_Walk;
		break;
	case May_Pickup_Walk:
		if (m_bInsert_Fuse)
		{
			pMesh->Set_Animationset(May_Pickup_PutinSocket);
			m_eMayAnim = May_Pickup_PutinSocket;
			m_bInsert_Fuse = false;
			return;
		}
		pMesh->Set_Animationset(m_eMayAnim);
		if (m_bPickUpDrop)
		{
			m_bPickUpDrop = false;
			m_eMayAnim = May_Pickup_PutDown;
		}
		else if (pMay->Get_PickUpJump())
			m_eMayAnim = May_Pickup_Jump_Start;
		else if (!m_bPickUpMove)
		{
			m_eMayAnim = May_Pickup_Idle;
			m_ePreMayAnim = May_Pickup_Walk;
		}
		break;
	case May_Pickup_Jump_Start:
		pMesh->Set_Animationset(m_eMayAnim);
		if (false == pMay->Get_PickUpJump())
			m_eMayAnim = May_Pickup_Jump_Land;
		else if (pMesh->Is_AnimationSetEnd(0.1f))
			m_eMayAnim = May_Pickup_Jump_Fall;
		break;
	case May_Pickup_Jump_Fall:
		pMesh->Set_Animationset(m_eMayAnim);
		if (false == pMay->Get_PickUpJump())
			m_eMayAnim = May_Pickup_Jump_Land;
		break;
	case May_Pickup_Jump_Land:
		pMesh->Set_Animationset(m_eMayAnim);
		if (m_bPickUpMove)
			m_eMayAnim = May_Pickup_Walk;
		else if (pMesh->Is_AnimationSetEnd(0.1f))
			m_eMayAnim = May_Pickup_Idle;
		break;
	case May_Pickup_PutDown:
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			const D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName("FaceManager");
			_float4x4 matBone = pBone->CombinedTranformationMatrix;
			_float4x4 matWorld;
			m_pTransformCom->Get_WorldMatrix(&matWorld);

			_float3 vPos = m_pTransformCom->Get_Position();
			matBone *= matWorld;
			m_pController->setFootPosition(PxExtendedVec3(matBone._41, matBone._42, matBone._43));
			m_pTransformCom->Set_Pos(_vec3(matBone._41, matBone._42, matBone._43));
			m_pTransformCom->Update_Component(m_fTimeDelta);
			m_pMeshCom->Set_Animationset(May_Idle, 1.0f, 0.01f);

			if (nullptr != m_pPickUpTarget)
			{
				static_cast<CFuseNPC*>(m_pPickUpTarget)->Set_Drop();
				m_pPickUpTarget = nullptr;
			}
			m_bPickUpEnd = true;
		}
		break;
	case May_Pickup_PutinSocket:
		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			pMay->Set_Insert(true);
			m_bPickUpStart = false;

		}
		break;
	default:
		m_eMayAnim = May_Pickup;
		break;
	}
}

#pragma region Crouch
void CMay::Crouch_Idle(Engine::CDynamicMesh * pMesh)
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

void CMay::Crouch_Walk_Start(Engine::CDynamicMesh * pMesh)
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

void CMay::Crouch_Walk(Engine::CDynamicMesh * pMesh)
{

	if (!m_bCrouchMove)
	{
		m_eMayAnim = May_Crouch_Idle;
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
	}
}

void CMay::Crouch_Start(Engine::CDynamicMesh * pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		m_eMayAnim = May_Crouch_Idle;
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
	}
}

#pragma endregion Crouch
#pragma region Jump

void CMay::Jump(Engine::CDynamicMesh * pMesh)
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
		pMesh->Set_Animationset(m_eMayAnim, 1.5f, 0.1f);
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

void CMay::Jump_Fall(Engine::CDynamicMesh * pMesh)
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
		pMesh->Set_Animationset(m_eMayAnim, 1.5f, 0.01f);
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

void CMay::Jump_Dash(CMay* pCody, Engine::CDynamicMesh * pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		pCody->Set_JumpDashStop();
		m_eMayAnim = May_Jump_Fall;
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.08f);
	}
}

void CMay::SecondJump(Engine::CDynamicMesh * pMesh)
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
		pMesh->Set_Animationset(m_eMayAnim, 1.5f, 0.1f);
		return;
	}
	else if (pMesh->Is_AnimationSetEnd(0.1f))
		m_eMayAnim = May_Jump_Fall;
}

void CMay::GroundPound_Start(CMay * pCody, Engine::CDynamicMesh * pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		pCody->Set_Jump(CMay::PL_GROUNDPOUND, true);
		m_eMayAnim = May_GroundPound_Fall;
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
	}
}

void CMay::GroundPound_Fall(CMay* pMay, Engine::CDynamicMesh * pMesh)
{
	if (m_bGroundPoundStop)
	{
		m_eMayAnim = May_GroundPound_Land;
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
	}
}

void CMay::GroundPound_Land(Engine::CDynamicMesh * pMesh)
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

void CMay::GroundPound_End(CMay * pMay, Engine::CDynamicMesh * pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		m_eMayAnim = May_Idle;
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.05f);
		pMay->Set_Jump(CMay::PL_GROUNDPOUND_STOP);

		m_b2ndJump = false;
		m_b2ndJumpStart = false;
		m_bJumpDash = false;
		m_bJumpDashStart = false;
	}
}
#pragma endregion Jump

void CMay::Set_Insert_Fuse()
{
	m_bInsert_Fuse = true;
}

void CMay::Set_ClearJump()
{
	m_b2ndJumpStart = false;
	m_b2ndJump = false;
	m_bJumpDash = false;
	m_bJumpDashStart = false;
}

