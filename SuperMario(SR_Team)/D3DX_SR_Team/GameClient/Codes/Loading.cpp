#include "framework.h"
#include "..\Headers\Loading.h"

//#include "Terrain.h"
//#include "MainCamera.h"
//#include "Monster.h"
//#include "BattlePlate.h"
//#include "Skybox.h"
//#include "Player.h"
//#include "UIFont.h"
//#include "MonsterA.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pDevice, ESceneType eSceneID)
	: m_pDevice(pDevice),
	m_eLoadingScene(eSceneID)
{
	Safe_AddReference(m_pDevice);
}

_bool CLoading::IsFinished()
{
	return m_IsFinished;
}

HRESULT CLoading::Ready_Loading()
{
	m_hLoadingThread = (HANDLE)_beginthreadex(0, 0, LoadingByThread, this, 0, nullptr);
	if (nullptr == m_hLoadingThread)
		return E_FAIL;

	InitializeCriticalSection(&m_CriticalSection);

	return S_OK;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pDevice, ESceneType eSceneID)
{
	CLoading* pInstance = new CLoading(pDevice, eSceneID);
	if (FAILED(pInstance->Ready_Loading()))
	{
		PRINT_LOG(L"Error", L"Failed To Create CLoading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading::Free()
{
	CloseHandle(m_hLoadingThread);
	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pDevice);
}

// 중요 : 여기 있는 주석은 나중에 정리가 되고 마무리 작업시 살릴거임.
unsigned CLoading::LoadingByThread(void * pParam)
{
	CLoading* pLoading = (CLoading*)pParam;
	if (nullptr == pLoading)
		return 0;

	EnterCriticalSection(&pLoading->m_CriticalSection);

	HRESULT hr = 0;

	switch (pLoading->m_eLoadingScene)
	{
	case ESceneType::Stage: 
		//hr = pLoading->Load_StageResources();
		break;
		case ESceneType::Stage02:
		//hr = pLoading->Load_StageTwoResources();
		break;
		/*case ESceneType::Stage2:
		hr = pLoading->Load_StageTwoResources();
		break;*/
	}

	if (FAILED(hr))
	{
		LeaveCriticalSection(&pLoading->m_CriticalSection);
		return 0;
	}

	pLoading->m_IsFinished = true;

	LeaveCriticalSection(&pLoading->m_CriticalSection);

	return 0;
}

#pragma region Stage Prototype
HRESULT CLoading::Load_StageResources()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(GameObject_Prototype(pManagement, ESceneType::Stage)))
	{
		PRINT_LOG(L"CLoading - Han", L"게임 오브젝트 프로토타입 실패");
		return E_FAIL;
	}
	
	if (FAILED(Component_Prototype(pManagement, ESceneType::Stage)))
	{
		PRINT_LOG(L"CLoading - Han", L"컴포넌트 프로토타입 실패");
		return E_FAIL;
	}

	if (FAILED(Texture_Prototype(pManagement, ESceneType::Stage)))
	{
		PRINT_LOG(L"CLoading - Han", L"텍스쳐 프로토타입 실패");
		return E_FAIL;
	}

//한승현 - loading 주석들 region으로 가림.
#pragma region 주석

//#pragma region Component
//	/* For.Component_VIBuffer_TerrainColor */
//	//if (FAILED(pManagement->Add_Component_Prototype(
//	//	(_uint)ESceneType::Stage,
//	//	L"Component_VIBuffer_TerrainColor",
//	//	CVIBuffer_TerrainColor::Create(m_pDevice, TERRAIN_VTX_COUNTX, TERRAIN_VTX_COUNTZ, TERRAIN_VTX_INTERVAL))))
//	//{
//	//	PRINT_LOG(L"Error", L"Failed To Add Component_VIBuffer_TerrainColor");
//	//	return E_FAIL;
//	//}
//
//	///* For.Component_VIBuffer_TerrainTexture */
//	//if (FAILED(pManagement->Add_Component_Prototype(
//	//	(_uint)ESceneType::Stage,
//	//	L"Component_VIBuffer_TerrainTexture",
//	//	CVIBuffer_TerrainTexture::Create(m_pDevice, TERRAIN_VTX_COUNTX, TERRAIN_VTX_COUNTZ, TERRAIN_VTX_INTERVAL))))
//	//{
//	//	PRINT_LOG(L"Error", L"Failed To Add Component_VIBuffer_TerrainTexture");
//	//	return E_FAIL;
//	//}
//
//	/* For.Component_Texture_Terrain */
//	if (FAILED(pManagement->Add_Component_Prototype(
//		(_uint)ESceneType::Stage,
//		L"Component_Texture_Terrain",
//		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Terrain/Terrain%d.png", 1))))
//	{
//		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Terrain");
//		return E_FAIL;
//	}
//
//	/* For.Component_Texture_Monster */
//	if (FAILED(pManagement->Add_Component_Prototype(
//		(_uint)ESceneType::Stage,
//		L"Component_Texture_Monster",
//		CTexture::Create(m_pDevice, ETextureType::Cube, L"../Resources/Monster%d.dds", 1))))
//	{
//		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Monster");
//		return E_FAIL;
//	}
//
//	/* For.Component_Texture_Grass */
//	if (FAILED(pManagement->Add_Component_Prototype(
//		(_uint)ESceneType::Stage,
//		L"Component_Texture_Grass",
//		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/BillboardGrass%d.png", 1))))
//	{
//		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Grass");
//		return E_FAIL;
//	}
//#pragma endregion

#pragma endregion

	return S_OK;
}

