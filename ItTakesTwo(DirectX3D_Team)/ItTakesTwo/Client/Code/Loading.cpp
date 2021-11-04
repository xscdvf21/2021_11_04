#include "stdafx.h"
#include "Loading.h"
#include "Export_Function.h"


CLoading::CLoading(LPDIRECT3DDEVICE9 pDevice)
	: m_pGraphicDev(pDevice), m_bFinish(false)
{
	ZeroMemory(m_szString, sizeof(_tchar) * 256);
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading()
{
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoadingID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eLoadingID = eLoadingID;

	return S_OK;
}
//#로딩 스태틱

_uint CLoading::Loading_Static()
{

	Engine::CComponent*	pComponent = nullptr;
	
	// Transform
	pComponent = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Transform", pComponent), E_FAIL);

	// Calculator
	pComponent = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Calculator", pComponent), E_FAIL);
	Engine::Load_SoundFile("../../Sound/Static/", RESOURCE_STATIC);
	
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Logo1", Engine::TEX_NORMAL, L"../../Resource/Logo/Logo1.tga", 1);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Logo2", Engine::TEX_NORMAL, L"../../Resource/Logo/Logo2.tga", 1);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Particle", Engine::TEX_NORMAL, L"../../Resource/Texture/Particle/Particle_%d.tga", 52);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Gradient", Engine::TEX_NORMAL, L"../../Resource/Texture/Gradient/Gradient_%d.tga", 8);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Dissolve", Engine::TEX_NORMAL, L"../../Resource/Texture/Dissolve/Dissolve_%d.tga", 4);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"VBCircle", Engine::TEX_NORMAL, L"../../Resource/Texture/VBCircle/VBCircle_%d.tga", 2);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"HP", Engine::TEX_NORMAL, L"../../Resource/Texture/HP/HP_%d.tga", 4);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"BossName", Engine::TEX_NORMAL, L"../../Resource/Texture/BossName/BossName_%d.png", 3);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"LoadingText", Engine::TEX_NORMAL, L"../../Resource/Texture/Loading/LoadingText.png", 1);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"LoadingImage", Engine::TEX_NORMAL, L"../../Resource/Texture/Loading/LoadingImage.tga", 1);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"White_BG", Engine::TEX_NORMAL, L"../../Resource/Menu/White_Bg.jpg", 1);


	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"CW_ImageUI", Engine::TEX_NORMAL, L"../../Resource/Texture/CF_UI/CodyWizard/CW_ImageUI_%d.tga", 21);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"MC_ImageUI", Engine::TEX_NORMAL, L"../../Resource/Texture/CF_UI/MayCastle/MC_ImageUI_%d.tga", 20);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Enermy_UI", Engine::TEX_NORMAL, L"../../Resource/Texture/Enermy_UI/Enermy_UI_%d.tga", 1);

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_STATIC, L"IceOrb1", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Player/IceOrb/", L"IceOrb.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_STATIC, L"BreakIce1", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/03_ChessField/BreakIce/", L"BreakIce1.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_STATIC, L"BreakIce2", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/03_ChessField/BreakIce/", L"BreakIce2.x");

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_RcTex", Engine::BUFFER_RCTEX), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_CubeTex", Engine::BUFFER_CUBETEX), E_FAIL);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Black_Bg", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Black_Bg.jpg");

	m_bFinish = true;
 
	return 0;
}
//#로딩 메뉴 선택

