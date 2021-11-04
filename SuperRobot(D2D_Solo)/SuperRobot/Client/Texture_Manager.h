#pragma once

class CTexture; 
class CTexture_Manager
{
public:
DECLEARE_SINGLETON(CTexture_Manager)
public:
	enum ID { SINGLE_TEX, MULTI_TEX};
private:
	CTexture_Manager();
	~CTexture_Manager();
	//복사생성자 정의.
	//대입연산자 정의. 
public:
	HRESULT LoadImagePath(const wstring& wstrFilePath); 
public:
	const TEXINFO* Get_TexInfo_TextureMgr(const wstring& wstrObjectKey, const wstring& wstrStateKey = L"", const DWORD& dwIndex = 0);
	HRESULT Insert_TextureMgr(ID eID, const wstring& wstrFilePath,const wstring& wstrObjectKey, const wstring& wstrStateKey = L"", const DWORD& dwCount = 0);
	void Release_TextureMgr(); 
private:
	//map<ObjectKey,CTexture*>
	map<wstring, CTexture*> m_mapTexture; 

};

