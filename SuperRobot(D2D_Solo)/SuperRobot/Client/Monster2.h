#pragma once
#include "GameObject.h"
class CMonster2 :
	public CGameObject
{
public:
	explicit CMonster2();
	virtual ~CMonster2();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private :
	void Move_Check();
	void Attack_Check();
	void Move_Route();

private :
	GROUND*		pTemp;

	TCHAR*		tTemp = L""; //유닛이름 담을때 쓸듯?

	bool		bAStarMonsterCheck;
	bool		bAStarCheck;

	int i = 0;
};

