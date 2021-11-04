#include "stdafx.h"
#include "..\Headers\TextureTool.h"


CTextureTool::CTextureTool(LPDIRECT3DDEVICE9 pDevice, ETextureType eTextureType, TCHAR * pFilePath, _uint iCount)
	: CComponentTool(pDevice)
	, m_pFilePath(pFilePath)
	, m_iCount(iCount)
	, m_eTextureType(eTextureType)
{
}

CTextureTool::CTextureTool(const CTextureTool & other)
	: CComponentTool(other)
	, m_pFilePath(other.m_pFilePath)
	, m_iCount(other.m_iCount)
	, m_eTextureType(other.m_eTextureType)
	, m_Textures(other.m_Textures)
{
}

HRESULT CTextureTool::Ready_Component_Prototype()
{
	TCHAR szFullPath[256] = L"";
	IDirect3DBaseTexture9* pTexture = nullptr;
	HRESULT hr = 0;

	m_Textures.reserve(m_iCount);

	for (_uint i = 0; i < m_iCount; ++i)
	{
		swprintf_s(szFullPath, m_pFilePath, i);

		switch (m_eTextureType)
		{
		case ETextureType::Normal:
			hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture);
			break;
		case ETextureType::Cube:
			hr = D3DXCreateCubeTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);
			break;
		}

		if (FAILED(hr))
		{
			TCHAR szLogMessage[256] = L"Failed To CreateTexture From %s.";
			swprintf_s(szLogMessage, szLogMessage, szFullPath);
			PRINT_LOG(L"Error", szLogMessage);
			return E_FAIL;
		}

		m_Textures.push_back(pTexture);
	}

	return S_OK;
}

HRESULT CTextureTool::Ready_Component(void * pArg)
{
	return S_OK;
}

HRESULT CTextureTool::Set_Texture(_uint iIndex)
{
	if (m_Textures.size() <= iIndex)
		return E_FAIL;

	/* 이 다음에 그려질 버텍스에 맵핑시킬 텍스처를 장치에게 넘겨줌. */
	return m_pDevice->SetTexture(0, m_Textures[iIndex]);
}

CTextureTool * CTextureTool::Create(LPDIRECT3DDEVICE9 pDevice, ETextureType eTextureType, TCHAR * pFilePath, _uint iCount)
{
	CTextureTool* pInstance = new CTextureTool(pDevice, eTextureType, pFilePath, iCount);
	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CTextureTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponentTool * CTextureTool::Clone(void * pArg)
{
	CTextureTool* pClone = new CTextureTool(*this);
	if (FAILED(pClone->Ready_Component(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CTextureTool");
		Safe_Release(pClone);
	}

	return pClone;
}

void CTextureTool::Free()
{
	for (auto& pTexture : m_Textures)
	{
		Safe_Release(pTexture);
	}

	m_Textures.clear();

	CComponentTool::Free();
}
