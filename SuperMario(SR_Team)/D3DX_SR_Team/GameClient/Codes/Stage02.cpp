#include "..\Headers\Stage02.h"
#include "framework.h"



CStage02::CStage02(LPDIRECT3DDEVICE9 pDevice)
	:CScene(pDevice)
{
}

HRESULT CStage02::Ready_Scene()
{


#pragma region 주석
	/*m_pLoading = CLoading::Create(m_pDevice, ESceneType::Stage02);
	if (nullptr == m_pLoading)
		return E_FAIL;*/
#pragma endregion

	//중복을 검사
	if (CManagement::Get_Instance()
		->Get_DuplicateChecker() == (_uint)ESceneType::Stage)
	{
		return S_OK;
	}

	if (FAILED(CScene::Ready_Scene()))
		return E_FAIL;

#pragma region Clone 모음

	if (FAILED(Add_Player_Layer(L"Layer_Player")))
	{
		PRINT_LOG(L"Error", L"Failed To Add_Player_Layer");
		return E_FAIL;
	}

	if (FAILED(Add_Camera_Layer(L"Layer_Camera")))
	{
		PRINT_LOG(L"Error", L"Failed To Add_Camera_Layer");
		return E_FAIL;
	}

	if (FAILED(Add_Terrain_Layer(L"Layer_Terrain")))
	{
		PRINT_LOG(L"Error", L"Failed To Add_Terrain_Layer");
		return E_FAIL;
	}

#pragma endregion

	return S_OK;
}

_uint CStage02::Update_Scene(_float fDeltaTime)
{
	CScene::Update_Scene(fDeltaTime);

	return NO_EVENT;
}

_uint CStage02::LateUpdate_Scene(_float fDeltaTime)
{
	CScene::LateUpdate_Scene(fDeltaTime);

	return NO_EVENT;
}

CStage02 * CStage02::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage02* pInstance = new CStage02(pDevice);
	if (FAILED(pInstance->Ready_Scene()))
	{
		PRINT_LOG(L"CStage02", L"CStage02 Create 실패.");
		Safe_Release(pInstance);
	}
	
	SetWindowText(g_hWnd, L"Stage02");

	return pInstance;
}


HRESULT CStage02::Add_Terrain_Layer(const wstring& LayerTag)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	HANDLE hFile = CreateFile(L"../Resources/Texture/Test/Stage02.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

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

		CGameObject* pObj = pManagement->
			Clone_GameObject_For_Load((_uint)ESceneType::Static
				, L"GameObject_Terrain"
				, (_uint)ESceneType::Stage02
				, LayerTag);

		dynamic_cast<CTerrain*>(pObj)->Set_TextureNumber(iTextureNumber);
		pObj->Get_Transform()->Set_Position(vPos);
		pObj->Get_Transform()->Set_Rotation(vRot);
		pObj->Get_Transform()->Set_Scale(vScale);
	}
	CloseHandle(hFile);

	return S_OK;
}


void CStage02::Free()
{
	CScene::Free();
}
