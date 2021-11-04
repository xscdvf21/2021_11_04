#pragma once

#ifndef __GRAPHIC_DEVICE_H__
#define __GRAPHIC_DEVICE_H__

#include "Base.h"

BEGIN(Engine)

enum class ENGINE_DLL EDisplayMode
{
	Full,
	Win
};

class ENGINE_DLL CGraphicDevice final : public CBase
{
	DECLARE_SINGLETON(CGraphicDevice)

private :
	CGraphicDevice();
	virtual ~CGraphicDevice() = default;

public:
	_Device	Get_Device() { return m_pDevice; }
	LPD3DXFONT Get_Font() { return m_pFont; }
	LPD3DXSPRITE Get_Sprite() { return m_pSprite; }
public:
	HRESULT Ready_GraphicDevice(HWND hWnd, int iWinCX, int iWinCY, EDisplayMode eDisplayMode);

public:
	virtual void Free() override;

private:
	LPDIRECT3D9		m_pSDK; // �׷���ī���� ������ �����ϰ� �����ϴ� ��ü. 
	_Device			m_pDevice; // �ְ� ���������� �׷��� ��ġ�� �����ϴ� ��ü. 
	LPD3DXSPRITE	m_pSprite;
	LPD3DXFONT		m_pFont;

};

END

#endif // !__GRAPHIC_DEVICE_H__