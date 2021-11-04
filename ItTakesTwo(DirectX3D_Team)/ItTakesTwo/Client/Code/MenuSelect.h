#ifndef __MENUSELECT_H__

#include "Base.h"
#include "Scene.h"
#include "Loading.h"
#include "MenuUI.h"

BEGIN(Engine)
END


class CMenuSelect : public CScene
{
private:
	explicit CMenuSelect(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual ~CMenuSelect();

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int	Update_Scene(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta) override;
	virtual void	Render_Scene(void) override;


private:
	HRESULT			Ready_LoadingUI_Layer(const _tchar* pLayerTag);

public:
	static CMenuSelect*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	virtual void	Free() override;

private:
	_bool				m_bKeyPress = false;
	_float				m_fInterval = 0.1f;
	_float				m_fCurTime = 0.f;
	_vec3				m_vSelectPos;
	_int				m_iSelectMenu;
	_float				m_fLocal = 0;
	_float				m_fOnline = 0;
	_float				m_fOption = 0;
	_float				m_fOption2 = 0;
	_float				m_fMade = 0;
	_float				m_fQuit = 0;
	_bool				m_bDir = false;

private:
	class CImageUI*		m_pMenuBG = nullptr;
	class CImageUI*		m_pUI = nullptr;
	class CImageUI*		m_pLocal = nullptr;
	class CImageUI*		m_pOnline = nullptr;
	class CImageUI*		m_pOption = nullptr;
	class CImageUI*		m_pOption2 = nullptr;
	class CImageUI*		m_pMade = nullptr;
	class CImageUI*		m_pQuit = nullptr;

private:
	CLoading*			m_pLoading;
};

#define __MENUSELECT_H__
#endif

