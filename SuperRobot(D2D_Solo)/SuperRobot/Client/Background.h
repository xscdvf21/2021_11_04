#pragma once
#include "GameObject.h"
class CBackground :
	public CGameObject
{
public:
	CBackground();
	virtual ~CBackground();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private :
	bool	m_bAlphaCheck;
	int		m_iCount; // 한번만 작동하고 셋 데드 시키기 위해.	
};

