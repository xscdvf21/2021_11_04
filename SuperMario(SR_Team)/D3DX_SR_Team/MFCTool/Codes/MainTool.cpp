#include "stdafx.h"
#include "..\Headers\MainTool.h"
#include "Plate.h"
#include "CubeColor.h"
#include "CubeTexture.h"
#include "CubeLine.h"
#include "SliderColor.h"
#include "BoxColor.h"

#include "TextureTool.h"

#include "VIBuffer_Plate.h"
#include "VIBuffer_CubeColorTool.h"
#include "VIBuffer_PlateTexture.h"
#include "VIBuffer_CubeTextureTool.h"
#include "VIBuffer_CubeLineTool.h"

#include "VIBuffer_SliderColorTool.h"


CMainTool::CMainTool()
	: m_pManagementTool(CManagementTool::Get_Instance())
{
	Safe_AddReference(m_pManagementTool);


}

HRESULT CMainTool::Ready_MainApp()
{
	if (nullptr == m_pManagementTool)
		return E_FAIL;

	if (FAILED(m_pManagementTool->Ready_Engine(g_hWndTool, 709/*WINCX*/, 709/*WINCY*/, EDisplayMode::Win, (_uint)0)))
	{
		PRINT_LOG(L"MainTool - err", L"그래픽 디바이스 초기 셋팅 실패.");
		return E_FAIL;
	}

	m_pDevice = m_pManagementTool->Get_Device();
	if (nullptr == m_pDevice)
	{
		PRINT_LOG(L"MainTool - err", L"그래픽 디바이스를 얻지 못 함.");
		return E_FAIL;
	}

	Safe_AddReference(m_pDevice);

#pragma region Prototype Function
	if (FAILED(Ready_StaticResources()))
	{
		PRINT_LOG(L"CMainTool", L"Prototype Ready 실패");
		return E_FAIL;
	}
#pragma endregion

#pragma region Clone Function
	if (FAILED(Ready_Clone()))
	{
		PRINT_LOG(L"CMainTool", L"Clone Ready 실패");
		return E_FAIL;
	}
#pragma endregion


	if (FAILED(Setup_DefaultSetting()))
	{
		PRINT_LOG(L"CMainTool", L"렌더링 초기 셋팅 실패.");
		return E_FAIL;
	}


	return S_OK;
}

_uint CMainTool::Update_MainApp()
{
	_uint iEvent = m_pManagementTool->Update_Engine();

	if (CHANGE_SCENE == iEvent)
		return NO_EVENT;
	
	if (FAILED(m_pManagementTool->Render_Engine(g_hWndTool)))
		return NO_EVENT;

	return NO_EVENT;
}

