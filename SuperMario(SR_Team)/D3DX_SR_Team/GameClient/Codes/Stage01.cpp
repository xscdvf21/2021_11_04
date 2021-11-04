#include "framework.h"
#include "..\Headers\Stage01.h"
#include "Terrain.h"

CStage01::CStage01(LPDIRECT3DDEVICE9 pDevice)
	:CScene(pDevice)
{
}

HRESULT CStage01::Ready_Scene()
{
	m_pLoading = CLoading::Create(m_pDevice, ESceneType::Stage02);
	if (nullptr == m_pLoading)
		return E_FAIL;

	//Safe_AddReference(m_pLoading);

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

	/*if (FAILED(Add_Camera_Layer(L"Layer_Camera")))
	{
		PRINT_LOG(L"Error", L"Failed To Add_Camera_Layer");
		return E_FAIL;
	}*/

	if (FAILED(ADD_Camera_Layer_Number2(L"Layer_Camera")))
	{
		PRINT_LOG(L"Error", L"Failed To Add_Camera_Layer");
		return E_FAIL;
	}

	if (FAILED(Add_Terrain_Layer(L"Layer_Terrain")))
	{
		PRINT_LOG(L"Error", L"Failed To Add_Terrain_Layer");
		return E_FAIL;
	}

	if (FAILED(Add_MiddleBossA(L"Layer_MiddleBossA")))
	{
		PRINT_LOG(L"Error", L"Failed to add_MiddleBossA_Layer");
		return E_FAIL;
	}

	if (FAILED(Add_Skybox_Layer(L"Layer_Skybox")))
	{
		PRINT_LOG(L"Error", L"Failed To Add_Skybox_Layer");
		return E_FAIL;
	}

	/*if (FAILED(Add_Effect_Layer_Han(L"Layer_Effecter")))
	{
		PRINT_LOG(L"Error", L"Failed To Add_Effect_Layer");
		return E_FAIL;
	}*/

#pragma region 주석
	/*if (FAILED(Add_Monster_Layer(L"Layer_Monster")))
	{
		PRINT_LOG(L"Error", L"Failed To Add_Monster_Layer (Clinet - Stage01.cpp)");
		return E_FAIL;
	}

	

	if (FAILED(Add_UI_Layer(L"Layer_UI")))
	{
		PRINT_LOG(L"Error", L"Failed To Layer_UI");
		return E_FAIL;
	}

	if (FAILED(Add_UIFont_Layer(L"Layer_UIFont")))
	{
		PRINT_LOG(L"Error", L"Failed To Layer_UIFont");
		return E_FAIL;
	}*/
#pragma endregion

#pragma endregion

	return S_OK;
}

_uint CStage01::Update_Scene(_float fDeltaTime)
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
			PRINT_LOG(L"CStage01 - Han", L" Stage02 생성 실패 Setup_CurrentScene To Stage02");
			return E_FAIL;
		}
		pManagement->Clear_ForScene((_int)ESceneType::Stage);
	}

	//스테이지 스왑 기능.
	if (pManagement->Key_Down(KEY_ENTER))
	{
		if (FAILED(pManagement->SetUp_ScnenSwap((_uint)ESceneType::Battle, CBattleStageA::Create(m_pDevice))))
		{
			PRINT_LOG(L"CStage01",L"CStage01 - BattleStage 넘어가는 코드 수정. ");
			return E_FAIL;
		}
	}

	return NO_EVENT;
}

_uint CStage01::LateUpdate_Scene(_float fDeltaTime)
{
	CScene::LateUpdate_Scene(fDeltaTime);

	return NO_EVENT;
}

CStage01 * CStage01::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage01* pInstance = new CStage01(pDevice);
	pInstance->m_tNowInfo.iSceneType = (_uint)ESceneType::Stage;
	pInstance->m_tNowInfo.iPrototypeType = (_uint)ESceneType::Stage;

	if (FAILED(pInstance->Ready_Scene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create Stage");
		Safe_Release(pInstance);
	}

	SetWindowText(g_hWnd, L"StageA");

	return pInstance;
}

CStage01 * CStage01::Swap(LPDIRECT3DDEVICE9 pDevice)
{
	CStage01* pInstance = new CStage01(pDevice);
	pInstance->m_tNowInfo.iSceneType = (_uint)ESceneType::Stage;
	pInstance->m_tNowInfo.iPrototypeType = (_uint)ESceneType::Stage;

	if (nullptr == pInstance)
	{
		PRINT_LOG(L"Error", L"Failed To Swap Stage");
	}

	SetWindowText(g_hWnd, L"StageA");

	return pInstance;
}

void CStage01::Free()
{
	Safe_Release(m_pLoading);
	CScene::Free();
}
