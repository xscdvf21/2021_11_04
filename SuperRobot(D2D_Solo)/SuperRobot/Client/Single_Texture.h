#pragma once
#include "Texture.h"
class CSingle_Texture final:
	public CTexture
{
public:
	explicit CSingle_Texture();
	virtual ~CSingle_Texture();
public:
	// CTexture��(��) ���� ��ӵ�
	virtual HRESULT Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey = L"", const DWORD & dwCount = 0) override;
	virtual const TEXINFO* Get_Texture(const wstring & wstrStateKey = L"", const DWORD & dwCount = 0) override;
	virtual void Release_Texture(); 
private:
	TEXINFO m_tTexInfo; 
};

