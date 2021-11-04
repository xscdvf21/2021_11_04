#pragma once
#ifndef __CRIGHTBULLET_H__
#define __CRIGHTBULLET_H__

#include "Obj.h"
class CRightBullet : public CObj
{
public:
	CRightBullet();
	virtual ~CRightBullet();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:

	DWORD			m_dwTime;
};

#endif // !__CRIGHTBULLET_H__


