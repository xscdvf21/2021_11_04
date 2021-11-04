#pragma once

#ifndef __OPENING_UI_H__
#define __OPENING_UI_H__

#include "GameObject.h"

USING(Engine)

class COpeningUI final : public CGameObject
{
private:
	explicit COpeningUI(_Device pDevice);
	explicit COpeningUI(const COpeningUI& other);
	virtual ~COpeningUI() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static COpeningUI* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private:
	HRESULT Add_Component();
	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;
};


#endif // !__OPENING_UI_H__