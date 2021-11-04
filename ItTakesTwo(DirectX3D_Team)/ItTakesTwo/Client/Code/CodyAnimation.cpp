#include "stdafx.h"
#include "Cody.h"
#include "FuseNPC.h"

#include "Export_Function.h"
#include "ImageUI.h"

void CCody::ChangeAnimation(CCody* pCody, CCody::PLAYER_STATE eCurState, CCody::PLAYER_STATE* ePreState, Engine::CDynamicMesh* pMesh)
{
	switch (eCurState)
	{
	case CCody::PL_IDLE:
		PlayIdle(pMesh, ePreState);
		break;
	case CCody::PL_MOVE:
		PlayWalk(pMesh, ePreState);
		break;
	case CCody::PL_JUMP:
		PlayJump(pCody, pMesh);
		break;
	case CCody::PL_SIT:
		PlaySit(pMesh);
		break;
	case CCody::PL_DASH:
		PlayDash(pCody, pMesh);
		break;
	case CCody::PL_RUN:
		PlayRun(pMesh);
		break;
	case CCody::PL_HANG:
		PlayHang(pCody, pMesh, ePreState);
		break;
	case CCody::PL_SLIDE:
		PlaySlide(pCody, pMesh, ePreState);
		break;
	case CCody::PL_CLIMBING:
		PlayClimbing(pCody, pMesh, ePreState);
		break;
	case CCody::PL_PICKUP:
		PlayPickUp(pCody, pMesh, ePreState);
		break;
	case CCody::PL_RIDE:
		PlayRide(pCody, pMesh, ePreState);
		break;
	case CCody::PL_LEVER:
		PlayLever(pCody, pMesh, ePreState);
		break;
	case CCody::PL_FLOAT:
		PlayFloat(pCody, pMesh, ePreState);
		break;
	case CCody::PL_FALL:
		PlayFall(pCody, pMesh, ePreState);
		break;
	case CCody::PL_HANGSWITCH:
		PlayHangSwitch(pCody, pMesh, ePreState);
		break;
	case CCody::PL_PUSH:
		PlayPush(pCody, pMesh, ePreState);
		break;
	case CCody::PL_HANDLE:
		PlayHandle();
		break;
	case CCody::PL_BUTTON:
		PlayButton();
		break;
	case CCody::PL_SAW_SHOOT:
		PlaySawShoot();
		break;
	case CCody::PL_SAW_SLIDING:
		PlaySawSliding();
		break;
	case CCody::PL_SHOOT:
		PlaySlide(pCody, pMesh, ePreState);
		break;
	case CCody::PL_HIT:
		PlayHit();
		break;
	}
}

void CCody::PlayAnimation(CCody* pCody, CDynamicMesh* pMesh, const _float& fTimeDelta, CCody::PLAYER_STATE eCurState, CCody::PLAYER_STATE* pPreState)
{
	ChangeAnimation(pCody, eCurState, pPreState, pMesh);
	pMesh->Play_Animationset(fTimeDelta);

}

void CCody::PlayIdle(Engine::CDynamicMesh* pMesh, CCody::PLAYER_STATE* pPreState)
{
	if (*pPreState != CCody::PL_IDLE)
	{
		switch (*pPreState)
		{
		case CCody::PL_JUMP:
		{
			m_b2ndJump = false;
			m_b2ndJumpStart = false;
			m_bJumpDash = false;
			m_bJumpDashStart = false;
			m_bGroundPoundStop = false;
			m_eCodyAnim = Cody_Jump_Land;
			pMesh->Set_Animationset(m_eCodyAnim);
			*pPreState = CCody::PL_IDLE;
			break;
		}
		case CCody::PL_SIT:
		{
			m_eCodyAnim = Cody_Crouch_End;
			pMesh->Set_Animationset(m_eCodyAnim, 0.5f);
			*pPreState = CCody::PL_IDLE;
			break;
		}
		case CCody::PL_DASH:
		{
			if (pMesh->Is_AnimationSetEnd(0.15f))
			{
				m_eCodyAnim = Cody_Idle;
				pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
			}
			*pPreState = CCody::PL_IDLE;
			break;
		}
		case CCody::PL_HANGSWITCH:
		{
			m_eCodyAnim = Cody_Idle;
			pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.01f);
			break;
		}
		case CCody::PL_HANG:
		{
			m_eCodyAnim = Cody_Idle;
			pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.01f);
			break;
		}
		default:
			m_eCodyAnim = Cody_Idle;
			pMesh->Set_Animationset(Cody_Idle);

			// Dash상태 때문에 번거롭게 모든곳에 넣어줌
			*pPreState = CCody::PL_IDLE;
			break;
		}

		return;
	}

	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		m_eCodyAnim = Cody_Idle;
		pMesh->Set_Animationset(Cody_Idle, 1.0f, 0.1f);
	}
}

