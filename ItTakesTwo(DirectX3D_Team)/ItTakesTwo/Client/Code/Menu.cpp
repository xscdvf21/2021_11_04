#include "stdafx.h"
#include "Menu.h"
#include "Logo.h"
#include "Export_Function.h"
#include "ImageUI.h"

CMenu::CMenu(LPDIRECT3DDEVICE9 pDevice)
	:Engine::CScene(pDevice)

{
}

CMenu::~CMenu()
{
}

HRESULT CMenu::Ready_Scene(void)
{
	Engine::Set_SoundScene(RESOURCE_MENUSELECT);

	FAILED_CHECK_RETURN(Ready_LoadingUI_Layer(L"UI_Resource"), E_FAIL);

	return S_OK;
}

_int CMenu::Update_Scene(const _float & fTimeDelta)
{
	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);

	if (!m_bPlayer1)
	{
		if (Engine::Key_Down(DIK_SPACE))
		{
			StopSound(CSoundMgr::CHANNELID::Menu);
			PlaySoundW(L"MenuChoice.wav", CSoundMgr::CHANNELID::Menu, 1.f);

			m_pPlayerBox1->Set_TextureIndex(1);
			m_bPlayer1 = true;
		}
	}
	else
	{
		if (Engine::Key_Down(DIK_SPACE))
		{

			StopSound(CSoundMgr::CHANNELID::Menu);
			PlaySoundW(L"MenuChoice.wav", CSoundMgr::CHANNELID::Menu, 1.f);

			m_pPlayerBox1->Set_TextureIndex(0);
			m_bPlayer1 = false;
		}
	}

	if (!m_bPlayer2)
	{
		if (Engine::Key_Down(PADKEYSTATE::DIC_A))
		{
			StopSound(CSoundMgr::CHANNELID::Menu);
			PlaySoundW(L"MenuChoice.wav", CSoundMgr::CHANNELID::Menu, 1.f);

			m_pPlayerBox2->Set_TextureIndex(2);
			m_bPlayer2 = true;
		}
	}
	else
	{
		if (Engine::Key_Down(PADKEYSTATE::DIC_A))
		{
			StopSound(CSoundMgr::CHANNELID::Menu);
			PlaySoundW(L"MenuChoice.wav", CSoundMgr::CHANNELID::Menu, 1.f);

			m_pPlayerBox2->Set_TextureIndex(0);
			m_bPlayer2 = false;
		}
	}

	if (Engine::Key_Down(DIK_RETURN) && m_bPlayer1 && m_bPlayer2)
	{	

		StopSound(CSoundMgr::CHANNELID::Menu);
		PlaySoundW(L"MenuSelect.wav", CSoundMgr::CHANNELID::Menu, 1.f);

		CHANGE_SCENE(m_pGraphicDev, SCENE_MENUPLAYERSELECT, OBJ_NOEVENT);

		return SCENE_CHANGE;
	}
	
	if (m_bPlayer1 && m_bPlayer2)
	{
		m_pContinueBox->Set_Visible(true);
		m_pContinueBtn->Set_Visible(true);
	}
	else
	{
		m_pContinueBox->Set_Visible(false);
		m_pContinueBtn->Set_Visible(false);
	}


	return iExit;
}

_int CMenu::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CMenu::Render_Scene(void)
{
}

