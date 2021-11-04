#pragma once
#ifndef __CYELLOWHOUSE_H__
#define __CYELLOWHOUSE_H__

#include "Obj.h"
class CYellowHouse : public CObj
{
public:
	CYellowHouse();
	virtual ~CYellowHouse();

public :
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};


#endif // !__CYELLOWHOUSE_H__



