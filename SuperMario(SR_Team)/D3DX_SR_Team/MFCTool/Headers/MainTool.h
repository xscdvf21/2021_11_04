#pragma once
#include "../../Reference/Headers/Base.h"
#include "ManagementTool.h"





USING(Engine)
class CMainTool:public CBase
{
private:
	CMainTool();
	virtual ~CMainTool() = default;

public:
	HRESULT Ready_MainApp();
	_uint Update_MainApp();

private:
	HRESULT Setup_DefaultSetting();
	HRESULT Ready_StaticResources();
	HRESULT Ready_Clone();
	
public:
	static CMainTool* Create();
	virtual void Free() override;

private:
	CManagementTool* m_pManagementTool = nullptr;
	_Device	m_pDevice = nullptr;

	//MFC를 위한 Clone 함수.
public://Clone 호출하고 리턴받기.
	CGameObj* Clone_CubeColor();
	CGameObj* Clone_CubeTexture(int _iTextuerNumber = 0);
	CGameObj* Clone_CubeLine();
	CGameObj* Clone_SliderColor();
	CGameObj* Clone_BoxColor();

	HRESULT PrototypeImageFile(wstring _PrototypeTag, TCHAR * _FilePath, int _iImageCount = 1);

public:
	int m_iTerrainTextureCount = 0;

};

