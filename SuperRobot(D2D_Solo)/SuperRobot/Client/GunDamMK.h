#pragma once
#include "GameObject.h"
class CGunDamMK :
	public CGameObject
{
public:
	explicit CGunDamMK();
	virtual ~CGunDamMK();

	// CGameObject��(��) ���� ��ӵ�
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
	TCHAR*		tTemp = L""; //�����̸� ������ ����?
	bool		bAStarCheck; //���̽�Ÿ �� ������ ���� �۵��ϱ�����
	bool		bAStarUnitCheck; //���̽�Ÿ ������ �����ϱ�����.
};

