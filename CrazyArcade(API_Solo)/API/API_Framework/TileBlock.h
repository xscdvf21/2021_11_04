#pragma once
#ifndef __CTILE_H__
#define __CTILE_H__


#include "Obj.h"
class CTileBlock : public CObj
{
public:
	CTileBlock();
	virtual ~CTileBlock();

public :
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__CTILE_H__


