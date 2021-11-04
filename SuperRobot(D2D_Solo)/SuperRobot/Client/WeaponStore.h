#pragma once
#include "Scene.h"
class CGameObject;
class CWeaponStore :
	public CScene
{
public:
	explicit CWeaponStore();
	virtual ~CWeaponStore();

	// CScene��(��) ���� ��ӵ�
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

	int iIndex; //ó���� ������ ������ �������� ���� vecObj�� �ε������� �ɰ�.
	int iStatsNum; //���� ���ۿ��� �� �ѹ��� ��� �� ����.
	int iYesNoNum; //�׷� �ƴ� ���� �� �ѹ�
	

	bool	m_bUpCheck; //���׷��̵� â ����ֱ� ����.
	bool	m_bYesNo; //���������� �׷� �ƴ� ���� �ϴ�â.
	bool	m_bGoldCheck; //���� �����ϰų�, ���� �Ǿ��� �� ��� ��������,
	vector<CGameObject*> vecObj;
};

