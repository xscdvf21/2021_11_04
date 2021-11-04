#pragma once
#ifndef  __CBOOMPOPO_H__
#define __CBOOMPOPO_H__


#include "Obj.h"
class CBoomPopo : public CObj
{
public:
	CBoomPopo();
	~CBoomPopo();

	void Scene_Change();

public :
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

};


#endif // ! __CBOOMPOPO_H__

