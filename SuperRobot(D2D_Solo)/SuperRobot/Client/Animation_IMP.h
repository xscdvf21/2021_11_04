#pragma once
#include "Effect_IMP.h"
class CAnimation_IMP final :
	public CEffect_IMP
{
public:
	explicit CAnimation_IMP();
	virtual ~CAnimation_IMP();
public:
	void Set_Frame(const wstring& wstrStateKey, const FRAME& tFrame);
public:
	// CEffect_IMP을(를) 통해 상속됨
	virtual HRESULT Ready_Effect_IMP() override;
	virtual int Update_Effect_IMP() override;
	virtual void Render_Effect_IMP(const _matrix & matWorld) override;
private:
	FRAME m_tFrame;
};
