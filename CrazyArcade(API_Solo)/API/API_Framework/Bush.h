#pragma once
#ifndef __CBUSH_H__
#define __CBUSH_H__

#include "Obj.h"
class CBush : public CObj
{
public:
	CBush();
	virtual ~CBush();

public :
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;


};


#endif // !__CBUSH_H__



