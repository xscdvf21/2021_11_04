#pragma once
#include "GameObject.h"
class CBackground :
	public CGameObject
{
public:
	CBackground();
	virtual ~CBackground();

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private :
	bool	m_bAlphaCheck;
	int		m_iCount; // �ѹ��� �۵��ϰ� �� ���� ��Ű�� ����.	
};

