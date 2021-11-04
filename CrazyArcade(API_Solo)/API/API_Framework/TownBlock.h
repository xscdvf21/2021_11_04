#pragma once
#ifndef __CTOWNBLOCK_H__
#define	__CTOWNBLOCK_H__

#include "Obj.h"
class CTownBlock : public CObj
{
public:
	CTownBlock();
	virtual ~CTownBlock();

public :
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__CTOWNBLOCK_H__