HRESULT CMenu::Ready_LoadingUI_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;
	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_MENUSELECT;

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pMenuBG = pGameObject->Is<CImageUI>();
	m_pMenuBG->Set_Image(L"MenuBG");
	m_pMenuBG->Set_Size(_vec3{ 1920.f, 1080.f, 0.f });
	m_pMenuBG->Set_Pos(_vec3{ 0.f, 0.f, 0.f });
	m_pMenuBG->Set_RenderID(RENDER_UI);
	m_pMenuBG->Set_TextureIndex(1);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MenuBG", pGameObject), E_FAIL);
	m_pMenuBG->AddRef();

	//가운데 챕터 사진
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pPicture = pGameObject->Is<CImageUI>();
	m_pPicture->Set_Image(L"Shed_Intro");
	m_pPicture->Set_Size(_vec3{ 768.f, 384.f ,0.f });
	m_pPicture->Set_Pos(_vec3{ 576.f, -270.f ,0.f });
	m_pPicture->Set_RenderID(RENDER_PRIUI);
	m_pPicture->Set_PassIndex(1);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Shed_Intro", pGameObject), E_FAIL);
	m_pPicture->AddRef();

	//가운데 챕터 사진 뒷배경
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pPictureBack = pGameObject->Is<CImageUI>();
	m_pPictureBack->Set_Image(L"PictureBack");
	m_pPictureBack->Set_Size(_vec3{ 580.f, 485.f ,0.f });
	m_pPictureBack->Set_Pos(_vec3{ 670.f, -200.f ,0.f });
	m_pPictureBack->Set_RenderID(RENDER_MENUUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PictureBack", pGameObject), E_FAIL);
	m_pPictureBack->AddRef();

	//가운데 챕터 제목
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pChapterTitle = pGameObject->Is<CImageUI>();
	m_pChapterTitle->Set_Image(L"CG");
	m_pChapterTitle->Set_Size(_vec3{ 62.f, 35.f ,0.f });
	m_pChapterTitle->Set_Pos(_vec3{ 929.f, -220.f ,0.f });
	m_pChapterTitle->Set_RenderID(RENDER_FINALUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CG", pGameObject), E_FAIL);
	m_pChapterTitle->AddRef();

	//가운데 챕터 사진 제목
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pPictureTitle = pGameObject->Is<CImageUI>();
	m_pPictureTitle->Set_Image(L"SOA");
	m_pPictureTitle->Set_Size(_vec3{ 480.f, 36.f ,0.f });
	m_pPictureTitle->Set_Pos(_vec3{ 720.f, -290.f ,0.f });
	m_pPictureTitle->Set_RenderID(RENDER_FINALUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SOA", pGameObject), E_FAIL);
	m_pPictureTitle->AddRef();


	//상단 로컬 플레이 
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pLocalPlay = pGameObject->Is<CImageUI>();
	m_pLocalPlay->Set_Image(L"HeaderBox");
	m_pLocalPlay->Set_Size(_vec3{ 350.f, 100.f ,0.f });
	m_pLocalPlay->Set_Pos(_vec3{ 405.f, -55.f ,0.f });
	m_pLocalPlay->Set_RoT(ROT_Z, D3DXToRadian(4.f));
	m_pLocalPlay->Set_RenderID(RENDER_MENUUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LocalHeaderBox", pGameObject), E_FAIL);
	m_pLocalPlay->AddRef();

	//상단 로컬 플레이 텍스트
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pLocalPlayText = pGameObject->Is<CImageUI>();
	m_pLocalPlayText->Set_Image(L"LocalMenu");
	m_pLocalPlayText->Set_Size(_vec3{ 197.f, 40.f ,0.f });
	m_pLocalPlayText->Set_Pos(_vec3{ 484.f, -85.f ,0.f });
	m_pLocalPlayText->Set_RenderID(RENDER_PRIUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LocalHeaderText", pGameObject), E_FAIL);
	m_pLocalPlayText->AddRef();


	//새계임, 계속 챕터 선택, 
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pMenuBar = pGameObject->Is<CImageUI>();
	m_pMenuBar->Set_Image(L"MenuRowBox2");
	m_pMenuBar->Set_Size(_vec3{ 1180.f, 60.f ,0.f });
	m_pMenuBar->Set_Pos(_vec3{ 370.f, -130.f ,0.f });
	m_pMenuBar->Set_RenderID(RENDER_PRIUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MenuBar", pGameObject), E_FAIL);
	m_pMenuBar->AddRef();

	// Menu Box
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pMenuBox = pGameObject->Is<CImageUI>();
	m_pMenuBox->Set_Image(L"HeaderBox_Wide2");
	m_pMenuBox->Set_Size(_vec3{ 280.f, 70.f ,0.f });
	m_pMenuBox->Set_Pos(_vec3{ 420.f, -125.f ,0.f });
	m_pMenuBox->Set_RoT(ROT_Z, D3DXToRadian(-3.f));
	m_pMenuBox->Set_RenderID(RENDER_FINALUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MenuBox", pGameObject), E_FAIL);
	m_pMenuBox->AddRef();

	//새계임
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pNewGame = pGameObject->Is<CImageUI>();
	m_pNewGame->Set_Image(L"NewGame");
	m_pNewGame->Set_Size(_vec3{ 77.f, 26.f ,0.f });
	m_pNewGame->Set_Pos(_vec3{ 518.f, -147.f ,0.f });
	m_pNewGame->Set_RenderID(RENDER_FINALUI);
	m_pNewGame->Set_TextureIndex(1);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NewGameText", pGameObject), E_FAIL);
	m_pNewGame->AddRef();

	//계속
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pContinue = pGameObject->Is<CImageUI>();
	m_pContinue->Set_Image(L"Continue");
	m_pContinue->Set_Size(_vec3{ 48.f, 26.f ,0.f });
	m_pContinue->Set_Pos(_vec3{ 800.f, -147.f ,0.f });
	m_pContinue->Set_RenderID(RENDER_FINALUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ContinueText", pGameObject), E_FAIL);
	m_pContinue->AddRef();

	//챕터선택
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pChapter = pGameObject->Is<CImageUI>();
	m_pChapter->Set_Image(L"Chapter");
	m_pChapter->Set_Size(_vec3{ 102.f, 26.f ,0.f });
	m_pChapter->Set_Pos(_vec3{ 1040.f, -147.f ,0.f });
	m_pChapter->Set_RenderID(RENDER_FINALUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChapterText", pGameObject), E_FAIL);
	m_pChapter->AddRef();

	//미니게임
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pMinigame = pGameObject->Is<CImageUI>();
	m_pMinigame->Set_Image(L"Minigame");
	m_pMinigame->Set_Size(_vec3{ 92.f, 26.f ,0.f });
	m_pMinigame->Set_Pos(_vec3{ 1310.f, -147.f ,0.f });
	m_pMinigame->Set_RenderID(RENDER_FINALUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MinigameText", pGameObject), E_FAIL);
	m_pMinigame->AddRef();

	

	//Player1
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pPlayer1 = pGameObject->Is<CImageUI>();
	m_pPlayer1->Set_Image(L"PlayerTexture");
	m_pPlayer1->Set_Size(_vec3{ 104.f, 27.f ,0.f });
	m_pPlayer1->Set_Pos(_vec3{ 735.f, -700.f ,0.f });
	m_pPlayer1->Set_RenderID(RENDER_PRIUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlayerTexture1", pGameObject), E_FAIL);
	m_pPlayer1->AddRef();

	//Player2
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pPlayer2 = pGameObject->Is<CImageUI>();
	m_pPlayer2->Set_Image(L"PlayerTexture");
	m_pPlayer2->Set_Size(_vec3{ 110.f, 27.f ,0.f });
	m_pPlayer2->Set_Pos(_vec3{ 1080.f, -700.f ,0.f });
	m_pPlayer2->Set_RenderID(RENDER_PRIUI);
	m_pPlayer2->Set_TextureIndex(1);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlayerTexture2", pGameObject), E_FAIL);
	m_pPlayer2->AddRef();


	//PlayerBox1
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pPlayerBox1 = pGameObject->Is<CImageUI>();
	m_pPlayerBox1->Set_Image(L"Join");
	m_pPlayerBox1->Set_Size(_vec3{ 241.f, 190.f ,0.f });
	m_pPlayerBox1->Set_Pos(_vec3{ 670.f, -740.f ,0.f });
	m_pPlayerBox1->Set_RenderID(RENDER_MENUUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Join1", pGameObject), E_FAIL);
	m_pPlayerBox1->AddRef();

	//PlayerBox1
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pPlayerBox2 = pGameObject->Is<CImageUI>();
	m_pPlayerBox2->Set_Image(L"Join");
	m_pPlayerBox2->Set_Size(_vec3{ 241.f, 190.f ,0.f });
	m_pPlayerBox2->Set_Pos(_vec3{ 1010.f, -740.f ,0.f });
	m_pPlayerBox2->Set_RenderID(RENDER_MENUUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Join2", pGameObject), E_FAIL);
	m_pPlayerBox2->AddRef();


	//Back Box
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pBackBox = pGameObject->Is<CImageUI>();
	m_pBackBox->Set_Image(L"HeaderBox_Wide2");
	m_pBackBox->Set_Size(_vec3{ 240.f, 60.f ,0.f });
	m_pBackBox->Set_Pos(_vec3{ 160.f, -960.f ,0.f });
	m_pBackBox->Set_RenderID(RENDER_PRIUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackBox", pGameObject), E_FAIL);
	m_pBackBox->AddRef();

	//Back
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pBack = pGameObject->Is<CImageUI>();
	m_pBack->Set_Image(L"Back");
	m_pBack->Set_Size(_vec3{ 47.f, 27.f ,0.f });
	m_pBack->Set_Pos(_vec3{ 255.f, -975.f ,0.f });
	m_pBack->Set_RenderID(RENDER_PRIUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackText", pGameObject), E_FAIL);
	m_pBack->AddRef();


	// Continue Box
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pContinueBox = pGameObject->Is<CImageUI>();
	m_pContinueBox->Set_Image(L"HeaderBox_Wide2");
	m_pContinueBox->Set_Size(_vec3{ 240.f, 60.f ,0.f });
	m_pContinueBox->Set_Pos(_vec3{ 1260.f, -960.f ,0.f });
	m_pContinueBox->Set_RenderID(RENDER_PRIUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ContinueBox", pGameObject), E_FAIL);
	m_pContinueBox->AddRef();

	// ContinueBtn
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pContinueBtn = pGameObject->Is<CImageUI>();
	m_pContinueBtn->Set_Image(L"ContinueBtn");
	m_pContinueBtn->Set_Size(_vec3{ 47.f, 27.f ,0.f });
	m_pContinueBtn->Set_Pos(_vec3{ 1360.f, -975.f ,0.f });
	m_pContinueBtn->Set_RenderID(RENDER_PRIUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ContinueBtnText", pGameObject), E_FAIL);
	m_pContinueBtn->AddRef();


	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}


CMenu * CMenu::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMenu*		pInstance = new CMenu(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMenu::Free()
{
	if (m_pMenuBG)
		Safe_Release(m_pMenuBG);

	if (m_pPicture)
		Safe_Release(m_pPicture);

	if (m_pPictureBack)
		Safe_Release(m_pPictureBack);

	if (m_pPictureTitle)
		Safe_Release(m_pPictureTitle);

	if (m_pChapterTitle)
		Safe_Release(m_pChapterTitle);

	if (m_pLocalPlay)
		Safe_Release(m_pLocalPlay);

	if (m_pLocalPlayText)
		Safe_Release(m_pLocalPlayText);

	if (m_pMenuBox)
		Safe_Release(m_pMenuBox);

	if (m_pMenuBar)
		Safe_Release(m_pMenuBar);

	if (m_pNewGame)
		Safe_Release(m_pNewGame);

	if (m_pContinue)
		Safe_Release(m_pContinue);

	if (m_pChapter)
		Safe_Release(m_pChapter);

	if (m_pMinigame)
		Safe_Release(m_pMinigame);

	if (m_pPlayer1)
		Safe_Release(m_pPlayer1);

	if (m_pPlayer2)
		Safe_Release(m_pPlayer2);

	if (m_pPlayerBox1)
		Safe_Release(m_pPlayerBox1);

	if (m_pPlayerBox2)
		Safe_Release(m_pPlayerBox2);

	if (m_pBackBox)
		Safe_Release(m_pBackBox);

	if (m_pBack)
		Safe_Release(m_pBack);

	if (m_pContinueBox)
		Safe_Release(m_pContinueBox);

	if (m_pContinueBtn)
		Safe_Release(m_pContinueBtn);

	Engine::CScene::Free();
}
