#include "framework.h"
#include "MainApp.h"

#include "TestCamera.h"
#include "MainCamera.h"
#include "BattleCamera.h"
#include "SupportCam.h"
#include "OSCamera.h"

#include "Player.h"
#include "Monster.h"
#include "OpeningUI.h"
#include "BattleUI.h"
#include "StageUI.h"
#include "Skybox.h"
#include "StageHP.h"
#include "Coin.h"

#include "EffectParticle.h"

#include "SkyBoxForBattle.h"
#include "BattlePlateCircle.h"
#include "BattlePlateSlide.h"
#include "BattlePlateSelectCube.h"
#include "BattlePlateSelectLine.h"

#include "MonsterTest.h"
#include "DropGoomba.h"

#include "MonsterC.h"
#include "BattleMonster.h"

#include "ActionUIBackground.h"
#include "TimerMark.h"
#include "RingCountBackground.h"


#pragma region GameObject Prototype

HRESULT CMainApp::Player_Prototype(CManagement * _pManagement)
{
	//플레이어 그 자체.
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_Player",
		CPlayer::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_Player");
		return E_FAIL;
	}

	//플레이어 공격판정 총알 프로토타입.
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_PlayerBullet",
		CPlayerBullet::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_PlayerBullet");
		return E_FAIL;
	}
	return S_OK;

}

HRESULT CMainApp::Monster_Prototype(CManagement * _pManagement)
{

	//플레이어 공격판정 총알 프로토타입.
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_MonsterBullet",
		CMonsterCBullet::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_PlayerBullet");
		return E_FAIL;
	}

	//배틀 몬스터
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_Monster_Lakitu",
		CBattleMonster::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_Monster");
		return E_FAIL;
	}


	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_Monster",
		CMonster::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_Monster");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_Monster_Goomba",
		CMonsterA::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_Monster_Goomba");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_Monster_ShyGuy",
		CMonsterC::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_Monster_ShyGuy");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_MiddleBossA",
		CMiddleBossA::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_Monster_Goomba");
		return E_FAIL;
	}

	//테스트용 몬스터임 나중에 지울거임 봐주셈
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_TestMonster",
		CMonsterTest::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_TestMonster");
		return E_FAIL;
	}

	//테스트용 끝
	return S_OK;
}

HRESULT CMainApp::BattlePlate_Prototype(CManagement * _pManagement)
{
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_BattlePlate",
		CBattlePlate::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_BattlePlate");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_BattlePlate_Circle",
		CBattlePlateCircle::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_BattlePlate_Circle");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_BattlePlate_Slide",
		CBattlePlateSlide::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_BattlePlate_Slide");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_BattlePlate_Select_Slide",
		CBattlePlateSelectLine::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_BattlePlate_Select_Slide");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_BattlePlate_Select_Cube",
		CBattlePlateSelectCube::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_BattlePlate_Select_Cube");
		return E_FAIL;
	}	


	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_BattlePlate_DropAttack_Goomba",
		CDropGoomba::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_BattlePlate_DropAttack_Goomba");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Camera_Prototype(CManagement * _pManagement)
{
	//TestCamera
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_TestCamera",
		CTestCamera::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_TestCamera");
		return E_FAIL;
	}

	//BattleCamera
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_BattleCamera",
		CBattleCamera::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_BattleCamera");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_SupportCamera",
		CSupportCamera::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_SupportCamera");
		return E_FAIL;
	}

	//오프닝씬용 카메라 하나
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_OpenningScene_Camera",
		COSCamera::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_OpenningScene_Camera");
		return E_FAIL;
	}



	return S_OK;
}

HRESULT CMainApp::UI_Prototype(CManagement * _pManagement)
{
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_BattleUI",
		CBattleUI::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_BattleUI");
		return E_FAIL;
	}
	

	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_Stage_UI",
		CStageUI::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_Stage_UI");
		return E_FAIL;
	}


	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_Opening_UI",
		COpeningUI::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_Opening_UI");
		return E_FAIL;
	}



	return S_OK;
}

HRESULT CMainApp::BATTLE_UI_Prototype(CManagement* _pManagement)
{
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_BattleUI_ActionCounter",
		CActionUIBackground::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_BattleUI_ActionCounter");
		return E_FAIL;
	}
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_BattleUI_TimeMark",
		CTimerMark::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_BattleUI_TimeMark");
		return E_FAIL;
	}
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_BattleUI_RingCounter",
		CRingCountBackground::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_BattleUI_RingCounter");
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CMainApp::Terrain_Prototype(CManagement * _pManagement)
{
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_Terrain",
		CTerrain::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_GameObject_Terrain");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::SkyBox_Prototype(CManagement * _pManagement)
{
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_Skybox",
		CSkybox::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add_GameObject_Prototype_Skybox");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_Skybox_Battle",
		CSkyBoxForBattle::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_Skybox_Battle");
		return E_FAIL;
	}

	return S_OK;
}

