#pragma once
class CEffect_IMP abstract
{
public:
	explicit CEffect_IMP();
	virtual ~CEffect_IMP();

public:
	virtual HRESULT Ready_Effect_IMP()PURE;
	virtual int		Update_Effect_IMP()PURE;
	virtual void	Render_Effect_IMP(const _matrix& matWorld)PURE;

protected:
	wstring m_wstrStateKey;
};

