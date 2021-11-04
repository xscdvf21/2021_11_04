#include "stdafx.h"
#include "CodyWizard_Animation.h"

#include "Export_Function.h"

IMPLEMENT_SINGLETON(CCodyWizard_Anim)

CCodyWizard_Anim::CCodyWizard_Anim()
{
}

CCodyWizard_Anim::~CCodyWizard_Anim()
{
	Free();
}

void CCodyWizard_Anim::ReadyCodyAnimation()
{
	m_eCodyAnim = CodyWizard_Idle;
	m_ePreCodyAnim = CodyWizard_Idle;
}

void CCodyWizard_Anim::ChangeAnimation(CCodyWizard * pCody, CCodyWizard::PLAYER_STATE eCurState, CCodyWizard::PLAYER_STATE * pPreState, CCodyWizard::PLAYER_ATT_STATE eAttState, Engine::CDynamicMesh * pMesh)
{
	switch (eCurState)
	{
	case CCodyWizard::PL_IDLE:
		PlayIdle(pMesh, pPreState, pCody);	
		break;
	case CCodyWizard::PL_MOVE:
		PlayWalk(pMesh);
		break;
	case CCodyWizard::PL_ATT:
		PlayAtt(pCody, pMesh, eAttState);
		break;
	case CCodyWizard::PL_DASH:
		PlayBlink(pCody, pMesh);
		break;
	case CCodyWizard::PL_HIT:
		PlayHit(pCody, pMesh);
		break;
	}
}

void CCodyWizard_Anim::PreAnimation(Engine::CDynamicMesh* pMesh)
{
	if (pMesh->Is_AnimationSetEnd(0.3f))
		m_ePreCodyAnim = m_eCodyAnim;
}

void CCodyWizard_Anim::PlayAnimation(CCodyWizard * pCody, Engine::CDynamicMesh * pMesh, const _float & fTimeDelta, CCodyWizard::PLAYER_STATE eCurState, CCodyWizard::PLAYER_STATE * pPreState, CCodyWizard::PLAYER_ATT_STATE eAttState)
{
	PreAnimation(pMesh);
	ChangeAnimation(pCody, eCurState, pPreState, eAttState, pMesh);
	pMesh->Play_Animationset(fTimeDelta);
}

void CCodyWizard_Anim::PlayIdle(Engine::CDynamicMesh * pMesh, CCodyWizard::PLAYER_STATE * pPreState, CCodyWizard * pCody)
{



	for (_uint i = 0; i < Sound_IceUti; ++i)
	{
		m_bSound[i] = false;
	}
	pCody->Set_DashEffect();

	//이건 지속시간이고 사운드가 길기때문에 따로초기화 정지시키고 초기화해줌.
	if (m_bSound[Sound_IceUti])
	{
		StopSound(CSoundMgr::Cody_Wizard);
		m_bSound[Sound_IceUti] = false;
	}

	if (*pPreState == CCodyWizard::PL_ATT)
	{
		m_eCodyAnim = CodyWizard_Idle;
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.08f);
		*pPreState = CCodyWizard::PL_IDLE;
	}
	else if (*pPreState == CCodyWizard::PL_DASH)
	{
		m_eCodyAnim = CodyWizard_Idle;
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		*pPreState = CCodyWizard::PL_IDLE;
	}
	else if (*pPreState == CCodyWizard::PL_HIT)
	{
		m_eCodyAnim = CodyWizard_Idle;
		pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);
		*pPreState = CCodyWizard::PL_IDLE;
	}
	else
	{
		m_eCodyAnim = CodyWizard_Idle;
		pMesh->Set_Animationset(m_eCodyAnim);
	}
}

void CCodyWizard_Anim::PlayWalk(Engine::CDynamicMesh* pMesh)
{

	Engine::PlaySoundW(L"CodyWizard_Walk.wav", CSoundMgr::CHANNELID::Cody_WizardWalk, 1.f);

	m_eCodyAnim = CodyWizard_Walk;
	pMesh->Set_Animationset(m_eCodyAnim);
}

void CCodyWizard_Anim::PlayAtt(CCodyWizard * pCody, Engine::CDynamicMesh * pMesh, CCodyWizard::PLAYER_ATT_STATE eAttState)
{
	switch (eAttState)
	{
	case CCodyWizard::ATT_BASIC:
		BasicAttack(pCody, pMesh);
		break;
	case CCodyWizard::ATT_ULTIMATE:
		UltimateAttack(pCody, pMesh);
		break;
	case CCodyWizard::ATT_ORB:
		OrbAttack(pCody, pMesh);
		break;
	}
}