#pragma endregion

#pragma region Effect Prototype

HRESULT CMainApp::Effecter_Prototype(CManagement * _pManagerment)
{
	if (FAILED(_pManagerment->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_Effecter",
		CEffecter::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_Effecter");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::Particle_Prototype(CManagement* _pManagerment)
{
	if (FAILED(_pManagerment->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_EffectParticle",
		CEffectParticle::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_EffectParticle");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::MiddleBossABullet_Prototype(CManagement * _pManagerment)
{
	if (FAILED(_pManagerment->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_MiddleBossABullet",
		CMiddleBossABullet::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_MiddleBossABullet");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::Item_Prototype(CManagement * _pManagement)
{
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_Item_Coin",
		CCoin::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"CCoin CMain.Cpp");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::TriggerBox_Prototype(CManagement * _pManagement)
{
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_TriggerBox",
		CMiddleBossABullet::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_TriggerBox");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::Teleporter_Prototype(CManagement * _pManagement)
{
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_Teleporter",
		CMiddleBossABullet::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_Teleporter");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::Gress_Prototype(CManagement * _pManagement)
{
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_Gress",
		CMiddleBossABullet::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_Gress");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::Tree_Prototype(CManagement * _pManagement)
{
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_Tree",
		CMiddleBossABullet::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_Tree");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::FakeTerrain_Prototype(CManagement * _pManagement)
{
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_FakeTerrain",
		CMiddleBossABullet::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_FakeTerrain");
		return E_FAIL;
	}
	return S_OK;
}

#pragma endregion


#pragma region Component Prototype

HRESULT CMainApp::Transform_Prototype(CManagement * _pManagement)
{
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Transform",
		CTransform::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Transform");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::VIBuffer_Prototype(CManagement * _pManagement)
{
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_Rect",
		CVIBuffer_Rect::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_VIBuffer_Rect");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_RectTexture",
		CVIBuffer_RectTexture::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_VIBuffer_RectTexture");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_CubeColor",
		CVIBuffer_CubeColor::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_VIBuffer_CubeColor");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_Terrain",
		CVIBuffer_Terrain::Create(m_pDevice, TERRAIN_VTX_COUNTX, TERRAIN_VTX_COUNTZ, TERRAIN_VTX_INTERVAL))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_VIBuffer_Terrain");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_CubeTexture",
		CVIBuffer_CubeTexture::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_VIBuffer_CubeTexture");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_CubeColor",
		CVIBuffer_CubeColor::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_VIBuffer_CubeColor");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_SlideCubeColor",
		CVIBuffer_SlideCubeColor::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_VIBuffer_SlideCubeColor");
		return E_FAIL;
	}
	
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_VIBuffer_Alpha_Round",
		CVIBuffer_Alpha_Round::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_VIBuffer_Alpha_Round");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Status_Comp_Prototype(CManagement* _pManagement) 
{
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_HealthState",
		CHealthCondition::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_HealthState");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::Font_Prototype(CManagement * _pManagement)
{
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Font",
		CFont_Component::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Font");
		return E_FAIL;
	}
	return S_OK;
}

#pragma endregion




#pragma region Texture Prototype
HRESULT CMainApp::Player_Texture_Prototype(CManagement * _pManagement)
{

	#pragma region 플레이어 공격 
		if (FAILED(_pManagement->Add_Component_Prototype(
			(_uint)ESceneType::Static,
			L"Component_Texture_Player_HammerAttack_Front",
			CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/HammerAttack/Front/Mario_HammerAttack_Front_%d.png", 3))))
		{
			PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Player");
			return E_FAIL;
		}

		//4
		if (FAILED(_pManagement->Add_Component_Prototype(
			(_uint)ESceneType::Static,
			L"Component_Texture_Player_HammerAttack_Back",
			CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/HammerAttack/Back/Mario_HammerAttack_Back_%d.png", 3))))
		{
			PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Player");
			return E_FAIL;
		}

		if (FAILED(_pManagement->Add_Component_Prototype(
			(_uint)ESceneType::Static,
			L"Component_Texture_Player_HammerAttack_Down",
			CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/HammerAttack/Down/Mario_HammerAttack_Down_%d.png", 3))))
		{
			PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Player");
			return E_FAIL;
		}
		if (FAILED(_pManagement->Add_Component_Prototype(
			(_uint)ESceneType::Static,
			L"Component_Texture_Player_HammerAttack_Up",
			CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/HammerAttack/Up/Mario_HammerAttack_Up_%d.png", 3))))
		{
			PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Player");
			return E_FAIL;
		}
	#pragma endregion

	#pragma region 플레이어 아이들
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_Idle_Front",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/Idle/Front/Mario_Idle_Front%d.png", 3))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Player");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_Idle_Back",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/Idle/Back/Mario_Idle_Back%d.png", 3))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Player");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_Idle_Up",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/Idle/Up/Mario_Idle_Up%d.png", 3))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Player");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_Idle_Down",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/Idle/Down/Mario_Idle_Down%d.png", 3))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Player");
		return E_FAIL;
	}
 #pragma endregion

	#pragma region 플레이어 이동
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_Move_Front",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/Move/Front/Mario_Move_Front%d.png", 6))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Player");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_Move_Back",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/Move/Back/Mario_Move_Back%d.png", 6))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Player");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_Move_Up",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/Move/Up/Mario_Move_Up%d.png", 6))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Player");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Player_Move_Down",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/Move/Down/Mario_Move_Down%d.png", 6))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Player");
		return E_FAIL;
	}
	#pragma endregion

	#pragma region 플레이어 점프
		if (FAILED(_pManagement->Add_Component_Prototype(
			(_uint)ESceneType::Static,
			L"Component_Texture_Player_Jump_Front",
			CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/Jump/Front/Mario_Jump_Fornt%d.png", 4))))
		{
			PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Player");
			return E_FAIL;
		}

		if (FAILED(_pManagement->Add_Component_Prototype(
			(_uint)ESceneType::Static,
			L"Component_Texture_Player_Jump_Back",
			CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/Jump/Back/Mario_Jump_Back%d.png", 4))))
		{
			PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Player");
			return E_FAIL;
		}

	#pragma endregion

	#pragma region 플레이어 페이퍼
			if (FAILED(_pManagement->Add_Component_Prototype(
				(_uint)ESceneType::Static,
				L"Component_Texture_Player_Paper_Front",
				CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/Paper/Front/Mario_Paper_Front%d.png", 3))))
			{
				PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Player");
				return E_FAIL;
			}

			if (FAILED(_pManagement->Add_Component_Prototype(
				(_uint)ESceneType::Static,
				L"Component_Texture_Player_Paper_Back",
				CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/Paper/Back/Mario_Paper_Back%d.png", 3))))
			{
				PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Player");
				return E_FAIL;
			}

			if (FAILED(_pManagement->Add_Component_Prototype(
				(_uint)ESceneType::Static,
				L"Component_Texture_Player_Paper_Up",
				CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/Paper/Up/Mario_Paper_Up%d.png", 3))))
			{
				PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Player");
				return E_FAIL;
			}

			if (FAILED(_pManagement->Add_Component_Prototype(
				(_uint)ESceneType::Static,
				L"Component_Texture_Player_Paper_Down",
				CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/Paper/Down/Mario_Paper_Down%d.png", 3))))
			{
				PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Player");
				return E_FAIL;
			}

	#pragma endregion
	return S_OK;

}

HRESULT CMainApp::Player_2DTexture_Prototype(CManagement * _pManagement)
{
	//idel
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_2DPlayer_Idle",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/2D/Mario2D_Idel_0.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_2DPlayer_Idle");
		return E_FAIL;
	}
	//move
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_2DPlayer_Move",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/2D/Mario2D_Move_%d.png", 3))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_2DPlayer_Move");
		return E_FAIL;
	}
	//turn
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_2DPlayer_Turn",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/2D/Mario2D_Turn_%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_2DPlayer_Turn");
		return E_FAIL;
	}
	//Sit
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_2DPlayer_Sit",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/2D/Mario2D_Sit_%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_2DPlayer_Turn");
		return E_FAIL;
	}
	//jump
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_2DPlayer_Jump",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/2D/Mario2D_Jump_%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_2DPlayer_Turn");
		return E_FAIL;
	}
	//SayGoodBye
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_2DPlayer_GoodBye",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Player/2D/Mario_SayGoodBye_%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_2DPlayer_Turn");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Monster_Texture_Prototype(CManagement * _pManagement)
{

#pragma region Lakitu(배틀몬스터)
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Monster_Lakitu",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Enemy/Lakitu/Lakitu%d.png", 2))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Monster_Lakitu");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Monster_Lakitu_Idle",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Enemy/Lakitu/Idle/Lakitu%d.png", 2))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Monster_Lakitu");
		return E_FAIL;
	}


#pragma endregion

#pragma region Goomba(몬스터A)
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Monster_Goomba",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Enemy/Goomba/Goomba%d.png", 6))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Monster_Goomba");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Monster_Goomba_Idle",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Enemy/Goomba/Idle/Goomba%d.png", 2))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Monster_Goomba");
		return E_FAIL;
	}

#pragma endregion


#pragma region Koopa 랑 Spike( 미들보스에서 사용)
	//미들 보스 A 에서 사용중.
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_MiddleA_Move",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Enemy/Koopa/Koopa%d.png", 8))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Monster_Koopa");
		return E_FAIL;
	}
	//미들 보스 A 에서 사용중.
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_MiddleA_Attack",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Enemy/Spike/Spike%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Monster_Koopa");
		return E_FAIL;
	}

#pragma endregion


#pragma region  ShyGuy(몬스터C)

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Monster_ShyGuy",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Enemy/ShyGuy/ShyGuy_%d.png", 6))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Monster_Koopa");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Monster_ShyGuy_Idle",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Enemy/ShyGuy/Idle/ShyGuy_%d.png", 2))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Monster_Koopa");
		return E_FAIL;
	}
#pragma endregion

	//몬스터는 Stage에 담아야함. 씬 체인지할때 그 전 씬은 밀어버리기떄문에. 몬스터도 같이밀어주어야함.
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Monster",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Texture/Test/Monster%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Monster");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_DropAttack_Goomba",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/ETC/Attack/Goomba%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_DropAttack_Goomba");
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CMainApp::BattlePlate_Texture_Prototype(CManagement * _pManagement)
{

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_BattlePlate",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Stage/BattleStage%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_BattlePlate");
		return E_FAIL;
	}
	//배틀플레이트 이펙트 관련
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_BattlePlate_Circle",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/ETC/Circle%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_BattlePlate_Circle");
		return E_FAIL;
	}
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_BattlePlate_Slide",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/ETC/Slide%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_BattlePlate_Slide");
		return E_FAIL;
	}
	//해머 판정
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_BattlePlate_SelectCube",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/ETC/SelectCube%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_BattlePlate_SelectCube");
		return E_FAIL;
	}
	//슬라이드 판정
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_BattlePlate_Select_Slide",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/ETC/SelectSlide%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_BattlePlate_Select_Slide");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::UI_Texture_Prototype(CManagement * _pManagement)
{
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_BattleUI_ActionBG",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Images/BattleUI/ActionBackground%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_BattleUI_ActionBG");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_BattleUI_RingBG",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Images/BattleUI/RingControlBackground%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_BattleUI_RingBG");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_BattleUI_Coin",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Images/UI/CoinUI%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_BattleUI_Coin");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_BattleUI_TimerBG",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Images/BattleUI/TimerBackGround%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_BattleUI_TimerBG");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_UIFont",
		CTexture::Create(m_pDevice, ETextureType::Font, L"../Resources/Texture/Test/Monster%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Monster");
		return E_FAIL;
	}


