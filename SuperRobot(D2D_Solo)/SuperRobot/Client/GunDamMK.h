#pragma once
#include "GameObject.h"
class CGunDamMK :
	public CGameObject
{
public:
	explicit CGunDamMK();
	virtual ~CGunDamMK();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private :
	void Key_Check();
	void Attack_Check();
	void Turn_Check();
	void Move_Route();
	
private :
	GROUND*		pTemp;
	TCHAR*		tTemp = L""; //유닛이름 담을때 쓸듯?
	bool		bAStarCheck; //에이스타 다 끝나고 나서 작동하기위해
	bool		bAStarUnitCheck; //에이스타 유닛을 구별하기위해.
};

