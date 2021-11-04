#pragma once
class CGameObject; 
class CGameObject_Manger 
{
	DECLEARE_SINGLETON(CGameObject_Manger)
public:
	enum OBJID{ OBJ_GROUND,  OBJ_TERRAIN, OBJ_MONSTER, OBJ_UNIT, OBJ_CURSOR, OBJ_UI, OBJ_BACKGROUND, OBJ_EFFECT,  OBJ_END};
private:
	CGameObject_Manger();
	~CGameObject_Manger();
	void operator=(const CGameObject_Manger& rObj) {};
	CGameObject_Manger(const CGameObject_Manger& rObj) {}; 

public:
	CGameObject* Get_Mouse() { return m_listObject[OBJID::OBJ_CURSOR].front(); }
	CGameObject* Get_Player() { return m_listObject[OBJID::OBJ_UNIT].front(); }
	CGameObject* Get_Tile() { return m_listObject[OBJID::OBJ_GROUND].front(); }
	CGameObject* Get_Effect() { return m_listObject[OBJID::OBJ_EFFECT].front(); }
	CGameObject* Get_Monster() { return m_listObject[OBJID::OBJ_MONSTER].front(); }
	list<CGameObject*> Get_list(OBJID _eID) { return m_listObject[_eID]; }

	list<CGameObject*>& Get_list2(OBJID _eID) { return m_listObject[_eID]; }

	list<CGameObject*>* Get_Alllist() { return m_listObject; }

public:
	void Add_GameObject(OBJID eID, CGameObject* pObj);
	void Remove_Monster(OBJID eID);
	void Update_GameObjectManager(); 
	void Late_Update_GameObjectManager(); 
	void Render_GameObjectManager(); 
	void Release_GameObjectManager(); 
private:
	list<CGameObject*> m_listObject[OBJ_END]; 

};