HRESULT CLoading::Load_StageTwoResources()
{
	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;
	
	if (FAILED(GameObject_Prototype(pManagement, ESceneType::Stage02)))
	{
		PRINT_LOG(L"CLoading - Han", L"게임 오브젝트 프로토타입 실패");
		return E_FAIL;
	}

	if (FAILED(Component_Prototype(pManagement, ESceneType::Stage02)))
	{
		PRINT_LOG(L"CLoading - Han", L"컴포넌트 프로토타입 실패");
		return E_FAIL;
	}

	if (FAILED(Texture_Prototype(pManagement, ESceneType::Stage02)))
	{
		PRINT_LOG(L"CLoading - Han", L"텍스쳐 프로토타입 실패");
		return E_FAIL;
	}

	return S_OK;
}


HRESULT CLoading::GameObject_Prototype(CManagement* _pManagement, ESceneType _eStage)
{



//#pragma region Monster
//	if (FAILED(_pManagement->Add_GameObject_Prototype(
//		(_uint)_eStage,
//		L"GameObject_Monster",
//		CMonster::Create(m_pDevice))))
//	{
//		PRINT_LOG(L"Error", L"Failed To Add GameObject_Monster");
//		return E_FAIL;
//	}
//#pragma endregion
//
//#pragma region BattlePlate
//	if (FAILED(_pManagement->Add_GameObject_Prototype(
//		(_uint)ESceneType::Static,
//		L"GameObject_BattlePlate",
//		CBattlePlate::Create(m_pDevice))))
//	{
//		PRINT_LOG(L"Error", L"Failed To Add GameObject_BattlePlate");
//		return E_FAIL;
//	}
//
//#pragma endregion
//
//#pragma region Terrain
//
//	if (FAILED(_pManagement->Add_GameObject_Prototype(
//		(_uint)_eStage,
//		L"GameObject_Terrain",
//		CTerrain::Create(m_pDevice))))
//	{
//		PRINT_LOG(L"Error", L"Failed To Add GameObject_GameObject_Terrain");
//		return E_FAIL;
//	}
//
//#pragma endregion
//
//
//#pragma region Skybox
//	if (FAILED(_pManagement->Add_GameObject_Prototype(
//		(_uint)_eStage,
//		L"GameObject_Skybox",
//		CSkybox::Create(m_pDevice)
//	)))
//	{
//		PRINT_LOG(L"Error", L"Failed To Add_GameObject_Prototype_Skybox (Client - CLoading.cpp)");
//		return E_FAIL;
//	}
//#pragma endregion

//
//#pragma region UIFont
//	if (FAILED(_pManagement->Add_GameObject_Prototype(
//		(_uint)_eStage,
//		L"GameObject_UIFont",
//		CUIFont::Create(m_pDevice))))
//	{
//		PRINT_LOG(L"Error", L"Failed To Add GameObject_UIFont");
//		return E_FAIL;
//	}

	//if (FAILED(_pManagement->Add_GameObject_Prototype(
	//	//(_uint)_eStage,
	//	(_uint)ESceneType::Stage,
	//	L"GameObject_Monster_Goomba",
	//	CMonsterA::Create(m_pDevice))))
	//{
	//	PRINT_LOG(L"Error", L"Failed To Add GameObject_Monster_Goomba");
	//	return E_FAIL;
	//}





	return S_OK;

}

HRESULT CLoading::Component_Prototype(CManagement* _pManagement, ESceneType _eStage)
{

//#pragma region Component_Texture_Monster
//	/* Monster*/
//	//몬스터는 Stage에 담아야함. 씬 체인지할때 그 전 씬은 밀어버리기떄문에. 몬스터도 같이밀어주어야함.
//	if (FAILED(_pManagement->Add_Component_Prototype(
//		(_uint)_eStage,
//		L"Component_Texture_Monster",
//		CTexture::Create(m_pDevice, ETextureType::Normal, L"../Resources/Texture/Test/Monster%d.png", 1))))
//	{
//		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Monster");
//		return E_FAIL;
//	}
//#pragma endregion
//
//	if (FAILED(_pManagement->Add_Component_Prototype(
//		(_uint)ESceneType::Stage,
//		L"Component_Texture_UIFont",
//		CTexture::Create(m_pDevice, ETextureType::Font, L"../Resources/Texture/Test/Monster%d.png", 1))))
//	{
//		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Monster");
//		return E_FAIL;
//	}
//
//
//#pragma region Component_Texture_Skybox
//
//	if (FAILED(_pManagement->Add_Component_Prototype(
//		(_uint)ESceneType::Stage,
//		L"Component_Texture_Skybox",
//		CTexture::Create(m_pDevice, ETextureType::Cube, L"../../Resources/Texture/ETC/SkyboxA%d.dds", 1))))
//	{
//		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Skybox");
//		return E_FAIL;
//	}
//#pragma endregion

//#pragma region  Component_Texture_Monster_Test
//	if (FAILED(_pManagement->Add_Component_Prototype(
//		//(_uint)_eStage,
//		(_uint)ESceneType::Stage,
//		L"Component_Texture_Monster_Goomba",
//		CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Enemy/Goomba/Goomba%d.png", 6))))
//	{
//		PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Monster_Goomba");
//		return E_FAIL;
//	}

	//if (FAILED(_pManagement->Add_Component_Prototype(
	//	//(_uint)_eStage,
	//	(_uint)ESceneType::Stage,
	//	L"Component_Texture_Monster_Koopa",
	//	CTexture::Create(m_pDevice, ETextureType::Normal, L"../../Resources/Texture/Enemy/Koopa/Koopa%d.png", 8))))
	//{
	//	PRINT_LOG(L"Error", L"Failed To Add Component_Texture_Monster_Koopa");
	//	return E_FAIL;
	//}


	return S_OK;
}

HRESULT CLoading::Texture_Prototype(CManagement* _pManagement, ESceneType _eStage)
{
	return S_OK;
}

