#pragma once

#ifndef __UI_PLAYER_HP_H__
#define __UI_PLAYER_HP_H__

#include "GameObject.h"

USING(Engine)

class CBattleUI_HP final : public CGameObject
{
public:
	CBattleUI_HP(_Device pDevice);
	CBattleUI_HP(const CBattleUI_HP& other);
	virtual ~CBattleUI_HP() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

	void Set_Position();

public:
	static CBattleUI_HP* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private:
	HRESULT Add_Component();



};


#endif // !__UI_PLAYER_HP_H__