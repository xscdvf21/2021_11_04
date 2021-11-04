#pragma once
#include "Texture.h"
class CMulti_Texture final :
	public CTexture
{
public:
	explicit CMulti_Texture();
	virtual ~CMulti_Texture();
public:
	// CTexture을(를) 통해 상속됨
	virtual HRESULT Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey = L"", const DWORD & dwCount = 0) override;
	virtual const TEXINFO * Get_Texture(const wstring & wstrStateKey = L"", const DWORD & dwCount = 0) override;
	virtual void Release_Texture() override;

private:
	//map<StateKey, VECTOR<TEXINFO>>
	map<wstring, vector<TEXINFO*>> m_mapTexture; 
};

