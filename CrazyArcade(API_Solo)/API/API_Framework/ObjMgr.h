#pragma once

#ifndef __OBJMGR_H__
#define __OBJMGR_H__

class CObj;
class CObjMgr
{
private:
	CObjMgr();
	~CObjMgr();

public:
	void Add_Object(CObj* _pObj, OBJID::ID _eID);
	void Update();
	void Late_Update();
	void Render(HDC _DC);
	void Release();

public:
	CObj* Get_Target(CObj* _pObj, OBJID::ID _eID);
	CObj* Get_Player() { return m_listObj[OBJID::PLAYER].front(); }
	
	list<CObj*> Get_list(OBJID::ID _eID) { return m_listObj[_eID]; } // 리스트를 반환하는 함수. Get은 여기있는 값을 저길로 가져가는것.
	//list<CObj*>자체가 여기있는 것이므로 이것을 반환하는데 원본값을 던져야 하기에 레퍼런스를 붙임. 원본값을 안던지면 모르기떄문.?

public:
	void Delete_ID(OBJID::ID _eID);

public:
	static CObjMgr* Get_Instance()
	{
		if(!m_pInstance)
			m_pInstance = new CObjMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	list<CObj*>			m_listObj[OBJID::END];
	list<CObj*>			m_listRender[RENDERID::END];
	static CObjMgr*		m_pInstance;
};



#endif // !__OBJMGR_H__
