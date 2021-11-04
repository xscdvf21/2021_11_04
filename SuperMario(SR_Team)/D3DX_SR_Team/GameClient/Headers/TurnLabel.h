#pragma once

#ifndef __UI_TURNLABEL_H__
#define __UI_TURNLABEL_H__

#include "GameObject.h"

USING(Engine)

enum class ETurnState
{
	PLAYER, ENEMY, NOEVENT
};

class CTurnLabel final : public CGameObject
{
public:
	CTurnLabel(_Device pDevice);
	CTurnLabel(const CTurnLabel& other);
	virtual ~CTurnLabel() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static CTurnLabel* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
	void Set_Visual() { bIsVisual = true; }

private:
	HRESULT Add_Component();

	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom[2] = {};

	_bool bIsVisual;

	LPD3DXFONT  m_pFont;

	ETurnState eTurn;
};


#endif // !__UI_TURNLABEL_H__