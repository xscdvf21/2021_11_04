#pragma once
class CTexture abstract
{
public:
	explicit CTexture();
	virtual ~CTexture();

	// sbcs, mbcs, wbcs
	/*
	sbcs - single byte charter set - ���� �ϳ��� ó���ϴµ� 1����Ʈ�� ����ϰڴ�. -��ǥ���� �ڵ�ǥ�� �ƽ�Ű �ڵ�ǥ.
	mbcs -							- ���� �ϳ��� ó���ϴµ� 1����Ʈ�� ó���� �����ϴٸ� 1����Ʈ�� �ȵǸ� 2����Ʈ�� ����ϰڴ�. cp949
	wbcs - wide byte				- ���ڸ� ó���Կ� �־� �ּ� 2����Ʈ ������ ����ϰڴ�. �����ڵ� 

	*/
public:
	virtual	HRESULT Insert_Texture(const wstring& wstrFilePath, const wstring& wstrStateKey = L"", const DWORD& dwCount = 0)PURE;
	virtual	const TEXINFO* Get_Texture(const wstring& wstrStateKey = L"", const DWORD& dwCount = 0)=0;
	virtual void Release_Texture() PURE; 
		
};

