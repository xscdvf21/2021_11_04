#pragma once
#ifndef __CMONSTERBULLET_H__
#define __CMONSTERBULLET_H__

#include "Obj.h"
class CMonsterBullet : public CObj
{
public:
	CMonsterBullet();
	virtual ~CMonsterBullet();

public :
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private :

	template <typename T>
	CObj* Create_Pado_Left(int _i)
	{
		return CAbstractFactory<T>::Create_Bullet((float)m_tInfo.fX - _i * 40, (float)m_tInfo.fY); //파도 위치정하는것.
	}

	template <typename T>
	CObj* Create_Pado_Right(int _i)
	{
			return CAbstractFactory<T>::Create_Bullet((float)m_tInfo.fX + _i * 40, (float)m_tInfo.fY); //파도 위치정하는것.
		
	}

	template <typename T>
	CObj* Create_Pado_Up(int _i)
	{
		return CAbstractFactory<T>::Create_Bullet((float)m_tInfo.fX, (float)m_tInfo.fY - _i * 40); //파도 위치정하는것.
	}

	template <typename T>
	CObj* Create_Pado_Down(int _i)
	{

			return CAbstractFactory<T>::Create_Bullet((float)m_tInfo.fX, (float)m_tInfo.fY + _i * 40); //파도 위치정하는것.
		
	}
	
	DWORD		m_dwTime;
	int			m_iBoomSize;

};


#endif // !__CMONSTERBULLET_H__



