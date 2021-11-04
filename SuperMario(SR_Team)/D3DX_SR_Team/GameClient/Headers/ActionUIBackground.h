#pragma once

#ifndef __UI_ACTION_BACKGROUND_H__
#define __UI_ACTION_BACKGROUND_H__

#include "GameObject.h"



USING(Engine)

class CActionUIBackground final : public CGameObject
{
public:
	CActionUIBackground(_Device pDevice);
	CActionUIBackground(const CActionUIBackground& other);
	virtual ~CActionUIBackground() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static CActionUIBackground* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

	void Set_ActionCount(_uint iActionCount);

private:
	HRESULT Add_Component();

	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;

	LPD3DXFONT  m_pFont;
	_uint m_iActionCount;
};



#endif // !__UI_ACTION_BACKGROUND_H__