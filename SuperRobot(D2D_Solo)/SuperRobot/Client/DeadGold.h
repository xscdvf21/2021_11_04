#pragma once
#include "GameObject.h"
class CDeadGold :
	public CGameObject
{
public:
	explicit CDeadGold();
	virtual ~CDeadGold();

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

