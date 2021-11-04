#pragma once

#ifndef __CDOWNBULLET_H__
#define __CDOWNBULLET_H__

#include "Obj.h"
class CDownBullet : public CObj
{
public:
	CDownBullet();
	virtual ~CDownBullet();


public:
	virtual void Initialize() override;
	virtual int	 Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	DWORD			m_dwTime;
};

#endif // !__CDOWNBULLET_H__



