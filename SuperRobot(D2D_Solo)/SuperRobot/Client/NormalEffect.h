#pragma once
#include "Effect.h"
class CNormalEffect :
	public CEffect
{
public:
	CNormalEffect(CEffect_IMP* pMotion);
	virtual ~CNormalEffect();

	// CEffect��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

