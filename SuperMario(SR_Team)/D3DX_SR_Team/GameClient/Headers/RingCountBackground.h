#pragma once

#ifndef __UI_RINGCOUNT_BACKGROUND_H__
#define __UI_RINGCOUNT_BACKGROUND_H__

#include "GameObject.h"



USING(Engine)

class CRingCountBackground final : public CGameObject
{
public:
	CRingCountBackground(_Device pDevice);
	CRingCountBackground(const CRingCountBackground& other);
	virtual ~CRingCountBackground() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static CRingCountBackground* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

	void Set_RingCount(_uint iRing);

private:
	HRESULT Add_Component();

	
	//class CTransform* m_pTransformComp = nullptr;
	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;

	LPD3DXFONT  m_pFont;
	_uint m_iRingCount;
};


#endif // !__UI_RINGCOUNT_BACKGROUND_H__