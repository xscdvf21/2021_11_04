#pragma once
#include "GameObject.h"
class CUnitInfo :
	public CGameObject
{
public:
	explicit CUnitInfo();
	virtual ~CUnitInfo();

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