_uint CLoading::Loading_Menu()
{
	Engine::Load_SoundFile("../../Sound/Menu/", RESOURCE_MENUSELECT);


	// Texture
	lstrcpy(m_szString, L"Texture Loading...............");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"ControllerIcon_Keyboard", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/ControllerIcon_Keyboard.tga");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"ControllerIcon_RemotePlayer", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/ControllerIcon_RemotePlayer.tga");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"HakimSpinner", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/HakimSpinner.tga");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"HeaderBox", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/HeaderBox.tga");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"HeaderBox_Wide", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/HeaderBox_Wide.tga");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"HeaderBox_Wide2", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/HeaderBox_Wide2.tga");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"Heart", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Heart.tga");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"MenuRowBox", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/MenuRowBox.tga");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"MenuRowBox2", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/MenuRowBox2.tga");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"Shed_Intro", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Shed_Intro%d.tga", 2);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"PictureBack", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/PictureBG.png");

	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"MenuBG", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/MenuBG_%d.tga", 3);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"White_Bg", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/White_Bg.jpg");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"Local", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Local_%d.png", 2);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"Online", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Online_%d.png", 2);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"Option", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Option_%d.png", 2);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"Option2", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Option2_%d.png", 2);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"Made", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Made_%d.png", 2);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"Quit", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Quit_%d.png", 2);

	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"PlayerTexture", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Player_%d.png", 2);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"Minigame", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Minigame_%d.png", 2);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"Chapter", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Chapter_%d.png", 2);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"NewGame", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/NewGame_%d.png", 2);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"Continue", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Continue_%d.png", 2);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"Back", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Back.png");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"ContinueBtn", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/ContinueBtn.png");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"CG", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/CG.png");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"Join", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Join_%d.png", 3);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"LocalMenu", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/LocalMenu.png");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"SOA", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/SOA.png");

	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"MaySelect", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/MaySelect_%d.png", 5);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"CodySelect", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/CodySelect_%d.png", 5);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"ChoiceChar", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/ChoiceChar.png");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUSELECT, L"ContinueTitle", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/ContinueTitle.png");





	m_bFinish = true;
	return 0;
}
//#로딩 챕터 선택
_uint CLoading::Loding_ChapterSelect()
{

	Engine::Load_SoundFile("../../Sound/Menu/", RESOURCE_MENUSELECT);

	m_bFinish = true;
	return 0;
}
//#로딩 플레이어 선택
_uint CLoading::Loding_PlayerSelect()
{
	Engine::Load_SoundFile("../../Sound/Menu/", RESOURCE_MENUSELECT);

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_MENUPLAYERSELECT, L"RoseSelectRight", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Rose/", L"RoseSelectRight.x", nullptr, true);
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_MENUPLAYERSELECT, L"RoseSelectLeft", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Rose/", L"RoseSelectLeft.x", nullptr, true);
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_MENUPLAYERSELECT, L"CodyDoll", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Rose/", L"CodyDoll.x", nullptr, true);
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_MENUPLAYERSELECT, L"MayDoll", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Rose/", L"MayDoll.x", nullptr, true);
	// Texture
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUPLAYERSELECT, L"ControllerIcon_Keyboard", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/ControllerIcon_Keyboard.tga");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUPLAYERSELECT, L"ControllerIcon_XBOX", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/ControllerIcon_XBOX.tga");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUPLAYERSELECT, L"Keyboard_Enter", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/PC_Enter.tga");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUPLAYERSELECT, L"XBOX_A", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Xbox_A.tga");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUPLAYERSELECT, L"Ready_Check", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Ready_CheckMark.tga");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUPLAYERSELECT, L"HakimSpinner", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/HakimSpinner.tga");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUPLAYERSELECT, L"HeaderBox", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/HeaderBox.tga");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUPLAYERSELECT, L"HeaderBox_Wide", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/HeaderBox_Wide.tga");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUPLAYERSELECT, L"Heart", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Heart.tga");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUPLAYERSELECT, L"MenuRowBox", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/MenuRowBox.tga");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_MENUPLAYERSELECT, L"Shed_Intro", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Shed_Intro.tga");


	m_bFinish = true;
	return 0;
}
//#로딩 청소기필드
Engine::_uint CLoading::Loading_VacuumField()
{
	Engine::Load_SoundFile("../../Sound/Vacuum/", RESOURCE_VF);
	Engine::Load_SoundFile("../../Sound/Static/", RESOURCE_STATIC);

	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"test", Engine::MESH_PHYS, L"../../Resource/Mesh/StaticMesh/test/", L"test.x");

	//DynamicMesh

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"Cody", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Cody/", L"Cody.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"May", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/May/", L"May.x");


	// StaticMesh
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"Water", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Water/", L"DeadWater.x");


	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"MiniVacuumHead", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/VacuumSmall/Head/", L"VacuumHead0.x");


	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"VacuumBlow", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Vacuum/Blow/", L"VacuumBlow.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"PowerSwitch", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/PowerSwitch/", L"PowerSwitch.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"StaticHoze4", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/StaticHoze/", L"StaticHoze4.x");

	// 퓨즈
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"FuseBox", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/FuseBox/", L"FuseBox.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"FuseBox_Fuse", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/FuseBox/", L"FuseBox_Fuse.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"FuseBox_Light", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/FuseBox/", L"FuseBox_Light.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"FuseNPC", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Fuse/", L"FuseCrab.x", nullptr, true);

	// 툴박스
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"ToolBox", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/ToolBox/", L"VacuumToolBox.x");

	// BandSaw
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"BandSaw", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/BandSaw/", L"BandSaw.x");

	// WallSlideCylinder
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"WallSlideCylinder", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/WallSlideCylinder/", L"WallSlideCylinder.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"WallSlideCylinderSurface", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/WallSlideCylinderSurface/", L"WallSlideCylinderSurface.x");

	// 돌
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"GrindStone", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/GrindStone/", L"GrindStone.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"LargeCogs", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Cogs/", L"LargeCogs.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"SmallCogs", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Cogs/", L"SmallCogs.x");

	// 파워버튼
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"PowerButton", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/PowerButton/", L"PowerButton.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"PowerButtonBase", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/PowerButton/", L"PowerButtonBase.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"VacuumField", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/VacuumField/", L"VacuumField.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"VacuumField_Metal", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/VacuumField/", L"VacuumField_Metal.x");

	// 전기톱
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"CircleSaw", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/CircleSaw/", L"CircleSaw.X");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"Pedestal", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Pedestal/", L"Pedestal.X");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"BigPlug", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Cord/", L"BigPlug.X");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"Cord", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Cord/", L"Cord.X");

	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_VF, L"Blue_Bg", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Blue_Bg.jpg");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_VF, L"Yellow_Bg", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Yellow_Bg.jpg");

	m_bFinish = true;
	return 0;
}

