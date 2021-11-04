#include "stdafx.h"
#include "Texture_Manager.h"
#include "Single_Texture.h"
#include "Multi_Texture.h"
CTexture_Manager* CTexture_Manager::m_pInstance = nullptr; 
// 안물어볼게. 

CTexture_Manager::CTexture_Manager()
{
}


CTexture_Manager::~CTexture_Manager()
{
	Release_TextureMgr(); 
}

const TEXINFO * CTexture_Manager::Get_TexInfo_TextureMgr(const wstring & wstrObjectKey, const wstring & wstrStateKey, const DWORD & dwIndex)
{
	auto& iter_find = m_mapTexture.find(wstrObjectKey); 
	if (iter_find == m_mapTexture.end())
		return nullptr;

	return iter_find->second->Get_Texture(wstrStateKey, dwIndex);
}

HRESULT CTexture_Manager::Insert_TextureMgr(ID eID, const wstring & wstrFilePath, const wstring & wstrObjectKey, const wstring & wstrStateKey, const DWORD & dwCount)
{
	auto& iter_find = m_mapTexture.find(wstrObjectKey); 
	// 즉, 제일 처음 호출되었을 경우. 
	if (iter_find == m_mapTexture.end())
	{
		// 데이터 삽입. 
		CTexture* pTexture = nullptr; 
		switch (eID)
		{
		case CTexture_Manager::SINGLE_TEX:
			pTexture = new CSingle_Texture; 
			break;
		case CTexture_Manager::MULTI_TEX:
			 pTexture = new CMulti_Texture; 
			break;
		default:
			break;
		}
		if (FAILED(pTexture->Insert_Texture(wstrFilePath, wstrStateKey, dwCount)))
		{
			ERR_MSG(L"InsertTexture Failed - TextureManager.cpp"); 
			delete pTexture; 
			pTexture = nullptr; 
			return E_FAIL; 
		}
		m_mapTexture.emplace(wstrObjectKey, pTexture); 
	}
	else if (MULTI_TEX == eID)
	{
		// 애들아 여기에서는 지금 키값으로 벨류값을 찾았다 찾아서 뭐해라? 그 벨류값의 Insert_Texture 함수를 호출해라. 
		// 여기서의 벨류값은?? CTexture 그것도 CMulti_Texture 객체. 어디서 만든거? 위 if문안에서 만든거. 
		if (FAILED(m_mapTexture[wstrObjectKey]->Insert_Texture(wstrFilePath, wstrStateKey, dwCount)))
		{
			ERR_MSG(L"MultiTexture Failed - TextureManager.cpp");
			return E_FAIL; 
		}
	}
	return S_OK;
}

void CTexture_Manager::Release_TextureMgr()
{
	for (auto& rPair : m_mapTexture)
		Safe_Delete(rPair.second);
	m_mapTexture.clear(); 
}
