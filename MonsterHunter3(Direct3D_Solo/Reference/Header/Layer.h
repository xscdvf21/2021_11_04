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
	CComponent*										Get_Component(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	HRESULT											Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject);
	CGameObject*									Get_GameObject(const _tchar* pObjTag);
	multimap<const _tchar*, CGameObject*>*			Get_LayerMap();
public:
	virtual HRESULT		Ready_Layer(void);
	virtual _int		Update_Layer(const _float& fTimeDelta);


	

private:
	multimap<const _tchar*, CGameObject*>		m_mapObject;
		
public:
	static CLayer*	Create(void);
	virtual void	Free(void);

};

END
#endif // Layer_h__
