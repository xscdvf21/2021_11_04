#pragma once
#ifndef __TEXTURE_H__

#include "Component.h"
BEGIN(Engine)

enum class ETextureType
{
	Normal,
	Cube,
	Font
};

class ENGINE_DLL CTexture : public CComponent
{
private :
	explicit CTexture(LPDIRECT3DDEVICE9 pDevice, ETextureType eTextureType, TCHAR* pFilePath, _uint iCount);
	explicit CTexture(const CTexture& other);
	virtual ~CTexture() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * pArg = nullptr) override;

public :
	HRESULT Set_Texture(_uint iIndex);
	HRESULT Load_Texture(const wstring & wstrFilePath);

public :
	static CTexture* Create(LPDIRECT3DDEVICE9 pDevice, ETextureType eTextureType, TCHAR* pFilePath, _uint iCount);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private :
	typedef vector<IDirect3DBaseTexture9*> TEXTURES;
	TEXTURES		m_Textures;
	TCHAR*			m_pFilePath;
	_uint			m_iCount;
	ETextureType	m_eTextureType;

};

END


#define __TEXTURE_H__
#endif // !__TEXTURE_H__
