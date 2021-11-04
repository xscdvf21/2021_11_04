#pragma once
#ifndef __REDHOUSE_H__
#define __REDHOUSE_H__

#include "Obj.h"
class CRedHouse : public CObj
{
public:
	CRedHouse();
	virtual ~CRedHouse();

public :
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};


#endif // !__REDHOUSE_H__


