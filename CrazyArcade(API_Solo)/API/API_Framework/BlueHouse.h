#pragma once
#ifndef __CBLUEHOUSE_H__
#define __CBLUEHOUSE_H__

#include "Obj.h"
class CBlueHouse : public CObj
{
public:
	CBlueHouse();
	virtual ~CBlueHouse();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};


#endif // !__CBLUEHOUSE_H__



