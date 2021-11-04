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
	
	list<CObj*> Get_list(OBJID::ID _eID) { return m_listObj[_eID]; } // ����Ʈ�� ��ȯ�ϴ� �Լ�. Get�� �����ִ� ���� ����� �������°�.
	//list<CObj*>��ü�� �����ִ� ���̹Ƿ� �̰��� ��ȯ�ϴµ� �������� ������ �ϱ⿡ ���۷����� ����. �������� �ȴ����� �𸣱⋚��.?

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