void CCody::PlayJump(CCody* pCody, Engine::CDynamicMesh* pMesh)
{
	switch (m_eCodyAnim)
	{
	case Cody_Jump:
		Jump(pMesh);
		break;
	case Cody_2ndJump:
		SecondJump(pMesh);
		break;
	case Cody_Jump_Fall:
		Jump_Fall(pMesh);
		break;
	case Cody_Jump_Dash:
		Jump_Dash(pCody, pMesh);
		break;
	case Cody_GroundPound_Start:
		GroundPound_Start(pCody, pMesh);
		break;
	case Cody_GroundPound_Fall:
		GroundPound_Fall(pCody, pMesh);
		break;
	case Cody_GroundPound_Land:
		GroundPound_Land(pMesh);
		break;
	case Cody_GroundPound_End:
		GroundPound_End(pCody, pMesh);
		break;
	}
}

void CCody::PlayWalk(Engine::CDynamicMesh* pMesh, CCody::PLAYER_STATE* pPreState)
{
	if (*pPreState != CCody::PL_MOVE)
	{
		switch (*pPreState)
		{
		case CCody::PL_SIT:
			m_eCodyAnim = Cody_Crouch_Walk_End;
			pMesh->Set_Animationset(m_eCodyAnim);

			*pPreState = CCody::PL_MOVE;
			break;
		case CCody::PL_JUMP:
		{
			m_b2ndJump = false;
			m_b2ndJumpStart = false;
			m_bJumpDash = false;
			m_bJumpDashStart = false;
			m_bGroundPoundStop = false;
			m_eCodyAnim = Cody_Jump_Land;

			*pPreState = CCody::PL_MOVE;
			pMesh->Set_Animationset(m_eCodyAnim);
			break;
		}
		case CCody::PL_DASH:
		{
			if (pMesh->Is_AnimationSetEnd(0.1f))
			{
				m_eCodyAnim = Cody_Walk;
				pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.05f);
			}
			break;
		}
		default:
			m_eCodyAnim = Cody_Walk;
			pMesh->Set_Animationset(m_eCodyAnim);

			// 대쉬 때문에 모든곳에 넣어줌
			*pPreState = CCody::PL_MOVE;
			break;
		}
		return;
	}

	if (pMesh->Is_AnimationSetEnd(0.3f))
	{

		m_eCodyAnim = Cody_Walk;
		pMesh->Set_Animationset(m_eCodyAnim);
	}

}

void CCody::PlayDash(CCody* pCody, Engine::CDynamicMesh* pMesh)
{
	switch (m_eCodyAnim)
	{
	case Cody_Dash_Start:
	{
		if (false == m_bPlaySound[PLAY_DASH])
		{
			CSoundMgr::GetInstance()->StopNPlaySound(L"Cody_Dash_Start.wav", CSoundMgr::CHANNELID::Cody_Dash, m_fEffectvolume);
			m_bPlaySound[PLAY_DASH] = true;
		}

		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			m_eCodyAnim = Cody_Dash_End;
			pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		}
		break;
	}
	case Cody_Dash_End:
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
		pCody->Set_Dash(CCody::PL_1STDASH);
		m_eCodyAnim = Cody_Dash_Start;
		pMesh->Set_Animationset(m_eCodyAnim);
		break;
	}
}

void CCody::PlaySit(Engine::CDynamicMesh* pMesh)
{
	switch (m_eCodyAnim)
	{
	case Cody_Crouch_Start:
		Crouch_Start(pMesh);
		break;
	case Cody_Crouch_Walk_Start:
		Crouch_Walk_Start(pMesh);
		break;
	case Cody_Crouch_Idle:
		Crouch_Idle(pMesh);
		break;
	case Cody_Crouch_Walk:
		Crouch_Walk(pMesh);
		break;
	case Cody_Walk:
		m_eCodyAnim = Cody_Crouch_Walk_Start;
		pMesh->Set_Animationset(m_eCodyAnim, 0.5f);
		break;
	default:
		m_eCodyAnim = Cody_Crouch_Start;
		pMesh->Set_Animationset(m_eCodyAnim, 0.5f);
		break;
	}
}

