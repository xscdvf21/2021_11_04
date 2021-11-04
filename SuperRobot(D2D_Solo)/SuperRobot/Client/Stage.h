#pragma once
#include "Scene.h"

class CStage :
	public CScene
{
public:
	explicit CStage();
	virtual ~CStage();
public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene() override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

private:
	void Unit_Turn();

private :
	float m_fTurnDelay;
	bool  m_bCutScene;
};

