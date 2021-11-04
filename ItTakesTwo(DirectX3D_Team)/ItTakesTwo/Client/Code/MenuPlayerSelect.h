#ifndef __MENUPLAYERSELECT_H__

#include "Base.h"
#include "Scene.h"
#include "Loading.h"
#include "MenuUI.h"

BEGIN(Engine)
END

class CMenuPlayerSelect : public CScene
{
	enum UI_NAME
	{
		UI_MENUHEADER,  UI_CONTINUETITLE, UI_TITLE,
		UI_PLAYER1_BOX, UI_PLAYER2_BOX, UI_BACK_BOX, UI_BACK, UI_BACKGROUND,
		UI_END
	};

private:
	explicit CMenuPlayerSelect(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual ~CMenuPlayerSelect();

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int	Update_Scene(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta) override;
	virtual void	Render_Scene(void) override;

private:
	HRESULT			Ready_LoadingUI_Layer(const _tchar* pLayerTag);
	HRESULT			Ready_GameObject_Layer(const _tchar* pLayerTag);
	HRESULT			Ready_LightInfo();

public:
	static	CMenuPlayerSelect*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free() override;

private:
	class CImageUI*		m_pUI[UI_NAME::UI_END];

private:
	//Player1
	_vec3				m_vLeftBoxPos;
	_vec3				m_vRightBoxPos;
	_vec3				m_vCenterBoxPos;

	_vec3				m_vCenterSize;
	_vec3				m_vMoveSize;

	_vec3				m_vLeftKeyboardPos;
	_vec3				m_vRightKeyBoardPos;
	_vec3				m_vCenterKeyBoardPos;

	
	_bool				m_bPlayer1_Ready;
	_bool				m_bLeftMove;
	_bool				m_bRightMove; 

private:
	//Player2
	_vec3				m_vSelectPos;
	_int				m_iKeyboardSelect;
	_int				m_iXBoxSelect;

	_bool				m_bPlayer2_Ready;

	_vec3				m_vLeftXBoxPos;
	_vec3				m_vRightXBoxPos;
	_vec3				m_vCenterXBoxPos;

	_vec3				m_vLeftBoxPos2;
	_vec3				m_vRightBoxPos2;
	_vec3				m_vCenterBoxPos2;

	_bool				m_bLeftMoveXBOX;
	_bool				m_bRightMoveXBOX;
	_bool				m_bMoveXBOXEnd;

private:
	CGameObject*		m_pLeftHand = nullptr;
	CGameObject*		m_pRightHand = nullptr;
	
private :
	_float				m_fInterval = 0.2f;
	_float				m_fCurTime = 0.f;
	

};

#define __MENUPLAYERSELECT_H__
#endif

