#pragma once
#include "GameObject.h"
class CUnitFight :
	public CGameObject
{
public:
	explicit CUnitFight();
	virtual ~CUnitFight();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	
public :
	void FrameMove(float fSpeed);

private :
	FRAME m_tFrame;

	float m_fx;
	float m_fsize;
	float m_fSpeed;
	float m_fSpeed2;
	float m_fAngle;

	int	iIndex;

	float m_fTime;
	bool  m_bCheck;
	bool m_bHpCheck;
	bool m_bSoundCheck; //무기 공격 사운드 체크
	bool m_bSoundCheck2; //무기 도착 후 터지는 사운드.


};