void CCodyWizard_Anim::PlayBlink(CCodyWizard * pCody, Engine::CDynamicMesh * pMesh)
{
	if (pCody->Get_BlinkEnd())
	{

		if (!m_bSound[Sound_Blink])
		{
			pCody->Set_DashCoolTime();
			pCody->Set_DashEffect(2);
			StopSound(CSoundMgr::CHANNELID::Cody_WizardDash);
			PlaySoundW(L"CodyWizard_Dash.wav", CSoundMgr::CHANNELID::Cody_WizardDash);

			m_bSound[Sound_Blink] = true;
		}
		m_eCodyAnim = CodyWizard_Blink;
		pMesh->Set_Animationset(m_eCodyAnim, 0.8f);

		
		if (pMesh->Is_AnimationSetEnd(0.1f))
		{
			pCody->Set_DashEnd();
			m_bSound[Sound_Blink] = false;
		}
	}
}

void CCodyWizard_Anim::PlayHit(CCodyWizard * pCOdy, Engine::CDynamicMesh * pMesh)
{
	m_eCodyAnim = CodyWizard_Damage;
	pMesh->Set_Animationset(m_eCodyAnim);

	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		pCOdy->Set_KnockBack(false);
		pCOdy->Set_HitEnd(true);
	}
}

void CCodyWizard_Anim::Set_AttState()
{
	m_byAttIndex++;
	if (m_byAttIndex > ThirdAtt)
		m_byAttIndex = ThirdAtt;

	m_eAtt = (AttAnimState)m_byAttIndex;
}

void CCodyWizard_Anim::BasicAttack(CCodyWizard * pCody, Engine::CDynamicMesh * pMesh)
{
	switch (m_eAtt)
	{
	case CCodyWizard_Anim::FirstAtt:
		FirstAttack(pCody, pMesh);
		break;
	case CCodyWizard_Anim::SecondAtt:
		SecondAttack(pCody, pMesh);
		break;
	case CCodyWizard_Anim::ThirdAtt:
		ThirdAttack(pCody, pMesh);
		break;
	}
}

void CCodyWizard_Anim::FirstAttack(CCodyWizard * pCody, Engine::CDynamicMesh * pMesh)
{
	if (!m_bSound[Sound_FirstAtt])
	{
		pCody->Set_AttDamage(CRandoms()(3, 7));
		pCody->CreateIceOrbLB(6.f);
		pCody->Set_DefaultAttackEffect(1);
		StopSound(CSoundMgr::CHANNELID::Cody_Wizard);
		PlaySoundW(L"CodyWizard_BasicAttak0.wav", CSoundMgr::CHANNELID::Cody_Wizard, 0.3f);
		m_bSound[Sound_FirstAtt] = true;
	}
	m_eCodyAnim = CodyWizard_Attack3_1;
	pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);

	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		pCody->Set_AttEnd();
		m_byAttIndex = 0;
		m_eAtt = FirstAtt;


	}
}

void CCodyWizard_Anim::SecondAttack(CCodyWizard * pCody, Engine::CDynamicMesh * pMesh)
{

	if (!m_bSound[Sound_SecondAtt])
	{
		pCody->Set_AttDamage(CRandoms()(4, 8));
		pCody->CreateIceOrbLB(7.f);
		pCody->Set_DefaultAttackEffect(2);
		StopSound(CSoundMgr::CHANNELID::Cody_Wizard);
		PlaySoundW(L"CodyWizard_BasicAttak1.wav", CSoundMgr::CHANNELID::Cody_Wizard, 0.3f);
		m_bSound[Sound_SecondAtt] = true;
	}

	m_eCodyAnim = CodyWizard_Attack2_1;
	pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);

	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		pCody->Set_AttEnd();
		m_byAttIndex = 0;
		m_eAtt = FirstAtt;

	}
}

