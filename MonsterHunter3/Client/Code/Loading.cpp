#include "stdafx.h"
#include "Loading.h"
#include "Export_Fucntion.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	ZeroMemory(m_szString, sizeof(_tchar) * 256);
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading(void)
{

}

HRESULT CLoading::Ready_Loading(LOADINGID eLoadingID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eLoadingID = eLoadingID;

	return S_OK;
}

_uint CLoading::Loading_ForStage(void)
{
	lstrcpy(m_szString, L"Loading...");
	// buffer
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev,
	RESOURCE_STATIC,
		L"Proto_Buffer_TerrainTex",
		Engine::BUFFER_TERRAINTEX, 129, 129, 1),
		E_FAIL);

	

	//FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC
	//	, L"Proto_Buffer_Sphere", 1.f, 30, 10, m_pMesh, NULL));

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC
		, L"Proto_Buffer_Sphere", 2.f, 30, 10, m_pMesh, NULL));

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Proto_Buffer_CubeTex",
		Engine::BUFFER_CUBETEX),
		E_FAIL);

		
	// texture

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_UI_Cursor",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Inventory/Cursor00.png",
		1),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_UI_Invectory",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Inventory/Inventory1.png",
		1),
		E_FAIL);



	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_UI_Stemina",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UI/Stemina.png",
		1),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_UI_Victory",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UI/Victory.png",
		1),
		E_FAIL);



	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Effect_Hit",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Effect/HitPlayer/HitPlayer.tga",
		1),
		E_FAIL);



	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Effect_BulletWind",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Effect/Circle/Circle1.tga",
		1),
		E_FAIL);



	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Effect_Wind",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Effect/Wind/Wind_%d.tga",
		4),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Effect_Dust",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Effect/Airbone/smoke%d.png",
		15),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Effect_Blood0",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Effect/Blood/Blood1/Blood1_0.tga",
		1),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Effect_Blood1",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Effect/Blood/Blood1_1.tga",
		1),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Effect_Blood2",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Effect/Blood/Blood1_2.tga",
		1),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Effect_Blood3",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Effect/Blood/Blood1_3.tga",
		1),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Effect_Aura",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Effect/Aura/Aura_%d.png",
		30),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Effect_Bust",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Effect/Bust5/Bust5_%d.png",
		32),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Effect_Ring",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/Effect/Ring/Ring_%d.tga",
		5),
		E_FAIL);
	///

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Texture_Terrain",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Terrain/Grass_%d.tga",
		2),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Texture_SkyBox",
		Engine::TEX_CUBE,
		L"../../Resource/Texture/SkyBox/SkyBox%d.dds",
		1),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Texture_Effect",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Explosion/Explosion%d.png",
		90),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Texture_UI",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/hpbar.png",
		1),
		E_FAIL);

#pragma region UI
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Texture_UI_Coin",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UI/UI_Coin.tga",
		1),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Texture_UI_Cross",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UI/UI_Cross.tga",
		1),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Texture_UI_Frame",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UI/UI_Frame.tga",
		1),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Texture_UI_Gogi",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UI/UI_Gogi.tga",
		1),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Texture_UI_Item_Back",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UI/UI_Item_Back.tga",
		1),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Texture_UI_UI_Item2Test",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UI/UI_Item2Test.tga",
		1),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Texture_UI_UI_Item2",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UI/UI_Item2.tga",
		1),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Texture_UI_Hp",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UI/UI_Hp.png",
		1),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Proto_Texture_UI_Stemina",
		Engine::TEX_NORMAL,
		L"../../Resource/Texture/UI/UI_Stemina.png",
		1),
		E_FAIL);


#pragma endregion

	
	//스태틱 매쉬

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, 
		RESOURCE_STAGE, 
		L"Mesh_Back_Tree",
		Engine::MESH_STATIC, 
		L"../../Resource/Mesh/StaticMesh/Map/Map0/", 
		L"Map0_Back_Tree.x"),
		E_FAIL);
	

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"town_top",
		Engine::MESH_STATIC,
		L"../../Resource/Mesh/StaticMesh/Town/",
		L"town_top.x"),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"town_Ground",
		Engine::MESH_STATIC,
		L"../../Resource/Mesh/StaticMesh/Town/",
		L"town_Ground.x"),
		E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Map0_Ground",
		Engine::MESH_STATIC,
		L"../../Resource/Mesh/StaticMesh/Map/Map0/",
		L"Map0_Ground.x"),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Map0_Smoke1",
		Engine::MESH_STATIC,
		L"../../Resource/Mesh/StaticMesh/Map/Map0/",
		L"Map0_Smoke1.x"),
		E_FAIL);


	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev, 
	//	RESOURCE_STAGE, 
	//	L"Mesh_Stone",
	//	Engine::MESH_STATIC, 
	//	L"../Bin/Resource/Mesh/StaticMesh/TombStone/", 
	//	L"TombStone.x"),
	//	E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Tree",
	//	Engine::MESH_STATIC,
	//	L"../Bin/Resource/Mesh/StaticMesh/Tree/",
	//	L"Tree01.x"),
	//	E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Player_Weapon",
		Engine::MESH_DYNAMIC,
		L"../../Resource/Mesh/StaticMesh/Hammer/",
		L"20.x"),
		E_FAIL);

	////다이나믹 매쉬
	//lstrcpy(m_szString, L"Mesh leia...............");


	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Iru",
		Engine::MESH_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/Iru/",
		L"Black_Iru.x"),
		E_FAIL);



	FAILED_CHECK_RETURN(Engine::Ready_MeshesBoss(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Boss",
		Engine::MESH_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/Boss/leia/",
		L"leia.x"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Bosstail",
		Engine::MESH_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/Boss/leia/",
		L"leia_tail.x"),
		E_FAIL);

	//메테오.
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Bossbullet",
		Engine::MESH_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/Leia_Stone/",
		L"Leia_Stone.x"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_BossTimebullet",
		Engine::MESH_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/Leia_Stone/",
		L"Leia_Stone.x"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Player",
		Engine::MESH_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/Player/Male/",
		L"male_b.x"),
		E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_MonsterJagi",
	//	Engine::MESH_DYNAMIC,
	//	L"../../Resource/Mesh/DynamicMesh/Monster/Jagi/",
	//	L"Jagi.x"),
	//	E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Sword",
	//	Engine::MESH_STATIC,
	//	L"../Bin/Resource/Mesh/StaticMesh/Sword/",
	//	L"Sword.x"),
	//	E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Navi",
	//	Engine::MESH_NAVI,
	//	NULL,
	//	NULL),
	//	E_FAIL);


	m_bFinish = true;

	lstrcpy(m_szString, L"Press Enter");

	return 0;
}

unsigned int __stdcall CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint	iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;

	case LOADING_BOSS:
		break;
	}
	
	LeaveCriticalSection(pLoading->Get_Crt());
	_endthreadex(0);

	return iFlag;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoadingID)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eLoadingID)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}

