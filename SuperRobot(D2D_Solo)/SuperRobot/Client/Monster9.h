#pragma once
#include "GameObject.h"
class CMonster9 :
	public CGameObject
{
public:
	explicit CMonster9();
	virtual ~CMonster9();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	void Move_Check();
	void Attack_Check();
	void Move_Route();

	//bool compare(pair<float, INFO>& _a, pair<float, INFO>& _b);
private:
	GROUND*		pTemp;

	TCHAR*		tTemp = L""; //유닛이름 담을때 쓸듯?

	bool		bAStarMonsterCheck;
	bool		bAStarCheck;

	int i = 0;
	/*map<float, INFO> mapDis;*/
};

