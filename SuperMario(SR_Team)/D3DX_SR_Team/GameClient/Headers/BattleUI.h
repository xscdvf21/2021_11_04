#pragma once

#ifndef __BATTLE_UI_H__
#define __BATTLE_UI_H__

#include "GameObject.h"


USING(Engine)

class CBattleUI final : public CGameObject
{
private:
	explicit CBattleUI(_Device pDevice);
	explicit CBattleUI(const CBattleUI& other);
	virtual ~CBattleUI() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static CBattleUI* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

	//Setter
public:
	void Set_Timer(_float fTime);
	void Set_Ring(_uint iRing);
	void Set_AttackCount(_uint iAttack);

	void Set_Visual_Timer() { m_bVisualTime = true; }
	void Set_Visual_Ring() { m_bVisualRing = true; }
	void Set_Visual_Attack() { m_bVisualAttack = true; }

private:
	HRESULT Add_Component();


private:
	//CCamera::CAMERA_DESC camDESC;
	_uint m_iRingCount;
	_uint m_iRemainAttack;

	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;


private: //UI Comp
	class CBattleUI_HP* m_pHP_UI = nullptr;
	class CTimerMark* m_pTimerMark_UI = nullptr;
	//class CTimeCount* m_pTimerCount_UI = nullptr;
	class CActionUIBackground* m_pActionBG_UI = nullptr;
	class CRingCountBackground* m_pRingBG_UI = nullptr;

	LPD3DXFONT  m_pFont;

private: //UI Comp On/Off
	_bool m_bVisualHP;
	_bool m_bVisualTime;
	_bool m_bVisualRing;
	_bool m_bVisualAttack;
	_bool m_bVisualCoin;


};


#endif // !__BATTLE_UI_H__