#pragma once
#ifndef __CTREE_H__
#define	__CTREE_H__

#include "Obj.h"
class CTree : public CObj
{
public:
	CTree();
	virtual ~CTree();

public:
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};


#endif // !__CTREE_H__



