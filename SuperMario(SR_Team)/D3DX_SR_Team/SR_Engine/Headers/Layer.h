#pragma once 
#ifndef __LAYER_H__

#include "Base.h"
BEGIN(Engine)

class CLayer final : public CBase
{
private :
	CLayer();
	virtual ~CLayer() = default;

public:
	class CGameObject* Get_GameObject(_uint iIndex = 0);
	class CComponent* Get_Component(const wstring& ComponentTag, _uint iIndex = 0);

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	_uint Update_Layer(_float fDeltaTime);
	_uint LateUpdate_Layer(_float fDeltaTime);

public :
	static CLayer*	Create();
	virtual void Free() override;

public:
	auto Get_GameObjects() { return m_GameObjects; }

private :
	typedef list<class CGameObject*> GAMEOBJECTS;
	GAMEOBJECTS m_GameObjects;		//오브젝트들의 복제본을 보관함.



};

END


#define __LAYER_H__
#endif // !__LAYER_H__