void CCody::PlayRun(Engine::CDynamicMesh * pMesh)
{
	m_eCodyAnim = Cody_Run;
	pMesh->Set_Animationset(m_eCodyAnim);
}

void CCody::PlayClimbing(CCody* pCody, Engine::CDynamicMesh * pMesh, CCody::PLAYER_STATE* pPreState)
{
	if (*pPreState != CCody::PL_CLIMBING)
	{
		switch (*pPreState)
		{
		case CCody::PL_JUMP:
			m_b2ndJump = false;
			m_b2ndJumpStart = false;
			m_bJumpDash = false;
			m_bJumpDashStart = false;
			m_bGroundPoundStop = false;
			*pPreState = CCody::PL_CLIMBING;
			break;
		}
	}

	switch (m_eCodyAnim)
	{
	case Cody_Wall_Idle:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		if (m_bClimbingJump)
		{
			m_eCodyAnim = Cody_Wall_Jump;
		}
		break;
	case Cody_Wall_Jump:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);

		if (m_bClimbingJumpEnd)
		{
			m_bClimbingJumpEnd = false;
			m_eCodyAnim = Cody_Wall_Idle;
		}
		else if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			m_eCodyAnim = Cody_Jump_Fall;
		}
		break;
	case Cody_Jump_Fall:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.01f);
		if (m_bClimbingJumpEnd)
		{
			m_bClimbingJumpEnd = false;
			m_eCodyAnim = Cody_Wall_Idle;
		}
		break;
	default:
		m_eCodyAnim = Cody_Wall_Idle;
		break;
	}
}

void CCody::PlayHang(CCody * pCody, Engine::CDynamicMesh * pMesh, CCody::PLAYER_STATE * pPreState)
{
	switch (m_eCodyAnim)
	{
	case Cody_Ledge_Start:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
			m_eCodyAnim = Cody_Ledge_Idle;
		break;
	case Cody_Ledge_Idle:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.01f);
		if (m_bRise)
		{
			m_bRise = false;
			m_eCodyAnim = Cody_Ledge_ClimbUp;
		}
		break;
	case Cody_Ledge_Drop:
		break;
	case Cody_Ledge_ClimbUp:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			m_bEndRise = true;
		}
		break;
	default:
		m_eCodyAnim = Cody_Ledge_Start;
		break;
	}
}
void CCody::PlaySlide(CCody* pCody, Engine::CDynamicMesh* pMesh, CCody::PLAYER_STATE* ePreState)
{
	switch (m_eCodyAnim)
	{
	case Cody_Hose_Inside:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		if (pCody->Get_SlideEnd())
			m_eCodyAnim = Cody_VacuumLaunch_Start;
		break;
	case Cody_VacuumLaunch_Start:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
			m_eCodyAnim = Cody_VacuumLaunch_Idle;
		break;
	case Cody_VacuumLaunch_Idle:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.05f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
			m_eCodyAnim = Cody_VacuumLaunch_IdleEnd;
		break;
	default:
		m_eCodyAnim = Cody_Hose_Inside;
		break;
	}
}

