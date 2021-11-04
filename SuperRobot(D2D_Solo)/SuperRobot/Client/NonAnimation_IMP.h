#pragma once
#include "Effect_IMP.h"
class CNonAnimation_IMP final :
	public CEffect_IMP
{
public:
	explicit CNonAnimation_IMP();
	virtual ~CNonAnimation_IMP();
public:
	void Set_StateKey(const wstring& wstrStateKey, float fLifeTime);
public:
	// CEffect_IMP을(를) 통해 상속됨
	virtual HRESULT Ready_Effect_IMP() override;
	virtual int Update_Effect_IMP() override;
	virtual void Render_Effect_IMP(const _matrix & matWorld) override;

private:
	float m_fLifeTime;
	float m_fCountTime;
};

