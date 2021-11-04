#include "stdafx.h"
#include "Texture_Manager.h"
#include "Single_Texture.h"
#include "Multi_Texture.h"
CTexture_Manager* CTexture_Manager::m_pInstance = nullptr; 
// �ȹ����. 

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
	// ��, ���� ó�� ȣ��Ǿ��� ���. 
	if (iter_find == m_mapTexture.end())
	{
		// ������ ����. 
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
		// �ֵ�� ���⿡���� ���� Ű������ �������� ã�Ҵ� ã�Ƽ� ���ض�? �� �������� Insert_Texture �Լ��� ȣ���ض�. 
		// ���⼭�� ��������?? CTexture �װ͵� CMulti_Texture ��ü. ��� �����? �� if���ȿ��� �����. 
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