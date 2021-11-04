#pragma once
#include "GameObject.h"
class CMonster9 :
	public CGameObject
{
public:
	explicit CMonster9();
	virtual ~CMonster9();

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

	//bool compare(pair<float, INFO>& _a, pair<float, INFO>& _b);
private:
	GROUND*		pTemp;

	TCHAR*		tTemp = L""; //�����̸� ������ ����?

	bool		bAStarMonsterCheck;
	bool		bAStarCheck;

	int i = 0;
	/*map<float, INFO> mapDis;*/
};

