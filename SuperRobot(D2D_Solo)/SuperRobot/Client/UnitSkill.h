#pragma once
#include "GameObject.h"
class CUnitSkill :
	public CGameObject
{
public:
	explicit CUnitSkill();
	virtual ~CUnitSkill();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	void Key_Check();

private :
	int		m_iSkillOne;
	int		m_iSkillTwo;
};

