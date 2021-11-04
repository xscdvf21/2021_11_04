#pragma once

class CTexture; 
class CTexture_Manager
{
public:
	static CTexture_Manager* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CTexture_Manager; 
		return m_pInstance; 
	}
	static void Destroy_Instance()
	{
		if (m_pInstance )
		{
			delete m_pInstance; 
			m_pInstance = nullptr; 
		}
	}
public:
	enum ID { SINGLE_TEX, MULTI_TEX};
private:
	CTexture_Manager();
	~CTexture_Manager();
	//복사생성자 정의.
	//대입연산자 정의. 
public:
	const TEXINFO* Get_TexInfo_TextureMgr(const wstring& wstrObjectKey, const wstring& wstrStateKey = L"", const DWORD& dwIndex = 0);
	HRESULT Insert_TextureMgr(ID eID, const wstring& wstrFilePath,const wstring& wstrObjectKey, const wstring& wstrStateKey = L"", const DWORD& dwCount = 0);
	void Release_TextureMgr(); 
private:
	static CTexture_Manager* m_pInstance; 
	//map<ObjectKey,CTexture*>
	map<wstring, CTexture*> m_mapTexture; 

};

