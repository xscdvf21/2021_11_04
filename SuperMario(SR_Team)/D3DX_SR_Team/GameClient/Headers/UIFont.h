#pragma once
#ifndef __UIFONT_H__

#include "GameObject.h"


USING(Engine)

class CUIFont final : public CGameObject
{
public:
	explicit CUIFont(_Device pDevice);
	explicit CUIFont(const CUIFont& other);
	virtual ~CUIFont() = default ;

public :
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

private:
	HRESULT Add_Component();
	HRESULT Movement(_float fDeltaTime);
	HRESULT IsOnTerrain();
	HRESULT Set_Transform();


public :
	static CUIFont* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private:
	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;
	class CFont_Component* m_pFontCom = nullptr;


private :
	int				m_iFPS;
	TCHAR			m_szFPS[64];
};




#define __UIFONT_H__
#endif // !__UIFONT_H__



