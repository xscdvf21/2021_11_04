#include "stdafx.h"
#include "GunDamMK.h"
#include "Texture_Manager.h"
#include "Tile.h"
#include "ScrollMgr.h"
#include "Key_Manager.h"
#include "MousePointer.h"
#include "GameObject_Manger.h"
#include "UnitMenu.h"
#include "UnitInfo.h"
#include "UnitFight.h"
#include "Astar_manager.h"
#include "Time_Manager.h"
#include "Scroll_Manager.h"
#include "SoundMgr.h"
#include "Effect.h"
#include "Animation_IMP.h"
#include "NormalEffect.h"

CGunDamMK::CGunDamMK()
	:bAStarCheck(false)
	, bAStarUnitCheck(false)
{

}


CGunDamMK::~CGunDamMK()
{
}

HRESULT CGunDamMK::Ready_GameObject()
{
	
	m_tInfo.vPos = { 60.f, 60.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };



	m_pTile = new CTile;
	m_pTile->Ready_GameObject();

	GROUND*		m_pTemp = m_pTile->Get_Index(m_tInfo.vPos);
	m_tInfo.vPos = m_pTemp->vPos;
	CGameObject* vecTemp = CGameObject_Manger::Get_Instance()->Get_Tile();


	m_pTextureUnit = L"GunDamMK";
	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", L"GunDamMK", 0);
	
	
	lstrcpy(m_tInfo.szUnitName, L"�Ǵ�MK");
	lstrcpy(m_tInfo.szName, L"�ƹ���");
	lstrcpy(m_tInfo.szToking, L"���ÿ�ȸ �غ� ���� ������...��");
	lstrcpy(m_tInfo.szToking2, L"�������� ������...��");
	lstrcpy(m_tInfo.szWeaponName, L"��ĭ ��");
	m_tInfo.iLevel = 2;
	m_tInfo.iHp = 100;
	m_tInfo.iMaxHp = 2200;
	m_tInfo.iMove = 10; //ĳ�� �̵���.
	m_tInfo.iArmor = 150;
	m_tInfo.iEnergy = 100;
	m_tInfo.iMaxEnergy = 100;
	m_tInfo.iAttack = 600;
	m_tInfo.iRange = 7;
	return S_OK;
}

int CGunDamMK::Update_GameObject()
{
	if (m_bDead)
	{
		CEffect_IMP * pAni = new CAnimation_IMP;
		pAni->Ready_Effect_IMP();
		dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"UnitDead", { 0.f, 9.f });
		_vec3 temp = { (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f ,0.f };
		CEffect* pNormal = new CNormalEffect(pAni);
		pNormal->Ready_GameObject();
		pNormal->Set_Pos(m_tInfo.vPos);
		CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"UnitDead.wav", CSoundMgr::MONSTER);

		return OBJ_DEAD;
	}
	if(bAStarUnitCheck)
		Move_Route();
	if(!m_tInfo.bTurn)
		Key_Check();

	//if(m_tInfo.vPos == CGameObject_Manger::Get_Instance()->Get_Mouse()->Get_Info().vPos 
	//	|| CGameObject_Manger::Get_Instance()->Get_Mouse()->Get_Info().vPos == CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER).front()->Get_Info().vPos) //���� Ű �ߺ�Ű üũ ������. Ŀ����ġ�� �������ָ� Ű
	//	Render_SelectMenu();
	return OBJ_NOEVENT;
}

void CGunDamMK::LateUpdate_GameObject()
{

}

void CGunDamMK::Render_GameObject()
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	//pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", L"GunDamMK", 0);
	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"CGunDamMK.Cpp, pTexInfo�� nullptr");
		return;
	}

	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	_matrix matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + iScrollX, m_tInfo.vPos.y + iScrollY, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	// ���� �� ����
	if (m_bSelect && !m_tInfo.bTurn)
	{
		m_iAlpha -= 5;

		if(0 >= m_iAlpha)
			m_iAlpha = 255;

		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
		m_bSelect = false;
	}
	else if(m_tInfo.bTurn) //���� ���������.
	{
		m_iAlpha = 255;
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 40, 40, 40));
	}
	else
	{
		m_iAlpha = 255;
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
		if (2 == static_iAllCheck)
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 50, 50, 50));
	}

}