void CCodyWizard_Anim::ThirdAttack(CCodyWizard * pCody, Engine::CDynamicMesh * pMesh)
{

	if (!m_bSound[Sound_ThirAtt])
	{
		pCody->Set_AttDamage(CRandoms()(5, 10));
		pCody->CreateIceOrbLB(8.f);
		pCody->Set_DefaultAttackEffect(3);
		StopSound(CSoundMgr::CHANNELID::Cody_Wizard);
		PlaySoundW(L"CodyWizard_BasicAttak2.wav", CSoundMgr::CHANNELID::Cody_Wizard);		

		if (m_iSound[Sound_ThirAtt] == 0)
		{
			StopSound(CSoundMgr::CHANNELID::Cody_Wizard);
			PlaySoundW(L"CodyWizard_BasicAttak2_1.wav", CSoundMgr::CHANNELID::Cody_Wizard, 0.3f);
		}
		else if (m_iSound[Sound_ThirAtt] == 1)
		{
			StopSound(CSoundMgr::CHANNELID::Cody_Wizard);
			PlaySoundW(L"CodyWizard_BasicAttak2_3.wav", CSoundMgr::CHANNELID::Cody_Wizard, 0.3f);
		}
		else if (m_iSound[Sound_ThirAtt] == 2)
		{
			StopSound(CSoundMgr::CHANNELID::Cody_Wizard);
			PlaySoundW(L"CodyWizard_BasicAttak2_2.wav", CSoundMgr::CHANNELID::Cody_Wizard, 0.3f);
		}


		m_bSound[Sound_ThirAtt] = true;
		m_iSound[Sound_ThirAtt] ++;
	}

	if (m_iSound[Sound_ThirAtt] > 2)
	{
		m_iSound[Sound_ThirAtt] = 0;
	}


	m_eCodyAnim = CodyWizard_Attack1_1;
	pMesh->Set_Animationset(m_eCodyAnim, 1.0f, 0.1f);

	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		pCody->Set_AttEnd();
		m_byAttIndex = 0;
		m_eAtt = FirstAtt;

	}
}

void CCodyWizard_Anim::OrbAttack(CCodyWizard * pCody, Engine::CDynamicMesh * pMesh)
{


	if (!m_bSound[Sound_IceOrb])
	{
		pCody->CreateIceOrbRB();
		pCody->Set_IceCoolTime();
		if (m_iSound[Sound_IceOrb] == 0)
		{
			StopSound(CSoundMgr::CHANNELID::Cody_WizardIceOrb);
			PlaySoundW(L"CodyWizard_IceOrb1.wav", CSoundMgr::CHANNELID::Cody_WizardIceOrb, 0.25f);
		}
		else if (m_iSound[Sound_IceOrb] == 1)
		{
			StopSound(CSoundMgr::CHANNELID::Cody_WizardIceOrb);
			PlaySoundW(L"CodyWizard_IceOrb0.wav", CSoundMgr::CHANNELID::Cody_WizardIceOrb, 0.25f);
		}
		else if (m_iSound[Sound_IceOrb] == 2)
		{
			StopSound(CSoundMgr::CHANNELID::Cody_WizardIceOrb);
			PlaySoundW(L"CodyWizard_IceOrb2.wav", CSoundMgr::CHANNELID::Cody_WizardIceOrb, 0.25f);
		}


		m_bSound[Sound_IceOrb] = true;
		m_iSound[Sound_IceOrb] ++;
	}

	if (m_iSound[Sound_IceOrb] > 2)
	{
		m_iSound[Sound_IceOrb] = 0;
	}

	m_eCodyAnim = CodyWizard_OrbAttack;
	pMesh->Set_Animationset(m_eCodyAnim);


	if (pMesh->Is_AnimationSetEnd(0.1f))
	{
		//m_bSound[Sound_IceOrb] = false;
		pCody->Set_AttEnd();
	}
}

void CCodyWizard_Anim::UltimateAttack(CCodyWizard * pCOdy, Engine::CDynamicMesh * pMesh)
{

	if (!m_bSound[Sound_IceUti])
	{

		StopSound(CSoundMgr::CHANNELID::Cody_Wizard);
		PlaySoundW(L"MayCastle_Wind0.wav", CSoundMgr::CHANNELID::Cody_Wizard, 0.25f);

		m_bSound[Sound_IceUti] = true;
	}

	m_eCodyAnim = CodyWizard_BeamAttack;
	pMesh->Set_Animationset(m_eCodyAnim);
}


void CCodyWizard_Anim::Free()
{
}