//#로딩 청소기필드2
Engine::_uint CLoading::Loading_VacuumField2()
{
	Engine::Load_SoundFile("../../Sound/Vacuum/", RESOURCE_VF);

	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"test", Engine::MESH_PHYS, L"../../Resource/Mesh/StaticMesh/test/", L"test.x");

	//Texture
	//Engine::Ready_Texture(m_pGraphicDev, RESOURCE_VF2, L"DeadWater", Engine::TEX_NORMAL, L"../../Resource/Texture/Water/Vacuum_Goo_01_T1.tga", 1);
	//Engine::Ready_Texture(m_pGraphicDev, RESOURCE_VF2, L"Vacuum_Goo", Engine::TEX_NORMAL, L"../../Resource/Texture/Water/Vacuum_Goo_01_T%d.tga", 3);

	//DynamicMesh
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"Cody", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Cody/", L"Cody.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"May", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/May/", L"May.x");


	// StaticMesh
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"BoxTower", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Box/BoxTower/", L"BoxTower0.x");
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"BoxTowerSide", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Box/BoxTowerSide/", L"BoxTowerSide0.x");
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"BoxSide", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Box/BoxSide/", L"BoxSide0.x");
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"BoxDefault", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Box/BoxDefault/", L"BoxDefault0.x");
	 
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF2, L"StaticHoze1", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/StaticHoze/", L"StaticHoze.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF2, L"StaticHoze2", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/StaticHoze/", L"StaticHoze2.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF2, L"StaticHoze3", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/StaticHoze/", L"StaticHoze3.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF2, L"VacuumHub", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/VacuumMedium/", L"VacuumHubMedium.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF2, L"MiniVacuumHub", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/VacuumSmall/Hub/", L"VacuumHubSmall0.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF2, L"MiniVacuumHead", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/VacuumSmall/Head/", L"VacuumHead0.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF2, L"MiniVacuumHoze", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/VacuumHoze/", L"Hoze4.x", nullptr,true);
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF2, L"MiniVacuumHoze2", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/VacuumHoze/", L"Hoze2.x", nullptr,true);
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF2, L"Nozzle", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Vacuum/Nozzle/", L"SucktionNozzle.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF2, L"VacuumBlow", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Vacuum/Blow/", L"VacuumBlow.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF2, L"PowerButton", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/PowerButton/", L"PowerButton.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF2, L"PowerButtonBase", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/PowerButton/", L"PowerButtonBase.x");

	// 작은팬
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF2, L"FanBody", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/FanHead/", L"FanHead_Body.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF2, L"FanHead", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/FanHead/", L"FanHead_Fan.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF2, L"FanHolder", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/FanHead/", L"FanHead_Holder.x");

	//허브 플랫폼
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF2, L"HubPlatform", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Hub_Platform/", L"Hub_Platform.x", nullptr, true);

	// water
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF2, L"Water", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Water/", L"DeadWater.x");


	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF2, L"VacuumField", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/TestField/", L"VF_Test.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF2, L"VacuumField", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/VacuumField2/", L"VacuumField2.x");

	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_VF2, L"Blue_Bg", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Blue_Bg.jpg");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_VF2, L"Yellow_Bg", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Yellow_Bg.jpg");

	m_bFinish = true;
	return 0;
}

//#로딩 청소기필드3
Engine::_uint CLoading::Loading_VacuumField3()
{
	Engine::Load_SoundFile("../../Sound/Vacuum/", RESOURCE_VF);

	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"test", Engine::MESH_PHYS, L"../../Resource/Mesh/StaticMesh/test/", L"test.x");

	//DynamicMesh
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"Cody", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Cody/", L"Cody.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"May", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/May/", L"May.x");


	// StaticMesh
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"BoxTower", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Box/BoxTower/", L"BoxTower0.x");
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"BoxTowerSide", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Box/BoxTowerSide/", L"BoxTowerSide0.x");
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"BoxSide", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Box/BoxSide/", L"BoxSide0.x");
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF, L"BoxDefault", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Box/BoxDefault/", L"BoxDefault0.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"Water", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Water/", L"DeadWater.x");


	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"MiniVacuumHub", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/VacuumSmall/Hub/", L"VacuumHubSmall0.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"MiniVacuumHead", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/VacuumSmall/Head/", L"VacuumHead0.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"MiniVacuumHoze", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/VacuumHoze/", L"Hoze2.x",nullptr,true);

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"VacuumBlow", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Vacuum/Blow/", L"VacuumBlow.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"PowerSwitch", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/PowerSwitch/", L"PowerSwitch.x");

	// 퓨즈
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"FuseNPC", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Fuse/", L"FuseCrab.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"FuseBox", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/FuseBox/", L"FuseBox.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"FuseBox_Fuse", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/FuseBox/", L"FuseBox_Fuse.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"FuseBox_Light", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/FuseBox/", L"FuseBox_Light.x");

	// 작은팬
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"FanBody", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/FanHead/", L"FanHead_Body.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"FanHead", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/FanHead/", L"FanHead_Fan.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"FanHolder", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/FanHead/", L"FanHead_Holder.x");

	// 레버
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"Lever", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Lever/", L"Lever.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"LeverBase", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Lever/", L"Lever_Base.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"VacuumCrusher", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Crusher/", L"Crusher.x");

	// 큰팬
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"BigFanBlade", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/BigFan/", L"Fan_BigBlade.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"BigFanSylinder", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/BigFan/", L"Fan_Sylinder.x");

	// 관람차
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"SmallBlade", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/BigFan/", L"Fan_SmallBlade.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"FerrisBody", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/BigFan/", L"Fan_Module1.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"FerrisModule", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/BigFan/", L"Fan_Module2.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"FerrisBase", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/BigFan/", L"Fan_Base.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"Valve_Base", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Valve/", L"Valve_Base.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"Valve_Handle", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Valve/", L"Valve_Handle.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"FanButton", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/BigFan_Button/", L"Fan_Button.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"FanButtonBase", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/BigFan_Button/", L"Fan_ButtonBase.x");

	// 풍량계
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"AirFlowMeter", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Weather/", L"Weather_Vane.x");

	// 덤벨
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"Dumbbell", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Dumbbell/", L"Dumbbell.x");

	// 제네레이터
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"VacuumGenerator", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/VacuumGenerator/", L"VacuumGenerator.x");

	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"VacuumField", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/TestField/", L"VF_Test.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"VacuumField3_Part01", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/VacuumField3/", L"VacuumField3_Part01.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"VacuumField3_Part02", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/VacuumField3/", L"VacuumField3_Part02.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"VacuumField3_Part03", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/VacuumField3/", L"VacuumField3_Part03.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"VacuumField3_Metal", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/VacuumField3/", L"VacuumField3_Metal.x");

	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VF3, L"VacuumField3_PartWall", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/TestWall/", L"Single.x",L"../../Data/Instancing/test.txt");


	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_VF3, L"Blue_Bg", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Blue_Bg.jpg");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_VF3, L"Yellow_Bg", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Yellow_Bg.jpg");

	m_bFinish = true;
	return 0;
}

