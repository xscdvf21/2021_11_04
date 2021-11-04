#pragma once

#ifndef __BULLET_H__
#define __BULLET_H__


#include "Obj.h"
class CBullet : public CObj
{
public:
	CBullet();
	virtual ~CBullet();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update();
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	void Scene_Change();

public:
	void Set_Dir(BULLET::DIR _eDir) { m_eDir = _eDir; }

private :
	template <typename T>
	CObj* Create_Pado_Left(int _i)
	{
		return CAbstractFactory<T>::Create_Bullet((float)m_tInfo.fX - _i*40, (float)m_tInfo.fY); //파도 위치정하는것.
	}

	template <typename T>
	CObj* Create_Pado_Right(int _i)
	{
		return CAbstractFactory<T>::Create_Bullet((float)m_tInfo.fX + _i * 40, (float)m_tInfo.fY); //파도 위치정하는것.

	}

	template <typename T>
	CObj* Create_Pado_Up(int _i)
	{
		return CAbstractFactory<T>::Create_Bullet((float)m_tInfo.fX , (float)m_tInfo.fY - _i * 40); //파도 위치정하는것.
	}

	template <typename T>
	CObj* Create_Pado_Down(int _i)
	{
		return CAbstractFactory<T>::Create_Bullet((float)m_tInfo.fX , (float)m_tInfo.fY + _i * 40); //파도 위치정하는것.
	}

private:
	bool		m_bStretch;
	BULLET::DIR		m_eDir;

	DWORD			m_dwTime;
	int				m_iFPS;
	int				m_iBoomSize;

	bool			m_boomcheck;
	
	TCHAR			m_szFPS[16];


};

#endif // !__BULLET_H__
