#pragma once
#ifndef TriggerView_h__

#include "GameObject.h"
#include "Engine_Define.h"

BEGIN(Engine)
END

class CTriggerView : public CGameObject
{
public:
	explicit CTriggerView(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTriggerView();

public:
	 HRESULT Ready_Object(TRIGGERTYPE eType, void* pArg = nullptr);
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual _int LateUpdate_Object(const _float & TimeDelta) override;
	virtual void Render_Object(const _int& iIndex = 0) override;

public:
	void Set_CodyRespwanPoint(_vec3 vCody) { m_vCodyRespwanPoint = vCody; }
	void Set_MayRespwanPoint(_vec3 vMay) { m_vMayRespwanPoint = vMay; }
	void Set_MayRespwanUp(_vec3 vMayUp) { m_vMayRespwanUp = vMayUp; }
	void Set_TriggerActive(_bool bActive = true) { m_bTriggerActive = bActive; }

public:
	_vec3 Get_CodyRespwanPoint() { return m_vCodyRespwanPoint; }
	_vec3 Get_MayRespwanPoint() { return m_vMayRespwanPoint; }
	_vec3 Get_MayRespwanUp() { return m_vMayRespwanUp; }

public:
	void*	On_Active(void* pCaller) override;

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, TRIGGERTYPE eType, void* pArg, _vec3 vCodyPos = { 0.0f, 0.0f, 0.0f }, _vec3 vMayPos = { 0.0f, 0.0f, 0.0f }, _vec3 vMayUp = {0.0f, 0.0f, 0.0f});

private:
	TRIGGERTYPE m_eTriggerType;
	CTrigger* m_pTrigger;
	_vec3	m_vCodyRespwanPoint = {};
	_vec3	m_vMayRespwanPoint = {};
	_vec3	m_vMayRespwanUp = {};
	_bool m_bTriggerActive = true;

public:
	virtual void Free() override;
};

#define TriggerView_h__
#endif