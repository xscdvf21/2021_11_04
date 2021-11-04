#include "framework.h"
#include "TestStage.h"


HRESULT CTestStage::Add_Player_Layer(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_Player",
		(_uint)ESceneType::TestStage,
		LayerTag)))
	{
		return E_FAIL;
	}

	return S_OK;
}



HRESULT CTestStage::Add_Camera_Layer(const wstring& LayerTag)
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
	CameraDesc.fFovY = D3DXToRadian(90.f);
	CameraDesc.fAspect = (_float)WINCX / WINCY;
	CameraDesc.fZNear = 1.f; /* 투영변환 후(z나누기까지)에 0이 되어야하고 */
	CameraDesc.fZFar = 500.f; /* 투영변환 후(z나누기까지)에 1이 되어야함 */

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_TestCamera",
		(_uint)ESceneType::TestStage,
		LayerTag,
		&CameraDesc)))
	{
		return E_FAIL;
	}

	return S_OK;
}



HRESULT CTestStage::Add_Monster_Layer(const wstring& LayerTag)
{


	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;




	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_Monster_MoveGoomba",
		(_uint)ESceneType::TestStage,
		L"Layer_Monster_MoveGoomba"
		)))
	{
		return E_FAIL;
	}


	//CTransform::TRANSFORM_DESC x;
	//ZeroMemory(&x, sizeof(CTransform::TRANSFORM_DESC));
	//x.vPos = { 10.f, 1.f, 10.f };

	//if (FAILED(pManagement->Clone_GameObject_ToLayer(
	//	(_uint)ESceneType::Static,
	//	L"GameObject_Monster_Goomba",
	//	(_uint)ESceneType::TestStage,
	//	L"Layer_Monster_Goomba",
	//	&x
	//	)))
	//{
	//	return E_FAIL;
	//}

	//ZeroMemory(&x, sizeof(CTransform::TRANSFORM_DESC));
	//x.vPos = { 15.f, 1.f, 10.f };


	//if (FAILED(pManagement->Clone_GameObject_ToLayer(
	//	(_uint)ESceneType::Static,
	//	L"GameObject_Monster_Goomba",
	//	(_uint)ESceneType::TestStage,
	//	L"Layer_Monster_Goomba",
	//	&x
	//)))
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(pManagement->Clone_GameObject_ToLayer(
	//	(_uint)ESceneType::Static,
	//	L"GameObject_Monster_ShyGuy",
	//	(_uint)ESceneType::TestStage,
	//	L"Layer_Monster_ShyGuy",
	//	&x
	//)))
	//{
	//	return E_FAIL;
	//}

	//if (FAILED(pManagement->Clone_GameObject_ToLayer(
	//	(_uint)ESceneType::Static,
	//	L"GameObject_MiddleBossB",
	//	(_uint)ESceneType::TestStage,
	//	L"Layer_Monster_MiddlBossB",
	//	&x
	//)))
	//{
	//	return E_FAIL;
	//}

	return S_OK;

}
HRESULT CTestStage::Add_MainCamera_Layer(const wstring& LayerTag)
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
		(_uint)ESceneType::TestStage,
		LayerTag,
		&CameraDesc)))
	{
		return E_FAIL;
	}

	return S_OK;
}
HRESULT CTestStage::Add_Skybox_Layer(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return E_FAIL;
	}

	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_Skybox",
		(_uint)ESceneType::TestStage,
		LayerTag)))
	{
		return E_FAIL;
	}


	return S_OK;
}
HRESULT CTestStage::Add_UI_Layer(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return E_FAIL;
	}


	if (FAILED(pManagement->Clone_GameObject_ToLayer(
		(_uint)ESceneType::Static,
		L"GameObject_Stage_UI",
		(_uint)ESceneType::TestStage,
		LayerTag)))
	{
		return E_FAIL;
	}

	return S_OK;
}
HRESULT CTestStage::Add_UIFont_Layer(const wstring& LayerTag)
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
	//	(_uint)ESceneType::Stage,
	//	LayerTag)))
	//{
	//	return E_FAIL;
	//}

	return S_OK;
}

HRESULT CTestStage::Add_Terrain_Layer(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//HANDLE hFile = CreateFile(L"../Resources/Texture/Test/Stage04.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
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
			, (_uint)ESceneType::TestStage
			, LayerTag);

		dynamic_cast<CTerrain*>(pObj)->Set_TextureNumber(iTextureNumber);
		pObj->Get_Transform()->Set_Position(vPos);
		pObj->Get_Transform()->Set_Rotation(vRot);
		pObj->Get_Transform()->Set_Scale(vScale);
	}
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CTestStage::Add_Effect_Layer(const wstring & LayerTag)
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
		PRINT_LOG(L"Effect가 또...", L"이펙트 추가. TestStage_For_Clone.cpp");
		return E_FAIL;
	}

	//if (FAILED(pManagement->Clone_GameObject_ToLayer(
	//	(_uint)ESceneType::Static,
	//	L"GameObject_EffectPaper",
	//	(_uint)ESceneType::Stage,
	//	LayerTag)))
	//{
	//	return E_FAIL;
	//}

	return S_OK;
}

HRESULT CTestStage::Add_GressTree_Layer(const wstring & LayerTag)
{

	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;


	//CGameObject* pObj = pManagement->Clone_GameObject_For_Load(
	//	(_uint)ESceneType::Static,
	//	L"GameObject_GressTree",
	//	(_uint)ESceneType::TestStage,
	//	LayerTag);

	//	pObj->Set_TextureIndex(0);
		


	CGameObject* pObj = pManagement->Clone_GameObject_For_Load(
			(_uint)ESceneType::Static,
			L"GameObject_GressTree",
			(_uint)ESceneType::TestStage,
			LayerTag);

		pObj->Set_TextureIndex(1);

	return S_OK;
}


