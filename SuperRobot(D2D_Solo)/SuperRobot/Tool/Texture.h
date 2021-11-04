#pragma once
class CTexture abstract
{
public:
	explicit CTexture();
	virtual ~CTexture();

	// sbcs, mbcs, wbcs
	/*
	sbcs - single byte charter set - 문자 하나를 처리하는데 1바이트를 사용하겠다. -대표적인 코드표는 아스키 코드표.
	mbcs -							- 문자 하나를 처리하는데 1바이트로 처리가 가능하다면 1바이트로 안되면 2바이트로 사용하겠다. cp949
	wbcs - wide byte				- 문자를 처리함에 있어 최소 2바이트 단위를 사용하겠다. 유니코드 

	*/
public:
	virtual	HRESULT Insert_Texture(const wstring& wstrFilePath, const wstring& wstrStateKey = L"", const DWORD& dwCount = 0)PURE;
	virtual	const TEXINFO* Get_Texture(const wstring& wstrStateKey = L"", const DWORD& dwCount = 0)=0;
	virtual void Release_Texture() PURE; 
		
};

