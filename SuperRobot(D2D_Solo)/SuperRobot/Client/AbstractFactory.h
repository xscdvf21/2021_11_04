#pragma once

class CGameObject;

template <typename T>
class CAbstractFactory
{
public :
	static CGameObject* Create()
	{
		CGameObject*	pObj = new T;
		pObj->Ready_GameObject();

		return pObj;
	}

	static CGameObject* Create( )
	{
		CGameObject*	pObj = new T;
		pObj->Ready_GameObject();

		return pObj;
	}
};
