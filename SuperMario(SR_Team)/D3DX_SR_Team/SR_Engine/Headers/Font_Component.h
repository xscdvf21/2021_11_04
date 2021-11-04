#pragma once
#ifndef __FONT_COMPONENT_H__

#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CFont_Component : public CComponent
{
public:
	explicit CFont_Component(LPDIRECT3DDEVICE9 pDevice);
	explicit CFont_Component(const CFont_Component& other);
	virtual ~CFont_Component() = default ;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * pArg = nullptr) override;

public:
	HRESULT Set_Font(_uint _iHeight, _uint _iWidth, _uint _iWeight,TCHAR* _pText);

public:
	static CFont_Component* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private :
	LPD3DXFONT  m_pFont;
	TCHAR*		m_pText;

};
END
#define __FONT_COMPONENT_H__
#endif // !__FONT_COMPONENT_H__


