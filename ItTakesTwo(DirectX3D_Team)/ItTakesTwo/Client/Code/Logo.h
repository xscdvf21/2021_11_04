#ifndef Logo_h__
#define Logo_h__


#include "Scene.h"
#include "Base.h"
#include "Loading.h"
#include "LogoBack.h"
#include "ImageUI.h"

BEGIN(Engine)
class CLogo : public CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLogo();

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int	Update_Scene(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta) override;
	virtual void	Render_Scene(void) override;

private:
	HRESULT			Ready_Resources(LPDIRECT3DDEVICE9& pGraphicDev, RESOURCEID eMax);
	HRESULT			Ready_Environment_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_Component_Layer(const _tchar*  pLayerTag);
	HRESULT			Ready_LoadingUI_Layer(const _tchar* pLayerTag);

public:
	static CLogo*	Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENEID eSceneID,const _vec3& vPos);
	static CLogo*	Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENEID eSceneID);

private:
	virtual void	Free() override;

private:
	_matrix			m_matWorld, m_matView, m_matProj;
	CLoading*		m_pLoading;
	SCENEID			m_eNextScene;
	_bool			m_bStart;
	_float			m_fBubbleTime;
	_int			m_iCount;

private:
	CImageUI*		m_pLoadingText = nullptr;
	CImageUI*		m_pLoadingImage = nullptr;
};
END
#endif // Logo_h__
