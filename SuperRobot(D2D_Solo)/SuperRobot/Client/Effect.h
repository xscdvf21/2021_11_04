#pragma once
#include "GameObject.h"
class CEffect_IMP;
class CEffect :
	public CGameObject
{
public:
	explicit CEffect(CEffect_IMP* pMotion);
	virtual ~CEffect();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() PURE;
	virtual int Update_GameObject() PURE;
	virtual void LateUpdate_GameObject() PURE;
	virtual void Render_GameObject() PURE;
	virtual void Release_GameObject() PURE;

protected:
	CEffect_IMP* m_pBridge;
};

