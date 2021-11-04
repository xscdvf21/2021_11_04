#ifndef Layer_h__
#define Layer_h__

#include "Engine_Define.h"
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer(void);
	virtual ~CLayer(void);

public:
	CComponent*		Get_Component(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	CGameObject*	Get_GameObject(const _tchar* pObjTag);
	CGameObject*	Get_GameObject(const PxActor* pActor);
	HRESULT			Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject);
	HRESULT			Add_GameObject(const PxActor* pActor, CGameObject* pGameObject);
	HRESULT			Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject, class CScene* CurScene);

public:
	virtual HRESULT		Ready_Layer(void);
	virtual _int		Update_Layer(const _float& fTimeDelta);
	virtual _int		LateUpdate_Layer(const _float & fTimeDelta);

public:
	map<_wstr, CGameObject*>		Get_GameObjectMap() { return m_mapObject; }

private:
	map<_wstr, CGameObject*>		m_mapObject;
	map<void*, CGameObject*>		m_mapPxObject;
		
public:
	static CLayer*	Create(void);
	virtual void	Free(void);

};

END
#endif // Layer_h__
