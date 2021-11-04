#pragma once
#ifndef __CODYANIMATION_H__

#include "Cody_Wizard.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Client_Define.h"

BEGIN(Engine)

class CDynamicMesh;

END

class CCodyWizard_Anim : public CBase
{
public:
	enum AttAnimState { FirstAtt, SecondAtt, ThirdAtt, AttEnd };
	enum AttSoundID {Sound_Walk,Sound_FirstAtt, Sound_SecondAtt, Sound_ThirAtt, Sound_IceOrb,Sound_Blink, Sound_IceUti,Sound_End};
	DECLARE_SINGLETON(CCodyWizard_Anim)

public:
	explicit	 CCodyWizard_Anim();
	virtual		~CCodyWizard_Anim();

public:
	void		ReadyCodyAnimation();
	void		PlayAnimation(CCodyWizard* pCOdy, Engine::CDynamicMesh* pMesh, const _float& fTimeDelta, CCodyWizard::PLAYER_STATE eCurState, CCodyWizard::PLAYER_STATE* pPreState, CCodyWizard::PLAYER_ATT_STATE eAttState);

private:
	void		ChangeAnimation(CCodyWizard* pCody, CCodyWizard::PLAYER_STATE eCurState, CCodyWizard::PLAYER_STATE* pPreState, CCodyWizard::PLAYER_ATT_STATE eAttState, Engine::CDynamicMesh* pMesh);
	void		PreAnimation(Engine::CDynamicMesh* pMesh);

private:
	void		PlayIdle(Engine::CDynamicMesh* pMesh, CCodyWizard::PLAYER_STATE* pPreState, CCodyWizard * pCody);
	void		PlayWalk(Engine::CDynamicMesh* pMesh);
	void		PlayAtt(CCodyWizard* pCOdy, Engine::CDynamicMesh* pMesh, CCodyWizard::PLAYER_ATT_STATE eAttState);
	void		PlayBlink(CCodyWizard* pCOdy, Engine::CDynamicMesh* pMesh);
	void		PlayHit(CCodyWizard* pCOdy, Engine::CDynamicMesh* pMesh);

public:
	void		Set_AttState();
	void		Set_ClearIndex() { m_byAttIndex = 0; }

private:
	void		BasicAttack(CCodyWizard* pCOdy, Engine::CDynamicMesh* pMesh);
	void		OrbAttack(CCodyWizard* pCOdy, Engine::CDynamicMesh* pMesh);
	void		UltimateAttack(CCodyWizard* pCOdy, Engine::CDynamicMesh* pMesh);

	//BasicAtt
	void		FirstAttack(CCodyWizard* pCOdy, Engine::CDynamicMesh* pMesh);
	void		SecondAttack(CCodyWizard* pCOdy, Engine::CDynamicMesh* pMesh);
	void		ThirdAttack(CCodyWizard* pCOdy, Engine::CDynamicMesh* pMesh);

public:
	virtual void Free() override;

private:
	CodyWizard_AnimID				m_eCodyAnim;
	CodyWizard_AnimID				m_ePreCodyAnim;

	_bool							m_bSound[Sound_End] = {false};
	_uint							m_iSound[Sound_End] = { 0 };
	// Att
	_byte							m_byAttIndex = 0;

	AttAnimState					m_eAtt = FirstAtt;


};

#define __CODYANIMATION_H__
#endif