//#로딩 청소기보스
Engine::_uint CLoading::Loading_VacuumBoss()
{
	Engine::Load_SoundFile("../../Sound/Vacuum/", RESOURCE_VF);

	//테스트피직스네비
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VB, L"test", Engine::MESH_PHYS, L"../../Resource/Mesh/StaticMesh/test/", L"test.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VB, L"VacuumBossField", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/VacuumBossField/", L"VacuumBossField0.x");
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VB, L"BossPlatform_Main", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Boss/BossPlatform_Main/", L"BossPlatform_Main0.x");
	

	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VB, L"VacuumBossPlatform", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/VacuumBossPlatform/", L"VacuumBossPlatform.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VB, L"VacuumPlatform_Part01", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/02_VacuumBoss/Vacuum/Platform/", L"VacuumPlatform_Part01.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VB, L"VacuumPlatform_Part02", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/02_VacuumBoss/Vacuum/Platform/", L"VacuumPlatform_Part02.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VB, L"VacuumPlatform_Part03", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/02_VacuumBoss/Vacuum/Platform/", L"VacuumPlatform_Part03.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VB, L"VacuumPlatform_Part04", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/02_VacuumBoss/Vacuum/Platform/", L"VacuumPlatform_Part04.x");


	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VB, L"VacuumPlatform_PartTest", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/02_VacuumBoss/Vacuum/", L"VacuumBossPlateform_Part0.x");


	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VB, L"VacuumDebris01", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/02_VacuumBoss/Boss/BossDebris/", L"BossDebris01.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VB, L"VacuumDebris02", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/02_VacuumBoss/Boss/BossDebris/", L"BossDebris02.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VB, L"VacuumMine", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/02_VacuumBoss/Boss/BossMine/", L"BossMine.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VB, L"VacuumBomb", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/02_VacuumBoss/Boss/BossBomb/", L"BossBomb.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VB, L"VacuumCircle", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Player/IceOrb/", L"IceOrb.x");


	//다이나믹 매쉬

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"Cody", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Cody/", L"Cody.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"May", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/May/", L"May.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VB, L"VacuumBoss", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/VacuumBoss/", L"VacuumBoss.x");


	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VB, L"MiniVacuumHub", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/VacuumSmall/Hub/", L"VacuumHubSmall0.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VB, L"MiniVacuumHead", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/VacuumSmall/Head/", L"VacuumHead0.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VB, L"MiniVacuumHoze", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/VacuumHoze/", L"Hoze2.x", nullptr,true);
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_VB, L"VacuumBlow", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/02_VacuumBoss/Vacuum/Blow/", L"VacuumBlow.x");

	m_bFinish = true;
	return 0;
}
//#로딩 체스 필드

