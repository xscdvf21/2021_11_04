#pragma once

#ifndef __BOSSMONSTER_H__
#define __BOSSMONSTER_H__

#include "Obj.h"

class CBossMonster : public CObj
{
public:
	CBossMonster();
	~CBossMonster();

public:
	enum STATE { IDLE, WALK, ATTACK, HIT, DEAD, DEAD_SCENE, END };

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Scene_Change();
	void Moving_Check();
	void off_Set();
private:
	template <typename T>
	CObj* Create_Bullet(float _x, float _y)
	{
		return CAbstractFactory<T>::Create((float)_x, (float)_y);
	}

private:
	POINT		m_tPoint[3];

	int			m_iNum;
	int			m_iNumSeed;

	CObj*		m_pTemp;
	TCHAR*		m_pFramekey;

	STATE		m_ePreScene;
	STATE		m_eCurScene;

	DWORD			m_dwTime;

	int				m_life;
};


#endif // !__MONSTER_H__
