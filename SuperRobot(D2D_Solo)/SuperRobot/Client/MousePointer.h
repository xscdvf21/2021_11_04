#pragma once
#include "GameObject.h"
class CMousePointer :
	public CGameObject
{
public:
	CMousePointer();
	~CMousePointer();

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;


private :
	void OffSet();
	void Key_Check();

};

