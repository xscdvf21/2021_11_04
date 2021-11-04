#pragma once
#include "Scene.h"
class CStagePalying :
	public CScene
{
public:
	explicit CStagePalying();
	virtual ~CStagePalying();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene() override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

private:
	void Key_Check();

private:
	float m_fTurnDelay;
};

