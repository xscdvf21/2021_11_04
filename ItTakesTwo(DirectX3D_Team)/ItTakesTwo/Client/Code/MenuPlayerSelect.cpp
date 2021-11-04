#include "stdafx.h"
#include "MenuPlayerSelect.h"
#include "Logo.h"
#include "Export_Function.h"
#include "ImageUI.h"
#include "MainCamera.h"
#include "RoseSelect.h"
#include "CodyDoll.h"
#include "MayDoll.h"
#include "RoseSelectRight.h"

CMenuPlayerSelect::CMenuPlayerSelect(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CScene(pGraphicDevice)
	, m_bPlayer1_Ready(false)
	, m_bPlayer2_Ready(false)
	, m_vLeftBoxPos(-400.f, -300.f, 0.f)
	, m_vRightBoxPos(400.f, -300.f, -0.f)
	, m_vCenterBoxPos(0.f, -100.f, 0.f)
	, m_vCenterBoxPos2(0.f, -300.f, 0.f)
	, m_bLeftMove(false)
	, m_bRightMove(false)
	, m_bLeftMoveXBOX(false)
	, m_bRightMoveXBOX(false)
	, m_iKeyboardSelect(0)
	, m_vMoveSize(194.f, 234.f, 0.f)
	, m_vCenterSize(192.f, 112.f, 0.f)
	, m_vLeftKeyboardPos(561.f, -693.f, 0.f)
	, m_vRightKeyBoardPos(1163.f, -693.f, 0.f)
	, m_vCenterKeyBoardPos(864.f, -633.f, 0.f)
	, m_vLeftXBoxPos(561.f, -693.f, 0.f)
	, m_vRightXBoxPos(1163.f, -693.f, 0.f)
	, m_vCenterXBoxPos(864.f, -753.f, 0.f)
	//, m_vLeftBoxPos2(-400.f, -300.f, 0.f)
	//, m_vRightBoxPos2(400.f, -300.f, 0.f)
	, m_iXBoxSelect(0)
	, m_bMoveXBOXEnd(false)
{
}

CMenuPlayerSelect::~CMenuPlayerSelect()
{
}

HRESULT CMenuPlayerSelect::Ready_Scene(void)
{
	Engine::Set_SoundScene(RESOURCE_MENUSELECT);

	Engine::End_Split();

	//g_vBackBufferColor = {255.f ,255.f, 255.f};
	FAILED_CHECK_RETURN(Ready_LoadingUI_Layer(L"UI_Resource"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameObject_Layer(L"GameObject"), E_FAIL);
	Ready_LightInfo();

	return S_OK;
}

_int CMenuPlayerSelect::Update_Scene(const _float & fTimeDelta)
{
	m_fCurTime += fTimeDelta;

	if (Key_Down(DIK_LEFTARROW) && !m_bPlayer1_Ready)
	{
		StopSound(CSoundMgr::CHANNELID::Menu);
		PlaySoundW(L"CodySelect.wav", CSoundMgr::CHANNELID::Menu, 1.f);



		m_iKeyboardSelect--;
		if (m_iKeyboardSelect <= -1)
		{
			if (m_iXBoxSelect == -1)
				m_iKeyboardSelect = 0;
			else
				m_iKeyboardSelect = -1;
		}

	}
	if (Key_Down(DIK_RIGHTARROW) && !m_bPlayer1_Ready)
	{
		StopSound(CSoundMgr::CHANNELID::Menu);
		PlaySoundW(L"CodySelect.wav", CSoundMgr::CHANNELID::Menu, 1.f);

		m_iKeyboardSelect++;
		if (m_iKeyboardSelect >= 1)
		{
			if (m_iXBoxSelect == 1)
				m_iKeyboardSelect = 0;
			else
				m_iKeyboardSelect = 1;
		}
	}

	_float fDIC_X = 0;
	if (m_fCurTime > m_fInterval)
	{
		fDIC_X = Engine::Get_DIPadMove(PADMOVESTATE::DIC_LS_X);
		if (fabs(fDIC_X) <= 0.8f)
		{
		}
		else
		{
			if (fDIC_X > 0 && !m_bPlayer2_Ready)
			{

				StopSound(CSoundMgr::CHANNELID::Menu);
				PlaySoundW(L"MaySelect.wav", CSoundMgr::CHANNELID::Menu, 1.f);

				m_iXBoxSelect++;
				m_fCurTime = 0.f;
				if (m_iXBoxSelect >= 1)
				{
					if (m_iKeyboardSelect == 1)
						m_iXBoxSelect = 0;
					else
						m_iXBoxSelect = 1;
				}
			}
			else if (fDIC_X < 0 && !m_bPlayer2_Ready) //왼쪽
			{

				StopSound(CSoundMgr::CHANNELID::Menu);
				PlaySoundW(L"MaySelect.wav", CSoundMgr::CHANNELID::Menu, 1.f);

				m_iXBoxSelect--;
				m_fCurTime = 0.f;
				if (m_iXBoxSelect <= -1)
				{
					if (m_iKeyboardSelect == -1)
						m_iXBoxSelect = 0;
					else
						m_iXBoxSelect = -1;
				}
			}
		}
	}
	
	//플레이어1 ( 키보드 선택)
	if (m_iKeyboardSelect == -1)
	{
		if (m_bLeftMove)
		{
			_bool	bMoveEnd = m_pUI[UI_PLAYER1_BOX]->Is_MoveEnd();
	
			if (!m_bPlayer1_Ready)
				m_pUI[UI_PLAYER1_BOX]->Set_TextureIndex(3);
			else if (bMoveEnd && m_bPlayer1_Ready)
				m_pUI[UI_PLAYER1_BOX]->Set_TextureIndex(4);

			if (bMoveEnd && Key_Down(DIK_RETURN))
			{


				StopSound(CSoundMgr::CHANNELID::Menu);
				PlaySoundW(L"MenuChoice.wav", CSoundMgr::CHANNELID::Menu, 1.f);


				m_bPlayer1_Ready = !m_bPlayer1_Ready;
			}
		}
		if (!m_bLeftMove)
		{
			m_pUI[UI_PLAYER1_BOX]->Change_Size_From_To(0.15f, m_vCenterSize, m_vMoveSize);
			m_pUI[UI_PLAYER1_BOX]->Change_Pos_From_To(0.15f, m_pUI[UI_PLAYER1_BOX]->Get_Pos(), m_vLeftKeyboardPos);

			m_bLeftMove = true;
		}

	}
	else if (m_iKeyboardSelect == 0)
	{
		m_pUI[UI_PLAYER1_BOX]->Set_TextureIndex(0);

		if (m_bLeftMove)
		{
			m_pUI[UI_PLAYER1_BOX]->Change_Size_From_To(0.15f, m_vMoveSize, m_vCenterSize);
			m_pUI[UI_PLAYER1_BOX]->Change_Pos_From_To(0.15f, m_pUI[UI_PLAYER1_BOX]->Get_Pos(), m_vCenterKeyBoardPos);
			m_bLeftMove = false;
		}
		else if (m_bRightMove)
		{
			m_pUI[UI_PLAYER1_BOX]->Change_Size_From_To(0.15f, m_vMoveSize, m_vCenterSize);
			m_pUI[UI_PLAYER1_BOX]->Change_Pos_From_To(0.15f, m_pUI[UI_PLAYER1_BOX]->Get_Pos(), m_vCenterKeyBoardPos);
			m_bRightMove = false;
		}
	}
	else if (m_iKeyboardSelect == 1)
	{
		if (m_bRightMove)
		{
			_bool	bMoveEnd = m_pUI[UI_PLAYER1_BOX]->Is_MoveEnd();

			if (!m_bPlayer1_Ready)
				m_pUI[UI_PLAYER1_BOX]->Set_TextureIndex(1);
			else if (bMoveEnd && m_bPlayer1_Ready)
				m_pUI[UI_PLAYER1_BOX]->Set_TextureIndex(2);

			if (bMoveEnd && Key_Down(DIK_RETURN))
			{
				StopSound(CSoundMgr::CHANNELID::Menu);
				PlaySoundW(L"MenuChoice.wav", CSoundMgr::CHANNELID::Menu, 1.f);

				m_bPlayer1_Ready = !m_bPlayer1_Ready;
			}
		}
		if (!m_bRightMove)
		{
			m_pUI[UI_PLAYER1_BOX]->Change_Size_From_To(0.15f, m_vCenterSize, m_vMoveSize);
			m_pUI[UI_PLAYER1_BOX]->Change_Pos_From_To(0.15f, m_pUI[UI_PLAYER1_BOX]->Get_Pos(), m_vRightKeyBoardPos);
			m_bRightMove = true;
		}
	}

	if (m_iXBoxSelect == -1)
	{
		if (m_bLeftMoveXBOX)
		{
			_bool	bMoveEnd = m_pUI[UI_PLAYER2_BOX]->Is_MoveEnd();

			if (!m_bPlayer2_Ready)
				m_pUI[UI_PLAYER2_BOX]->Set_TextureIndex(3);
			else if (bMoveEnd && m_bPlayer2_Ready)
				m_pUI[UI_PLAYER2_BOX]->Set_TextureIndex(4);

			if (bMoveEnd && Key_Down(DIC_A))
			{
				StopSound(CSoundMgr::CHANNELID::Menu);
				PlaySoundW(L"MenuChoice.wav", CSoundMgr::CHANNELID::Menu, 1.f);

				m_bPlayer2_Ready = !m_bPlayer2_Ready;
			}
		}
		if (!m_bLeftMoveXBOX)
		{
			m_pUI[UI_PLAYER2_BOX]->Change_Size_From_To(0.15f, m_vCenterSize, m_vMoveSize);
			m_pUI[UI_PLAYER2_BOX]->Change_Pos_From_To(0.15f, m_pUI[UI_PLAYER2_BOX]->Get_Pos(), m_vLeftXBoxPos);

			m_bLeftMoveXBOX = true;
		}

	}
	else if (m_iXBoxSelect == 0)
	{
		m_pUI[UI_PLAYER2_BOX]->Set_TextureIndex(0);

		if (m_bLeftMoveXBOX)
		{
			m_pUI[UI_PLAYER2_BOX]->Change_Size_From_To(0.15f, m_vMoveSize, m_vCenterSize);
			m_pUI[UI_PLAYER2_BOX]->Change_Pos_From_To(0.15f, m_pUI[UI_PLAYER2_BOX]->Get_Pos(), m_vCenterXBoxPos);
			m_bLeftMoveXBOX = false;
		}
		else if (m_bRightMoveXBOX)
		{
			m_pUI[UI_PLAYER2_BOX]->Change_Size_From_To(0.15f, m_vMoveSize, m_vCenterSize);
			m_pUI[UI_PLAYER2_BOX]->Change_Pos_From_To(0.15f, m_pUI[UI_PLAYER2_BOX]->Get_Pos(), m_vCenterXBoxPos);
			m_bRightMoveXBOX = false;
		}
	}
	else if (m_iXBoxSelect == 1)
	{
		if (m_bRightMoveXBOX)
		{
			_bool	bMoveEnd = m_pUI[UI_PLAYER2_BOX]->Is_MoveEnd();

			if (!m_bPlayer2_Ready)
				m_pUI[UI_PLAYER2_BOX]->Set_TextureIndex(1);
			else if (bMoveEnd && m_bPlayer2_Ready)
				m_pUI[UI_PLAYER2_BOX]->Set_TextureIndex(2);

			if (bMoveEnd && Key_Down(DIC_A))
			{
				StopSound(CSoundMgr::CHANNELID::Menu);
				PlaySoundW(L"MenuChoice.wav", CSoundMgr::CHANNELID::Menu, 1.f);

				m_bPlayer2_Ready = !m_bPlayer2_Ready;
			}
		}
		if (!m_bRightMoveXBOX)
		{
			m_pUI[UI_PLAYER2_BOX]->Change_Size_From_To(0.15f, m_vCenterSize, m_vMoveSize);
			m_pUI[UI_PLAYER2_BOX]->Change_Pos_From_To(0.15f, m_pUI[UI_PLAYER2_BOX]->Get_Pos(), m_vRightXBoxPos);
			m_bRightMoveXBOX = true;
		}
	}

	if (m_iKeyboardSelect == -1 && m_iXBoxSelect == -1)
	{
		static_cast<CRoseSelect*>(m_pLeftHand)->Set_Animation(2);
		static_cast<CRoseSelectRight*>(m_pRightHand)->Set_Animation(1);
	}

	if (m_iKeyboardSelect == -1 && m_iXBoxSelect == 1)
	{
		static_cast<CRoseSelect*>(m_pLeftHand)->Set_Animation(2);
		static_cast<CRoseSelectRight*>(m_pRightHand)->Set_Animation(2);
	}
	
	if (m_iKeyboardSelect == -1 && m_iXBoxSelect == 0)
	{
		static_cast<CRoseSelect*>(m_pLeftHand)->Set_Animation(2);
		static_cast<CRoseSelectRight*>(m_pRightHand)->Set_Animation(1);
	}
	
	if (m_iKeyboardSelect == 0 && m_iXBoxSelect == -1)
	{
		static_cast<CRoseSelect*>(m_pLeftHand)->Set_Animation(2);
		static_cast<CRoseSelectRight*>(m_pRightHand)->Set_Animation(1);
	}
	
	if (m_iKeyboardSelect == 0 && m_iXBoxSelect == 0)
	{
		static_cast<CRoseSelect*>(m_pLeftHand)->Set_Animation(1);
		static_cast<CRoseSelectRight*>(m_pRightHand)->Set_Animation(1);
	}
	
	if (m_iKeyboardSelect == 0 && m_iXBoxSelect == 1)
	{
		static_cast<CRoseSelect*>(m_pLeftHand)->Set_Animation(1);
		static_cast<CRoseSelectRight*>(m_pRightHand)->Set_Animation(2);
	}
	
	if (m_iKeyboardSelect == 1 && m_iXBoxSelect == -1)
	{
		static_cast<CRoseSelect*>(m_pLeftHand)->Set_Animation(2);
		static_cast<CRoseSelectRight*>(m_pRightHand)->Set_Animation(2);
	}
	
	if (m_iKeyboardSelect == 1 && m_iXBoxSelect == 0)
	{
		static_cast<CRoseSelect*>(m_pLeftHand)->Set_Animation(1);
		static_cast<CRoseSelectRight*>(m_pRightHand)->Set_Animation(2);
	}
	
	if (m_iKeyboardSelect == 1 && m_iXBoxSelect == 1)
	{
		static_cast<CRoseSelect*>(m_pLeftHand)->Set_Animation(1);
		static_cast<CRoseSelectRight*>(m_pRightHand)->Set_Animation(2);
	}

	//씬전환용 다음씬 설정해줘야함. 둘다 레디가 끝나면.
	if (m_bPlayer1_Ready && m_bPlayer2_Ready)
	{
		StopSound(CSoundMgr::CHANNELID::Menu);
		PlaySoundW(L"MenuSelect.wav", CSoundMgr::CHANNELID::Menu, 1.f);

		CHANGE_SCENE(m_pGraphicDev, SCENE_VF,OBJ_NOEVENT);
		return SCENE_CHANGE;
	}

	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);

	return iExit;
}

_int CMenuPlayerSelect::LateUpdate_Scene(const _float & fTimeDelta)
{
	_int iExit = Engine::CScene::LateUpdate_Scene(fTimeDelta);

	return iExit;
}

void CMenuPlayerSelect::Render_Scene(void)
{
}

HRESULT CMenuPlayerSelect::Ready_LoadingUI_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	OBJINFO tObj;
	ZeroMemory(&tObj, sizeof(OBJINFO));
	tObj.eSceneID = RESOURCE_MENUSELECT;

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	//캐릭터 선택 폰트 쓸 헤더박스.
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pUI[UI_MENUHEADER] = pGameObject->Is<CImageUI>();
	m_pUI[UI_MENUHEADER]->Set_Image(L"HeaderBox_Wide");
	m_pUI[UI_MENUHEADER]->Set_Size(_vec3{ 600.f, 120.f, 0.f });
	m_pUI[UI_MENUHEADER]->Set_Pos(_vec3{ 660.f, -130.f, 0.f });
	m_pUI[UI_MENUHEADER]->Set_RoT(ROT_Z, D3DXToRadian(2.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PlayerSelectBox", pGameObject), E_FAIL);
	m_pUI[UI_MENUHEADER]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pUI[UI_TITLE] = pGameObject->Is<CImageUI>();
	m_pUI[UI_TITLE]->Set_Image(L"ChoiceChar");
	m_pUI[UI_TITLE]->Set_Size(_vec3{ 198.f, 40.f, 0.f });
	m_pUI[UI_TITLE]->Set_Pos(_vec3{ 860.f, -170.f, 0.f });
	m_pUI[UI_TITLE]->Set_RenderID(RENDER_MENUUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ChoiceChar", pGameObject), E_FAIL);
	m_pUI[UI_TITLE]->AddRef();

	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pUI[UI_CONTINUETITLE] = pGameObject->Is<CImageUI>();
	m_pUI[UI_CONTINUETITLE]->Set_Image(L"ContinueTitle");
	m_pUI[UI_CONTINUETITLE]->Set_Size(_vec3{ 75.f, 42.f, 0.f });
	m_pUI[UI_CONTINUETITLE]->Set_Pos(_vec3{ 920.f, -70.f, 0.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ContinueTitle", pGameObject), E_FAIL);
	m_pUI[UI_CONTINUETITLE]->AddRef();

	//플레이어1 박스.
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pUI[UI_PLAYER1_BOX] = pGameObject->Is<CImageUI>();
	m_pUI[UI_PLAYER1_BOX]->Set_Image(L"CodySelect");
	m_pUI[UI_PLAYER1_BOX]->Set_Size(_vec3{ 192.f, 112.f, 0.f });
	m_pUI[UI_PLAYER1_BOX]->Set_Pos(_vec3{ 864.f, -633.f, 0.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player1_Box", pGameObject), E_FAIL);
	m_pUI[UI_PLAYER1_BOX]->AddRef();

	//플레이어2 박스.
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pUI[UI_PLAYER2_BOX] = pGameObject->Is<CImageUI>();
	m_pUI[UI_PLAYER2_BOX]->Set_Image(L"MaySelect");
	m_pUI[UI_PLAYER2_BOX]->Set_Size(_vec3{ 192.f, 112.f, 0.f });
	m_pUI[UI_PLAYER2_BOX]->Set_Pos(_vec3{ 864.f, -753.f, 0.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player2_Box", pGameObject), E_FAIL);
	m_pUI[UI_PLAYER2_BOX]->AddRef();

	//Back Box
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pUI[UI_BACK_BOX] = pGameObject->Is<CImageUI>();
	m_pUI[UI_BACK_BOX]->Set_Image(L"HeaderBox_Wide2");
	m_pUI[UI_BACK_BOX]->Set_Size(_vec3{ 240.f, 60.f ,0.f });
	m_pUI[UI_BACK_BOX]->Set_Pos(_vec3{ 420.f, -960.f ,0.f });
	m_pUI[UI_BACK_BOX]->Set_RenderID(RENDER_PRIUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackBox", pGameObject), E_FAIL);
	m_pUI[UI_BACK_BOX]->AddRef();

	//Back
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pUI[UI_BACK] = pGameObject->Is<CImageUI>();
	m_pUI[UI_BACK]->Set_Image(L"Back");
	m_pUI[UI_BACK]->Set_Size(_vec3{ 47.f, 27.f ,0.f });
	m_pUI[UI_BACK]->Set_Pos(_vec3{ 520.f, -975.f ,0.f });
	m_pUI[UI_BACK]->Set_RenderID(RENDER_PRIUI);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackText", pGameObject), E_FAIL);
	m_pUI[UI_BACK]->AddRef();

	//Back
	pGameObject = CImageUI::Create(m_pGraphicDev, &tObj);
	m_pUI[UI_BACKGROUND] = pGameObject->Is<CImageUI>();
	m_pUI[UI_BACKGROUND]->Set_Image(L"MenuBG");
	m_pUI[UI_BACKGROUND]->Set_Size(_vec3{ _float(WINCX),_float( WINCY ),0.f });
	m_pUI[UI_BACKGROUND]->Set_Pos(_vec3{ 0.f, 0.f ,0.f });
	m_pUI[UI_BACKGROUND]->Set_RenderID(RENDER_PRIORITY);
	//m_pUI[UI_BACKGROUND]->Set_PassIndex();
	m_pUI[UI_BACKGROUND]->Set_TextureIndex(2);
	m_pUI[UI_BACKGROUND]->Set_PassIndex(15);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Background", pGameObject), E_FAIL);
	m_pUI[UI_BACKGROUND]->AddRef();

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;

}

HRESULT CMenuPlayerSelect::Ready_GameObject_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// 오브젝트 추가
	Engine::CGameObject*		pGameObject = nullptr;

	

	m_pLeftHand = pGameObject = CRoseSelect::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RoseSelectL", pGameObject, this), E_FAIL);
	m_pLeftHand->AddRef();

	m_pRightHand = pGameObject = CRoseSelectRight::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RoseSelectR", pGameObject, this), E_FAIL);
	m_pRightHand->AddRef();

	pGameObject = CCodyDoll::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CodyDoll", pGameObject, this), E_FAIL);

	pGameObject = CMayDoll::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MayDoll", pGameObject, this), E_FAIL);

	// MainCamera
	pGameObject = CMainCamera::Create(m_pGraphicDev, &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f, &_vec3(0.f, 0.f, 0.f), 25.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MainCamera", pGameObject), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);
	return S_OK;
}

HRESULT CMenuPlayerSelect::Ready_LightInfo()
{
	D3DLIGHT9				tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);


	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Position = _vec3(0.f, 0.0f, 0.f);
	tLightInfo.Range = 50.f;
	tLightInfo.Direction = { 0.f,-1.f,-1.f };
	Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0);

	return S_OK;
}

CMenuPlayerSelect * CMenuPlayerSelect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMenuPlayerSelect*		pInstance = new CMenuPlayerSelect(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMenuPlayerSelect::Free()
{
	for (_int i = 0; i < UI_END; ++i)
	{
		if (m_pUI[i])
			Safe_Release(m_pUI[i]);
	}

	if (m_pLeftHand)
		Safe_Release(m_pLeftHand);

	if (m_pRightHand)
		Safe_Release(m_pRightHand);
	Engine::Delete_AllResource(RESOURCE_MENUPLAYERSELECT);
	Engine::Delete_AllResource(RESOURCE_MENUSELECT);
	Engine::CScene::Free();
}
