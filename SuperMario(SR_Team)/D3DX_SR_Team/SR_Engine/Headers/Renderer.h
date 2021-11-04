#pragma once

#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Base.h"

BEGIN(Engine)
enum class ENGINE_DLL ERenderPriority
{
	Priority,			//�ֿ켱 ���
	NoAlpha,			//������
	HalfAlpha,			//������
	Alpha,				//����
	WireFrame,			//���̾�������
	UI,					//UI
	End
};

class ENGINE_DLL CRenderer : public CBase
{
	DECLARE_SINGLETON(CRenderer)

private:
	/*explicit*/ CRenderer(/*LPDIRECT3DDEVICE9 pDevice*/);
	virtual ~CRenderer() = default;

public:
	HRESULT Set_Device(_Device pDevice, LPD3DXSPRITE pSprite, LPD3DXFONT pFont);

public:
	HRESULT Add_RenderList(ERenderPriority ePriority, class CGameObject* pGameObject);
	HRESULT Render_RenderList(HWND hWND = nullptr);

private:
	HRESULT Render_Priority();
	HRESULT Render_NoAlpha();
	HRESULT Render_Alpha();
	HRESULT Render_HalfAlpha();
	HRESULT Render_Wireframe();
	HRESULT Render_UI();

public:
	virtual void Free() override;

private:
	typedef list<class CGameObject*>	GAMEOBJECTS;
	GAMEOBJECTS	m_GameObjects[(_uint)ERenderPriority::End];

	_Device m_pDevice;
	LPD3DXSPRITE m_pSprite;
	LPD3DXFONT	m_pFont;

};
END
#endif