void CCody::PlayRide(CCody * pCody, Engine::CDynamicMesh * pMesh, CCody::PLAYER_STATE * ePreState)
{
	switch (m_eCodyAnim)
	{
	case Cody_Jump:
		pMesh->Set_Animationset(m_eCodyAnim);
		if (pCody->Get_RideEnd())
		{
			m_eCodyAnim = Cody_Hose_Start;
			m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.01f);
		}
		else if (pMesh->Is_AnimationSetEnd(0.1f))
			m_eCodyAnim = Cody_Jump_Fall;
		break;
	case Cody_Jump_Fall:
		pMesh->Set_Animationset(m_eCodyAnim);
		if (pCody->Get_RideEnd() && !pCody->Get_TakeOff())
		{
			m_eCodyAnim = Cody_Hose_Start;
			m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.01f);
		}
		break;
	case Cody_Hose_Start:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			m_eCodyAnim = Cody_Hose_Idle;
			m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.01f);
		}
		break;
	case Cody_Hose_Idle:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.09f);
		if (m_bTakeOff)
			m_eCodyAnim = Cody_Hose_End;
		else if (pCody->Click_Left())
			m_eCodyAnim = Cody_Hose_Left;
		else if (pCody->Click_Right())
			m_eCodyAnim = Cody_Hose_Right;
		break;
	case Cody_Hose_Up:
		pMesh->Set_Animationset(m_eCodyAnim);
		if (m_bTakeOff)
			m_eCodyAnim = Cody_Hose_End;
		else if (!pCody->Click_Down())
			m_eCodyAnim = Cody_Hose_Idle;
		break;
	case Cody_Hose_Left:
		pMesh->Set_Animationset(m_eCodyAnim);
		if (m_bTakeOff)
			m_eCodyAnim = Cody_Hose_End;
		else if (!pCody->Click_Left())
			m_eCodyAnim = Cody_Hose_Idle;
		break;
	case Cody_Hose_Right:
		pMesh->Set_Animationset(m_eCodyAnim);
		if (m_bTakeOff)
			m_eCodyAnim = Cody_Hose_End;
		else if (!pCody->Click_Right())
			m_eCodyAnim = Cody_Hose_Idle;
		break;
	case Cody_Hose_End:
		pMesh->Set_Animationset(m_eCodyAnim);
		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			pCody->Set_RideJumpMax();
			m_eCodyAnim = Cody_Jump_Fall;
		}
		break;
	case Cody_Hose_SuckedIn:
		break;
	case Cody_Hose_Inside:
		break;
	case Cody_Hose_BlowOut:
		break;
	default:
		m_eCodyAnim = Cody_Jump;
		break;
	}

	if (true == m_bTakeOff)
		m_bTakeOff = false;
}

void CCody::PlayLever(CCody * pCody, Engine::CDynamicMesh * pMesh, CCody::PLAYER_STATE * ePreState)
{
	if (pCody->Get_LeverStart())
	{
		switch (m_eLever)
		{
		case CCody::LEFT_TO_RIGHT:
			m_eCodyAnim = Cody_LeverLeft;
			pMesh->Set_Animationset(m_eCodyAnim);
			if (pMesh->Is_AnimationSetEnd(0.1f))
			{
				pCody->Set_Lever(true);
				pMesh->Set_Animationset(Cody_Idle, 1.0f, 0.1f);
			}
			break;
		case CCody::RIGHT_TO_LEFT:
			m_eCodyAnim = Cody_LeverRight;
			pMesh->Set_Animationset(m_eCodyAnim);
			if (pMesh->Is_AnimationSetEnd(0.1f))
			{
				pCody->Set_Lever(true);
				pMesh->Set_Animationset(Cody_Idle, 1.0f, 0.09f);
			}
			break;
		}
	}
	else
	{
		pMesh->Set_Animationset(Cody_Walk);
	}
}
void CCody::PlayHangSwitch(CCody* pCody, Engine::CDynamicMesh* pMesh, CCody::PLAYER_STATE* ePreState)
{
	switch (m_eCodyAnim)
	{
	case Cody_PowerSwitch_Start:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.01f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
			m_eCodyAnim = Cody_PowerSwitch_Idle;
		break;
	case Cody_PowerSwitch_Idle:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.01f);
		pCody->Set_SwitchReady();
		if (m_bHangSwitchSuccess)
			m_eCodyAnim = Cody_PowerSwitch_Success;
		break;
	case Cody_PowerSwitch_Success:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.01f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			pCody->Set_Success();

			const D3DXFRAME_DERIVED* pBone = pMesh->Get_FrameByName("Align");
			_matrix matBone = pBone->CombinedTranformationMatrix;
			_matrix matPlayer;
			pCody->Get_Component(L"Com_Transform", ID_DYNAMIC)->Is<CTransform>()->Get_WorldMatrix(&matPlayer);

			_matrix matCombined = matBone * matPlayer;
			pCody->Set_SwitchEndPos(_vec3(matCombined._41, matCombined._42, matCombined._43));
		}
		break;
	case Cody_Jump:
		pMesh->Set_Animationset(m_eCodyAnim, 1.5f, 0.1f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
			m_eCodyAnim = Cody_PowerSwitch_Start;
		break;
	default:
		m_eCodyAnim = Cody_Jump;
		break;
	}
}

void CCody::PlayFloat(CCody * pCody, Engine::CDynamicMesh * pMesh, CCody::PLAYER_STATE * ePreState)
{
	m_eCodyAnim = Cody_Zerogravity_Idle;
	pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
}

