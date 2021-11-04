#pragma once
#ifndef __MAYCASTLEANIM_H__

#include "May_Castle.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Client_Define.h"

BEGIN(Engine)

class CDynamicMesh;

END

class CMayCastle_Anim : public CBase
{
public:
	enum AttAnimState{ FirstAtt, SecondAtt, ThirdAtt, Smash, Ultimate, AttEnd };
	enum AttSoundID { Sound_Walk, Sound_FirstAtt, Sound_SecondAtt, Sound_ThirAtt,Sound_Smash, Sound_Blink, Sound_WindAtt, Sound_End };

	DECLARE_SINGLETON(CMayCastle_Anim)

public:
	explicit	 CMayCastle_Anim();
	virtual		~CMayCastle_Anim();

public:
	void		ReadyMayAnimation();
	void		PlayAnimation(CMayCastle* pMay, Engine::CDynamicMesh* pMesh, const _float& fTimeDelta, CMayCastle::PLAYER_STATE eCurState, CMayCastle::PLAYER_STATE* pPreState, CMayCastle::PLAYER_ATT_STATE eAttState);

private:
	void		ChangeAnimation(CMayCastle* pCody, CMayCastle::PLAYER_STATE eCurState, CMayCastle::PLAYER_STATE* pPreState, CMayCastle::PLAYER_ATT_STATE eAttState, Engine::CDynamicMesh* pMesh);
	void		PreAnimation(Engine::CDynamicMesh* pMesh);

private:
	void		PlayIdle(CMayCastle* pMay, Engine::CDynamicMesh* pMesh, CMayCastle::PLAYER_STATE* pPreState);
	void		PlayWalk(CMayCastle* pMay, Engine::CDynamicMesh* pMesh, CMayCastle::PLAYER_STATE* pPreState);
	void		PlayAtt(CMayCastle* pMay, Engine::CDynamicMesh* pMesh, CMayCastle::PLAYER_STATE* pPreState, CMayCastle::PLAYER_ATT_STATE eAttState);
	void		PlayDash(CMayCastle* pMay, Engine::CDynamicMesh* pMesh, CMayCastle::PLAYER_STATE* pPreState);
	void		PlayHit(CMayCastle* pMay, Engine::CDynamicMesh* pMesh, CMayCastle::PLAYER_STATE* pPreState);
	//void		PlayDead(CMayCastle* pMay, Engine::CDynamicMesh* pMesh, CMayCastle::PLAYER_STATE* pPreState);

public:
	void		Set_AttState(CMayCastle* pMay);
	void		Set_ClearIndex() { m_byAttIndex = 0; }

private:
	void		BasicAttack(CMayCastle* pMay, Engine::CDynamicMesh* pMesh);
	void		SmashAttack(CMayCastle* pMay, Engine::CDynamicMesh* pMesh);
	void		UltimateAttack(CMayCastle* pMay, Engine::CDynamicMesh* pMesh);

	//BasicAtt
	void		FirstAttack(CMayCastle* pMay, Engine::CDynamicMesh* pMesh);
	void		SecondAttack(CMayCastle* pMay, Engine::CDynamicMesh* pMesh);
	void		ThirdAttack(CMayCastle* pMay, Engine::CDynamicMesh* pMesh);

public:
	virtual void Free() override;

private:
	MayCastle_AnimID				m_eMayAnim;
	MayCastle_AnimID				m_ePreMayAnim;

	// Att
	_byte							m_byAttIndex = 0;
	AttAnimState					m_eAtt = FirstAtt;

	AttAnimState					m_eCurAtt = AttEnd;
	AttAnimState					m_ePreAtt = AttEnd;

	_bool							m_bAnimationStart = false;

	_bool							m_bSound[Sound_End] = { false };
	_uint							m_iSound[Sound_End] = { 0 };

	_bool							m_bSmash = false; //스매쉬 한번만 하기위해.

};

#define __CODYANIMATION_H__
#endif