#pragma once
#include "GameObject.h"
class CUnitGold :
	public CGameObject
{
public:
	explicit CUnitGold();
	virtual ~CUnitGold();

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