void CCody::PlayFall(CCody * pCody, Engine::CDynamicMesh * pMesh, CCody::PLAYER_STATE * ePreState)
{
	m_eCodyAnim = Cody_Jump_Fall;
	pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
}

void CCody::PlayPush(CCody * pCody, Engine::CDynamicMesh * pMesh, CCody::PLAYER_STATE * ePreState)
{
	switch (m_eCodyAnim)
	{
	case Cody_Push:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		if (!pCody->Click_Up())
			m_eCodyAnim = Cody_Push_Idle;
		break;
	case Cody_Push_Idle:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		if (pCody->Click_Up())
			m_eCodyAnim = Cody_Push;
		break;
	default:
		m_eCodyAnim = Cody_Push_Idle;
		break;
	}
}

void CCody::PlayAttach()
{
	switch (m_eCodyAnim)
	{
	case Cody_Wall_Idle:
		m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		if (m_bClimbingJump)
		{
			m_eCodyAnim = Cody_Wall_Jump;
		}
		break;
	case Cody_Wall_Jump:
		m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);

		if (m_bClimbingJumpEnd)
		{
			m_bClimbingJumpEnd = false;
			m_eCodyAnim = Cody_Wall_Idle;
		}
		else if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{
			m_eCodyAnim = Cody_Jump_Fall;
		}
		break;
	case Cody_Jump_Fall:
		m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.01f);
		if (m_bClimbingJumpEnd)
		{
			m_bClimbingJumpEnd = false;
			m_eCodyAnim = Cody_Wall_Idle;
		}
		break;
	default:
		m_eCodyAnim = Cody_Wall_Idle;
		break;
	}
}

void CCody::PlayHandle()
{
	switch (m_eCodyAnim)
	{
	case Cody_Valve_Idle:
		m_pMeshCom->Set_Animationset(m_eCodyAnim);
		if (true == m_bHandleRot)
			m_eCodyAnim = Cody_Valve_RotateRight;
		break;
	case Cody_Valve_RotateRight:
		m_pMeshCom->Set_Animationset(m_eCodyAnim);
		if (false == m_bHandleRot)
			m_eCodyAnim = Cody_Valve_Idle;
		break;
	}
}

void CCody::PlayButton()
{
	m_pMeshCom->Set_Animationset(m_eCodyAnim);
	if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_bButtonEnd = true;
	}
}

void CCody::PlaySawShoot()
{
	switch (m_eCodyAnim)
	{
	case Cody_Grind_Start:
		m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{
			m_eCodyAnim = Cody_Grind_Idle;
		}
		break;
	case Cody_Grind_Idle:
		m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		break;
	case Cody_VacuumLaunch_Idle:
		m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		break;
	}
}

void CCody::PlaySawSliding()
{
	switch (m_eCodyAnim)
	{
	case Cody_Slide_Start:
		m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
		{
			m_eCodyAnim = Cody_Slide;
		}
		break;
	case Cody_Slide:
		m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		break;
	case Cody_Slide_Left:
		m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		break;
	case Cody_Slide_Right:
		m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		break;
	case Cody_VacuumLaunch_Idle:
		m_pMeshCom->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		break;
	}
}

void CCody::PlayHit()
{
	m_pMeshCom->Set_Animationset(Cody_Hit, 1.0f, 0.01f);
	if (m_pMeshCom->Is_AnimationSetEnd(0.1f))
	{
		m_bHitEnd = true;
	}
}

