#include "stdafx.h"
#include "Graphic_Device.h"

CGraphic_Device* CGraphic_Device::m_pInstance = nullptr;
CGraphic_Device::CGraphic_Device()
{
}


CGraphic_Device::~CGraphic_Device()
{
	Release_Graphic_Device();
}

HRESULT CGraphic_Device::Ready_Graphic_Device()
{
	D3DCAPS9 DeviceCaps; 
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9)); 

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION); 

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		ERR_MSG(L"GetDevice Caps Failed");
		return E_FAIL;
	}
	/*
	여기에서 한개를 더 조사를 해야하는데 버텍스 프로세싱이라는걸 하드웨어에서 할수 있는지를 조사를 해야한다. 
	정점변환 + 조명연산 = 버텍스 프로세싱 

	*/
	DWORD vp = 0; 
	if (DeviceCaps.DevCaps & D3DCREATE_HARDWARE_VERTEXPROCESSING)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING ;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; 

	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp)); 
	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality= 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = g_hWND;
	d3dpp.Windowed = TRUE;// TRUE일 경우 창모드, FALSE 전체화면. 
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWND, vp, &d3dpp, &m_pDevice)))
	{
		ERR_MSG(L"GraphicDevice Creating Failed"); 
		return E_FAIL;
	}
	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		ERR_MSG(L"Sprite Creating Failed");
		return E_FAIL;
	}
	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW)); 

	tFontInfo.Height = 20;
	tFontInfo.Width = 10; 
	tFontInfo.Weight = FW_HEAVY; 
	tFontInfo.CharSet = HANGUL_CHARSET; 
	lstrcpy(tFontInfo.FaceName, L"@휴먼둥근헤드라인");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		ERR_MSG(L"Font Creating Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CGraphic_Device::Release_Graphic_Device()
{
	if (m_pFont)
		m_pFont->Release(); 

	if (m_pSprite)
		m_pSprite->Release(); 
	if (m_pDevice)
		m_pDevice->Release();
	if (m_pSDK)
		m_pSDK->Release(); 

}

void CGraphic_Device::Render_Begin()
{
	// 랜더링 되는 과정은 1. 지운다. 2. 그리기 시작한다. 3. 다그린다.4.시연한다. 
	m_pDevice->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);
	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CGraphic_Device::Render_End(HWND hWND /*= nullptr*/)
{
	m_pSprite->End(); 
	m_pDevice->EndScene(); 
	m_pDevice->Present(nullptr, nullptr, hWND, nullptr);
}
