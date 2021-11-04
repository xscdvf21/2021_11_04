#pragma once
#include "Effect.h"
class CBuffEffect :
	public CEffect
{
public:
	explicit CBuffEffect(CEffect_IMP* pMotion);
	virtual ~CBuffEffect();
public:
	// CEffect을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
private:
	float m_fAngle = 0.f;
	float m_fSpeed = 0.f;
};

