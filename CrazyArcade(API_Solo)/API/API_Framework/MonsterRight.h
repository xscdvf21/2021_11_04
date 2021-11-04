#pragma once
#ifndef __COMSTERRIGHT_H__
#define __COMSTERRIGHT_H__

#include "Obj.h"
class CMonsterRight : public CObj
{
public:
	CMonsterRight();
	virtual ~CMonsterRight();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
	

private :

	DWORD			m_dwTime;
};

#endif // !__COMSTERRIGHT_H__


