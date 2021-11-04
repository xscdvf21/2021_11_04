#pragma once

#ifndef __UI_TIMER_BACKGROUND_H__
#define __UI_TIMER_BACKGROUND_H__

#include "GameObject.h"



USING(Engine)

class CTimerMark final : public CGameObject
{
public:
	CTimerMark(_Device pDevice);
	CTimerMark(const CTimerMark& other);
	virtual ~CTimerMark() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;
	void Set_RemainTime(_float fRemain) { m_fRemainTime = fRemain; }

public:
	static CTimerMark* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private:
	HRESULT Add_Component();

	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;

	LPD3DXFONT  m_pFont;
	float m_fRemainTime;
};


#endif // !__UI_TIMER_BACKGROUND_H__