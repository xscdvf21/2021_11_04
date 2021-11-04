#ifndef __MENU_H__


#include "Scene.h"
#include "Base.h"
#include "Loading.h"
#include "MenuUI.h"

BEGIN(Engine)
END

class CMenu : public CScene
{
private:
	explicit CMenu(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMenu();

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int	Update_Scene(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta) override;
	virtual void	Render_Scene(void) override;

private:
	HRESULT			Ready_LoadingUI_Layer(const _tchar* pLayerTag);

private:
	class CImageUI* m_pMenuBG = nullptr;
	class CImageUI* m_pPicture = nullptr;
	class CImageUI* m_pPictureBack = nullptr;
	class CImageUI* m_pChapterTitle = nullptr;
	class CImageUI* m_pPictureTitle = nullptr;
	class CImageUI* m_pLocalPlay = nullptr;
	class CImageUI* m_pLocalPlayText = nullptr;
	class CImageUI* m_pMenuBox = nullptr;
	class CImageUI* m_pMenuBar = nullptr;
	class CImageUI* m_pNewGame = nullptr;
	class CImageUI* m_pContinue = nullptr;
	class CImageUI* m_pChapter = nullptr;
	class CImageUI* m_pMinigame = nullptr;
	class CImageUI* m_pPlayer1 = nullptr;
	class CImageUI* m_pPlayer2 = nullptr;
	class CImageUI* m_pPlayerBox1 = nullptr;
	class CImageUI* m_pPlayerBox2 = nullptr;
	class CImageUI* m_pBackBox = nullptr;
	class CImageUI* m_pBack = nullptr;
	class CImageUI* m_pContinueBox = nullptr;
	class CImageUI* m_pContinueBtn = nullptr;

private:
	_bool	m_bPlayer1 = false;
	_bool	m_bPlayer2 = false;

public:
	static CMenu*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free() override;
};


#define __MENU_H__
#endif
