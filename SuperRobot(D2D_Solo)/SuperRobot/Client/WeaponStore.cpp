#include "stdafx.h"
#include "WeaponStore.h"
#include "Texture.h"
#include "Texture_Manager.h"
#include "Key_Manager.h"
#include "Scene_Manager.h"
#include "SoundMgr.h"
#include "GameObject_Manger.h"
#include "GameObject.h"

CWeaponStore::CWeaponStore()
	:iX(0)
	,iY(0)
	,iIndex(0)
	,m_bUpCheck(false)
	,m_bYesNo(false)
	,iStatsNum(0)
	,iYesNoNum(0)
	, m_bGoldCheck(false)
{
}


CWeaponStore::~CWeaponStore()
{
}

HRESULT CWeaponStore::Ready_Scene()
{
	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"WeaponStore.mp3");

	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
	{
		vecObj.emplace_back(_rList);
	}
	return S_OK;
}

_int CWeaponStore::Update_Scene()
{
	if (CKey_Manager::Get_Instance()->Key_Down(VK_RETURN))
	{
		CScene_Manager::Get_Instance()->SceneChange_SceneManager(CScene_Manager::SCENE_STAGE2);
	}

	Key_Check();
	return OBJ_NOEVENT;
}

void CWeaponStore::LateUpdate_Scene()
{
}

