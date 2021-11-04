#include "stdafx.h"
#include "MayCastle_Animation.h"
#include "ToySword.h"

#include "Export_Function.h"

IMPLEMENT_SINGLETON(CMayCastle_Anim)

CMayCastle_Anim::CMayCastle_Anim()
{
}

CMayCastle_Anim::~CMayCastle_Anim()
{
	Free();
}

void CMayCastle_Anim::ReadyMayAnimation()
{
	m_eMayAnim = MayCastle_Idle;
	m_ePreMayAnim = MayCastle_Idle;
}

void CMayCastle_Anim::ChangeAnimation(CMayCastle* pMay, CMayCastle::PLAYER_STATE eCurState, CMayCastle::PLAYER_STATE* pPreState, CMayCastle::PLAYER_ATT_STATE eAttState, Engine::CDynamicMesh* pMesh)
{
	switch (eCurState)
	{
	case CMayCastle::PL_IDLE:
		PlayIdle(pMay, pMesh, pPreState);
		break;
	case CMayCastle::PL_MOVE:
		PlayWalk(pMay, pMesh, pPreState);
		break;
	case CMayCastle::PL_ATT:
		PlayAtt(pMay, pMesh,pPreState, eAttState);
		break;
	case CMayCastle::PL_DASH:
		PlayDash(pMay, pMesh, pPreState);
		break;
	case CMayCastle::PL_HIT:
		PlayHit(pMay, pMesh, pPreState);
		break;
		/*case CMayCastle::PL_DEAD:
			PlayDead(pMay, pMesh, pPreState);
			break;*/
	}
}

void CMayCastle_Anim::PreAnimation(Engine::CDynamicMesh* pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.3f))
		m_ePreMayAnim = m_eMayAnim;
}

void CMayCastle_Anim::PlayAnimation(CMayCastle* pCody, CDynamicMesh* pMesh, const _float& fTimeDelta, CMayCastle::PLAYER_STATE eCurState, CMayCastle::PLAYER_STATE* pPreState, CMayCastle::PLAYER_ATT_STATE eAttState)
{
	PreAnimation(pMesh);
	ChangeAnimation(pCody, eCurState, pPreState, eAttState, pMesh);
	pMesh->Play_Animationset(fTimeDelta);
}

void CMayCastle_Anim::PlayIdle(CMayCastle* pMay, Engine::CDynamicMesh* pMesh, CMayCastle::PLAYER_STATE* pPreState)
{

	for (_uint i = 0; i < Sound_WindAtt; ++i)
	{
		m_bSound[i] = false;

	}
	if (m_bSound[Sound_WindAtt])
	{

		StopSound(CSoundMgr::May_Castle);
		m_bSound[Sound_WindAtt] = false;
	}



	if (*pPreState == CMayCastle::PL_HIT)
	{
		m_eMayAnim = MayCastle_Idle;
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		*pPreState = CMayCastle::PL_IDLE;
	}
	else if (*pPreState == CMayCastle::PL_ATT)
	{
		m_eMayAnim = MayCastle_Idle;
		pMay->Set_AttAble(false);
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.08f);
		*pPreState = CMayCastle::PL_IDLE;
	}
	else if (*pPreState == CMayCastle::PL_DASH)
	{
		m_eMayAnim = MayCastle_Idle;
		pMesh->Set_Animationset(m_eMayAnim, 1.0f, 0.1f);
		*pPreState = CMayCastle::PL_IDLE;
	}

	else
	{
		m_eMayAnim = MayCastle_Idle;
		pMesh->Set_Animationset(m_eMayAnim);
	}
}

void CMayCastle_Anim::PlayWalk(CMayCastle* pMay, Engine::CDynamicMesh* pMesh, CMayCastle::PLAYER_STATE* pPreState)
{

	PlaySoundW(L"MayCastle_Walk.wav", CSoundMgr::CHANNELID::May_CastleWalk, 1.f);

	if (*pPreState == CMayCastle::PL_ATT)
		pMay->Set_AttAble(false);

	m_eMayAnim = MayCastle_Walk;
	pMesh->Set_Animationset(m_eMayAnim);
}

