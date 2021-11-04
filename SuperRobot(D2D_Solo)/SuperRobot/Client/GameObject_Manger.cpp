#include "stdafx.h"
#include "GameObject_Manger.h"
#include "GameObject.h"
#include "CollisionMgr.h"
#include "MousePointer.h"
#include "GunDamMK.h"

IMPLEMENT_SINGLETON(CGameObject_Manger)
CGameObject_Manger::CGameObject_Manger()
{
}


CGameObject_Manger::~CGameObject_Manger()
{
	Release_GameObjectManager();
}

void CGameObject_Manger::Add_GameObject(OBJID eID, CGameObject * pObj)
{
	m_listObject[eID].emplace_back(pObj); 
}

void CGameObject_Manger::Remove_Monster(OBJID eID)
{
	m_listObject[eID].pop_front();
}

void CGameObject_Manger::Update_GameObjectManager()
{
	for (int i = 0 ; i < OBJ_END ; ++i)
	{
		list<CGameObject*>::iterator iter = m_listObject[i].begin(); 
		for (; iter != m_listObject[i].end(); )
		{
			int iEvent = (*iter)->Update_GameObject();
			if (OBJ_DEAD == iEvent)
			{
				Safe_Delete(*iter);
				iter = m_listObject[i].erase(iter);
			}
			else
				++iter; 
		}
	}

	CCollisionMgr::Collision_Rect(m_listObject[OBJID::OBJ_UNIT], m_listObject[OBJID::OBJ_CURSOR]);
	CCollisionMgr::Collision_Rect2(m_listObject[OBJID::OBJ_MONSTER], m_listObject[OBJID::OBJ_CURSOR]);
}

void CGameObject_Manger::Late_Update_GameObjectManager()
{
	for (int i = 0 ; i < OBJ_END ; ++i)
	{
		for (auto& pObject : m_listObject[i])
		{
			pObject->LateUpdate_GameObject(); 
		}
	}
}

void CGameObject_Manger::Render_GameObjectManager()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObject : m_listObject[i])
		{
			pObject->Render_GameObject();
		}
	}
}

void CGameObject_Manger::Release_GameObjectManager()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObject : m_listObject[i])
			Safe_Delete(pObject); 

		m_listObject[i].clear(); 
	}
}