void CGunDamMK::Release_GameObject()
{

}



void CGunDamMK::Key_Check() //�����̴°� üũ
{
	//static_iAllCheck = 1  ó�� ���� �����ϰ�, �����̵�, ����, �ɷ� �� �� �ִ� �޴�
	//static_iAllCheck = 2  ���� �̵� ���� ��, Ÿ�� ���̰�, ������ �� �ֵ���
	//static_iAllCheck = 3  ���� ���Ž�ų �� �� �ִ� �޴�
	//static_iAllCheck = 4	���� �ɷ�ġ â
	//static_iAllCheck = 5	���� �̵� �� Ÿ�� �˻��ؼ�, ���Ͱ� ������ ����, ��� �޴�â
	//static_iAllCheck = 6  ������ ���� ������ Ŀ�� �����ϼ� �ְ��ϰ�, ���콺 Ŀ���� ���� �ö󰣻��¿��� A�� ������ ���� 7�γѾ.
	//static_iAllCheck = 7  ������ ���͸� ������ ��� �����â.
	//static_iAllCheck = 8  ���⸦ �� �� ���� �� ���� �����߷� ��� ������.
	//static_iAllCheck = 9  ���� ��.
	if (m_tInfo.vPos == dynamic_cast<CMousePointer*>(CGameObject_Manger::Get_Instance()->Get_Mouse())->Get_Info().vPos
		&& static_iAllCheck == 0
		&& CKey_Manager::Get_Instance()->Key_Down('A')) //���� �޴��� ���̵��� ����.
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);

		//CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		static_iAllCheck = 1;
		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
		{
			if (!lstrcmp(_rList->Get_TextureKey(), L"UnitMenu"))
			{
				_rList->Set_static_iAllCheck(static_iAllCheck);
				_rList->Set_Pos(m_tInfo.vPos);
			}
		}

		CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(static_iAllCheck);
		m_bUnitSelect = true;

		if (m_vVec3Temp.empty())
		{
			m_vVec3Temp.emplace_back(m_tInfo.vPos);
		}



	}


	//S�� ������ �޴� ��ȣ�� 0���� �ٲ��ִµ�. �̶� �ʱ�ȭ �Ǿ�� �� ��ü���׵� �װ��� �Ѱ������
	if (m_bUnitSelect && CKey_Manager::Get_Instance()->Key_Down('S'))
	{
		static_iAllCheck = 0;
		m_iSceneNum = 0;

		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
		{
			_rList->Set_iSceneNum(m_iSceneNum);
			_rList->Set_static_iAllCheck(static_iAllCheck);
		}

		CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(static_iAllCheck);
		CGameObject_Manger::Get_Instance()->Get_Tile()->Set_static_iAllCheck(static_iAllCheck);

		m_bUnitSelect = false;


		if (!m_vVec3Temp.empty()) //���������� ��, ������ �� ��ġ�� ��� �ִ°� �����.
		{
			m_tInfo.vPos = m_vVec3Temp.front();
			m_vVec3Temp.clear();
			m_vVec3Temp.shrink_to_fit();
		}

		if (!vecTemp.empty()) //������� ��� ������ ��ġ�� �ǵ��ư���, ���� ��Ҵ� �̵������� Ÿ���� �����ֱ⶧���� ����־����.
		{
			vecTemp.clear();
			vecTemp.shrink_to_fit();
		}

	} //��� ��ư

	  //////////////////////////////////////////////////////////////////////////////////////
	if (static_iAllCheck == 1 && CKey_Manager::Get_Instance()->Key_Down('A'))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		if (m_iSceneNum == 0)
		{
			//�̵� �����ϸ� �׶� �̵��� �� �ִ� Ÿ�� ����.
			if (!vecTemp.empty()) 
			{
				vecTemp.clear();
				vecTemp.shrink_to_fit();
			}


			if (vecTemp.empty())
			{

				pTemp = m_pTile->Get_Index(m_tInfo.vPos);
				int iIndex = pTemp->iIndex;

				pTemp = m_pTile->Get_TileMove(iIndex);
				pTemp->byOption = 1;
				vecTemp.emplace_back(pTemp);
				/*m_pTile->Set_TileAhpal(iIndex);*/
				//////////////////////////////////������ ���� �Ÿ��� ////////////////////////////////////
				for (int i = 1; i <= m_tInfo.iMove; ++i) //���η� �̵��Ÿ�
				{

					if (/*(iIndex%TILEX)*TILEX <= iIndex + i && ((iIndex%TILEX)+1)*TILEX > iIndex+i &&*/ 0 <= iIndex + i && 900 >= iIndex + i) //������ �̵����� Ÿ��
					{

						if ((iIndex + i) % 30 == 0)
							break;
						pTemp = m_pTile->Get_TileMove(iIndex + i); //������ �̵����� Ÿ��
						pTemp->byOption = 1;
						vecTemp.emplace_back(pTemp);




					}
				}
				////////////////////////���� �������θ� /////////////////////////////////////
				for (int i = 1; i <= m_tInfo.iMove; ++i)
				{
					if (/*(iIndex%TILEX)*TILEX <= iIndex - i && ((iIndex%TILEX) + 1)*TILEX > iIndex-i &&*/ 0 <= iIndex - i && 900 >= iIndex - i)
					{
						pTemp = m_pTile->Get_TileMove(iIndex - i); //���� �̵����� Ÿ��
						pTemp->byOption = 1;

						vecTemp.emplace_back(pTemp);

						if ((iIndex - i) % 30 == 0)
							break;
					}
				}


				///////////////////////////// ���� �ö󰡸鼭 ���� Ÿ�� /////////////////////////////////////////////
				for (int i = 1; i <= m_tInfo.iMove; ++i)
				{
					if (0 <= iIndex - i * 30 && 900 >= iIndex - i * 30) //���� �̵� ����Ÿ��
					{
						pTemp = m_pTile->Get_TileMove(iIndex - i * 30);
						pTemp->byOption = 1;
						vecTemp.emplace_back(pTemp);

						int iTemp = iIndex - i * 30;
						for (int j = 1; j <= m_tInfo.iMove; j++)
						{
							if (0 <= iTemp - j && 900 >= iTemp - j)
							{

								pTemp = m_pTile->Get_TileMove(iTemp - j); //��ĭ �� ���� �ö󰡸鼭 ���� �� Ÿ�ϵ� ����.
								pTemp->byOption = 1;
								vecTemp.emplace_back(pTemp);
								if ((iTemp - j) % 30 == 0)
									break;
							}
						}

						for (int j = 1; j <= m_tInfo.iMove; j++) //��ĭ �� ���� �ö󰡸鼭 ������ �� Ÿ�ϵ� ����.
						{
							if (0 <= iTemp + j && 900 >= iTemp + j)
							{

								if ((iIndex + j) % 30 == 0)
									break;
								pTemp = m_pTile->Get_TileMove(iTemp + j);
								pTemp->byOption = 1;
								vecTemp.emplace_back(pTemp);


							}
						}

					}
				}

				/////////////////////////////////////////////////////////////////////////////////////////


				/////////////////////////////�Ʒ��� �������鼭 ���� Ÿ�� /////////////////////////////////////////////////////////
				for (int i = 1; i <= m_tInfo.iMove; ++i)
				{
					if (0 <= iIndex + i * 30 && 900 >= iIndex + i * 30)
					{
						pTemp = m_pTile->Get_TileMove(iIndex + i * 30);//�Ʒ� �Ƶ� ����Ÿ��
						pTemp->byOption = 1;
						vecTemp.emplace_back(pTemp);

						int iTemp = iIndex + i * 30;
						for (int j = 1; j <= m_tInfo.iMove; j++)
						{
							if (0 <= iTemp - j && 900 >= iTemp - j)
							{
								pTemp = m_pTile->Get_TileMove(iTemp - j); //��ĭ �� ���� �������鼭 ���� �� Ÿ�ϵ� ����.
								pTemp->byOption = 1;
								vecTemp.emplace_back(pTemp);
							}

							if ((iTemp - j) % 30 == 0)
								break;
						}

						for (int j = 1; j <= m_tInfo.iMove; j++)
						{
							if (0 <= iTemp + j && 900 >= iTemp + j)
							{
								if ((iIndex + j) % 30 == 0)
									break;

								pTemp = m_pTile->Get_TileMove(iTemp + j); //��ĭ �� ���� �ö󰡸鼭 ������ �� Ÿ�ϵ� ����.
								pTemp->byOption = 1;
								vecTemp.emplace_back(pTemp);
							}
						}
					}

				}
				/////////////////////////////////////////////////////////////////////////////////////////////////////////
			}



			static_iAllCheck = 2; //�̵� ���� �� Ÿ�� ���̰� ���� 
			for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
			{
				if (!lstrcmp(_rList->Get_TextureKey(), L"UnitMenu"))
				{
					//_rList->Set_TileCheck(m_bTileCheck); //���� ����
					_rList->Set_static_iAllCheck(static_iAllCheck);

				}
			}
			//m_bTileCheck = true;
			//m_bMoveTile = true;
			CGameObject_Manger::Get_Instance()->Get_Tile()->Set_TileCheck(true);
			CGameObject_Manger::Get_Instance()->Get_Tile()->Set_MoveTileCheck(true);
			//m_bTileCheck = false;
			//m_bMoveTile = false;
			CGameObject_Manger::Get_Instance()->Get_Tile()->Set_static_iAllCheck(static_iAllCheck);
			CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(static_iAllCheck);

		}
		else if (m_iSceneNum == 1)
		{
			static_iAllCheck = 3;
			for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
			{
				if (!lstrcmp(_rList->Get_TextureKey(), L"UnitSkill"))
				{
					_rList->Set_static_iAllCheck(static_iAllCheck);
					_rList->Set_UnitInfo(m_tInfo);
					_rList->Set_TextureUnitKey(m_pTextureUnit);
					_rList->Set_UnitSkillCheck(true);
					_rList->Set_iSceneNum(0);
				}
			}
			
		}
		else if (m_iSceneNum == 2)
		{
			static_iAllCheck = 4;
			for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
			{
				if (!lstrcmp(_rList->Get_TextureKey(), L"UnitINFO"))
				{
					_rList->Set_static_iAllCheck(static_iAllCheck);
					_rList->Set_UnitInfo(m_tInfo);
					_rList->Set_TextureUnitKey(m_pTextureUnit);
				}
			}
		}

	}

	if (static_iAllCheck == 1 && CKey_Manager::Get_Instance()->Key_Down(VK_UP))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		m_iSceneNum--;
		if (m_iSceneNum < 0)
		{
			m_iSceneNum = 2;
		}

		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
		{
			if (!lstrcmp(_rList->Get_TextureKey(), L"UnitMenu"))
			{
				_rList->Set_iSceneNum(m_iSceneNum);
			}
		}

	}
	else if (static_iAllCheck == 1 && CKey_Manager::Get_Instance()->Key_Down(VK_DOWN))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		m_iSceneNum++;

		if (m_iSceneNum > 2)
		{
			m_iSceneNum = 0;
		}

		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
		{
			if (!lstrcmp(_rList->Get_TextureKey(), L"UnitMenu"))
			{
				_rList->Set_iSceneNum(m_iSceneNum);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	if (static_iAllCheck == 5 && CKey_Manager::Get_Instance()->Key_Down(VK_UP)) //���� , ��� �޴�â���� ��� ��������.
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		m_iSceneNum--;
		if (m_iSceneNum < 0)
		{
			m_iSceneNum = 1;
		}

		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
		{
			if (!lstrcmp(_rList->Get_TextureKey(), L"UnitAttack"))
			{
				_rList->Set_iSceneNum(m_iSceneNum);
			}
		}

		CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(static_iAllCheck);
	}
	else if (static_iAllCheck == 5 && CKey_Manager::Get_Instance()->Key_Down(VK_DOWN))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		m_iSceneNum++;

		if (m_iSceneNum > 1)
		{
			m_iSceneNum = 0;
		}

		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
		{
			if (!lstrcmp(_rList->Get_TextureKey(), L"UnitAttack"))
			{
				_rList->Set_iSceneNum(m_iSceneNum);
			}
		}

		CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(static_iAllCheck);
	}


	if (static_iAllCheck == 5 && m_iSceneNum == 0 && CKey_Manager::Get_Instance()->Key_Down('A')) //������ ������
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		static_iAllCheck = 6;
		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
		{
			if (!lstrcmp(_rList->Get_TextureKey(), L"UnitAttack"))
			{
				_rList->Set_static_iAllCheck(static_iAllCheck);
			}
		}
		m_bTileCheck = true;
		CGameObject_Manger::Get_Instance()->Get_Tile()->Set_TileCheck(m_bTileCheck);
		m_bTileCheck = false;
		CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(static_iAllCheck);
		CGameObject_Manger::Get_Instance()->Get_Tile()->Set_static_iAllCheck(static_iAllCheck);
	}
	else if (static_iAllCheck == 5 && m_iSceneNum == 1 && CKey_Manager::Get_Instance()->Key_Down('A')) //��⸦������
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		////m_iSceneNum = 0;
		////static_iAllCheck = 0;

		////for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
		////{
		////	if (!lstrcmp(_rList->Get_TextureKey(), L"UnitAttack"))
		////	{
		////		_rList->Set_static_iAllCheck(static_iAllCheck);
		////		_rList->Set_iSceneNum(m_iSceneNum);
		////	}
		////}

		////CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(static_iAllCheck);
		Turn_Check();
		m_tInfo.bTurn = true;

		if (!m_vVec3Temp.empty()) //���������� ��, ������ �� ��ġ�� ��� �ִ°� �����.
		{
			m_vVec3Temp.clear();
			m_vVec3Temp.shrink_to_fit();
		}

	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	if (static_iAllCheck == 6 && CKey_Manager::Get_Instance()->Key_Down('A')) //���� ���� �� AŰ�� ������ ���� ����â.
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
		{
			if (_rList->Get_MonsterInfo().vPos == CGameObject_Manger::Get_Instance()->Get_Mouse()->Get_Info().vPos)
			{
				static_iAllCheck = 7;

				for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
				{
					if (!lstrcmp(_rList->Get_TextureKey(), L"UnitWeaponSelect"))
					{
						_rList->Set_static_iAllCheck(static_iAllCheck);
						_rList->Set_UnitInfo(m_tInfo);
					}
				}

				CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(static_iAllCheck);
			}
		}
	}


	///////////////////////////////////////////////////////////////////////////////
	if (static_iAllCheck == 7 && CKey_Manager::Get_Instance()->Key_Down('A')) // ���� ����â���� ���� ����.
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
		{
			if (_rList->Get_MonsterInfo().vPos == CGameObject_Manger::Get_Instance()->Get_Mouse()->Get_Info().vPos)
			{
				CGameObject* pObj = new CUnitFight;
				pObj->Ready_GameObject();
				CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UI, pObj);
				static_iAllCheck = 8;
				m_tMonInfo = _rList->Get_MonsterInfo();
				tTemp = _rList->Get_TextureMonsterKey();
			}
		}
		CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(static_iAllCheck);
		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
		{
			if (!lstrcmp(_rList->Get_TextureKey(), L"UnitFight"))
			{
				_rList->Set_static_iAllCheck(static_iAllCheck);
				_rList->Set_UnitInfo(m_tInfo);
				_rList->Set_MonsterInfo(m_tMonInfo);
				_rList->Set_TextureUnitKey(m_pTextureUnit);
				_rList->Set_TextureMonsterKey(tTemp);
				m_bAttaker = true;
				break;
			}
		}

		////������ ������, ���� ��������.
		//if (m_tInfo.bTurn) //UnitFight���� ���� ������ �µ��� ���� ���� �ѱ�.
		//{
		//	m_iSceneNum = 0;
		//	static_iAllCheck = 0;
		//	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
		//	{
		//		_rList->Set_iSceneNum(m_iSceneNum);
		//		_rList->Set_static_iAllCheck(static_iAllCheck);
		//	}
		//	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_CURSOR))
		//	{
		//		_rList->Set_iSceneNum(m_iSceneNum);
		//		_rList->Set_static_iAllCheck(static_iAllCheck);
		//	}
		//	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_GROUND))
		//	{
		//		_rList->Set_iSceneNum(m_iSceneNum);
		//		_rList->Set_static_iAllCheck(static_iAllCheck);
		//	}
		//}

	}


	if (static_iAllCheck == 7 && CKey_Manager::Get_Instance()->Key_Down(VK_UP)) //���� �ִ� ������ ���� �ٲ���ҵ�.
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		m_iSceneNum--;

		if (m_iSceneNum < 0)
			m_iSceneNum = 3;

		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
		{
			if (!lstrcmp(_rList->Get_TextureKey(), L"UnitWeaponSelect"))
			{
				_rList->Set_iSceneNum(m_iSceneNum);
			}
		}
	}

	if (static_iAllCheck == 7 && CKey_Manager::Get_Instance()->Key_Down(VK_DOWN)) //���� �ִ� ������ ���� �ٲ���ҵ�.
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		m_iSceneNum++;

		if (m_iSceneNum > 3)
			m_iSceneNum = 0;

		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
		{
			if (!lstrcmp(_rList->Get_TextureKey(), L"UnitWeaponSelect"))
			{
				_rList->Set_iSceneNum(m_iSceneNum);
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////


	if (static_iAllCheck == 2 && CKey_Manager::Get_Instance()->Key_Down('A') || bAStarCheck) //�̵� �ض�.,
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"Cursor.wav", CSoundMgr::PLAYER);
		/*CGameObject_Manger::Get_Instance()->Get_Tile()->Set_MoveTileCheck(m_bMoveTile);*/ //�̵� ������, �޽��ιٲ���, 
		for (auto& pvec : vecTemp)
		{

				if (pvec->vPos == dynamic_cast<CMousePointer*>(CGameObject_Manger::Get_Instance()->Get_Mouse())->Get_Info().vPos)
				{

					if (m_tInfo.vPos == dynamic_cast<CMousePointer*>(CGameObject_Manger::Get_Instance()->Get_Mouse())->Get_Info().vPos)
						bAStarCheck = true;
					else
					{
						CAstar_Manager::Get_Instance()->StartAstar(m_tInfo.vPos, CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_CURSOR).front()->Get_Info().vPos);
						bAStarUnitCheck = true; //Ÿ�� �ۿ� Ŭ���ϴ°� ���Ϸ���...����ſ�����...
					}
					//m_tInfo.vPos = dynamic_cast<CMousePointer*>(CGameObject_Manger::Get_Instance()->Get_Mouse())->Get_Info().vPos; //���콺 Ŀ�� ��ġ�� . ��������.
	/*				static_iAllCheck = 0;
					CGameObject_Manger::Get_Instance()->Get_Tile()->Set_static_iAllCheck(static_iAllCheck);*/ //�̵�������, Ÿ�� ���̴°� ���ְ�
				}
			
		}
		if (!vecTemp.empty() && bAStarUnitCheck)
		{
			
			vecTemp.clear();
			vecTemp.shrink_to_fit();
		}
		if (bAStarCheck)
		{
			static_iAllCheck = 0;
			CGameObject_Manger::Get_Instance()->Get_Tile()->Set_static_iAllCheck(static_iAllCheck); //�̵�������, Ÿ�� ���̴°� ���ְ�
			GROUND*		m_pTemp = m_pTile->Get_Index(m_tInfo.vPos);
			m_tInfo.vPos = m_pTemp->vPos;
			Attack_Check();

			bAStarUnitCheck = false;
			bAStarCheck = false;
		}
	}

	

}

