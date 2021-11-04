#pragma once
#ifndef __STAGECOIN_H__

#include "GameObject.h"
USING(Engine)

class CStageCoin final : public CGameObject
{
private:
	explicit CStageCoin(LPDIRECT3DDEVICE9 pDevice);
	explicit CStageCoin(const CStageCoin& other);
	virtual ~CStageCoin() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();

public :
	static CStageCoin* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

	LPD3DXFONT  m_pFont;
	int i = 0;

	_float m_fAngle = 0.f;

	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;
};


#define __STAGECOIN_H__
#endif // !__STAGECOIN_H__


