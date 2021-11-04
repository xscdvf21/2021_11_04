#pragma once
#ifndef __CYELLOWBLOCK_H__
#define __CYELLOWBLOCK_H__

#include "Obj.h"
class CYellowBlock : public CObj
{
public:
	CYellowBlock();
	virtual ~CYellowBlock();

public:
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};


#endif // !__CYELLOWBLOCK_H__

