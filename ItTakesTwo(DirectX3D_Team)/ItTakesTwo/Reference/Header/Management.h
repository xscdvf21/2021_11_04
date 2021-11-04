#ifndef Management_h__
#define Management_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Scene.h"

BEGIN(Engine)

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement(void);
	virtual ~CManagement(void);

public:
	CComponent*		Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	CGameObject*	Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);
	CLayer*			Get_Layer(const _tchar* pLayerTag);
	CScene*			Get_Scene() { return m_pScene; }

public:
	HRESULT			Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	HRESULT			Add_Layer(const _tchar* pLayerTag, CLayer* pLayer);

public:
	HRESULT			Set_Scene(CScene* pScene, LPDIRECT3DDEVICE9 pDevice);
	_int			Update_Scene(const _float& fTimeDelta);
	_int			LateUpdate_Scene(const _float& fTimeDelta);
	void			Render_Scene(const _float& fTimeDelta, const _vec3& vBackBuffer);

private:
	CScene*			m_pScene;

public:
	virtual void	Free(void);

};

END
#endif // Management_h__
