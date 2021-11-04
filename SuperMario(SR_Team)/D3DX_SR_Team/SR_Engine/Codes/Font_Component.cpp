#include "..\Headers\Font_Component.h"


USING(Engine)

Engine::CFont_Component::CFont_Component(LPDIRECT3DDEVICE9 pDevice)
	:CComponent(pDevice)
{
}

Engine::CFont_Component::CFont_Component(const CFont_Component & other)
	:CComponent(other)
{

}

HRESULT Engine::CFont_Component::Ready_Component_Prototype()
{
	if (FAILED(CComponent::Ready_Component_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT Engine::CFont_Component::Ready_Component(void * pArg)
{
	if (FAILED(CComponent::Ready_Component(pArg)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Engine::CFont_Component::Set_Font(_uint _iHeight, _uint _iWidth, _uint _iWeight, TCHAR* _pText)
{


	D3DXCreateFont(m_pDevice, _iHeight, _iWidth,_iWeight, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"맑은고딕", &m_pFont);

	TCHAR szBuf[128] = L"";

	lstrcmp(szBuf, _pText);

	//사각형 영역 설정 , -1은 옵션에 NOCLIP을 넣었기 때문에 , 값을 주지 않은것이다

	RECT rc = { 100, 100, -1, -1 };

	return m_pFont->DrawText(NULL, szBuf, -1, &rc, DT_LEFT | DT_NOCLIP, 0xffffff00);;
}

CFont_Component * Engine::CFont_Component::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFont_Component* pInstance = new CFont_Component(pDevice);
	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CTexture (Engine - CFont_Component.cpp)");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * Engine::CFont_Component::Clone(void * pArg)
{
	CFont_Component* pClone = new CFont_Component(*this);
	if (FAILED(pClone->Ready_Component(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CTexture");
		Safe_Release(pClone);
	}

	return pClone;
}

void Engine::CFont_Component::Free()
{
	CComponent::Free();
}