Engine::_uint CLoading::Loading_ChessField()
{
	Engine::Load_SoundFile("../../Sound/Chess/", RESOURCE_CF);

	//Engine::Ready_Texture(m_pGraphicDev, RESOURCE_CF, L"FireWall", Engine::TEX_NORMAL, L"../../Resource/Logo/Logo1.tga", 1);
	//Engine::Ready_Texture(m_pGraphicDev, RESOURCE_CF, L"SwirlEffect", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/FireEffect/SwirlEffect_%d.tga", 3);
	//Engine::Ready_Texture(m_pGraphicDev, RESOURCE_CF, L"TorchTexture", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Mesh/StaticMesh/Torch/Torch_%d.tga", 2);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_CF, L"SwordEffect", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/FireEffect/SwordEffect_%d.tga", 5);

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"Cody_Wizard", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Cody_Wizard/", L"Cody_Wizard.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"IceOrb", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Player/IceOrb/", L"IceOrb.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"Cone", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Player/IceOrb/", L"Cone.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"May_Castle", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/May_Castle/", L"May_Castle.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"ToySword", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Player/ToySword/", L"ToySword.x");

	// Toy Ogre
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ToyOgre", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Toy/Ogre/", L"ToyOgre.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"OgreChain", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Toy/ToyChain/", L"ToyChain.x", nullptr, true);

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"OgreStartDoor", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/03_ChessField/Bar/", L"BarDoor.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"OgreStone", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/02_VacuumBoss/Boss/BossDebris/", L"BossDebris01.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"OgreCenterHatch", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/03_ChessField/Bar/", L"Hatch.x");
	
	//맵 오브젝트
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessOpenDoor", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/03_ChessField/Door/", L"SoloDoor.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessDuoDoor", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/03_ChessField/Door/", L"DuoDoor.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessDoorSwitch", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/03_ChessField/Plate/", L"PlateButton.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessDoorSwitchFrame", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/03_ChessField/Plate/", L"PlateButton_Frame.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessBridge", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/03_ChessField/Bridge/", L"Bridge.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessSpinTrab", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/03_ChessField/Trap/", L"SpinTrap.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"Crusher_BreakWall", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/03_ChessField/Crusher/", L"Crusher_BreakWall.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"Crusher_Bridge", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/03_ChessField/Crusher/", L"Crusher_Bridge.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"Crusher_Bridge_Base", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/03_ChessField/Crusher/", L"Crusher_Bridge_Base.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"Crusher_Bridge_HitPoint", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/03_ChessField/Crusher/", L"Crusher_Bridge_HitPoint.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"Crusher_Bridge_Holder", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/03_ChessField/Crusher/", L"Crusher_Bridge_Holder.x");
	// water
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"Lava", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/03_ChessField/Lava/", L"Lava.x");

	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"TestCol", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Box/BoxDefault/", L"BoxDefault0.x");

	////ToyCrusher
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ToyCrusher", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Toy/ToyCrusher/", L"ToyCrusher.x");

	//// Toy Knight
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ToyKnight", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Toy/Knight/", L"ToyKnight.x");
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ToyKnight_Sword", Engine::MESH_STATIC, L"../../Resource/Mesh/DynamicMesh/Toy/Knight/", L"ToyKnight_Sword.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ToyKnight_MorningStar", Engine::MESH_STATIC, L"../../Resource/Mesh/DynamicMesh/Toy/Knight/", L"ToyKnight_MorningStar.x");

	//// Toy Sheilder
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ToySheilder", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Toy/Sheilder/", L"ToySheilder.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ToySheild", Engine::MESH_STATIC, L"../../Resource/Mesh/DynamicMesh/Toy/Sheilder/", L"ToySheild.x");

	// Toy Arbalist
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ToyArbalist", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Toy/Arbalist/", L"ToyArbalist.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"CrossBow", Engine::MESH_STATIC, L"../../Resource/Mesh/DynamicMesh/Toy/Arbalist/", L"CrossBow.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"Arrow", Engine::MESH_STATIC, L"../../Resource/Mesh/DynamicMesh/Toy/Arbalist/", L"Arrow.x");

	//// Toy Mage

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ToyMage", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Toy/Mage/", L"ToyMage.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ToyMage_Staff", Engine::MESH_STATIC, L"../../Resource/Mesh/DynamicMesh/Toy/Mage/", L"ToyMage_Staff.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ToyMage_Bullet", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Player/IceOrb/", L"IceOrb.x");

	//Map

	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Part01", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/", L"ChessField_Part01.x");
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Part02", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/", L"ChessField_Part02.x");
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Part03", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/", L"ChessField_Part03.x");
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Part04", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/", L"ChessField_Part04.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Part01", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/", L"ChessField_Part01.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Part02", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/", L"ChessField_Part02.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Part03", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/", L"ChessField_Part03.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Part04", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/", L"ChessField_Part04.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Part05_Floor", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/", L"ChessField_Part05_Floor.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Part05_Wall", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/", L"ChessField_Part05_Wall.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Light", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/", L"ChessField_Light.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Dark", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/", L"ChessField_Dark.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Dark1", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/Dark/", L"Dark1.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Dark2", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/Dark/", L"Dark2.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Dark3", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/Dark/", L"Dark3.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Dark4", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/Dark/", L"Dark4.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Dark5", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/Dark/", L"Dark5.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Dark6", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/Dark/", L"Dark6.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Dark7", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/Dark/", L"Dark7.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Dark8", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/Dark/", L"Dark8.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Dark9", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/Dark/", L"Dark9.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Dark10", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/Dark/", L"Dark10.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Dark11", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/Dark/", L"Dark11.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Dark12", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/Dark/", L"Dark12.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessField_Dark13", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessField/Dark/", L"Dark13.x");


	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CF, L"ChessTest", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessTestField/", L"ChessTest.x");


	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_CF, L"Blue_Bg", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Blue_Bg.jpg");
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_CF, L"Yellow_Bg", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Menu/Yellow_Bg.jpg");


	m_bFinish = true;
	return 0;
}
//#로딩 체스 보스
Engine::_uint CLoading::Loading_ChessBoss()
{

	Engine::Load_SoundFile("../../Sound/Chess/", RESOURCE_CF);

	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_CB, L"SwordEffect", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/FireEffect/SwordEffect_%d.tga", 5);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_CB, L"SwirlEffect", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/FireEffect/SwirlEffect_%d.tga", 3);


	//Engine::Ready_Texture(m_pGraphicDev, RESOURCE_CB, L"TorchTexture", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Mesh/StaticMesh/04_ChessBoss/Torch/Torch_%d.tga", 2);
	
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"Torch", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/04_ChessBoss/Torch/", L"Torch.x");

	//Engine::Ready_Texture(m_pGraphicDev, RESOURCE_CB, L"TorchTexture", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Mesh/StaticMesh/Torch/Torch_%d.tga", 2);
	
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"ChessBoard", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/test/", L"ChessBoard.x");

	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"Firewall", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Firewall/", L"Firewall.x");

	//Engine::Ready_Texture(m_pGraphicDev, RESOURCE_CB, L"ToySword_N", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Mesh/StaticMesh/Player/ToySword/ToySword_C2.tga");
	//Engine::Ready_Texture(m_pGraphicDev, RESOURCE_CB, L"ToySword_E", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Mesh/StaticMesh/Player/ToySword/ToySword_C4.tga");
	//Engine::Ready_Texture(m_pGraphicDev, RESOURCE_CB, L"SwirlEffect", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/FireEffect/SwirlEffect_%d.tga", 3);
	//Engine::Ready_Texture(m_pGraphicDev, RESOURCE_CB, L"TorchTexture", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Mesh/StaticMesh/Torch/Torch_%d.tga", 2);
	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_CB, L"SwordEffect", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/FireEffect/SwordEffect_%d.tga", 5);

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"Cody_Wizard", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Cody_Wizard/", L"Cody_Wizard.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"IceOrb", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Player/IceOrb/", L"IceOrb.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"Cone", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Player/IceOrb/", L"Cone.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"May_Castle", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/May_Castle/", L"May_Castle.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"ToySword", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Player/ToySword/", L"ToySword.x");

	// Chess
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"Chess_Bishop", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Chess/", L"Chess_Bishop.x");
 	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"ChessBishop_MoveBullet", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Player/IceOrb/", L"IceOrb.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"ChessBishop_StopBullet", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Player/IceOrb/", L"IceOrb.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"Chess_Knight", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Chess/", L"Chess_Knight.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"Chess_Rook", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Chess/", L"Chess_Rook.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"ChessRook_MoveBullet", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Player/IceOrb/", L"IceOrb.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"ChessRook_StopBullet", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Player/IceOrb/", L"IceOrb.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"Chess_Pawn", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Chess/", L"Chess_Pawn.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"ChessPawn_Bullet", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Player/IceOrb/", L"IceOrb.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"Chess_King", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Chess/", L"King.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"Chess_Queen", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Chess/", L"Queen.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"CutieDoor", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/CutieDoor/", L"CutieDoor.x", nullptr, true);
	// Field

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"ChessBossField", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/ChessBossField/", L"ChessBossField0.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"SpaceSkybox", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/Space_Sphere/", L"Space_Sphere.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"ChessEscape_Shuttle", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/04_ChessBoss/ChessEscape_Shuttle/", L"ChessEscape_Shuttle.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"ChessEscape_ShuttleDoor", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/04_ChessBoss/ChessEscape_Shuttle/", L"ChessEscape_ShuttleDoor.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"ChessEscape_ShuttleRamp", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/04_ChessBoss/ChessEscape_Shuttle/", L"ChessEscape_ShuttleRamp.x");
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_CB, L"test", Engine::MESH_PHYS, L"../../Resource/Mesh/StaticMesh/test/", L"test.x");
	m_bFinish = true;
	return 0;
}
//#로딩 스페이스 필드

