#pragma once
#include "GameObject.h"
class CUnitAttack :
	public CGameObject
{
public:
	explicit CUnitAttack();
	virtual ~CUnitAttack();

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

