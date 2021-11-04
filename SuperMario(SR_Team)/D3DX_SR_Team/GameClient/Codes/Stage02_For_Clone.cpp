#include "framework.h"
#include "Stage02.h"

HRESULT CStage02::Add_Player_Layer(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_Player",
		(_uint)ESceneType::Stage02,
		LayerTag)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CStage02::Add_Camera_Layer(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	CTestCamera::CAMERA_DESC CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CTestCamera::CAMERA_DESC));
	CameraDesc.vUp = _float3(0.f, 1.f, 0.f);
	CameraDesc.fFovY = D3DXToRadian(90.f);
	CameraDesc.fAspect = (_float)WINCX / WINCY;
	CameraDesc.fZNear = 1.f;
	CameraDesc.fZFar = 500.f;

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Stage,
		L"GameObject_TestCamera",
		(_uint)ESceneType::Stage02,
		LayerTag,
		&CameraDesc)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CStage02::Add_MainCamera_Layer(const wstring & LayerTag)
{
	auto pManagement = CManagement::Get_Instance();

	if (nullptr == pManagement)
	{
		return E_FAIL;
	}

	CMainCamera::CAMERA_DESC CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CMainCamera::CAMERA_DESC));
	CameraDesc.vUp = _float3(0.f, 1.0f, 0.f);
	CameraDesc.fFovY = D3DXToRadian(90.f);
	CameraDesc.fAspect = (_float)WINCX / WINCY;
	CameraDesc.fZNear = 1.f;
	CameraDesc.fZFar = 500.f;

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Stage,
		L"GameObject_MainCamera",
		(_uint)ESceneType::Stage02,
		LayerTag,
		&CameraDesc)))
	{
		return E_FAIL;
	}     

	return S_OK;
}