Engine::_uint CLoading::Loading_SpaceField()
{
	Engine::Load_SoundFile("../../Sound/Space/", RESOURCE_SF);
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"SpaceSkybox", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/Space_Sphere/", L"Space_Sphere.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"ChessEscape_Shuttle", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/04_ChessBoss/ChessEscape_Shuttle/", L"ChessEscape_Shuttle.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"ChessEscape_ShuttleDoor", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/04_ChessBoss/ChessEscape_Shuttle/", L"ChessEscape_ShuttleDoor.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"ChessEscape_ShuttleRamp", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/04_ChessBoss/ChessEscape_Shuttle/", L"ChessEscape_ShuttleRamp.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"Cody", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Cody_Space/", L"Cody_Space.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"May", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/May_Space/", L"May_Space.x");
	
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"Gravity_Corner", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Graviry_Corner/", L"Gravity_Corner.x");
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"Gravity_Corner2", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Graviry_Corner/", L"Gravity_Corner2.x");
	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"Gravity_Corner3", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Graviry_Corner/", L"Gravity_Corner3.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"Gravity_Corner4", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Tutorial/Graviry_Corner/", L"Bend.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"RobotHead", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/Generator_Head/", L"Generator_Head.x", nullptr, true);


	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"SpaceBridge", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Tutorial/Space_Bridge/", L"Space_Bridge.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"BigCody_Button", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Tutorial/BigCody_Button/", L"BigCody_Button.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"BigCody_ButtonBase", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Tutorial/BigCody_Button/", L"BigCody_ButtonBase.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"SFSpaceGlass", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField/Instance/", L"SpaceGlass.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"SFSpaceBox", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField/Instance/", L"SpaceBox.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"SFRectGlass", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField/Instance/", L"SpaceGlass2.x");


	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"SpaceFloor", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField/Floor/", L"SpaceFloor.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"Gravity_Straight", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField/Gravity_Straight/", L"Gravity_Straight.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"Lamp", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField/Lamp/", L"Lamp.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"SpaceTutorial", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField/Tutorial/", L"SpaceTutorial.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"DoorWindow", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField/Window/", L"DoorWindow.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"SpaceWindow", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField/Window/", L"SpaceWindow.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"Laser", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/2D/Laser/", L"Laser.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"Generator", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField/Generator/", L"TutorialGenerator.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"GeneratorBattery", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/Generator_Battery/", L"Generator_Battery.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"GeneratorLever", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/Space_Lever/", L"Space_Lever.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"GeneratorStair", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/Generator_Stair/", L"Generator_Stair.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"HangDoor", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Tutorial/HangDoor/", L"HangDoor.X");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"Earth", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/Earth/", L"Earth.X");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF, L"BatteryBox", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/BatteryBox/", L"GeneratorBattery.X");

	m_bFinish = true;
	return 0;
}
//#로딩 스페이스 필드2

