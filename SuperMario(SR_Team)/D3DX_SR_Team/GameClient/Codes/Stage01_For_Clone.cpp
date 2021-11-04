#include "framework.h"
#include "Stage01.h"

HRESULT CStage01::Add_Player_Layer(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_Player",
		(_uint)ESceneType::Stage,
		LayerTag, &m_tNowInfo)))
	{
		return E_FAIL;
	}

	return S_OK;
}
HRESULT CStage01::Add_Camera_Layer(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//?
	CBattleCamera::CAMERA_DESC CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CBattleCamera::CAMERA_DESC));
	CameraDesc.vEye = _float3(0.f, 10.f, -10.f);
	CameraDesc.vUp = _float3(0.f, 1.f, 0.f);
	CameraDesc.fFovY = D3DXToRadian(90.f);
	CameraDesc.fAspect = (_float)WINCX / WINCY;
	CameraDesc.fZNear = 1.f; /* 투영변환 후(z나누기까지)에 0이 되어야하고 */
	CameraDesc.fZFar = 500.f; /* 투영변환 후(z나누기까지)에 1이 되어야함 */

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_BattleCamera",
		(_uint)ESceneType::Stage,
		LayerTag,
		&CameraDesc)))
	{
		return E_FAIL;
	}

	return S_OK;
}



HRESULT CStage01::Add_Monster_Layer(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_Monster",
		(_uint)ESceneType::Stage,
		LayerTag)))
	{
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CStage01::Add_MiddleBossA(const wstring & LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_MiddleBossA",
		(_uint)ESceneType::Stage,
		LayerTag)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CStage01::Add_MainCamera_Layer(const wstring & LayerTag)
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
	CameraDesc.fZNear = 1.f; /* 투영변환 후(z나누기까지)에 0이 되어야하고 */
	CameraDesc.fZFar = 500.f; /* 투영변환 후(z나누기까지)에 1이 되어야함 */

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_MainCamera",
		(_uint)ESceneType::Stage,
		LayerTag,
		&CameraDesc)))
	{
		return E_FAIL;
	}

	return S_OK;
}
HRESULT CStage01::Add_Skybox_Layer(const wstring & LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return E_FAIL;
	}

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_Skybox",
		(_uint)ESceneType::Stage,
		LayerTag)))
	{
		return E_FAIL;
	}


	return S_OK;
}
HRESULT CStage01::Add_UI_Layer(const wstring & LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return E_FAIL;
	}


	//if (FAILED(pManagement->Clone_GameObject_ToLayer(
	//	(_uint)ESceneType::Stage,
	//	L"GameObject_UIHP",
	//	(_uint)ESceneType::Stage,
	//	LayerTag)))
	//{
	//	return E_FAIL;
	//}

	return S_OK;
}
HRESULT CStage01::Add_UIFont_Layer(const wstring & LayerTag)
{
#pragma region 주석.

	//auto pManagement = CManagement::Get_Instance();
	//if (nullptr == pManagement)
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(pManagement->Clone_GameObject_ToLayer(
	//	(_uint)ESceneType::Stage,
	//	L"GameObject_UIFont",
	//	(_uint)ESceneType::Stage,
	//	LayerTag)))
	//{
	//	return E_FAIL;
	//}

#pragma endregion
	return S_OK;
}

HRESULT CStage01::Add_Terrain_Layer(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	HANDLE hFile = CreateFile(L"../../Resources/00.Terrain/00.Stage01/TerrainSave9.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		PRINT_LOG(L"파일 오픈 실패", L"파일 오픈 실패");
		return E_FAIL;
	}

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	while (true)
	{
		_float3 vPos, vRot, vScale;
		int iTextureNumber = 0;

		ReadFile(hFile, &vPos, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &vRot, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &vScale, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &iTextureNumber, sizeof(int), &dwByte, nullptr);

		if (0 == dwByte)
		{
			break;
		}

		CGameObject* pObj = pManagement->Clone_GameObject_For_Load(
			(_uint)ESceneType::Static
				, L"GameObject_Terrain"
				, (_uint)ESceneType::Stage
				, LayerTag);

		dynamic_cast<CTerrain*>(pObj)->Set_TextureNumber(iTextureNumber);
		pObj->Get_Transform()->Set_Position(vPos);
		pObj->Get_Transform()->Set_Rotation(vRot);
		pObj->Get_Transform()->Set_Scale(vScale);
	}
	CloseHandle(hFile);

	return S_OK;
}


HRESULT CStage01::Add_Effect_Layer_Han(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_EffectFire",
		(_uint)ESceneType::ParticleSpace,
		LayerTag)))
	{
		return E_FAIL;
	}

	//if (FAILED(pManagement->Clone_GameObject_ToLayer(
	//	(_uint)ESceneType::Static,
	//	L"GameObject_EffectPaper",
	//	(_uint)ESceneType::ParticleSpace,
	//	LayerTag)))
	//{
	//	return E_FAIL;
	//}

	return S_OK;
}

HRESULT CStage01::ADD_Camera_Layer_Number2(const wstring & LayerTage)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//CBattleCamera::CAMERA_DESC CameraDesc;
	//ZeroMemory(&CameraDesc, sizeof(CBattleCamera::CAMERA_DESC));
	//CameraDesc.vEye = _float3(0.f, 10.f, -10.f);
	//CameraDesc.vUp = _float3(0.f, 1.f, 0.f);
	//CameraDesc.fFovY = D3DXToRadian(90.f);
	//CameraDesc.fAspect = (_float)WINCX / WINCY;
	//CameraDesc.fZNear = 1.f; /* 투영변환 후(z나누기까지)에 0이 되어야하고 */
	//CameraDesc.fZFar = 500.f; /* 투영변환 후(z나누기까지)에 1이 되어야함 */

	CTestCamera::CAMERA_DESC CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CTestCamera::CAMERA_DESC));
	CameraDesc.vUp = _float3(0.f, 1.f, 0.f);
	CameraDesc.fFovY = D3DXToRadian(130.f);
	CameraDesc.fAspect = (_float)WINCX / WINCY;
	CameraDesc.fZNear = 1.f; /* 투영변환 후(z나누기까지)에 0이 되어야하고 */
	CameraDesc.fZFar = 500.f; /* 투영변환 후(z나누기까지)에 1이 되어야함 */

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_TestCamera",
		(_uint)ESceneType::Stage,
		LayerTage,
		&CameraDesc)))
	{
		return E_FAIL;
	}

	return S_OK;
}