void CGunDamMK::Attack_Check() //�̵� �� �� ���� �� ���� ã��.
{


	if (!vecTemp.empty())
	{
		vecTemp.clear();
		vecTemp.shrink_to_fit();
	}


	pTemp = m_pTile->Get_Index(m_tInfo.vPos);

	int iIndex = pTemp->iIndex;

	pTemp = m_pTile->Get_TileMove(iIndex);
	pTemp->byOption = 1;
	vecTemp.emplace_back(pTemp);
	//////////////////////////////////������ ���� �Ÿ��� ////////////////////////////////////
	for (int i = 1; i <= m_tInfo.iRange; ++i) //���η� �̵��Ÿ�
	{

		if (/*(iIndex%TILEX)*TILEX <= iIndex + i && ((iIndex%TILEX)+1)*TILEX > iIndex+i &&*/ 0 <= iIndex + i && 900 >= iIndex + i) //������ �̵����� Ÿ��
		{

			if ((iIndex + i) % 30 == 0)
				break;
			pTemp = m_pTile->Get_TileMove(iIndex + i); //������ �̵����� Ÿ��
			pTemp->byOption = 1;
			vecTemp.emplace_back(pTemp);
		}
	}
	////////////////////////���� �������θ� /////////////////////////////////////
	for (int i = 1; i <= m_tInfo.iRange; ++i)
	{
		if (/*(iIndex%TILEX)*TILEX <= iIndex - i && ((iIndex%TILEX) + 1)*TILEX > iIndex-i &&*/ 0 <= iIndex - i && 900 >= iIndex - i)
		{
			pTemp = m_pTile->Get_TileMove(iIndex - i); //���� �̵����� Ÿ��
			pTemp->byOption = 1;

			vecTemp.emplace_back(pTemp);

			if ((iIndex - i) % 30 == 0)
				break;
		}
	}


	///////////////////////////// ���� �ö󰡸鼭 ���� Ÿ�� /////////////////////////////////////////////
	for (int i = 1; i <= m_tInfo.iRange; ++i)
	{
		if (0 <= iIndex - i * 30 && 900 >= iIndex - i * 30) //���� �̵� ����Ÿ��
		{
			pTemp = m_pTile->Get_TileMove(iIndex - i * 30);
			pTemp->byOption = 1;
			vecTemp.emplace_back(pTemp);

			int iTemp = iIndex - i * 30;
			for (int j = 1; j <= m_tInfo.iRange; j++)
			{
				if (0 <= iTemp - j && 900 >= iTemp - j)
				{

					pTemp = m_pTile->Get_TileMove(iTemp - j); //��ĭ �� ���� �ö󰡸鼭 ���� �� Ÿ�ϵ� ����.
					pTemp->byOption = 1;
					vecTemp.emplace_back(pTemp);
					if ((iTemp - j) % 30 == 0)
						break;
				}
			}

			for (int j = 1; j <= m_tInfo.iRange; j++) //��ĭ �� ���� �ö󰡸鼭 ������ �� Ÿ�ϵ� ����.
			{
				if (0 <= iTemp + j && 900 >= iTemp + j)
				{

					if ((iIndex + j) % 30 == 0)
						break;
					pTemp = m_pTile->Get_TileMove(iTemp + j);
					pTemp->byOption = 1;
					vecTemp.emplace_back(pTemp);


				}
			}

		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////


	/////////////////////////////�Ʒ��� �������鼭 ���� Ÿ�� /////////////////////////////////////////////////////////
	for (int i = 1; i <= m_tInfo.iRange; ++i)
	{
		if (0 <= iIndex + i * 30 && 900 >= iIndex + i * 30)
		{
			pTemp = m_pTile->Get_TileMove(iIndex + i * 30);//�Ʒ� �Ƶ� ����Ÿ��
			pTemp->byOption = 1;
			vecTemp.emplace_back(pTemp);

			int iTemp = iIndex + i * 30;
			for (int j = 1; j <= m_tInfo.iRange; j++)
			{
				if (0 <= iTemp - j && 900 >= iTemp - j)
				{
					pTemp = m_pTile->Get_TileMove(iTemp - j); //��ĭ �� ���� �������鼭 ���� �� Ÿ�ϵ� ����.
					pTemp->byOption = 1;
					vecTemp.emplace_back(pTemp);
				}

				if ((iTemp - j) % 30 == 0)
					break;
			}

			for (int j = 1; j <= m_tInfo.iRange; j++)
			{
				if (0 <= iTemp + j && 900 >= iTemp + j)
				{
					if ((iIndex + j) % 30 == 0)
						break;

					pTemp = m_pTile->Get_TileMove(iTemp + j); //��ĭ �� ���� �ö󰡸鼭 ������ �� Ÿ�ϵ� ����.
					pTemp->byOption = 1;
					vecTemp.emplace_back(pTemp);
				}
			}
		}

	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////



	bool m_bCheckTemp = false;
	for (auto& pvec : vecTemp)
	{

		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
		{
			if (pvec->vPos == _rList->Get_MonsterInfo().vPos)
			{
				static_iAllCheck = 5;
				for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
				{
					if (!lstrcmp(_rList->Get_TextureKey(), L"UnitAttack"))
					{
						CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(static_iAllCheck);
						_rList->Set_static_iAllCheck(static_iAllCheck);
						_rList->Set_Pos(m_tInfo.vPos);
					}
				}
				m_bCheckTemp = true;
				return;
			}
		}

	}
	if (!m_bCheckTemp) // �ٵ��� ����, Ÿ�Ͽ� ���� ������.
	{
		m_tInfo.bTurn = true;

		if (!vecTemp.empty() && bAStarUnitCheck)
		{

			vecTemp.clear();
			vecTemp.shrink_to_fit();
		}

		if (!m_vVec3Temp.empty()) //���������� ��, ������ �� ��ġ�� ��� �ִ°� �����.
		{
			m_vVec3Temp.clear();
			m_vVec3Temp.shrink_to_fit();
		}
	}

}

void CGunDamMK::Turn_Check()
{

		m_iSceneNum = 0;
		static_iAllCheck = 0;
		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
		{
			_rList->Set_iSceneNum(m_iSceneNum);
			_rList->Set_static_iAllCheck(static_iAllCheck);
		}
		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_CURSOR))
		{
			_rList->Set_iSceneNum(m_iSceneNum);
			_rList->Set_static_iAllCheck(static_iAllCheck);
		}
		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_GROUND))
		{
			_rList->Set_iSceneNum(m_iSceneNum);
			_rList->Set_static_iAllCheck(static_iAllCheck);
		}
	
}

void CGunDamMK::Move_Route()
{
	list<GROUND*>& BestList = CAstar_Manager::Get_Instance()->Get_BestList();
	

	if (BestList.empty())
	{
		GROUND*		m_pTemp = m_pTile->Get_Index(m_tInfo.vPos);
		m_tInfo.vPos = m_pTemp->vPos;
		return;
	}
	m_tInfo.vDir = BestList.front()->vPos - m_tInfo.vPos;
	float fDist = D3DXVec3Length(&m_tInfo.vDir);
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	if (fDist < 3.f)
	{
		BestList.pop_front();
		if (BestList.empty())
		{
			CGameObject_Manger::Get_Instance()->Get_Tile()->Set_MoveTileCheck(false); //���̵��ϸ� Ÿ�� ����.
			bAStarCheck = true;
		}
	}
	m_tInfo.vPos += m_tInfo.vDir * CTime_Manager::Get_Instance()->Get_TimeDelta() * 300.f;
}