#pragma region 스테이지 UI
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Stage_UI_Coin",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Images/UI/CoinUI%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_BattleUI_Coin");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Stage_UI_Hp",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Images/UI/HpUI%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_BattleUI_Coin");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Stage_UI_Paper",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Images/UI/PaperUI%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_BattleUI_Coin");
		return E_FAIL;
	}
#pragma endregion
	return S_OK;
}

HRESULT CMainApp::Terrain_Texture_Prototype(CManagement * _pManagement)
{
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Terrain",
		//CTexture::Create(m_pDevice, ETextureType::Cube, L"../Resources/Texture/Test/Terrain%d.dds", 15))))
		CTexture::Create(m_pDevice, ETextureType::Cube, L"../Resources/Texture/Test/Terrain%d.dds", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Terrain");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::SkyBox_Texture_Prototype(CManagement * _pManagement)
{
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Skybox",
		CTexture::Create(m_pDevice, ETextureType::Cube, L"../../Resources/Texture/ETC/SkyboxA%d.dds", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Skybox");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Skybox_Battle",
		CTexture::Create(m_pDevice, ETextureType::Cube, L"../../Resources/Texture/ETC/SkyboxBattle%d.dds", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Skybox");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Item_Texture_Prototype(CManagement* _pManagement)
{
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Item_Coin",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Item/CoinItem%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Item_Coin");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMainApp::BackGround_Texture_Prototype(CManagement* _pManagement)
{
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_OpeningScene",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Images/Openening/Opening.jpg", 1))))
	{
		PRINT_LOG(L"CMainApp", L"오프닝 씬 이미지 로드 실패.");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::CLEAR_PROTOTYPE(CManagement* _pManagement)
{
	return S_OK;
}

HRESULT CMainApp::Gress_Texture_Prototype_Han(CManagement * _pManagement)
{
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Gress",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Images/00.Object/00.Gress/Gress%d.png", 2))))
	{
		PRINT_LOG(L"CMainApp", L"풀때기 텍스쳐 프로토타입 실패.");
		return E_FAIL;
	}
	return S_OK;
}

#pragma endregion
