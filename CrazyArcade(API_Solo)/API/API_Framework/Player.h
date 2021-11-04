#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "Obj.h"
class CPlayer : public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	enum STATE { IDLE, WALK, ATTACK, HIT, DEAD, DEAD_SCENE, END };

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update();
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	
	
private:
	template <typename T>
	CObj* Create_Bullet(float _x, float _y)
	{
		return CAbstractFactory<T>::Create((float)_x, (float)_y);
	}

  

public :
	static		int		m_BoomSize;
	static		int		m_BoomCount;
	static		float		m_PlayerSpeed;
private:
	CObj* Create_Shield();
	void Scene_Change();

private:
	void Key_Check();
	void Jumping();
	void OffSet();
private:
	POINT		m_tPosin;
	float		m_fPosinDis;
	float		m_fTest;

	bool		m_bStretch;
	bool		m_bJump;
	bool		m_bJumpStart;
	bool		m_bBoomCheck;

	float		m_fJumpPower;
	float		m_fJumpTime;
	float		m_fJumpStart;

	list<BOOMINFO>	m_listBoomInfo;

	BOOMINFO		m_BoomInfo;

	STATE		m_ePreScene;
	STATE		m_eCurScene;

	DWORD			m_dwTime;

	list<CObj*>			m_listObj[OBJID::END];
	CObj*			m_pTemp;

	bool		m_pPlayerDel;
};


#endif // !__PLAYER_H__
