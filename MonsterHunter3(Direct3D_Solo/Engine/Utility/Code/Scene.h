#ifndef Scene_h__
#define Scene_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit	CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		~CScene(void);

public:
	CComponent*		Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	CGameObject*	Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);

	CLayer*			Get_Layer(const _tchar* pLayerTag);
public:
	virtual HRESULT		Ready_Scene(void);
	virtual _int		Update_Scene(const _float& fTimeDelta);
	virtual void		Render_Scene(void) PURE;

protected:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	map<const _tchar*, CLayer*>		m_mapLayer;


public:
	virtual void	Free(void);

};

END
#endif // Scene_h__
