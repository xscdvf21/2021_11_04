#include "stdafx.h"
#include "MenuSelect.h"
#include "Menu.h"
#include "Logo.h"
#include "Export_Function.h"
#include "ImageUI.h"

CMenuSelect::CMenuSelect(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CScene(pGraphicDevice)
	, m_iSelectMenu(0)

{
}

CMenuSelect::~CMenuSelect()
{
}

HRESULT CMenuSelect::Ready_Scene(void)
{
	Engine::Set_SoundScene(RESOURCE_MENUSELECT);

	FAILED_CHECK_RETURN(Ready_LoadingUI_Layer(L"UI_Resource"), E_FAIL);

	StopAll();
	PlayBGM(L"MenuBGM.wav", 0.8f);

	return S_OK;
}

_int CMenuSelect::Update_Scene(const _float & fTimeDelta)
{
	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);
	m_fCurTime += fTimeDelta;

	if (m_fCurTime > m_fInterval)
	{

		if (Engine::Key_Down(DIK_DOWN))
		{
			StopSound(CSoundMgr::CHANNELID::Menu);
			PlaySoundW(L"MenuChoice.wav", CSoundMgr::CHANNELID::Menu, 1.f);

			m_iSelectMenu++;
			m_fCurTime = 0.f;

			if (m_iSelectMenu > 5)
			{
				m_iSelectMenu = 5;
				m_pUI->Set_Pos(_vec3{ 130.f, -866.f, 0.f });
				return 0;
			}
			else if (m_iSelectMenu < 0)
			{
				m_iSelectMenu = 0;
				m_pUI->Set_Pos(_vec3{ 130.f, -517.f, 0.f });

				return 0;
			}
			m_pUI->Set_AddPos(_vec3{ 0.f, -70.f, 0.f });

			m_fLocal = 0.f;
			m_fOnline = 0.f;
			m_fOption = 0.f;
			m_fOption2 = 0.f;
			m_fMade = 0.f;
			m_fQuit = 0.f;

			m_bDir = false;
		}
		else if (Engine::Key_Down(DIK_UP))
		{
			StopSound(CSoundMgr::CHANNELID::Menu);
			PlaySoundW(L"MenuChoice.wav", CSoundMgr::CHANNELID::Menu, 1.f);

			m_fCurTime = 0.f;
			m_iSelectMenu--;

			if (m_iSelectMenu > 5)
			{
				m_iSelectMenu = 5;
				m_pUI->Set_Pos(_vec3{ 130.f, -866.f, 0.f });
				return 0;
			}
			else if (m_iSelectMenu < 0)
			{
				m_iSelectMenu = 0;
				m_pUI->Set_Pos(_vec3{ 130.f, -517.f, 0.f });

				return 0;
			}
			m_pUI->Set_AddPos(_vec3{ 0.f, 70.f, 0.f });

			m_fLocal = 0.f;
			m_fOnline = 0.f;
			m_fOption = 0.f;
			m_fOption2 = 0.f;
			m_fMade = 0.f;
			m_fQuit = 0.f;
			m_bDir = true;
		}
	}



	if (Engine::Key_Down(DIK_RETURN))
	{
		StopSound(CSoundMgr::CHANNELID::Menu);
		PlaySoundW(L"MenuSelect.wav", CSoundMgr::CHANNELID::Menu, 1.f);

		Engine::CScene*		pScene = nullptr;

		//각옵션
		switch (m_iSelectMenu)
		{
		case 0:
		{

			CHANGE_SCENE(m_pGraphicDev, SCENE_CHAPTERSELECT, OBJ_NOEVENT);
		}
		break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		}
	}

	return iExit;
}

