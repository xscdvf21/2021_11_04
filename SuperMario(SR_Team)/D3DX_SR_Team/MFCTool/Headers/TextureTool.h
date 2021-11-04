#pragma once
#include"ComponentTool.h"

USING(Engine)

enum class ETextureType
{
	Normal,
	Cube
};


class CTextureTool :
	public CComponentTool
{
private:
	explicit CTextureTool(LPDIRECT3DDEVICE9 pDevice, ETextureType eTextureType, TCHAR* pFilePath, _uint iCount);
	explicit CTextureTool(const CTextureTool& other);
	virtual ~CTextureTool() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void * pArg = nullptr) override;

public:
	HRESULT Set_Texture(_uint iIndex);

public:
	static CTextureTool* Create(LPDIRECT3DDEVICE9 pDevice, ETextureType eTextureType, TCHAR* pFilePath, _uint iCount);
	virtual CComponentTool * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

private:
	typedef vector<IDirect3DBaseTexture9*>	TEXTURES;
	TEXTURES		m_Textures;
	TCHAR*			m_pFilePath;
	_uint			m_iCount;
	ETextureType	m_eTextureType;
};

