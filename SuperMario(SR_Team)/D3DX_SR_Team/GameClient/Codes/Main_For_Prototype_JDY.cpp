#include "framework.h"
#include "MainApp.h"


#include "MonsterE.h"
#include "GressTree.h"
//ÁÂ¿ì·Î ¿òÁ÷ÀÌ´Â ±À¹Ù.
HRESULT CMainApp::Monster_Texture_Prototype_JDY(CManagement * _pManagement)
{
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Monster_MoveGoomba",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Enemy/Goomba/Goomba%d.png", 6))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Monster_Goomba");
		return E_FAIL;
	}

	//ÁÂ¿ì·Î ¿òÁ÷ÀÌ´Â ±À¹Ù.
	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_Monster_MoveGoomba",
		CMonsterE::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_Monster_Goomba");
		return E_FAIL;
	}

	//Ç®
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Gress",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Images/00.Object/00.Gress/Gress%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Monster_Goomba");
		return E_FAIL;
	}
	//³ª¹«.
	if (FAILED(_pManagement->Add_Component_Prototype(
		(_uint)ESceneType::Static,
		L"Component_Texture_Tree",
		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Images/00.Object/00.Gress/Tree%d.png", 1))))
	{
		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Monster_Goomba");
		return E_FAIL;
	}

	if (FAILED(_pManagement->Add_GameObject_Prototype(
		(_uint)ESceneType::Static,
		L"GameObject_GressTree",
		CGressTree::Create(m_pDevice))))
	{
		PRINT_LOG(L"Error", L"Failed To Add GameObject_Monster_Goomba");
		return E_FAIL;
	}

}