_int CMenuSelect::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(fTimeDelta);



	switch (m_iSelectMenu)
	{
	case 0:
		m_pLocal->Set_TextureIndex(1);
		m_pOnline->Set_TextureIndex(0);
		m_pOption->Set_TextureIndex(0);
		m_pOption2->Set_TextureIndex(0);
		m_pMade->Set_TextureIndex(0);
		m_pQuit->Set_TextureIndex(0);

		if (!m_bDir)
		{
			if (m_fLocal < 10.f)
			{
				m_fLocal += 2.f;
				m_pLocal->Set_AddPos(_vec3(m_fLocal, 0.f, 0.f));
			}
		}
		else
		{
			if (m_fLocal < 10.f)
			{
				m_fLocal += 2.f;
				m_pLocal->Set_AddPos(_vec3(m_fLocal, 0.f, 0.f));
			}
			if (m_fOnline > -10.f)
			{
				m_fOnline -= 2.f;
				m_pOnline->Set_AddPos(_vec3(m_fOnline, 0.f, 0.f));
			}
		}
		break;
	case 1:
		m_pLocal->Set_TextureIndex(0);
		m_pOnline->Set_TextureIndex(1);
		m_pOption->Set_TextureIndex(0);
		m_pOption2->Set_TextureIndex(0);
		m_pMade->Set_TextureIndex(0);
		m_pQuit->Set_TextureIndex(0);

		if (!m_bDir)
		{
			if (m_fLocal > -10.f)
			{
				m_fLocal -= 2.f;
				m_pLocal->Set_AddPos(_vec3(m_fLocal, 0.f, 0.f));
			}

			if (m_fOnline < 10.f)
			{
				m_fOnline += 2.f;
				m_pOnline->Set_AddPos(_vec3(m_fOnline, 0.f, 0.f));
			}
		}
		else
		{
			if (m_fOnline < 10.f)
			{
				m_fOnline += 2.f;
				m_pOnline->Set_AddPos(_vec3(m_fOnline, 0.f, 0.f));
			}

			if (m_fOption > -10.f)
			{
				m_fOption -= 2.f;
				m_pOption->Set_AddPos(_vec3(m_fOption, 0.f, 0.f));
			}
		}

		break;
	case 2:
		m_pLocal->Set_TextureIndex(0);
		m_pOnline->Set_TextureIndex(0);
		m_pOption->Set_TextureIndex(1);
		m_pOption2->Set_TextureIndex(0);
		m_pMade->Set_TextureIndex(0);
		m_pQuit->Set_TextureIndex(0);

		if (!m_bDir)
		{
			if (m_fOnline > -10.f)
			{
				m_fOnline -= 2.f;
				m_pOnline->Set_AddPos(_vec3(m_fOnline, 0.f, 0.f));
			}

			if (m_fOption < 10.f)
			{
				m_fOption += 2.f;
				m_pOption->Set_AddPos(_vec3(m_fOption, 0.f, 0.f));
			}
		}
		else
		{
			if (m_fOption < 10.f)
			{
				m_fOption += 2.f;
				m_pOption->Set_AddPos(_vec3(m_fOption, 0.f, 0.f));
			}

			if (m_fOption2 > -10.f)
			{
				m_fOption2 -= 2.f;
				m_pOption2->Set_AddPos(_vec3(m_fOption2, 0.f, 0.f));
			}
		}
		break;
	case 3:
		m_pLocal->Set_TextureIndex(0);
		m_pOnline->Set_TextureIndex(0);
		m_pOption->Set_TextureIndex(0);
		m_pOption2->Set_TextureIndex(1);
		m_pMade->Set_TextureIndex(0);
		m_pQuit->Set_TextureIndex(0);

		if (!m_bDir)
		{
			if (m_fOption > -10.f)
			{
				m_fOption -= 2.f;
				m_pOption->Set_AddPos(_vec3(m_fOption, 0.f, 0.f));
			}

			if (m_fOption2 < 10.f)
			{
				m_fOption2 += 2.f;
				m_pOption2->Set_AddPos(_vec3(m_fOption2, 0.f, 0.f));
			}
		}
		else
		{
			if (m_fOption2 < 10.f)
			{
				m_fOption2 += 2.f;
				m_pOption2->Set_AddPos(_vec3(m_fOption2, 0.f, 0.f));
			}
			if (m_fMade > -10.f)
			{
				m_fMade -= 2.f;
				m_pMade->Set_AddPos(_vec3(m_fMade, 0.f, 0.f));
			}
		}


		break;
	case 4:
		m_pLocal->Set_TextureIndex(0);
		m_pOnline->Set_TextureIndex(0);
		m_pOption->Set_TextureIndex(0);
		m_pOption2->Set_TextureIndex(0);
		m_pMade->Set_TextureIndex(1);
		m_pQuit->Set_TextureIndex(0);

		if (!m_bDir)
		{
			if (m_fOption2 > -10.f)
			{
				m_fOption2 -= 2.f;
				m_pOption2->Set_AddPos(_vec3(m_fOption2, 0.f, 0.f));
			}

			if (m_fMade < 10.f)
			{
				m_fMade += 2.f;
				m_pMade->Set_AddPos(_vec3(m_fMade, 0.f, 0.f));
			}
		}
		else
		{
			if (m_fMade < 10.f)
			{
				m_fMade += 2.f;
				m_pMade->Set_AddPos(_vec3(m_fMade, 0.f, 0.f));
			}
			if (m_fQuit > -10.f)
			{
				m_fQuit -= 2.f;
				m_pQuit->Set_AddPos(_vec3(m_fQuit, 0.f, 0.f));
			}
		}

		break;
	case 5:
		m_pLocal->Set_TextureIndex(0);
		m_pOnline->Set_TextureIndex(0);
		m_pOption->Set_TextureIndex(0);
		m_pOption2->Set_TextureIndex(0);
		m_pMade->Set_TextureIndex(0);
		m_pQuit->Set_TextureIndex(1);

		if (!m_bDir)
		{
			if (m_fMade > -10.f)
			{
				m_fMade -= 2.f;
				m_pMade->Set_AddPos(_vec3(m_fMade, 0.f, 0.f));
			}

			if (m_fQuit < 10.f)
			{
				m_fQuit += 2.f;
				m_pQuit->Set_AddPos(_vec3(m_fQuit, 0.f, 0.f));
			}
		}
		else
		{
			if (m_fQuit < 10.f)
			{
				m_fQuit += 2.f;
				m_pQuit->Set_AddPos(_vec3(m_fQuit, 0.f, 0.f));
			}
		}

		break;
	}

	return iExit;
}