Engine::_uint CLoading::Loading_SpaceField2()
{
	Engine::Load_SoundFile("../../Sound/Space/", RESOURCE_SF);
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"SpaceSkybox", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/Space_Sphere/", L"Space_Sphere.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"Cody", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Cody_Space/", L"Cody_Space.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"May", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/May_Space/", L"May_Space.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"RobotHead", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/Generator_Head/", L"Generator_Head.x", nullptr, true);
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"SpaceCup", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/SpaceCup/", L"SpaceCup.x", nullptr, true);

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"GroundBounce1", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/2D/GroundBounce/", L"SF2_GroundBounce_1.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"GroundBounce2", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/2D/GroundBounce/", L"SF2_GroundBounce_2.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"GroundBounce3", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/2D/GroundBounce/", L"SF2_GroundBounce_3.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"Move_Platform", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/2D/Move_Platform/", L"Move_Platform.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"Move_Wall", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/2D/Move_Wall/", L"Move_Wall.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"Lever", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/01_VacuumField/Lever/", L"Lever.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"Laser_Lamp", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/2D/Laser/", L"Laser_Lamp.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"Push_Platform", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/2D/Push_Platform/", L"Push_Platform.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"GroundBounce_Panel", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/2D/GroundBounce_Panel/", L"GroundBounce_Panel.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"Space_Valve", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/2D/Space_Valve/", L"Space_Valve.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"Spring", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/2D/Spring/", L"Spring.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"Spring_Platform", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/2D/Spring/", L"Spring_Platform.x");


	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"SpacePlatform", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField2D/Platform/", L"SF2_Platform.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"Panel", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField2D/Panel/", L"SF2_Panel.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"Gravity", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField2D/Gravity/", L"SF2_Gravity.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"GravityCorner", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField2D/Gravity/", L"SF2_GravityCorner.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"Background", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField2D/Background/", L"SF2_Background.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"Alpha", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField2D/Alpha/", L"SF2_Alpha.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"Wall", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField2D/Wall/", L"SF2_Wall.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"Generator", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField/Generator/", L"TutorialGenerator.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"GeneratorBattery", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/Generator_Battery/", L"Generator_Battery.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"GeneratorLever", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/Space_Lever/", L"Space_Lever.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"WarpGate_Star", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/WarpGate_Star/", L"WarpGate_Star.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"BatteryBox", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/BatteryBox/", L"GeneratorBattery.X");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF2, L"GeneratorStair", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/Generator_Stair/", L"Generator_Stair.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"SpaceLaser", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/2D/Laser/", L"Laser.x");
	m_bFinish = true;
	return 0;

}
//#로딩 스페이스 필드3

