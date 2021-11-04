#include "framework.h"
#include "..\Headers\TestStage.h"
#include "Terrain.h"

CTestStage::CTestStage(LPDIRECT3DDEVICE9 pDevice)
	:CScene(pDevice)
{
}

HRESULT CTestStage::Ready_Scene()
{
	m_pLoading = CLoading::Create(m_pDevice, ESceneType::Stage02);
	if (nullptr == m_pLoading)
		return E_FAIL;

	SetWindowText(g_hWnd, L"TestStage");


	//중복을 검사해서 추가 Clone을 방지함.
	if (CManagement::Get_Instance()
		->Get_DuplicateChecker() == (_uint)ESceneType::Stage)
	{
		return S_OK;
	}

	if (FAILED(CScene::Ready_Scene()))
		return E_FAIL;

#pragma region 게임 오브젝트 Clone

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

	//if (FAILED(Add_SupportCamera_Layer(L"Layer_Camera")))
	//{
	//	PRINT_LOG(L"Error", L"Failed To Add_Camera_Layer");
	//	return E_FAIL;
	//}

	//if (FAILED(Add_Terrain_Layer(L"Layer_Terrain")))
	//{
	//	PRINT_LOG(L"Error", L"Failed To Add_Terrain_Layer");
	//	return E_FAIL;
	//}

	

	if (FAILED(Add_Monster_Layer()))
	{
		PRINT_LOG(L"Error", L"Failed To Add_Monster_Layer (Clinet - Stage01.cpp)");
		return E_FAIL;
	}

	if (FAILED(Add_Skybox_Layer(L"Layer_Skybox")))
	{
		PRINT_LOG(L"Error", L"Failed To Add_Skybox_Layer");
		return E_FAIL;
	}

	if (FAILED(Add_UI_Layer(L"Layer_UI")))
	{
		PRINT_LOG(L"Error", L"Failed To Layer_UI");
		return E_FAIL;
	}

	if (FAILED(Add_GressTree_Layer(L"Layer_GressTree")))
	{
		PRINT_LOG(L"Error", L"Failed To Layer_GressTree");
		return E_FAIL;
	}

	//if (FAILED(Add_UIFont_Layer(L"Layer_UIFont")))
	//{
	//	PRINT_LOG(L"Error", L"Failed To Layer_UIFont");
	//	return E_FAIL;
	//}

	//if (FAILED(Add_Effect_Layer(L"Layer_Effecter")))
	//{
	//	PRINT_LOG(L"Error", L"Failed To Add_Effect_Layer");
	//	return E_FAIL;
	//}
#pragma endregion


	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return 0;
	}

	pManagement->Load_Sound();


	return S_OK;
}

_uint CTestStage::Update_Scene(_float fDeltaTime)
{
	CScene::Update_Scene(fDeltaTime);

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return 0;
	}

	if (pManagement->Key_Down(KEY_ENTER) && true == m_pLoading->IsFinished())
	{
		if (FAILED(pManagement->Setup_CurrentScene((_uint)ESceneType::Stage02, CStage02::Create(m_pDevice))))
		{
			PRINT_LOG(L"CTestStage - Han", L" Stage02 생성 실패 Setup_CurrentScene To Stage02");
			return E_FAIL;
		}
		pManagement->Clear_ForScene((_int)ESceneType::TestStage);
	}

	//스테이지 스왑 기능.
	if (pManagement->Key_Down(KEY_ENTER))
	{
		if (FAILED(pManagement->SetUp_ScnenSwap((_uint)ESceneType::Battle, CBattleStageA::Create(m_pDevice))))
		{
			PRINT_LOG(L"CTestStage", L"CTestStage - BattleStage 넘어가는 코드 수정. ");
			return E_FAIL;
		}
	}

	if (pManagement->Key_Down(KEY_Z))
	{
		pManagement->Play_Sound(L"SE1_ITEM_GET_COIN1.wav", SOUND_CHANNELID::EFFECTA);
		pManagement->Play_BGM(L"1-06 Super Paper Mario.flac");
	}

	return NO_EVENT;
}

_uint CTestStage::LateUpdate_Scene(_float fDeltaTime)
{
	CScene::LateUpdate_Scene(fDeltaTime);

	return NO_EVENT;
}

CTestStage* CTestStage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTestStage* pInstance = new CTestStage(pDevice);

	if (FAILED(pInstance->Ready_Scene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Stage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTestStage* CTestStage::Swap(LPDIRECT3DDEVICE9 pDevice)
{
	CTestStage* pInstance = new CTestStage(pDevice);
	if (nullptr == pInstance)
	{
		PRINT_LOG(L"Error", L"Failed To Swap Stage");
	}

	SetWindowText(g_hWnd, L"StageA");

	return pInstance;
}

void CTestStage::Free()
{
	Safe_Release(m_pLoading);
	CScene::Free();
}