void CMenuSelect::Render_Scene(void)
{
}

HRESULT CMenuSelect::Ready_LoadingUI_Layer(const _tchar * pLayerTag)
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
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MenuBG", pGameObject), E_FAIL);
	m_pMenuBG->AddRef();

	// MenuBar
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pUI = pGameObject->Is<CImageUI>();
	m_pUI->Set_Image(L"MenuRowBox");
	m_pUI->Set_Size(_vec3{ 600.f, 75.f ,0.f });
	m_pUI->Set_Pos(_vec3{ 130.f, -517.f, 0.f });
	m_pUI->Set_RenderID(RENDER_UI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MenuRowBox", pGameObject), E_FAIL);
	m_pUI->AddRef();

	// Local
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pLocal = pGameObject->Is<CImageUI>();
	m_pLocal->Set_Image(L"Local");
	m_pLocal->Set_Size(_vec3{ 134.f, 28.f ,0.f });
	m_pLocal->Set_Pos(_vec3{ 190.f, -540.f, 0.f });
	m_pLocal->Set_RenderID(RENDER_MENUUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Local", pGameObject), E_FAIL);
	m_pLocal->AddRef();

	// Online
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pOnline = pGameObject->Is<CImageUI>();
	m_pOnline->Set_Image(L"Online");
	m_pOnline->Set_Size(_vec3{ 159.f, 28.f ,0.f });
	m_pOnline->Set_Pos(_vec3{ 190.f, -610.f, 0.f });
	m_pOnline->Set_RenderID(RENDER_MENUUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Online", pGameObject), E_FAIL);
	m_pOnline->AddRef();

	// Option
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pOption = pGameObject->Is<CImageUI>();
	m_pOption->Set_Image(L"Option");
	m_pOption->Set_Size(_vec3{ 49.f, 28.f ,0.f });
	m_pOption->Set_Pos(_vec3{ 190.f, -680.f, 0.f });
	m_pOption->Set_RenderID(RENDER_MENUUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Option", pGameObject), E_FAIL);
	m_pOption->AddRef();

	// Option2
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pOption2 = pGameObject->Is<CImageUI>();
	m_pOption2->Set_Image(L"Option2");
	m_pOption2->Set_Size(_vec3{ 134.f, 28.f ,0.f });
	m_pOption2->Set_Pos(_vec3{ 190.f, -750.f, 0.f });
	m_pOption2->Set_RenderID(RENDER_MENUUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Option2", pGameObject), E_FAIL);
	m_pOption2->AddRef();

	// Made
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pMade = pGameObject->Is<CImageUI>();
	m_pMade->Set_Image(L"Made");
	m_pMade->Set_Size(_vec3{ 76.f, 28.f ,0.f });
	m_pMade->Set_Pos(_vec3{ 190.f, -820, 0.f });
	m_pMade->Set_RenderID(RENDER_MENUUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Made", pGameObject), E_FAIL);
	m_pMade->AddRef();

	// MenuBar
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pQuit = pGameObject->Is<CImageUI>();
	m_pQuit->Set_Image(L"Quit");
	m_pQuit->Set_Size(_vec3{ 51.f, 28.f ,0.f });
	m_pQuit->Set_Pos(_vec3{ 190.f, -890.f, 0.f });
	m_pQuit->Set_RenderID(RENDER_MENUUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Quit", pGameObject), E_FAIL);
	m_pQuit->AddRef();


	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

CMenuSelect * CMenuSelect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMenuSelect*		pInstance = new CMenuSelect(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMenuSelect::Free()
{
	if (m_pUI)
		Safe_Release(m_pUI);

	if (m_pMenuBG)
		Safe_Release(m_pMenuBG);

	if (m_pLocal)
		Safe_Release(m_pLocal);

	if (m_pOnline)
		Safe_Release(m_pOnline);

	if (m_pOption)
		Safe_Release(m_pOption);

	if (m_pOption2)
		Safe_Release(m_pOption2);

	if (m_pMade)
		Safe_Release(m_pMade);

	if (m_pQuit)
		Safe_Release(m_pQuit);

	Engine::CScene::Free();
}
