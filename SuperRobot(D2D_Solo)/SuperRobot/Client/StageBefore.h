#pragma once
#include "Scene.h"
class CStageBefore :
	public CScene
{
public:
	explicit CStageBefore();
	virtual ~CStageBefore();

	// CScene��(��) ���� ��ӵ�
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene() override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;


private:
	void Key_Check();

private :
	float m_fTurnDelay;
};

