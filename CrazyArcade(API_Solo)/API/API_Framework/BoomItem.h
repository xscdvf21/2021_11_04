#pragma once
#include "Obj.h"
class CBoomItem : public CObj
{
public:
	CBoomItem();
	virtual ~CBoomItem();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private :
	vector<CObj*>		m_vecItem;
};