void CMayCastle_Anim::PlayAtt(CMayCastle* pMay, Engine::CDynamicMesh * pMesh, CMayCastle::PLAYER_STATE* pPreState, CMayCastle::PLAYER_ATT_STATE eAttState)
{
	switch (eAttState)
	{
	case CMayCastle::ATT_BASIC:
		BasicAttack(pMay, pMesh);
		break;
	case CMayCastle::ATT_ULTIMATE:
		UltimateAttack(pMay, pMesh);
		break;
	case CMayCastle::ATT_SMASH:
		SmashAttack(pMay, pMesh);
		break;
	}

	if (m_eCurAtt != m_ePreAtt)
	{
		m_ePreAtt = m_eCurAtt;

	}
}

void CMayCastle_Anim::PlayDash(CMayCastle * pMay, Engine::CDynamicMesh * pMesh, CMayCastle::PLAYER_STATE* pPreState)
{
	if (!m_bSound[Sound_Blink])
	{
		pMay->Set_DashCoolTime();
		if (m_iSound[Sound_Blink] == 0)
		{
			StopSound(CSoundMgr::CHANNELID::May_CastleDash);
			PlaySoundW(L"MayCastle_Dash0.wav", CSoundMgr::CHANNELID::May_CastleDash, 0.25f);
		}
		else if (m_iSound[Sound_Blink] == 1)
		{
			StopSound(CSoundMgr::CHANNELID::May_CastleDash);
			PlaySoundW(L"MayCastle_Dash1.wav", CSoundMgr::CHANNELID::May_CastleDash, 0.25f);
		}
		else if (m_iSound[Sound_Blink] == 2)
		{
			StopSound(CSoundMgr::CHANNELID::May_CastleDash);
			PlaySoundW(L"MayCastle_Dash2.wav", CSoundMgr::CHANNELID::May_CastleDash, 0.25f);
		}


		m_iSound[Sound_Blink] ++;
		m_bSound[Sound_Blink] = true;
	}

	if (m_iSound[Sound_Blink] > 2)
	{
		m_iSound[Sound_Blink] = 0;
	}

	if (*pPreState == CMayCastle::PL_ATT)
		pMay->Set_AttAble(false);

	m_eMayAnim = MayCastle_Dash;
	pMesh->Set_Animationset(m_eMayAnim, 2.0f);

	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		pMay->Set_DashEnd();
		m_bSound[Sound_Blink] = false;
	}

}

void CMayCastle_Anim::PlayHit(CMayCastle * pMay, Engine::CDynamicMesh * pMesh, CMayCastle::PLAYER_STATE * pPreState)
{
	m_eMayAnim = MayCastle_Damage;
	pMesh->Set_Animationset(m_eMayAnim);

	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		pMay->Set_KnockBack(false);
		pMay->Set_HitEnd(true);
	}

}

//void CMayCastle_Anim::PlayDead(CMayCastle * pMay, Engine::CDynamicMesh * pMesh, CMayCastle::PLAYER_STATE * pPreState)
//{
//}

void CMayCastle_Anim::Set_AttState(CMayCastle* pMay)
{
	m_byAttIndex++;
	if (m_byAttIndex > ThirdAtt)
		m_byAttIndex = ThirdAtt;

	m_eAtt = (AttAnimState)m_byAttIndex;
	//pMay->Get_Sword()->Clear_CollisionList();
}

void CMayCastle_Anim::FirstAttack(CMayCastle * pMay, Engine::CDynamicMesh * pMesh)
{


	if (!m_bSound[Sound_FirstAtt])
	{

		CToySword*		pSword = (CToySword*)Engine::Get_GameObject(L"GameObject", L"ToySword");
		pSword->Set_AttDamage(CRandoms()(3, 7));
	
		StopSound(CSoundMgr::CHANNELID::May_Castle);
		PlaySoundW(L"MayCastle_BasicAttack0.wav", CSoundMgr::CHANNELID::May_Castle, 0.3f);
		m_bSound[Sound_FirstAtt] = true;
		pMay->Get_Sword()->Clear_CollisionList();
	}


	m_eMayAnim = MayCastle_Attack1_1;
	pMesh->Set_Animationset(m_eMayAnim);
	// 현재 공격 애니메이션 상태
	// 변경됬을경우 콜라이더 리스트를 초기화하기위한 용도
	m_eCurAtt = FirstAtt;

	// 공격가능한 상태
	pMay->Set_AttAble(true);

	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		pMay->Set_AttEnd();
		pMay->Get_Sword()->Set_AttAble(false);
		m_byAttIndex = 0;
		m_eAtt = FirstAtt;
		

	}
}

