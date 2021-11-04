#pragma once
#ifndef __CITEM_H__
#define __CITEM_H__

#include "Obj.h"
class CItem : public CObj
{
public:
	CItem();
	virtual ~CItem();


public :
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

public :
	vector<CObj*>		m_vecItem;


};



#endif // !__CITEM_H__

