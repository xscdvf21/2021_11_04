#pragma once
#ifndef __CUPBULLET_H__
#define __CUPBULLET_H__

#include "Obj.h"
class CUpBullet :
	public CObj
{
public:
	CUpBullet();
	virtual ~CUpBullet();

public:
	virtual void Initialize() override;
	virtual int	 Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	DWORD			m_dwTime;
};

#endif // !__CUPBULLET_H__



