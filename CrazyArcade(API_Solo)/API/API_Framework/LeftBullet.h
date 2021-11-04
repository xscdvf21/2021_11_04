#pragma once
#ifndef __CLEFTBULLET_H__
#define __CLEFTBULLET_H__

#include "Obj.h"
class CLeftBullet : public CObj
{
public:
	CLeftBullet();
	virtual ~CLeftBullet();

public:
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:

	DWORD			m_dwTime;

};

#endif // !__CLEFTBULLET_H__