void CCody::PlayPickUp(CCody * pCody, Engine::CDynamicMesh * pMesh, CCody::PLAYER_STATE * pPreState)
{
	switch (m_eCodyAnim)
	{
	case Cody_Pickup:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			m_ePreCodyAnim = Cody_Pickup;
			m_eCodyAnim = Cody_Pickup_Idle;
		}
		break;
	case Cody_Pickup_Idle:
		if (m_bInsert_Fuse)
		{
			pMesh->Set_Animationset(Cody_Pickup_FusePutinSocket);
			m_eCodyAnim = Cody_Pickup_FusePutinSocket;
			m_bInsert_Fuse = false;
			return;
		}
		if (m_ePreCodyAnim == Cody_Pickup)
			pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.01f);
		else
			pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);

		if (m_bPickUpDrop)
		{
			m_bPickUpDrop = false;
			m_eCodyAnim = Cody_Pickup_PutDown;
		}
		else if (pCody->Get_PickUpJump())
			m_eCodyAnim = Cody_Pickup_Jump_Start;
		else if (m_bPickUpMove)
			m_eCodyAnim = Cody_Pickup_Walk;
		break;
	case Cody_Pickup_Walk:
		if (m_bInsert_Fuse)
		{
			pMesh->Set_Animationset(Cody_Pickup_FusePutinSocket);
			m_eCodyAnim = Cody_Pickup_FusePutinSocket;
			m_bInsert_Fuse = false;
			return;
		}
		pMesh->Set_Animationset(Cody_Pickup_Walk);

		if (m_bPickUpDrop)
		{
			m_bPickUpDrop = false;
			m_eCodyAnim = Cody_Pickup_PutDown;
		}
		else if (pCody->Get_PickUpJump())
			m_eCodyAnim = Cody_Pickup_Jump_Start;
		else if (!m_bPickUpMove)
		{
			m_eCodyAnim = Cody_Pickup_Idle;
			m_ePreCodyAnim = Cody_Pickup_Walk;
		}
		break;
	case Cody_Pickup_Jump_Start:
		pMesh->Set_Animationset(m_eCodyAnim);
		if (false == pCody->Get_PickUpJump())
			m_eCodyAnim = Cody_Pickup_Jump_Land;
		else if (pMesh->Is_AnimationSetEnd(0.1f))
			m_eCodyAnim = Cody_Pickup_Jump_Fall;
		break;
	case Cody_Pickup_Jump_Fall:
		pMesh->Set_Animationset(m_eCodyAnim);
		if (false == pCody->Get_PickUpJump())
			m_eCodyAnim = Cody_Pickup_Jump_Land;
		break;
	case Cody_Pickup_Jump_Land:
		pMesh->Set_Animationset(m_eCodyAnim);
		if (m_bPickUpMove)
			m_eCodyAnim = Cody_Pickup_Walk;
		else if (pMesh->Is_AnimationSetEnd(0.1f))
			m_eCodyAnim = Cody_Pickup_Idle;
		break;
	case Cody_Pickup_PutDown:
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
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
			m_pMeshCom->Set_Animationset(Cody_Idle, 1.0f, 0.01f);


			if (nullptr != m_pPickUpTarget)
			{
				static_cast<CFuseNPC*>(m_pPickUpTarget)->Set_Drop();
				m_pPickUpTarget = nullptr;
			}
			m_bPickUpEnd = true;
		}
		break;
	case Cody_Pickup_FusePutinSocket:
		/*if (pMesh->Is_AnimationSetEnd(0.1f) && !pCody->Get_Insert())
		{
			pCody->Insert_Fuse_End();
		}*/
		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			pCody->Set_Insert(true);
			m_bPickUpStart = false;
			
		}
		break;
	default:
		m_eCodyAnim = Cody_Pickup;
		break;
	}
}


#pragma region Crouch

void CCody::Crouch_Idle(Engine::CDynamicMesh * pMesh)
{
	if (m_bCrouchMove)
	{
		m_eCodyAnim = Cody_Crouch_Walk;
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
	}
	else
	{
		m_eCodyAnim = Cody_Crouch_Idle;
		pMesh->Set_Animationset(m_eCodyAnim);
	}
}


void CCody::Crouch_Walk_Start(Engine::CDynamicMesh * pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		if (m_bCrouchMove)
		{
			m_eCodyAnim = Cody_Crouch_Walk;
			pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		}
		else
		{
			m_eCodyAnim = Cody_Crouch_Idle;
			pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		}
	}
}

void CCody::Crouch_Walk(Engine::CDynamicMesh * pMesh)
{
	if (!m_bCrouchMove)
	{
		m_eCodyAnim = Cody_Crouch_Idle;
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
	}
}

void CCody::Crouch_Start(Engine::CDynamicMesh * pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		m_eCodyAnim = Cody_Crouch_Idle;
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
	}
}