void CMayCastle_Anim::SecondAttack(CMayCastle * pMay, Engine::CDynamicMesh * pMesh)
{

	if (!m_bSound[Sound_SecondAtt])
	{
		CToySword*		pSword = (CToySword*)Engine::Get_GameObject(L"GameObject", L"ToySword");
		pSword->Set_AttDamage(CRandoms()(4, 8));

		StopSound(CSoundMgr::CHANNELID::May_Castle);
		PlaySoundW(L"MayCastle_BasicAttack1.wav", CSoundMgr::CHANNELID::May_Castle, 0.3f);
		m_bSound[Sound_SecondAtt] = true;
		pMay->Get_Sword()->Clear_CollisionList();
	}


	m_eMayAnim = MayCastle_Attack1_2;
	pMesh->Set_Animationset(m_eMayAnim);
	// 현재 공격 애니메이션 상태
	// 변경됬을경우 콜라이더 리스트를 초기화하기위한 용도
	m_eCurAtt = SecondAtt;

	// 공격가능한 상태
	pMay->Set_AttAble(true);

	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		pMay->Set_AttEnd();
		pMay->Get_Sword()->Set_AttAble(false);
		m_byAttIndex = 0;
		m_eAtt = FirstAtt;
		pMay->Get_Sword()->Clear_CollisionList();
	}
}

void CMayCastle_Anim::ThirdAttack(CMayCastle * pMay, Engine::CDynamicMesh * pMesh)
{
	if (!m_bSound[Sound_ThirAtt])
	{
		CToySword*		pSword = (CToySword*)Engine::Get_GameObject(L"GameObject", L"ToySword");
		pSword->Set_AttDamage(CRandoms()(5, 10));
		//pMay->Get_Sword()->Clear_CollisionList();
		if (m_iSound[Sound_ThirAtt] == 0)
		{
			StopSound(CSoundMgr::CHANNELID::May_Castle);
			PlaySoundW(L"MayCastle_BasicAttack2_1.wav", CSoundMgr::CHANNELID::May_Castle, 0.3f);
		}
		else if (m_iSound[Sound_ThirAtt] == 1)
		{
			StopSound(CSoundMgr::CHANNELID::May_Castle);
			PlaySoundW(L"MayCastle_BasicAttack2_2.wav", CSoundMgr::CHANNELID::May_Castle, 0.3f);
		}
		else if (m_iSound[Sound_ThirAtt] == 2)
		{
			StopSound(CSoundMgr::CHANNELID::May_Castle);
			PlaySoundW(L"MayCastle_BasicAttack2_3.wav", CSoundMgr::CHANNELID::May_Castle, 0.3f);

		}

		m_bSound[Sound_ThirAtt] = true;
		m_iSound[Sound_ThirAtt] ++;
		pMay->Get_Sword()->Clear_CollisionList();
	}

	if (m_iSound[Sound_ThirAtt] > 2)
	{
		m_iSound[Sound_ThirAtt] = 0;
	}


	m_eMayAnim = MayCastle_Attack1_3;
	pMesh->Set_Animationset(m_eMayAnim);
	// 현재 공격 애니메이션 상태
	// 변경됬을경우 콜라이더 리스트를 초기화하기위한 용도
	m_eCurAtt = ThirdAtt;

	// 공격가능한 상태
	pMay->Set_AttAble(true);

	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		pMay->Set_AttEnd();
		pMay->Get_Sword()->Set_AttAble(false);
		m_byAttIndex = 0;
		m_eAtt = FirstAtt;

	}
	
}

void CMayCastle_Anim::BasicAttack(CMayCastle * pMay, Engine::CDynamicMesh * pMesh)
{
	switch (m_eAtt)
	{
	case CMayCastle_Anim::FirstAtt:
		FirstAttack(pMay, pMesh);
		break;
	case CMayCastle_Anim::SecondAtt:
		SecondAttack(pMay, pMesh);
		break;
	case CMayCastle_Anim::ThirdAtt:
		ThirdAttack(pMay, pMesh);
		break;
	}
}

