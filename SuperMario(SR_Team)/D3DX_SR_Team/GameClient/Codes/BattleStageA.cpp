#include "framework.h"
#include "..\Headers\BattleStageA.h"
#include <BattleUI.h>
#include <Player.h>


CBattleStageA::CBattleStageA(_Device pDevice)
	:CScene(pDevice)
{
}

HRESULT CBattleStageA::Ready_Scene()
{

	if (CManagement::Get_Instance()
		->Get_DuplicateChecker() == (_uint)ESceneType::Battle)
	{
		return S_OK;
	}

	if (FAILED(CScene::Ready_Scene()))
		return E_FAIL;


	//if (FAILED(Add_Player_Layer(L"Layer_Player")))
	//{
	//	PRINT_LOG(L"Error", L"Failed To Add_Player_Layer");
	//	return E_FAIL;
	//}


	if (FAILED(Add_Camera_Layer(L"Layer_Camera")))
	{
		PRINT_LOG(L"Error", L"Failed To Add_Camera_Layer");
		return E_FAIL;
	}

	if (FAILED(Add_SkyBox_Layer(L"Layer_SkyBox")))
	{
		PRINT_LOG(L"Error", L"Failed To Add_Layer_SkyBox");
		return E_FAIL;
	}

	if (FAILED(Add_BattlePlate_Layer(L"Layer_BattlePlate")))
	{
		PRINT_LOG(L"Error", L"Failed To Add_Layer_BattlePlate (Client - BattleStageA.cpp)");
		return E_FAIL;
	}

	//////배틀플레이트보다 나중에 생성해야됨
	if (FAILED(Add_Monster_Layer(L"Layer_Monster")))
	{
		PRINT_LOG(L"Error", L"Failed To Add_Layer_Monster (Client - BattleStageA.cpp)");
		return E_FAIL;
	}

	if (FAILED(Add_Monster_Object_Layer(L"Layer_Monster_Object")))
	{
		PRINT_LOG(L"Error", L"Failed To Add_Layer_Monster (Client - BattleStageA.cpp)");
		return E_FAIL;
	}

	if (FAILED(Add_Battle_UI(L"Layer_BattleUI")))
	{
		PRINT_LOG(L"Error", L"Failed To Add_BattleUI_Layer");
		return E_FAIL;
	}

    return S_OK;
}

_uint CBattleStageA::Update_Scene(_float fDeltaTime)
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return 0;
	}

	if (pManagement->Key_Down(KEY_SPACE))
	{
		if (FAILED(pManagement->SetUp_ScnenSwap((_uint)ESceneType::Stage, CStage01::Create(m_pDevice))))
		{
			PRINT_LOG(L"CStage01", L"CStage01 - BattleStage 넘어가는 코드 수정. ");
			return E_FAIL;
		}
	}

	pBattlePlate = static_cast<CBattlePlate*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, L"Layer_BattlePlate", 0));
	if (pBattlePlate == nullptr)
	{
			return E_FAIL;
	}

	pBattleUI = static_cast<CBattleUI*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, L"Layer_BattleUI", 0));
	if (pBattleUI == nullptr)
	{
		return E_FAIL;
	}

	switch (pBattlePlate->Get_BattleState())
	{
	case EBattleState::ROTATEMODE:
		pBattleUI->Set_Visual_Timer();
		pBattleUI->Set_Visual_Ring();
		break;
	case EBattleState::TARGETSELECTMODE:
		pBattleUI->Set_Visual_Attack();
		break;
	case EBattleState::ATTACK:
		break;
	case EBattleState::ENEMYATTACK:
		break;
	case EBattleState::REROLL:
		break;
	case EBattleState::GAMESET:
		break;
	case EBattleState::RELEASE:
		break;
	default:
		break;
	}




	if (pBattlePlate->Get_BattleState() == EBattleState::TARGETSELECTMODE)
	{
		auto* pBattleCam = pManagement->Get_GameObject((_uint)ESceneType::Battle, L"Layer_Camera", 0);

		if (pBattlePlate->Get_Position() == EPosisition::LEFT)
		//if (pManagement->Key_Down(KEY_LEFT))
		{
			static_cast<CBattleCamera*>(pBattleCam)->Cam_Rotate_CW();
			pBattlePlate->Set_CamCol(-1);
		}

		if (pBattlePlate->Get_Position() == EPosisition::RIGHT)
		//if (pManagement->Key_Down(KEY_RIGHT))
		{
			static_cast<CBattleCamera*>(pBattleCam)->Cam_Rotate_CCW();
			pBattlePlate->Set_CamCol(1);
		}
	}


    return _uint();
}

_uint CBattleStageA::LateUpdate_Scene(_float fDeltaTime)
{
	auto pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//if (pManagement->Key_Pressing(KEY_C)) {
	//	static_cast<CPlayer*>(pManagement->Get_GameObject((_uint)ESceneType::Battle, L"Layer_Player", 0))->Set_CoinState();
	//}

	if (pBattlePlate == nullptr)
	{
		return 0;
	}

	pBattleUI->Set_Timer(pBattlePlate->Get_RemainTimer());
	pBattleUI->Set_Ring(pBattlePlate->Get_RemainCircle());
	pBattleUI->Set_AttackCount(pBattlePlate->Get_RemainAttackCount());

    return _uint();
}



CBattleStageA* CBattleStageA::Create(_Device pDevice)
{
	CBattleStageA* pInstance = new CBattleStageA(pDevice);
	if (FAILED(pInstance->Ready_Scene()))
	{
		PRINT_LOG(L"Error", L"Failed To Create BattleStageA");
		Safe_Release(pInstance);
	}

	SetWindowText(g_hWnd, L"Battle!");

	return pInstance;
}

void CBattleStageA::Free()
{
	CScene::Free();
}
