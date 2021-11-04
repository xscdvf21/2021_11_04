#include "..\Headers\Texture.h"

USING(Engine)
CTexture::CTexture(LPDIRECT3DDEVICE9 pDevice, ETextureType eTextureType, TCHAR* pFilePath, _uint iCount)
	:CComponent(pDevice),
	m_eTextureType(eTextureType),
	m_pFilePath(pFilePath),
	m_iCount(iCount)
{
}

CTexture::CTexture(const CTexture & other)
	:CComponent(other),
	m_eTextureType(other.m_eTextureType),
	m_pFilePath(other.m_pFilePath),
	m_iCount(other.m_iCount),
	m_Textures(other.m_Textures)
{
	for (auto& pObj : m_Textures)
	{
		Safe_AddReference(pObj);
	}
}

HRESULT CTexture::Ready_Component_Prototype()
{
	/* ex) m_pFilePath: L"../Resources/Monster%d.png" */
	TCHAR szFullPath[256] = L"";
	IDirect3DBaseTexture9* pTexture = nullptr;
	HRESULT hr = 0;

	m_Textures.reserve(m_iCount); //������ ȣ�� �Ҷ� ���� ���ڸ�ŭ �Ҵ�

	for (_uint i = 0; i < m_iCount; ++i)
	{
		swprintf_s(szFullPath, m_pFilePath, i); //���ڷ� ���� ��θ� szFullPath�� ���µ�

		switch (m_eTextureType)
		{
		case Engine::ETextureType::Normal:
			hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture);
			break;
		case Engine::ETextureType::Cube:
			hr = D3DXCreateCubeTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);		
			break;
		case Engine::ETextureType::Font:
			hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture);
			break;


		}

		if (FAILED(hr))
		{
			TCHAR szInputMessage[256] = L"Failed To CreateTexture From %s. (Engine - Texture.cpp)";
			TCHAR szLogMessage[256] = L"";
			swprintf_s(szLogMessage, szInputMessage, szFullPath);
			PRINT_LOG(L"Error", szLogMessage);
			return E_FAIL;
		}
		Safe_AddReference(pTexture);
		m_Textures.push_back(pTexture); //������ ���ٸ� ����
	}



	return S_OK;
}

HRESULT CTexture::Ready_Component(void * pArg)
{


	return S_OK;
}

HRESULT CTexture::Set_Texture(_uint iIndex)
{
	if (m_Textures.size() <= iIndex) //���� ���ں��� ����� ������ �ȵ�.
		return E_FAIL;

	/* �� ������ �׷��� ���ؽ��� ���ν�ų �ؽ�ó�� ��ġ���� �Ѱ���. */
	return m_pDevice->SetTexture(0, m_Textures[iIndex]);
}

HRESULT CTexture::Load_Texture(const wstring & wstrFilePath)
{
	//wifstream  fin;
	//fin.open(wstrFilePath);

	//if (fin.fail())
	//{
	//	PRINT_LOG(L"Error" ,L"File Open Failed CTextrue.cpp ");

	//	return E_FAIL;
	//}

	//TCHAR szPath[MAX_PATH] = L"";
	//TCHAR szObjectKey[MAX_PATH] = L"";
	//TCHAR szStateKey[MAX_PATH] = L"";
	//TCHAR szCount[MAX_PATH] = L"";

	//while (true)
	//{
	//	fin.getline(szPath, MAX_PATH, L'|');
	//	fin.getline(szObjectKey, MAX_PATH, L'|');
	//	fin.getline(szStateKey, MAX_PATH, L'|');
	//	fin.getline(szCount, MAX_PATH);
	//	DWORD dwCount = _ttoi(szCount);

	//	if (0 == dwCount)
	//		break;

	//	if (FAILED(Ready_Component_Prototype(MULTI_TEX, szPath, szObjectKey, szStateKey, dwCount)))
	//	{
	//		PRINT_LOG(L"Error",L"���� �ִٰ� ����.");
	//		fin.close();
	//		return E_FAIL;
	//	}
	//}
	//fin.close();
	return S_OK;
}

CTexture *CTexture::Create(LPDIRECT3DDEVICE9 pDevice, ETextureType eTextureType, TCHAR* pFilePath, _uint iCount)
{
	CTexture* pInstance = new CTexture(pDevice, eTextureType, pFilePath, iCount);
	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CTexture (Engine - Texture.cpp)");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent *CTexture::Clone(void * pArg)
{
	CTexture* pClone = new CTexture(*this);
	if (FAILED(pClone->Ready_Component(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CTexture");
		Safe_Release(pClone);
	}

	return pClone;
}

void CTexture::Free()
{
	for (auto& pTexture : m_Textures)
	{
		Safe_Release(pTexture);
	}
	
	m_Textures.clear();

	CComponent::Free();
}
