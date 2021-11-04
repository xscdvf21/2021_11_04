#include "GraphicDevice.h"

USING(Engine)
IMPLEMENT_SINGLETON(CGraphicDevice)


CGraphicDevice::CGraphicDevice()
{
}

HRESULT CGraphicDevice::Ready_GraphicDevice(HWND hWnd, int iWinCX, int iWinCY, EDisplayMode eDisplayMode)
{
	D3DCAPS9 DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		PRINT_LOG(L"Error", L"GetDevice Caps Failed");
		return E_FAIL;
	}
	//버텍스 프로세싱 체크 파트인데 굳이 소프트웨어 파트를 남겨놔야하나 싶기도 하고
	DWORD vp = 0;
	if (DeviceCaps.DevCaps & D3DCREATE_HARDWARE_VERTEXPROCESSING) 
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	//멀티 쓰레드 사용 체크
	vp |= D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = iWinCX;
	d3dpp.BackBufferHeight = iWinCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = (int)eDisplayMode;// TRUE일 경우 창모드, FALSE 전체화면. 
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, vp, &d3dpp, &m_pDevice)))
	{
		PRINT_LOG(L"Error", L"GraphicDevice Creating Failed");
		return E_FAIL;
	}

	//if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	//{
	//	PRINT_LOG(L"Error", L"Sprite Creating Failed");
	//	return E_FAIL;
	//}

	//D3DXFONT_DESCW tFontInfo;
	//ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	//tFontInfo.Height = 30;
	//tFontInfo.Width = 15;
	//tFontInfo.Weight = FW_HEAVY;
	//tFontInfo.CharSet = HANGUL_CHARSET;
	//lstrcpy(tFontInfo.FaceName, L"맑은 고딕");

	//if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	//{
	//	PRINT_LOG(L"Error",L"Font Creating Failed");
	//	return E_FAIL;
	//}


	return S_OK;
}

void CGraphicDevice::Free()
{
	//마지막으로 해제될 Free, 여기서 값이 남아있다면 로그를 띄운다.
	//if (Safe_Release(m_pSprite))
	//{
	//	PRINT_LOG(L"Warning", L"Failed To Release LPD3DXSPRITE");
	//}
	//if (Safe_Release(m_pFont))
	//{
	//	PRINT_LOG(L"Warning", L"Failed To Release LPD3DXFONT");
	//}
	_uint refCount = Safe_Release(m_pDevice);

	if (refCount)
	{
		TCHAR tDevice[] = L"Failed To Release LPDIRECT3DDEVICE9 : %d";
		TCHAR tAnswer[256] = L"";
		swprintf_s(tAnswer, tDevice, refCount);

		PRINT_LOG(L"Warning", tAnswer);
	}
	refCount = Safe_Release(m_pSDK);
	if (refCount)
	{
		TCHAR tD3D[] = L"Failed To Release LPDIRECT3D9 : %d";
		TCHAR tAnswer[256] = L"";
		swprintf_s(tAnswer, tD3D, refCount);
		PRINT_LOG(L"Warning", tAnswer);
	}


}