void CWeaponStore::Render_Scene()
{


	D3DXMATRIX matScale, matTrans, matParents,matWorld;

	const TEXINFO*	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitWeaponStore", iSceneNum);
	assert(!(nullptr == pTexInfo));
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, 1.0, 1.0f, 0.f);
	D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);

	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR szGold[64] = L""; //익스턴 잡은 총 골드.
	swprintf_s(szGold, L"%dP", exGold);
	D3DXMatrixTranslation(&matTrans, 450.f, 30.f, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
		szGold, lstrlen(szGold), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

	//상점 대장장이 대사.
	if (!m_bUpCheck ) 
	{
		TCHAR	szBlacksmith[64] = L"";
		swprintf_s(szBlacksmith, L"선생님 「 %s! 코딩 대신 쳐줄까?  」", vecObj[iSceneNum]->Get_Info().szUnitName);
		D3DXMatrixTranslation(&matTrans, 150.f, 500.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szBlacksmith, lstrlen(szBlacksmith), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	for (int i = 0; i < vecObj.size(); ++i)
	{
		TCHAR szUnitName[64] = L""; //유닛 기체 이름
		TCHAR szUnitMove[64] = L"";	//유닛 이동력
		TCHAR szUnitArmor[64] = L"";//유닛 아머
		TCHAR szUnitEnery[64] = L"";//유닛 에너지
		TCHAR szUnitHpmax[64] = L"";//유닛 최대체력

		swprintf_s(szUnitName, L"%s", vecObj[i]->Get_Info().szUnitName);
		D3DXMatrixTranslation(&matTrans, 30.f, 160.f + (45 * i), 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szUnitName, lstrlen(szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		swprintf_s(szUnitMove, L"%d", vecObj[i]->Get_Info().iMove);
		D3DXMatrixTranslation(&matTrans, 350.f, 160.f + (45 * i), 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szUnitMove, lstrlen(szUnitMove), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


		swprintf_s(szUnitName, L"%d", vecObj[i]->Get_Info().iArmor);
		D3DXMatrixTranslation(&matTrans, 450.f, 160.f + (45 * i), 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szUnitName, lstrlen(szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		swprintf_s(szUnitName, L"%d", vecObj[i]->Get_Info().iMaxEnergy);
		D3DXMatrixTranslation(&matTrans, 570.f, 160.f + (45 * i), 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szUnitName, lstrlen(szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


		swprintf_s(szUnitName, L"%d", vecObj[i]->Get_Info().iMaxHp);
		D3DXMatrixTranslation(&matTrans, 700.f, 160.f + (45 * i), 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szUnitName, lstrlen(szUnitName), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_bUpCheck)
	{
		const TEXINFO*	pTexInfo2 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"UnitStatsUp", iStatsNum);
		assert(!(nullptr == pTexInfo2));

		float fCenterX = float(pTexInfo2->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo2->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, 1.0, 1.0f, 0.f);
		D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0.f);

		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		TCHAR szUnitMove[64] = L""; //유닛 이동력
		TCHAR szUnitArmor[64] = L"";//유닛 아머
		TCHAR szUnitEnery[64] = L"";//유닛 에너지
		TCHAR szUnitHpmax[64] = L"";//유닛 최대체력


		swprintf_s(szUnitMove, L"%d", vecObj[iIndex]->Get_Info().iMove);
		D3DXMatrixTranslation(&matTrans, 240.f, 310.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szUnitMove, lstrlen(szUnitMove), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		swprintf_s(szUnitArmor, L"%d", vecObj[iIndex]->Get_Info().iArmor);
		D3DXMatrixTranslation(&matTrans, 320.f, 310.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szUnitArmor, lstrlen(szUnitArmor), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		swprintf_s(szUnitEnery, L"%d", vecObj[iIndex]->Get_Info().iMaxEnergy);
		D3DXMatrixTranslation(&matTrans, 400.f, 310.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szUnitEnery, lstrlen(szUnitEnery), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));

		swprintf_s(szUnitHpmax, L"%d", vecObj[iIndex]->Get_Info().iMaxHp);
		D3DXMatrixTranslation(&matTrans, 500.f, 310.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szUnitHpmax, lstrlen(szUnitHpmax), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));


		if (0 == iStatsNum && !m_bYesNo && !m_bGoldCheck)
		{
			TCHAR	szBlacksmith[64] = L"";
			swprintf_s(szBlacksmith, L"선생님 「1000P를 내면 반응을 +1임 빠름 」");
			D3DXMatrixTranslation(&matTrans, 150.f, 500.f, 0.f);
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
				szBlacksmith, lstrlen(szBlacksmith), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if (1 == iStatsNum && !m_bYesNo && !m_bGoldCheck)
		{
			TCHAR	szBlacksmith[64] = L"";
			swprintf_s(szBlacksmith, L"선생님 「2000P를 내면 장갑을 +50임 개딴딴 」");
			D3DXMatrixTranslation(&matTrans, 150.f, 500.f, 0.f);
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
				szBlacksmith, lstrlen(szBlacksmith), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if (2 == iStatsNum && !m_bYesNo && !m_bGoldCheck)
		{
			TCHAR	szBlacksmith[64] = L"";
			swprintf_s(szBlacksmith, L"선생님 「1500P를 내면 EN +20임 마나넘침 」");
			D3DXMatrixTranslation(&matTrans, 150.f, 500.f, 0.f);
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
				szBlacksmith, lstrlen(szBlacksmith), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if (3 == iStatsNum && !m_bYesNo && !m_bGoldCheck)
		{
			TCHAR	szBlacksmith[64] = L"";
			swprintf_s(szBlacksmith, L"선생님 「5000P를 내면 HP +300 개꿀임」");
			D3DXMatrixTranslation(&matTrans, 150.f, 500.f, 0.f);
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
				szBlacksmith, lstrlen(szBlacksmith), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	if (m_bYesNo)
	{
		const TEXINFO*	pTexInfo3 = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"SystemSelect", L"YesNo", iYesNoNum);
		assert(!(nullptr == pTexInfo3));

		float fCenterX = float(pTexInfo3->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo3->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, 1.0, 1.0f, 0.f);
		D3DXMatrixTranslation(&matParents, 0.f, 0.f, 0.f);

		matWorld = matScale * matTrans * matParents;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo3->pTexture, nullptr, &D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		if (0 == iYesNoNum && !m_bGoldCheck)
		{
			TCHAR	szBlacksmith[64] = L"";
			swprintf_s(szBlacksmith, L"선생님 「확실해?! 개조할꺼야? 」");
			D3DXMatrixTranslation(&matTrans, 150.f, 500.f, 0.f);
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
				szBlacksmith, lstrlen(szBlacksmith), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
		}


		if (1 == iYesNoNum && !m_bGoldCheck)
		{
			TCHAR	szBlacksmith[64] = L"";
			swprintf_s(szBlacksmith, L"선생님 「왜왜왜 왜 안해 다시 생각해」");
			D3DXMatrixTranslation(&matTrans, 150.f, 500.f, 0.f);
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
			CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
				szBlacksmith, lstrlen(szBlacksmith), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	if (!m_bYesNo && m_bGoldCheck)
	{
		TCHAR	szBlacksmith[64] = L"";
		swprintf_s(szBlacksmith, L"선생님 「돈 가져와!! 돈! 」");
		D3DXMatrixTranslation(&matTrans, 150.f, 500.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(),
			szBlacksmith, lstrlen(szBlacksmith), 0, 25, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CWeaponStore::Release_Scene()
{
}

void CWeaponStore::Key_Check()
{
	//씬에서 메뉴 선택 /////////////////////////////////////
	if (!m_bUpCheck &&CKey_Manager::Get_Instance()->Key_Down(VK_UP))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		iSceneNum--;
		if (iSceneNum < 0)
		{
			iSceneNum = vecObj.size() - 1;
		}
		iIndex = iSceneNum;
	}

	if (!m_bUpCheck && CKey_Manager::Get_Instance()->Key_Down(VK_DOWN))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		iSceneNum++;
		if (iSceneNum > vecObj.size() - 1)
		{
			iSceneNum = 0;
		}
		iIndex = iSceneNum;
	}

	if (!m_bUpCheck && CKey_Manager::Get_Instance()->Key_Down('A'))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		m_bUpCheck = true;
	}

	//////////////////////////////////////////////////////
	if (m_bUpCheck && !m_bYesNo && CKey_Manager::Get_Instance()->Key_Down(VK_LEFT))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		m_bGoldCheck = false;
		iStatsNum--;
		if (iStatsNum < 0)
		{
			iStatsNum = 3;
		}

	}

	if (m_bUpCheck && !m_bYesNo && CKey_Manager::Get_Instance()->Key_Down(VK_RIGHT))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		m_bGoldCheck = false;
		iStatsNum++;
		if (iStatsNum > 3)
		{
			iStatsNum = 0;
		}

	}
	if (m_bUpCheck &&  !m_bYesNo && CKey_Manager::Get_Instance()->Key_Down('S'))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		iStatsNum = 0;
		//iYesNoNum = 0;
		m_bUpCheck = false;
		m_bGoldCheck = false;
		/*m_bYesNo = false;*/
	}


	if (m_bUpCheck && !m_bYesNo && CKey_Manager::Get_Instance()->Key_Down('A')) // 그래 아니 띄워줌.
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		m_bYesNo = true;
		m_bGoldCheck = false;
	}

//////////////////////////////////////////////////////////////////////////////
	if (m_bYesNo && iYesNoNum == 0 &&  CKey_Manager::Get_Instance()->Key_Down('A')) //최종적으로 업그레이드 선택함.
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		m_bYesNo = false;
		iYesNoNum = 0;

		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
		{
			if (!lstrcmp(vecObj[iIndex]->Get_Info().szUnitName, _rList->Get_Info().szUnitName))
			{
				if (0 == iStatsNum)
				{
					
				
					if (exGold - 1000 >= 0)
					{
						INFO InfoTemp = _rList->Get_Info();
						InfoTemp.iMove = InfoTemp.iMove + 1;
						_rList->Set_UnitInfo(InfoTemp);
						exGold -= 1000;

					}
					else
					{
						m_bGoldCheck = true;
						break;
					}
				}
				if (1 == iStatsNum)
				{
					if (exGold - 2000 >= 0)
					{
						INFO InfoTemp = _rList->Get_Info();
						InfoTemp.iArmor = InfoTemp.iArmor + 50;
						_rList->Set_UnitInfo(InfoTemp);
						exGold -= 1500;
					}
					else
					{
						m_bGoldCheck = true;
						break;
					}
			/*		swprintf_s(szBlacksmith, L"선생님 「2000P를 내면 장갑을 +50임 끌리지? 」");*/
				}

				if (2 == iStatsNum)
				{
					if (exGold - 1500 >= 0)
					{
						INFO InfoTemp = _rList->Get_Info();
						InfoTemp.iMaxEnergy = InfoTemp.iMaxEnergy + 20;
						_rList->Set_UnitInfo(InfoTemp);
						exGold -= 1500;

					}
					else
					{
						m_bGoldCheck = true;
						break;
					}
					/*swprintf_s(szBlacksmith, L"선생님 「1500P를 내면 EN +20임 끌리지? 」");*/
				}

				if (3 == iStatsNum)
				{
					if (exGold - 5000 >= 0)
					{
						INFO InfoTemp = _rList->Get_Info();
						InfoTemp.iMaxHp = InfoTemp.iMaxHp + 300;
						_rList->Set_UnitInfo(InfoTemp);
						exGold -= 5000;
					}
					else
					{
						m_bGoldCheck = true;
						break;
					}
	/*				swprintf_s(szBlacksmith, L"선생님 「5000P를 내면 HP +300임 끌리지? 」");*/
				}
			}
		}

	}

	if (m_bYesNo && iYesNoNum == 1 &&  CKey_Manager::Get_Instance()->Key_Down('A'))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		m_bGoldCheck = false;
		m_bYesNo = false;
		iYesNoNum = 0;

				//if (1 == iStatsNum)
				//{
				//	swprintf_s(szBlacksmith, L"선생님 「2000P를 내면 장갑을 +50임 끌리지? 」");
				//}

				//if (2 == iStatsNum)
				//{
				//	swprintf_s(szBlacksmith, L"선생님 「1500P를 내면 EN +20임 끌리지? 」");
				//}

				//if (3 == iStatsNum)
				//{
				//	swprintf_s(szBlacksmith, L"선생님 「5000P를 내면 HP +300임 끌리지? 」");
				//}
	
	}
	if (m_bYesNo && CKey_Manager::Get_Instance()->Key_Down(VK_UP))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		iYesNoNum--;
		if (iYesNoNum < 0)
		{
			iYesNoNum = 1;
		}
	}
	if (m_bYesNo && CKey_Manager::Get_Instance()->Key_Down(VK_DOWN))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		iYesNoNum++;
		if (iYesNoNum > 1)
		{
			iYesNoNum = 0;
		}
	}

	///////////////////////////////////////////////////////////
}