void CMayCastle_Anim::SmashAttack(CMayCastle * pMay, Engine::CDynamicMesh * pMesh)
{

	if (!m_bSound[Sound_Smash])
	{
		pMay->Set_SmashCoolTime();

		if (m_iSound[Sound_Smash] == 0)
		{
			StopSound(CSoundMgr::CHANNELID::May_CastleSmash);
			PlaySoundW(L"MayCastle_Smash0.wav", CSoundMgr::CHANNELID::May_CastleSmash, 0.25f);
		}
		else if (m_iSound[Sound_Smash] == 1)
		{
			StopSound(CSoundMgr::CHANNELID::May_CastleSmash);
			PlaySoundW(L"MayCastle_Smash1.wav", CSoundMgr::CHANNELID::May_CastleSmash, 0.25f);
		}
		else if (m_iSound[Sound_Smash] == 2)
		{
			StopSound(CSoundMgr::CHANNELID::May_CastleSmash);
			PlaySoundW(L"MayCastle_Smash2.wav", CSoundMgr::CHANNELID::May_CastleSmash, 0.25f);
		}


		m_bSound[Sound_Smash] = true;
		m_iSound[Sound_Smash] ++;
	}

	if (m_iSound[Sound_Smash] > 2)
	{
		m_iSound[Sound_Smash] = 0;
	}

	pMay->Set_SmashEffect(1);

	m_eMayAnim = MayCastle_GroundSmash;
	pMesh->Set_Animationset(m_eMayAnim);
	m_eCurAtt = Smash;
	pMay->Set_AttAble(true);

	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		//m_bSound[Sound_Smash] = false;
		//
		//pMay->Set_SmashAtt();
		pMay->Set_AttEnd();
		pMay->Get_Sword()->Set_AttAble(false);
		pMay->Get_Sword()->Clear_CollisionList();
		m_bSmash = false;
	}
	else if (pMesh->Is_AnimationSetEnd(0.7f))
	{
		if (!m_bSmash)
		{
			pMay->Set_SmashAtt();
			//pMay->SmashAtt();
			pMay->Set_SmashEffect(2);
			m_bSmash = true;
		}
	}
}

void CMayCastle_Anim::UltimateAttack(CMayCastle * pMay, Engine::CDynamicMesh * pMesh)
{
	if (!m_bSound[Sound_WindAtt])
	{
		if (m_iSound[Sound_WindAtt] == 0)
		{
			StopSound(CSoundMgr::CHANNELID::May_Castle);
			PlaySoundW(L"MayCastle_Wind0.wav", CSoundMgr::CHANNELID::May_Castle, 0.3f);
		}
		else if (m_iSound[Sound_WindAtt] == 1)
		{
			StopSound(CSoundMgr::CHANNELID::May_Castle);
			PlaySoundW(L"MayCastle_Wind1.wav", CSoundMgr::CHANNELID::May_Castle, 0.3f);
		}
		else if (m_iSound[Sound_WindAtt] == 2)
		{
			StopSound(CSoundMgr::CHANNELID::May_Castle);
			PlaySoundW(L"MayCastle_Wind2.wav", CSoundMgr::CHANNELID::May_Castle, 0.3f);
		}

		m_bSound[Sound_WindAtt] = true;
		m_iSound[Sound_WindAtt] ++;
	}

	if (m_iSound[Sound_WindAtt] > 2)
	{
		m_iSound[Sound_WindAtt] = 0;
	}

	m_eMayAnim = MayCastle_Ultimate;
	pMesh->Set_Animationset(m_eMayAnim);
	m_eCurAtt = Ultimate;
	pMay->Get_Sword()->Set_AttAble(true);

	if (pMesh->Is_AnimationSetEnd(0.1f, true))
	{
		pMay->Set_WindAtt();
		pMay->Get_Sword()->Set_AttAble(false);
		pMay->Get_Sword()->Clear_CollisionList();
		pMesh->Get_AniCtrl();
	}
}

void CMayCastle_Anim::Free()
{
}