Engine::_uint CLoading::Loading_SpaceField3()
{
	Engine::Load_SoundFile("../../Sound/Space/", RESOURCE_SF);
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"SpaceSkybox", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/Space_Sphere/", L"Space_Sphere.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"Cody", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Cody_Space/", L"Cody_Space.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"May", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/May_Space/", L"May_Space.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"RobotHead", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/Generator_Head/", L"Generator_Head.x", nullptr, true);


	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"Seesaw", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/3D/Seesaw/", L"Seesaw.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"SeesawBase", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/3D/Seesaw/", L"SeesawBase.x");

	//Spring
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"SpaceWeight_Platform", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/3D/Spring/", L"SpaceWeight_Platform.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"SpaceWeight_Spring", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/3D/Spring/", L"SpaceWeight_Spring.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"SpaceWeight_SpringBase", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/3D/Spring/", L"SpaceWeight_SpringBase.x");

	//GroundPound_Box
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"GroundPound_Box", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/3D/GroundPound_Box/", L"GroundPound_Box.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"GroundPound_BoxButton", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/3D/GroundPound_Box/", L"GroundPound_BoxButton.x");

	//GlassBox
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"GlassBox", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/3D/GlassBox/", L"GlassBox.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"GlassBox_Handle", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/3D/GlassBox/", L"GlassBox_Handle.x");

	//튜브
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"Tube_Blue", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/3D/Tube/", L"Tube_Blue.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"Tube_Orange", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/3D/Tube/", L"Tube_Orange.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"Tube_Purple", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/3D/Tube/", L"Tube_Purple.x");


	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"Area1_Platform", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField3D/Area1/Platform/", L"SF3_A1_Platform.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"Area1_Platform2", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField3D/Area1/Platform/", L"SF3_A1_Platform2.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"Area1_Generator", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField3D/Area1/Generator/", L"SF3_A1_Generator.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"Area1_Gravity", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField3D/Area1/Gravity/", L"SF3_A1_Gravity.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"Area1_GravityCorner", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField3D/Area1/Gravity/", L"SF3_A1_GravityCorner.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"Area1_Window", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField3D/Area1/Window/", L"SF3_A1_Window.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"Area2_Platform", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField3D/Area2/Platform/", L"SF3_A2_Platform.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"Area2_Generator", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField3D/Area2/Generator/", L"SF3_A2_Generator.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"Area2_Gravity", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField3D/Area2/Gravity/", L"SF3_A2_Gravity.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"Area2_Gravity_OutCorner", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField3D/Area2/Gravity/", L"SF3_A2_Gravity_OutCorner.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"Area2_Gravity_InCorner", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField3D/Area2/Gravity/", L"SF3_A2_Gravity_InCorner.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"Area2_Alpha", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceField3D/Area2/Alpha/", L"SF3_A2_Alpha.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"GeneratorBattery", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/Generator_Battery/", L"Generator_Battery.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"GeneratorLever", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/Space_Lever/", L"Space_Lever.x");
	
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"WarpGate_Star", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/WarpGate_Star/", L"WarpGate_Star.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"Earth", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/Earth/", L"Earth.X");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"BatteryBox", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/BatteryBox/", L"GeneratorBattery.X");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"GeneratorStair", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/Generator_Stair/", L"Generator_Stair.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SF3, L"SpaceRing", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/3D/Ring/", L"SpaceRing.x");

	m_bFinish = true;
	return 0;

}
//#로딩 스페이스 보스
Engine::_uint CLoading::Loading_SpaceBoss()
{
	Engine::Load_SoundFile("../../Sound/Space/", RESOURCE_SF);
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"SpaceSkybox", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/Common/Space_Sphere/", L"Space_Sphere.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"Cody", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/Cody_Space/", L"Cody_Space.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"May", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/May_Space/", L"May_Space.x");

	//Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"SpaceLaser", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Player/IceOrb/", L"Cone.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_PLAYER, L"SpaceLaser", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/05_SpaceField/2D/Laser/", L"Laser.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SB, L"SpaceShip", Engine::MESH_DYNAMIC, L"../../Resource/Mesh/DynamicMesh/SpaceShip/", L"SpaceShip.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SB, L"BaboonGround", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceBoss/Platform/", L"SpaceBoss_Platform.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SB, L"SpaceBossFieldWall", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceBoss/Wall/", L"SpaceBoss_Wall.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SB, L"SpaceBossFieldGlassWall", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceBoss/Wall/", L"SpaceBoss_GlassWall.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SB, L"BaboonMissile", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/06_SpaceBoss/Rocket/", L"Rocket.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SB, L"VacuumDebris01", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/02_VacuumBoss/Boss/BossDebris/", L"BossDebris01.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SB, L"CoreCover", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/06_SpaceBoss/CorePillar/", L"CoreCover.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SB, L"CoreGlass", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/06_SpaceBoss/CorePillar/", L"CoreGlass.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SB, L"CoreGlass_Broken", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/06_SpaceBoss/CorePillar/", L"CoreGlass_Broken.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SB, L"CorePillar", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/06_SpaceBoss/CorePillar/", L"CorePillar.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SB, L"IceOrb", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Player/IceOrb/", L"IceOrb.x");

	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SB, L"WindLaser", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/06_SpaceBoss/WindLaser/", L"WindLaser.x"); //임시용.
	
	
	//맵 중력발판
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SB, L"SpaceBoss_Gravity", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceBoss/Gravity/", L"SpaceBoss_Gravity.x");
	Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_SB, L"SpaceBoss_Gravity_InCorner", Engine::MESH_STATIC, L"../../Resource/Mesh/StaticMesh/Map/SpaceBoss/Gravity/", L"SpaceBoss_Gravity_InCorner.x");
	m_bFinish = true;
	return 0;

}

//#로딩 테스트 필드
_uint CLoading::Loading_TestField()
{

	m_bFinish = true;
	return 0;
}

_uint CLoading::Loading_Ending()
{
	Engine::Load_SoundFile("../../Sound/Static/", RESOURCE_ENDING);

	Engine::Ready_Texture(m_pGraphicDev, RESOURCE_ENDING, L"EndingCredit", Engine::TEXTURETYPE::TEX_NORMAL, L"../../Resource/Texture/Ending/EndingCredit.jpg", 1);


	m_bFinish = true;
	return _uint();
}

unsigned int __stdcall CLoading::Thread_Main(void * pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint	iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STATIC:
		iFlag = pLoading->Loading_Static();
		break;
	case LOADING_MENU:
		iFlag = pLoading->Loading_Menu();
		break;
	case LOADING_CHAPTERSELECT:
		iFlag = pLoading->Loding_ChapterSelect();
		break;
	case LOADING_MENUPLAYERSELECT:
		iFlag = pLoading->Loding_PlayerSelect();
		break;
	case LOADING_VACUUM_FIELD:
		iFlag = pLoading->Loading_VacuumField();
		break;

	case LOADING_VACUUM_FIELD2:
		iFlag = pLoading->Loading_VacuumField2();
		break;

	case LOADING_VACUUM_FIELD3:
		iFlag = pLoading->Loading_VacuumField3();
		break;

	case LOADING_VACUUM_BOSS:
		iFlag = pLoading->Loading_VacuumBoss();
		break;
	case LOADING_CHESS_FIELD:
		iFlag = pLoading->Loading_ChessField();
		break;
	case LOADING_CHESS_BOSS:
		iFlag = pLoading->Loading_ChessBoss();
		break;
	case LOADING_SPACE_FIELD:
		iFlag = pLoading->Loading_SpaceField();
		break;
	case LOADING_SPACE_FIELD2:
		iFlag = pLoading->Loading_SpaceField2();
		break;
	case LOADING_SPACE_FIELD3:
		iFlag = pLoading->Loading_SpaceField3();
		break;
	case LOADING_SPACE_BOSS:
		iFlag = pLoading->Loading_SpaceBoss();
		break;
	case LOADING_TEST_FIELD:
		iFlag = pLoading->Loading_TestField();
		break;

		//iFlag = pLoading
	case LOADING_ENDING:
		iFlag = pLoading->Loading_Ending();
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());
	_endthreadex(0);

	return iFlag;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoadingID)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eLoadingID)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}
