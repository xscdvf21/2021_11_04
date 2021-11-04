#pragma once
#include "Scene.h"
class CTile;
class CStage2 :
	public CScene
{
public:
	explicit CStage2();
	virtual ~CStage2();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual _int Update_Scene() override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

private:
	void Unit_Turn();

private:
	CTile*		m_pTile;
	GROUND*		pTemp;
	float m_fTurnDelay;
};