HRESULT CMainTool::Setup_DefaultSetting()
{
	/* 조명 off */
	if (FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
		return E_FAIL;

	/*
	D3DRS_CULLMODE: 후면 추려내기 옵션
	D3DCULL_CCW(Counter Clock Wise): 반시계방향인 면을 추려낸다. 디폴트값.
	D3DCULL_CW(Clock Wise): 시계방향인 면을 추려낸다.
	D3DCULL_NONE: 아무것도 추려내지 않음.
	*/

	if (FAILED(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainTool::Ready_StaticResources()
{
	//plate 프로토타입 set
#pragma region GameObj
	//Plate
	if (FAILED(m_pManagementTool->Add_GameObject_Prototype(
		L"GameObject_Plate",
		CPlate::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"판때기 프로토타입 생성 실패");
		return E_FAIL;
	}

	//CubeColor
	if (FAILED(m_pManagementTool->Add_GameObject_Prototype(
		L"GameObject_CubeColor",
		CCubeColor::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"CCubeColor 프로토타입 생성 실패");
		return E_FAIL;
	}

	//CubeTexture
	if (FAILED(m_pManagementTool->Add_GameObject_Prototype(
		L"GameObject_CubeTexture",
		CCubeTexture::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"CCubeTexture 프로토타입 생성 실패");
		return E_FAIL;
	}

	//CubeLine
	if (FAILED(m_pManagementTool->Add_GameObject_Prototype(
		L"GameObject_CubeLine",
		CCubeLine::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"CCubeLine 프로토타입 생성 실패");
		return E_FAIL;
	}
	//sliderColor
	if (FAILED(m_pManagementTool->Add_GameObject_Prototype(
		L"GameObject_SliderColor",
		CSliderColor::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"CSliderColor 프로토타입 생성 실패");
		return E_FAIL;
	}
	//BoxColor

	if (FAILED(m_pManagementTool->Add_GameObject_Prototype(
		L"GameObject_BoxColor",
		CBoxColor::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"BoxColor 프로토타입 생성 실패");
		return E_FAIL;
	}



#pragma endregion

	//컴포넌트 추가.
	/* For.Component_Transform */
	//Transform
#pragma region Transform

	if (FAILED(m_pManagementTool->Add_Component_Prototype(
		L"Component_Transform",
		CTransformTool::Create(m_pDevice))))
	{
		PRINT_LOG(L"CMainTool", L"TrasformTool / Component / Prototype 실패.");
		return E_FAIL;
	}
#pragma endregion

#pragma region VIBufferTool
	//VIBuffer_Plate 
	if (FAILED(m_pManagementTool->Add_Component_Prototype(
		L"Component_VIBuffer_Plate",
		CVIBuffer_Plate::Create(m_pDevice))))
	{
		PRINT_LOG(L"CMainTool", L"VIBuffer_Plate / Component / Prototype 실패");
		return E_FAIL;
	}

	//CVIBuffer_PlateTexture 
	if (FAILED(m_pManagementTool->Add_Component_Prototype(
		L"Component_VIBuffer_PlateTexture",
		CVIBuffer_PlateTexture::Create(m_pDevice))))
	{
		PRINT_LOG(L"CMainTool", L"VIBuffer_PlateTexture / Component / Prototype 실패");
		return E_FAIL;
	}

	//CVIBuffer_CubeColorTool
	if (FAILED(m_pManagementTool->Add_Component_Prototype(
		L"Component_VIBuffer_CubeColor",
		CVIBuffer_CubeColorTool::Create(m_pDevice))))
	{
		PRINT_LOG(L"CMainTool", L"CVIBuffer_CubeColorTool / Component / Prototype 실패");
		return E_FAIL;
	}

	//CVIBuffer_CubeLine
	if (FAILED(m_pManagementTool->Add_Component_Prototype(
		L"Component_VIBuffer_CubeLine",
		CVIBuffer_CubeLineTool::Create(m_pDevice))))
	{
		PRINT_LOG(L"CMainTool", L"CVIBuffer_CubeLineTool / Component / Prototype 실패");
		return E_FAIL;
	}

	//CVIBuffer_CubeTextuer
	if (FAILED(m_pManagementTool->Add_Component_Prototype(
		L"Component_VIBuffer_CubeTexture",
		CVIBuffer_CubeTextureTool::Create(m_pDevice))))
	{
		PRINT_LOG(L"CMainTool", L"CVIBuffer_CubeTextureTool / Component / Prototype 실패");
		return E_FAIL;
	}

	//CVIBuffer_SliderColorTool
	if (FAILED(m_pManagementTool->Add_Component_Prototype(
		L"Component_VIBuffer_SliderColor",
		CVIBuffer_CubeTextureTool::Create(m_pDevice))))
	{
		PRINT_LOG(L"CMainTool", L"Component_VIBuffer_SliderColor / Component / Prototype 실패");
		return E_FAIL;
	}


#pragma endregion


	//TextureTool
#pragma region TextureTool

	if (FAILED(m_pManagementTool->Add_Component_Prototype(
		L"Component_Texture_Plate",
		CTextureTool::Create(m_pDevice, ETextureType::Normal, L"../Resources/Plate_Texture%d.png", 1))))
	{
		PRINT_LOG(L"CMainTool", L"Plate / Texture / Prototype 실패");
		return E_FAIL;
	}

	m_iTerrainTextureCount = 15;
	if (FAILED(m_pManagementTool->Add_Component_Prototype(
		L"Component_Texture_Terrain",
		CTextureTool::Create(m_pDevice, ETextureType::Cube, L"../Resources/Terrain%d.dds", 15))))
	{
		PRINT_LOG(L"CMainTool", L"CCubeTextuer / Texture / Prototype 실패");
		return E_FAIL;
	}



#pragma endregion
	
	return S_OK;
}

HRESULT CMainTool::Ready_Clone()
{

	auto pManagementTool = CManagementTool::Get_Instance();
	if (nullptr == pManagementTool)
		return E_FAIL;

#pragma region Plate Clone
	if (FAILED(pManagementTool->Clone_GameObject_ToLayer(L"GameObject_Plate")))
	{
		return E_FAIL;
	}
#pragma endregion

#pragma region CubeColor
	/*if (FAILED(pManagementTool->Clone_GameObject_ToLayer(L"GameObject_CubeColor")))
	{
		return E_FAIL;
	}*/
#pragma endregion


	return S_OK;
}


CMainTool * CMainTool::Create()
{
	CMainTool* pInstance = new CMainTool;
	if (FAILED(pInstance->Ready_MainApp()))
	{
		PRINT_LOG(L"MainTool - err", L"MainTool Create 실패.");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainTool::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pManagementTool);
}

CGameObj* CMainTool::Clone_CubeColor()
{
	auto pManagementTool = CManagementTool::Get_Instance();

	CGameObj* pClone = (pManagementTool->Get_Clone_GameObject(L"GameObject_CubeColor"));

	if (nullptr == pClone)
	{
		return nullptr;
	}
	dynamic_cast<CCubeColor*>(pClone)->m_pTransformCom->Update_Transform();

	return pClone;
}

CGameObj* CMainTool::Clone_CubeTexture(int _iTextuerNumber)
{
	auto pManagementTool = CManagementTool::Get_Instance();

	CGameObj* pClone = pManagementTool->Get_Clone_GameObject(L"GameObject_CubeTexture");

	if (nullptr == pClone)
	{
		return nullptr;
	}

	dynamic_cast<CCubeTexture*>(pClone)->m_pTransformCom->Update_Transform();
	pClone->m_iTextuer = _iTextuerNumber;

	return pClone;
}

CGameObj * CMainTool::Clone_CubeLine()
{
	auto pManagementTool = CManagementTool::Get_Instance();

	CGameObj* pClone = pManagementTool->Get_Clone_GameObject(L"GameObject_CubeLine");

	if (nullptr == pClone)
	{
		return nullptr;
	}
	dynamic_cast<CCubeLine*>(pClone)->m_pTransformCom->Update_Transform();

	return pClone;
}

CGameObj * CMainTool::Clone_SliderColor()
{
	auto pManagementTool = CManagementTool::Get_Instance();

	CGameObj* pClone = (pManagementTool->Get_Clone_GameObject(L"GameObject_SliderColor"));

	if (nullptr == pClone)
	{
		return nullptr;
	}
	dynamic_cast<CSliderColor*>(pClone)->m_pTransformCom->Update_Transform();

	return pClone;
}

CGameObj * CMainTool::Clone_BoxColor()
{
	auto pManagementTool = CManagementTool::Get_Instance();

	CGameObj* pClone = (pManagementTool->Get_Clone_GameObject(L"GameObject_BoxColor"));

	if (nullptr == pClone)
	{
		return nullptr;
	}
	dynamic_cast<CBoxColor*>(pClone)->m_pTransformCom->Update_Transform();

	return pClone;
}

HRESULT CMainTool::PrototypeImageFile(wstring _PrototypeTag,TCHAR* _FilePath,int _iImageCount)
{

	if (FAILED(m_pManagementTool->Add_Component_Prototype(_PrototypeTag,
		CTextureTool::Create(m_pDevice, ETextureType::Normal, _FilePath, _iImageCount))))
	{
		PRINT_LOG(L"CMainTool", L"이미지 가져오기 실패");
		return E_FAIL;
	}
}

