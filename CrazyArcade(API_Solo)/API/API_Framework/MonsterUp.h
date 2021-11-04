#pragma once
#ifndef __CMONSTERUP_H__
#define __CMONSTERUP_H__

#include "Obj.h"
class CMonsterUp : public CObj
{
public:
	CMonsterUp();
	virtual ~CMonsterUp();

public :
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private :
	DWORD			m_dwTime;
};

#endif // !__CMONSTERUP_H__


