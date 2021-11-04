#pragma once
#include "GameObject.h"
class CUnitWeapon :
	public CGameObject
{
public:
	explicit CUnitWeapon();
	virtual ~CUnitWeapon();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

