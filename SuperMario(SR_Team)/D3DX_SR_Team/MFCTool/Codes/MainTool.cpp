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
		PRINT_LOG(L"MainTool - err", L"�׷��� ����̽� �ʱ� ���� ����.");
		return E_FAIL;
	}

	m_pDevice = m_pManagementTool->Get_Device();
	if (nullptr == m_pDevice)
	{
		PRINT_LOG(L"MainTool - err", L"�׷��� ����̽��� ���� �� ��.");
		return E_FAIL;
	}

	Safe_AddReference(m_pDevice);

#pragma region Prototype Function
	if (FAILED(Ready_StaticResources()))
	{
		PRINT_LOG(L"CMainTool", L"Prototype Ready ����");
		return E_FAIL;
	}
#pragma endregion

#pragma region Clone Function
	if (FAILED(Ready_Clone()))
	{
		PRINT_LOG(L"CMainTool", L"Clone Ready ����");
		return E_FAIL;
	}
#pragma endregion


	if (FAILED(Setup_DefaultSetting()))
	{
		PRINT_LOG(L"CMainTool", L"������ �ʱ� ���� ����.");
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
	/* ���� off */
	if (FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
		return E_FAIL;

	/*
	D3DRS_CULLMODE: �ĸ� �߷����� �ɼ�
	D3DCULL_CCW(Counter Clock Wise): �ݽð������ ���� �߷�����. ����Ʈ��.
	D3DCULL_CW(Clock Wise): �ð������ ���� �߷�����.
	D3DCULL_NONE: �ƹ��͵� �߷����� ����.
	*/

	if (FAILED(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainTool::Ready_StaticResources()
{
	//plate ������Ÿ�� set
#pragma region GameObj
	//Plate
	if (FAILED(m_pManagementTool->Add_GameObject_Prototype(
		L"GameObject_Plate",
		CPlate::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"�Ƕ��� ������Ÿ�� ���� ����");
		return E_FAIL;
	}

	//CubeColor
	if (FAILED(m_pManagementTool->Add_GameObject_Prototype(
		L"GameObject_CubeColor",
		CCubeColor::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"CCubeColor ������Ÿ�� ���� ����");
		return E_FAIL;
	}

	//CubeTexture
	if (FAILED(m_pManagementTool->Add_GameObject_Prototype(
		L"GameObject_CubeTexture",
		CCubeTexture::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"CCubeTexture ������Ÿ�� ���� ����");
		return E_FAIL;
	}

	//CubeLine
	if (FAILED(m_pManagementTool->Add_GameObject_Prototype(
		L"GameObject_CubeLine",
		CCubeLine::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"CCubeLine ������Ÿ�� ���� ����");
		return E_FAIL;
	}
	//sliderColor
	if (FAILED(m_pManagementTool->Add_GameObject_Prototype(
		L"GameObject_SliderColor",
		CSliderColor::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"CSliderColor ������Ÿ�� ���� ����");
		return E_FAIL;
	}
	//BoxColor

	if (FAILED(m_pManagementTool->Add_GameObject_Prototype(
		L"GameObject_BoxColor",
		CBoxColor::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"BoxColor ������Ÿ�� ���� ����");
		return E_FAIL;
	}



#pragma endregion

	//������Ʈ �߰�.
	/* For.Component_Transform */
	//Transform
#pragma region Transform

	if (FAILED(m_pManagementTool->Add_Component_Prototype(
		L"Component_Transform",
		CTransformTool::Create(m_pDevice))))
	{
		PRINT_LOG(L"CMainTool", L"TrasformTool / Component / Prototype ����.");
		return E_FAIL;
	}
#pragma endregion

#pragma region VIBufferTool
	//VIBuffer_Plate 
	if (FAILED(m_pManagementTool->Add_Component_Prototype(
		L"Component_VIBuffer_Plate",
		CVIBuffer_Plate::Create(m_pDevice))))
	{
		PRINT_LOG(L"CMainTool", L"VIBuffer_Plate / Component / Prototype ����");
		return E_FAIL;
	}

	//CVIBuffer_PlateTexture 
	if (FAILED(m_pManagementTool->Add_Component_Prototype(
		L"Component_VIBuffer_PlateTexture",
		CVIBuffer_PlateTexture::Create(m_pDevice))))
	{
		PRINT_LOG(L"CMainTool", L"VIBuffer_PlateTexture / Component / Prototype ����");
		return E_FAIL;
	}

	//CVIBuffer_CubeColorTool
	if (FAILED(m_pManagementTool->Add_Component_Prototype(
		L"Component_VIBuffer_CubeColor",
		CVIBuffer_CubeColorTool::Create(m_pDevice))))
	{
		PRINT_LOG(L"CMainTool", L"CVIBuffer_CubeColorTool / Component / Prototype ����");
		return E_FAIL;
	}

	//CVIBuffer_CubeLine
	if (FAILED(m_pManagementTool->Add_Component_Prototype(
		L"Component_VIBuffer_CubeLine",
		CVIBuffer_CubeLineTool::Create(m_pDevice))))
	{
		PRINT_LOG(L"CMainTool", L"CVIBuffer_CubeLineTool / Component / Prototype ����");
		return E_FAIL;
	}

	//CVIBuffer_CubeTextuer
	if (FAILED(m_pManagementTool->Add_Component_Prototype(
		L"Component_VIBuffer_CubeTexture",
		CVIBuffer_CubeTextureTool::Create(m_pDevice))))
	{
		PRINT_LOG(L"CMainTool", L"CVIBuffer_CubeTextureTool / Component / Prototype ����");
		return E_FAIL;
	}

	//CVIBuffer_SliderColorTool
	if (FAILED(m_pManagementTool->Add_Component_Prototype(
		L"Component_VIBuffer_SliderColor",
		CVIBuffer_CubeTextureTool::Create(m_pDevice))))
	{
		PRINT_LOG(L"CMainTool", L"Component_VIBuffer_SliderColor / Component / Prototype ����");
		return E_FAIL;
	}


#pragma endregion


	//TextureTool
#pragma region TextureTool

	if (FAILED(m_pManagementTool->Add_Component_Prototype(
		L"Component_Texture_Plate",
		CTextureTool::Create(m_pDevice, ETextureType::Normal, L"../Resources/Plate_Texture%d.png", 1))))
	{
		PRINT_LOG(L"CMainTool", L"Plate / Texture / Prototype ����");
		return E_FAIL;
	}

	m_iTerrainTextureCount = 15;
	if (FAILED(m_pManagementTool->Add_Component_Prototype(
		L"Component_Texture_Terrain",
		CTextureTool::Create(m_pDevice, ETextureType::Cube, L"../Resources/Terrain%d.dds", 15))))
	{
		PRINT_LOG(L"CMainTool", L"CCubeTextuer / Texture / Prototype ����");
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
		PRINT_LOG(L"MainTool - err", L"MainTool Create ����.");
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
		PRINT_LOG(L"CMainTool", L"�̹��� �������� ����");
		return E_FAIL;
	}
}