#pragma endregion Crouch
#pragma region Jump
void CCody::Jump(Engine::CDynamicMesh * pMesh)
{
	pMesh->Set_Animationset(m_eCodyAnim);
	if (m_bJump[PL_GROUNDPOUND_START])
	{
		m_eCodyAnim = Cody_GroundPound_Start;
		pMesh->Set_Animationset(m_eCodyAnim);
		return;
	}
	else if (m_bJumpDash && !m_bJumpDashStart)
	{
		m_bJumpDashStart = true;
		m_eCodyAnim = Cody_Jump_Dash;
		pMesh->Set_Animationset(m_eCodyAnim, 2.0f, 0.2f);
		return;
	}

	else if (!m_b2ndJumpStart && m_b2ndJump)
	{
		m_b2ndJumpStart = true;
		m_eCodyAnim = Cody_2ndJump;
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		return;
	}

	if (pMesh->Is_AnimationSetEnd(0.3f))
		m_eCodyAnim = Cody_Jump_Fall;
}

void CCody::Jump_Fall(Engine::CDynamicMesh * pMesh)
{
	if (m_bJump[PL_GROUNDPOUND_START])
	{
		m_eCodyAnim = Cody_GroundPound_Start;
		pMesh->Set_Animationset(m_eCodyAnim);
		return;
	}
	else if (m_bJumpDash && !m_bJumpDashStart)
	{
		m_bJumpDashStart = true;
		m_eCodyAnim = Cody_Jump_Dash;
		pMesh->Set_Animationset(m_eCodyAnim, 1.5f, 0.2f);
		return;
	}
	else if (!m_b2ndJumpStart && m_b2ndJump)
	{
		m_b2ndJumpStart = true;
		m_eCodyAnim = Cody_2ndJump;
		pMesh->Set_Animationset(m_eCodyAnim);
		return;
	}

	m_eCodyAnim = Cody_Jump_Fall;
	pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
}

void CCody::Jump_Dash(CCody* pCody, Engine::CDynamicMesh * pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		pCody->Set_JumpDashStop();
		m_eCodyAnim = Cody_Jump_Fall;
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.01f);
	}
}

void CCody::SecondJump(Engine::CDynamicMesh * pMesh)
{
	if (m_bJump[PL_GROUNDPOUND_START])
	{
		m_eCodyAnim = Cody_GroundPound_Start;
		pMesh->Set_Animationset(m_eCodyAnim);
		return;
	}
	else if (m_bJumpDash && !m_bJumpDashStart)
	{
		m_bJumpDashStart = true;
		m_eCodyAnim = Cody_Jump_Dash;
		pMesh->Set_Animationset(m_eCodyAnim, 1.5f, 0.2f);
		return;
	}
	else if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		m_eCodyAnim = Cody_Jump_Fall;
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.08f);
	}
}

void CCody::GroundPound_Start(CCody* pCody, Engine::CDynamicMesh * pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		pCody->Set_Jump(CCody::PL_GROUNDPOUND, true);
		m_eCodyAnim = Cody_GroundPound_Fall;
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
	}
}

void CCody::GroundPound_Fall(CCody* pCody, Engine::CDynamicMesh * pMesh)
{
	if (m_bGroundPoundStop)
	{
		m_eCodyAnim = Cody_GroundPound_Land;
		pMesh->Set_Animationset(m_eCodyAnim);
	}
}

void CCody::GroundPound_Land(Engine::CDynamicMesh * pMesh)
{
	if (false == m_bPlaySound[PLAY_GROUNDPOUND_IMPACT])
	{
		CSoundMgr::GetInstance()->StopNPlaySound(L"Cody_GroundPound_Impact.wav", CSoundMgr::CHANNELID::Cody_Jump, m_fEffectvolume);
		m_bPlaySound[PLAY_GROUNDPOUND_IMPACT] = true;
	}

	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		m_eCodyAnim = Cody_GroundPound_End;
		pMesh->Set_Animationset(m_eCodyAnim);
	}
}

void CCody::GroundPound_End(CCody* pCody, Engine::CDynamicMesh * pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		m_eCodyAnim = Cody_GroundPound_End;
		pMesh->Set_Animationset(m_eCodyAnim);
		pCody->Set_Jump(CCody::PL_GROUNDPOUND_STOP);

		m_b2ndJump = false;
		m_b2ndJumpStart = false;
		m_bJumpDash = false;
		m_bJumpDashStart = false;
		m_bGroundPoundStop = false;
	}
}


#pragma endregion Jump

void CCody::Set_Insert_Fuse()
{
	m_bInsert_Fuse = true;
}

void CCody::Set_ClearJump()
{
	m_b2ndJumpStart = false;
	m_b2ndJump = false;
	m_bJumpDash = false;
	m_bJumpDashStart = false;
}




