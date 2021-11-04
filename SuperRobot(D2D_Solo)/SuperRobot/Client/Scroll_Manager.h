#pragma once
class CScroll_Manager
{
public:
	enum SCROLL{SCROLL_X,SCROLL_Y};
public:
	CScroll_Manager();
	~CScroll_Manager();
public:
	static _vec3 Get_Scroll(){ return m_vScroll; }
	static float Get_Scroll(SCROLL eScrollID); 
	static void Set_Scroll(const _vec3& vScroll);

	static void Scroll_Lock();

	
private:
	static _vec3 m_vScroll; 


};

