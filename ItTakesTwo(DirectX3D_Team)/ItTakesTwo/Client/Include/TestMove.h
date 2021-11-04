#pragma once
#ifndef TestMove_h__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

class CTestMove : public CBase
{
	DECLARE_SINGLETON(CTestMove)

public:
	CTestMove();
	~CTestMove();

public:
	void			Set(wstring tagName, _bool bTrigger = false);
	void			Move(CTransform* pTransform, const _float& fTimeDelta, wstring tagName);
	void			MoveTrigger(class CBoundingBox* pBox,const _float& fTimeDelta, wstring tagName);
	//void			TriggerMove();

public:
	virtual void Free();

private:
	_float			m_fSpeed = 5.0f;
	_float			m_fTriggerSpeed = 5.0f;
	_float			m_fRotaAngle = 5.0f;
	_bool			m_bRotation = false;
};

#define TestMove_h__
#endif