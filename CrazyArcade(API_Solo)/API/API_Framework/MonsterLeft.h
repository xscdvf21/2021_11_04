#pragma once
#ifndef __CMONSTERLEFT_H__
#define __CMONSTERLEFT_H__

#include "Obj.h"
class CMonsterLeft : public CObj
{
public:
	CMonsterLeft();
	virtual ~CMonsterLeft();

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

#endif // !__CMONSTERLEFT_H__




