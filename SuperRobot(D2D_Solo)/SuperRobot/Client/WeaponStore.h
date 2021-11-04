#pragma once
#include "Scene.h"
class CGameObject;
class CWeaponStore :
	public CScene
{
public:
	explicit CWeaponStore();
	virtual ~CWeaponStore();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene() override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

private :
	void Key_Check();

private:
	int iX;
	int iY;

	int iIndex; //처음에 유닛의 정보를 가져오기 위한 vecObj의 인덱스값이 될것.
	int iStatsNum; //스텟 업글에서 씬 넘버로 사용 될 변수.
	int iYesNoNum; //그래 아니 선택 씬 넘버
	

	bool	m_bUpCheck; //업그레이드 창 띄워주기 위해.
	bool	m_bYesNo; //최종적으로 그래 아니 선택 하는창.
	bool	m_bGoldCheck; //돈이 부족하거나, 충족 되었을 때 대사 쓰기위해,
	vector<CGameObject*> vecObj;
};

