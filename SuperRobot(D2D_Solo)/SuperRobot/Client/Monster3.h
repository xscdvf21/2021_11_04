#pragma once
#include "GameObject.h"
class CMonster3 :
	public CGameObject
{
public:
	explicit CMonster3();
	virtual ~CMonster3();

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	void Move_Check();
	void Attack_Check();
	void Move_Route();

private:
	GROUND*		pTemp;

	TCHAR*		tTemp = L""; //�����̸� ������ ����?

	bool		bAStarMonsterCheck;
	bool		bAStarCheck;

	int i = 0;
};
