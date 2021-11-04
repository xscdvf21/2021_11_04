#include "stdafx.h"
#include "Multi_Texture.h"


CMulti_Texture::CMulti_Texture()
{
}


CMulti_Texture::~CMulti_Texture()
{
	Release_Texture(); 
}

// 일단 내가 여기에 멀티 텍스쳐를 넣어줄땐 어떻게 넣어 줄 것인가 !? 
//D:\박병건\112A\FrameWork\Texture\Stage\Terrain\Tile\AKIHA_AKI13_00%d.png
HRESULT CMulti_Texture::Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey /*= L""*/, const DWORD & dwCount /*= 0*/)
{
	auto& iter_find = m_mapTexture.find(wstrStateKey);

	if (iter_find == m_mapTexture.end())
	{
		TCHAR szBuf[MAX_PATH] = L""; 
		for (DWORD i = 0; i < dwCount; ++i)
		{
			wsprintf(szBuf, wstrFilePath.c_str(), i);
			TEXINFO* pTexInfo = new TEXINFO; 
			if (FAILED(D3DXGetImageInfoFromFile(szBuf, &pTexInfo->tImageInfo)))
			{
				wstring wstrErr = wstrFilePath + L"Load Failed";
				ERR_MSG(wstrErr.c_str());
				return E_FAIL;
			}
			if (FAILED(D3DXCreateTextureFromFileEx(CGraphic_Device::Get_Instance()->Get_Device(),
				szBuf,
				pTexInfo->tImageInfo.Width,
				pTexInfo->tImageInfo.Height,
				pTexInfo->tImageInfo.MipLevels,
				0,
				pTexInfo->tImageInfo.Format,
				D3DPOOL_MANAGED,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				0,
				nullptr,
				nullptr,
				&pTexInfo->pTexture
			)))
			{
				wstring wstrErr = wstring(szBuf) + L"Texture Creating Failed";
				ERR_MSG(wstrErr.c_str());
				return E_FAIL;
			}
			m_mapTexture[wstrStateKey].emplace_back(pTexInfo);
		}
		return S_OK;
	}
	return E_FAIL;
}

const TEXINFO * CMulti_Texture::Get_Texture(const wstring & wstrStateKey /*= L""*/, const DWORD & dwCount /*= 0*/)
{
	auto& iter_find = m_mapTexture.find(wstrStateKey);
	if (iter_find == m_mapTexture.end())
		return nullptr;

	//return /*m_mapTexture[wstrStateKey]*//*[dwCount]*/;
	return iter_find->second[dwCount];
}

void CMulti_Texture::Release_Texture()
{
	for (auto& rPair : m_mapTexture )
	{
		for (auto& pTexInfo : rPair.second)
		{
			pTexInfo->pTexture->Release(); 
			Safe_Delete(pTexInfo);
		}
		rPair.second.clear();
		//capacity 아 ... 
		//rPair.second.swap(vector<TEXINFO*>()); 
		rPair.second.shrink_to_fit(); 
		//아.. 사쿠라.... 내가 아귀라니... 
	}
	m_mapTexture.clear(); 
}
