#pragma once
#include "GameObject.h"
class CUnitMenu :
	public CGameObject
{
public:
	explicit CUnitMenu();
	virtual ~CUnitMenu();

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private :
	int i = 0;
};

