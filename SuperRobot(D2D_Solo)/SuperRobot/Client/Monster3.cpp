#include "stdafx.h"
#include "Monster3.h"
#include "Texture_Manager.h"
#include "Tile.h"
#include "ScrollMgr.h"
#include "MousePointer.h"
#include "GameObject_Manger.h"
#include "Astar_manager.h"
#include "Time_Manager.h"
#include "UnitFight.h"
#include "Background.h"
#include "Effect.h"
#include "Animation_IMP.h"
#include "NormalEffect.h"
#include "SoundMgr.h"
#include "DeadGold.h"

CMonster3::CMonster3()
	: bAStarCheck(false)
	, bAStarMonsterCheck(false)
{
}


CMonster3::~CMonster3()
{
}

HRESULT CMonster3::Ready_GameObject()
{
	m_tMonInfo.bTurn = false;
	m_tMonInfo.vPos = _vec3(360.f,680.f, 0.f);
	m_tMonInfo.vSize = _vec3(1.f, 1.f, 0.f);

	m_pTile = new CTile;
	m_pTile->Ready_GameObject();

	GROUND*		m_pTemp = m_pTile->Get_Index(m_tMonInfo.vPos);
	m_tMonInfo.vPos = m_pTemp->vPos;

	m_pTextureMonster = L"Monster02";
	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Monster", L"Monster02", 0);


	lstrcpy(m_tMonInfo.szUnitName, L"로봇..");
	lstrcpy(m_tMonInfo.szName, L"이름귀찮아");
	lstrcpy(m_tMonInfo.szToking, L"「할말이없어..」");
	lstrcpy(m_tMonInfo.szToking2, L"「때리지마..」");
	m_tMonInfo.iLevel = 2;
	m_tMonInfo.iHp = 100;
	m_tMonInfo.iMaxHp = 700;
	m_tMonInfo.iMove = 2; //캐릭 이동력.
	m_tMonInfo.iAttack = 500;
	m_tMonInfo.iArmor = 250;
	m_tMonInfo.iEnergy = 160;
	m_tMonInfo.iGold = 870;
	return S_OK;
}

int CMonster3::Update_GameObject()
{
	if (m_bDead)
	{
		CEffect_IMP * pAni = new CAnimation_IMP;
		pAni->Ready_Effect_IMP();
		dynamic_cast<CAnimation_IMP*>(pAni)->Set_Frame(L"UnitDead", { 0.f, 9.f });
		_vec3 temp = { (WINCX >> 1) + 70.f, (WINCY >> 1) + 20.f ,0.f };
		CEffect* pNormal = new CNormalEffect(pAni);
		pNormal->Ready_GameObject();
		pNormal->Set_Pos(m_tMonInfo.vPos);
		CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_EFFECT, pNormal);

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"UnitDead.wav", CSoundMgr::MONSTER);

		CGameObject*	pObj = new CDeadGold;
		pObj->Ready_GameObject();
		pObj->Set_MonsterInfo(m_tMonInfo);
		CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UI, pObj);

		exGold += m_tMonInfo.iGold;
		return OBJ_DEAD;
	}

	if (m_bMonsterTurnCheck)
	{
		bool UnitCheck = false;

		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
		{
			if (!_rList->Get_Info().bTurn)
			{

				UnitCheck = true;
				break;
			}
		}
		if (UnitCheck)
			return OBJ_NOEVENT;

		if (!UnitCheck && !m_tMonInfo.bTurn) //유닛 턴이 끝나면 값을 넘겨서, 이걸 시작함.
		{

			if (!lstrcmp(m_pTextureMonster, CGameObject_Manger::Get_Instance()->Get_Monster()->Get_TextureMonsterKey()))
			{
				if (m_fTurnDelay == 0.f) // 몬스터 턴 시작하기 전에, 백그라운드 한번 실행하기 위해, 첫 몬스터한테만 적용해주면됨.
				{
					CGameObject*	pObj = new CBackground;
					pObj->Ready_GameObject();
					CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_BACKGROUND, pObj);
				}
			}
			m_fTurnDelay += CTime_Manager::Get_Instance()->Get_TimeDelta();
			if (m_fTurnDelay >= 3.0f)
				Move_Check();
		}
		if (bAStarMonsterCheck)
			Move_Route();

	}


	return OBJ_NOEVENT;
}

void CMonster3::LateUpdate_GameObject()
{

	if (m_tMonInfo.bTurn && m_bMonsterTurnCheck)
	{
		//다음 몬스터 차례 
		if (m_fTurnDelay >= 3.f) //업데이트에서 첫 몬스터 유닛움직이기 전까지 텀을 주기위해, 시간을 썻기 때문에 얘만 한번 초기화 해줘야댐.
			m_fTurnDelay = 0.f;

			m_fTurnDelay += CTime_Manager::Get_Instance()->Get_TimeDelta();


			list<CGameObject*>::iterator iter;
			for (iter = CGameObject_Manger::Get_Instance()->Get_list2(CGameObject_Manger::OBJ_MONSTER).begin();
				iter != CGameObject_Manger::Get_Instance()->Get_list2(CGameObject_Manger::OBJ_MONSTER).end(); iter++)
			{

				if (!lstrcmp(m_pTextureMonster, (*iter)->Get_TextureMonsterKey()))
				{
					iter++;
					if (iter == CGameObject_Manger::Get_Instance()->Get_list2(CGameObject_Manger::OBJ_MONSTER).end())
					{
						m_bMonsterTurnCheck = false;
						break;
					}
					(*iter)->Set_MonsterTurnCheck(true);
					m_bMonsterTurnCheck = false;
					break;
				}

			}
			m_fTurnDelay = 0.f;
		
	}
}

void CMonster3::Render_GameObject()
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	//pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo_TextureMgr(L"Unit", L"GunDamMK", 0);
	if (nullptr == pTexInfo)
	{
		ERR_MSG(L"Monster3.Cpp, pTexInfo가 nullptr");
		return;
	}

	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	_matrix matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tMonInfo.vPos.x + iScrollX, m_tMonInfo.vPos.y + iScrollY, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	// 선택 시 깜박
	if (m_bSelect && !m_tMonInfo.bTurn)
	{
		m_iAlpha -= 5;

		if (0 >= m_iAlpha)
			m_iAlpha = 255;

		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &_vec3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
		m_bSelect = false;
	}
	else if (m_tMonInfo.bTurn) //턴을 사용했으면.
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

void CMonster3::Release_GameObject()
{
}

void CMonster3::Move_Check()
{
	//////////////////////
	CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_Pos(m_tMonInfo.vPos);

	//////////////////////////////////// 0 == static_iAllCheck //////////////////////////////////
	if (m_tMonInfo.vPos == dynamic_cast<CMousePointer*>(CGameObject_Manger::Get_Instance()->Get_Mouse())->Get_Info().vPos
		&& static_iAllCheck == 0)
	{
		static_iAllCheck = 1;
	}

	///////////////////////////////////// 1 == static_iAllCheck //////////////////////////////////
	if (1 == static_iAllCheck)
	{
		if (!vecTemp.empty())
		{
			vecTemp.clear();
			vecTemp.shrink_to_fit();
		}

		if (vecTemp.empty())
		{
			pTemp = m_pTile->Get_Index(m_tMonInfo.vPos);
			int iIndex = pTemp->iIndex;

			pTemp = m_pTile->Get_TileMove(iIndex);
			pTemp->byOption = 1;
			vecTemp.emplace_back(pTemp);
			/*m_pTile->Set_TileAhpal(iIndex);*/
			//////////////////////////////////오른쪽 직선 거리만 ////////////////////////////////////
			for (int i = 1; i <= m_tMonInfo.iMove; ++i) //가로로 이동거리
			{

				if (/*(iIndex%TILEX)*TILEX <= iIndex + i && ((iIndex%TILEX)+1)*TILEX > iIndex+i &&*/ 0 <= iIndex + i && 900 >= iIndex + i) //오른쪽 이동가능 타일
				{

					if ((iIndex + i) % 30 == 0)
						break;
					pTemp = m_pTile->Get_TileMove(iIndex + i); //오른쪽 이동가능 타일
					pTemp->byOption = 1;
					vecTemp.emplace_back(pTemp);




				}
			}
			////////////////////////왼쪽 직선으로만 /////////////////////////////////////
			for (int i = 1; i <= m_tMonInfo.iMove; ++i)
			{
				if (/*(iIndex%TILEX)*TILEX <= iIndex - i && ((iIndex%TILEX) + 1)*TILEX > iIndex-i &&*/ 0 <= iIndex - i && 900 >= iIndex - i)
				{
					pTemp = m_pTile->Get_TileMove(iIndex - i); //왼쪽 이동가능 타일
					pTemp->byOption = 1;

					vecTemp.emplace_back(pTemp);

					if ((iIndex - i) % 30 == 0)
						break;
				}
			}


			///////////////////////////// 위쪽 올라가면서 양쪽 타일 /////////////////////////////////////////////
			for (int i = 1; i <= m_tMonInfo.iMove; ++i)
			{
				if (0 <= iIndex - i * 30 && 900 >= iIndex - i * 30) //위쪽 이동 가능타일
				{
					pTemp = m_pTile->Get_TileMove(iIndex - i * 30);
					pTemp->byOption = 1;
					vecTemp.emplace_back(pTemp);

					int iTemp = iIndex - i * 30;
					for (int j = 1; j <= m_tMonInfo.iMove; j++)
					{
						if (0 <= iTemp - j && 900 >= iTemp - j)
						{

							pTemp = m_pTile->Get_TileMove(iTemp - j); //한칸 씩 위로 올라가면서 왼쪽 옆 타일도 포함.
							pTemp->byOption = 1;
							vecTemp.emplace_back(pTemp);
							if ((iTemp - j) % 30 == 0)
								break;
						}
					}

					for (int j = 1; j <= m_tMonInfo.iMove; j++) //한칸 씩 위로 올라가면서 오른쪽 옆 타일도 포함.
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


			/////////////////////////////아래로 내려가면서 양쪽 타일 /////////////////////////////////////////////////////////
			for (int i = 1; i <= m_tMonInfo.iMove; ++i)
			{
				if (0 <= iIndex + i * 30 && 900 >= iIndex + i * 30)
				{
					pTemp = m_pTile->Get_TileMove(iIndex + i * 30);//아래 아동 가능타일
					pTemp->byOption = 1;
					vecTemp.emplace_back(pTemp);

					int iTemp = iIndex + i * 30;
					for (int j = 1; j <= m_tMonInfo.iMove; j++)
					{
						if (0 <= iTemp - j && 900 >= iTemp - j)
						{
							pTemp = m_pTile->Get_TileMove(iTemp - j); //한칸 씩 위로 내려가면서 왼쪽 옆 타일도 포함.
							pTemp->byOption = 1;
							vecTemp.emplace_back(pTemp);
						}

						if ((iTemp - j) % 30 == 0)
							break;
					}

					for (int j = 1; j <= m_tMonInfo.iMove; j++)
					{
						if (0 <= iTemp + j && 900 >= iTemp + j)
						{
							if ((iIndex + j) % 30 == 0)
								break;

							pTemp = m_pTile->Get_TileMove(iTemp + j); //한칸 씩 위로 올라가면서 오른쪽 옆 타일도 포함.
							pTemp->byOption = 1;
							vecTemp.emplace_back(pTemp);
						}
					}
				}

			}
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
		}

		//CAstar_Manager::Get_Instance()->Set_AStarMonsterCheck(true); AStar몬스터 판단하기위해했는데. 타일로이동하기에 없앰.
		m_bTileCheck = true;
		m_bMoveTile = true;
		CGameObject_Manger::Get_Instance()->Get_Tile()->Set_TileCheck(m_bTileCheck);
		CGameObject_Manger::Get_Instance()->Get_Tile()->Set_MoveTileCheck(m_bMoveTile);
		m_bTileCheck = false;
		m_bMoveTile = false;

		bAStarMonsterCheck = true;

		static_iAllCheck = 2;
	}

	/////////////////////2 == static_iAllCheck/////////////////////////
	if (2 == static_iAllCheck)
	{
		/*	vector<pair<float, TILE>>	vecDisTemp;*/
		//이동 가능한 타일 돌려서 유닛이있으면, 담지 마라.


		vector<map<int, _vec3>>	vecDisTemp;
		for (auto& pvec : vecTemp)
		{

			for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
			{
				bool bTemp = false;
				for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_MONSTER))
				{
					if (pvec->vPos == _rList->Get_MonsterInfo().vPos)
						bTemp = true;
				}
				if (pvec->vPos == _rList->Get_Info().vPos || bTemp)
					continue;
				map<int, _vec3> mapTemp;
				float fDis = sqrt(pow(pvec->vPos.x - _rList->Get_Info().vPos.x, 2) + pow(pvec->vPos.y - _rList->Get_Info().vPos.y, 2));
				mapTemp.emplace(int(fDis), pvec->vPos);
				vecDisTemp.emplace_back(mapTemp);
			}


		}

		sort(vecDisTemp.begin(), vecDisTemp.end());



		auto& iter = vecDisTemp[0];

		if (CGameObject_Manger::Get_Instance()->Get_Tile()->Get_MoveTileCheck()) //이동 가능한 타일을 먼저 그려주기위해.
		{
			if (m_tMonInfo.vPos == iter.begin()->second)
				bAStarCheck = true;
			else
				CAstar_Manager::Get_Instance()->StartAstar(m_tMonInfo.vPos, iter.begin()->second);

		}
		static_iAllCheck = 3;

	}

	if (bAStarCheck)
	{
		static_iAllCheck = 0;
		GROUND*		m_pTemp = m_pTile->Get_Index(m_tMonInfo.vPos);
		m_tMonInfo.vPos = m_pTemp->vPos;
		CGameObject_Manger::Get_Instance()->Get_Tile()->Set_static_iAllCheck(static_iAllCheck); //이동했으면, 타일 보이는거 꺼주고
		Attack_Check();
		bAStarMonsterCheck = false;
		/*m_tMonInfo.bTurn = true;*/
		bAStarCheck = false;

		if (!vecTemp.empty())
		{
			vecTemp.clear();
			vecTemp.shrink_to_fit();
		}


	}

	////////////////////////////////////////////////////////////
	//if (static_iAllCheck == 3 ) // 
	//{
	//	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
	//	{
	//		if (_rList->Get_MonsterInfo().vPos == CGameObject_Manger::Get_Instance()->Get_Mouse()->Get_Info().vPos)
	//		{
	//			CGameObject* pObj = new CUnitFight;
	//			pObj->Ready_GameObject();
	//			CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UI, pObj);
	//			static_iAllCheck = 8;
	//			m_tInfo = _rList->Get_Info();  //여기가 문제였음. 턴이끝나면, 체력이 다시 이상하게 참 ??? 마징가가 때리면 체력이 다차는거같은데 ?
	//			tTemp = _rList->Get_TextureMonsterKey();
	//		}
	//	}
	//	CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(static_iAllCheck);
	//	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
	//	{
	//		if (!lstrcmp(_rList->Get_TextureKey(), L"UnitFight"))
	//		{
	//			_rList->Set_static_iAllCheck(static_iAllCheck);
	//			_rList->Set_UnitInfo(m_tInfo);
	//			_rList->Set_MonsterInfo(m_tMonInfo);
	//			_rList->Set_TextureUnitKey(m_pTextureUnit);
	//			_rList->Set_TextureMonsterKey(tTemp);
	//			m_bAttaker = true;
	//			break;
	//		}
	//	}
	//}




}

void CMonster3::Attack_Check()
{

	pTemp = m_pTile->Get_Index(m_tMonInfo.vPos);

	int iIndex = pTemp->iIndex;

	pTemp = m_pTile->Get_TileMove(iIndex);
	pTemp->byOption = 1;
	vecTemp.emplace_back(pTemp);
	//////////////////////////////////오른쪽 직선 거리만 ////////////////////////////////////
	for (int i = 1; i <= m_tMonInfo.iMove; ++i) //가로로 이동거리
	{

		if (/*(iIndex%TILEX)*TILEX <= iIndex + i && ((iIndex%TILEX)+1)*TILEX > iIndex+i &&*/ 0 <= iIndex + i && 900 >= iIndex + i) //오른쪽 이동가능 타일
		{

			if ((iIndex + i) % 30 == 0)
				break;
			pTemp = m_pTile->Get_TileMove(iIndex + i); //오른쪽 이동가능 타일
			pTemp->byOption = 1;
			vecTemp.emplace_back(pTemp);
		}
	}
	////////////////////////왼쪽 직선으로만 /////////////////////////////////////
	for (int i = 1; i <= m_tMonInfo.iMove; ++i)
	{
		if (/*(iIndex%TILEX)*TILEX <= iIndex - i && ((iIndex%TILEX) + 1)*TILEX > iIndex-i &&*/ 0 <= iIndex - i && 900 >= iIndex - i)
		{
			pTemp = m_pTile->Get_TileMove(iIndex - i); //왼쪽 이동가능 타일
			pTemp->byOption = 1;

			vecTemp.emplace_back(pTemp);

			if ((iIndex - i) % 30 == 0)
				break;
		}
	}


	///////////////////////////// 위쪽 올라가면서 양쪽 타일 /////////////////////////////////////////////
	for (int i = 1; i <= m_tMonInfo.iMove; ++i)
	{
		if (0 <= iIndex - i * 30 && 900 >= iIndex - i * 30) //위쪽 이동 가능타일
		{
			pTemp = m_pTile->Get_TileMove(iIndex - i * 30);
			pTemp->byOption = 1;
			vecTemp.emplace_back(pTemp);

			int iTemp = iIndex - i * 30;
			for (int j = 1; j <= m_tMonInfo.iMove; j++)
			{
				if (0 <= iTemp - j && 900 >= iTemp - j)
				{

					pTemp = m_pTile->Get_TileMove(iTemp - j); //한칸 씩 위로 올라가면서 왼쪽 옆 타일도 포함.
					pTemp->byOption = 1;
					vecTemp.emplace_back(pTemp);
					if ((iTemp - j) % 30 == 0)
						break;
				}
			}

			for (int j = 1; j <= m_tMonInfo.iMove; j++) //한칸 씩 위로 올라가면서 오른쪽 옆 타일도 포함.
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


	/////////////////////////////아래로 내려가면서 양쪽 타일 /////////////////////////////////////////////////////////
	for (int i = 1; i <= m_tMonInfo.iMove; ++i)
	{
		if (0 <= iIndex + i * 30 && 900 >= iIndex + i * 30)
		{
			pTemp = m_pTile->Get_TileMove(iIndex + i * 30);//아래 아동 가능타일
			pTemp->byOption = 1;
			vecTemp.emplace_back(pTemp);

			int iTemp = iIndex + i * 30;
			for (int j = 1; j <= m_tMonInfo.iMove; j++)
			{
				if (0 <= iTemp - j && 900 >= iTemp - j)
				{
					pTemp = m_pTile->Get_TileMove(iTemp - j); //한칸 씩 위로 내려가면서 왼쪽 옆 타일도 포함.
					pTemp->byOption = 1;
					vecTemp.emplace_back(pTemp);
				}

				if ((iTemp - j) % 30 == 0)
					break;
			}

			for (int j = 1; j <= m_tMonInfo.iMove; j++)
			{
				if (0 <= iTemp + j && 900 >= iTemp + j)
				{
					if ((iIndex + j) % 30 == 0)
						break;

					pTemp = m_pTile->Get_TileMove(iTemp + j); //한칸 씩 위로 올라가면서 오른쪽 옆 타일도 포함.
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

		for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
		{
			if (pvec->vPos == _rList->Get_Info().vPos)
			{
				static_iAllCheck = 6;
				CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(static_iAllCheck);
				CGameObject_Manger::Get_Instance()->Get_Tile()->Set_static_iAllCheck(static_iAllCheck);
				m_bCheckTemp = true;
			}
		}

	}

	if (!m_bCheckTemp) // 다돌고 나서, 타일에 적이 없으면.
	{
		m_tMonInfo.bTurn = true;

		if (!vecTemp.empty())
		{

			vecTemp.clear();
			vecTemp.shrink_to_fit();
		}
		return;
	}


	//////////////////////////////////////////////////////////
	//if (static_iAllCheck == 6) // 
	//{

	//	map<float, CGameObject*> mapDis; //멤버 변수로 줬더니 이전 맵도 남아있어서 추적이 안되는거였는듯. 제일가까운(즉 이동했던곳이 남아서)

	//	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
	//	{
	//		float fDis = sqrt(pow(m_tMonInfo.vPos.x - _rList->Get_Info().vPos.x, 2) + pow(m_tMonInfo.vPos.y - _rList->Get_Info().vPos.y, 2));
	//		mapDis.emplace(fDis, _rList);

	//	}
	//	auto&	iter = mapDis.begin();

	//	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
	//	{
	//		if (_rList->Get_MonsterInfo().vPos == CGameObject_Manger::Get_Instance()->Get_Mouse()->Get_Info().vPos)
	//		{
	//			CGameObject* pObj = new CUnitFight;
	//			pObj->Ready_GameObject();
	//			CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UI, pObj);
	//			static_iAllCheck = 8;
	//			m_tInfo = _rList->Get_Info();  //여기가 문제였음. 턴이끝나면, 체력이 다시 이상하게 참 ??? 마징가가 때리면 체력이 다차는거같은데 ?
	//			tTemp = _rList->Get_TextureMonsterKey();
	//		}
	//	}
	//	CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(static_iAllCheck);
	//	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
	//	{
	//		if (!lstrcmp(_rList->Get_TextureKey(), L"UnitFight"))
	//		{
	//			_rList->Set_static_iAllCheck(static_iAllCheck);
	//			_rList->Set_UnitInfo(m_tInfo);
	//			_rList->Set_MonsterInfo(m_tMonInfo);
	//			_rList->Set_TextureUnitKey(m_pTextureUnit);
	//			_rList->Set_TextureMonsterKey(tTemp);
	//			m_bAttaker = true;
	//			break;
	//		}
	//	}
	//}

	//for (auto& pvec : vecTemp)
	//{

	//	if (pvec->vPos == dynamic_cast<CMousePointer*>(CGameObject_Manger::Get_Instance()->Get_Mouse())->Get_Info().vPos)
	//	{
	//	
	//		//CGameObject_Manger::Get_Instance()->Get_Tile()->Set_static_iAllCheck(2);
	//		CAstar_Manager::Get_Instance()->Set_AStarMonsterCheck(true);
	//		CAstar_Manager::Get_Instance()->StartAstar(m_tMonInfo.vPos, iter->second->Get_Info().vPos);
	//		bAStarMonsterCheck = true;

	//	}
	//}
	map<float, CGameObject*> mapDis; //멤버 변수로 줬더니 이전 맵도 남아있어서 추적이 안되는거였는듯. 제일가까운(즉 이동했던곳이 남아서)

	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UNIT))
	{
		float fDis = sqrt(pow(m_tMonInfo.vPos.x - _rList->Get_Info().vPos.x, 2) + pow(m_tMonInfo.vPos.y - _rList->Get_Info().vPos.y, 2));
		mapDis.emplace(fDis, _rList);

	}
	auto&	iter = mapDis.begin();

	CGameObject* pObj = new CUnitFight;
	pObj->Ready_GameObject();
	CGameObject_Manger::Get_Instance()->Add_GameObject(CGameObject_Manger::OBJ_UI, pObj);
	static_iAllCheck = 8;
	m_tInfo = iter->second->Get_Info();
	tTemp = iter->second->Get_TextureUnitKey();


	CGameObject_Manger::Get_Instance()->Get_Mouse()->Set_static_iAllCheck(static_iAllCheck);
	for (auto& _rList : CGameObject_Manger::Get_Instance()->Get_list(CGameObject_Manger::OBJ_UI))
	{
		if (!lstrcmp(_rList->Get_TextureKey(), L"UnitFight"))
		{
			_rList->Set_static_iAllCheck(static_iAllCheck);
			_rList->Set_UnitInfo(m_tInfo);
			_rList->Set_MonsterInfo(m_tMonInfo);
			_rList->Set_TextureMonsterKey(m_pTextureMonster);
			_rList->Set_TextureUnitKey(tTemp);
			m_bAttaker = true;
			break;
		}
	}

}

void CMonster3::Move_Route()
{
	list<GROUND*>& BestList = CAstar_Manager::Get_Instance()->Get_BestList();


	if (BestList.empty())
	{
		GROUND*		m_pTemp = m_pTile->Get_Index(m_tMonInfo.vPos);
		m_tMonInfo.vPos = m_pTemp->vPos;
		return;
	}
	m_tMonInfo.vDir = BestList.front()->vPos - m_tMonInfo.vPos;
	float fDist = D3DXVec3Length(&m_tMonInfo.vDir);
	D3DXVec3Normalize(&m_tMonInfo.vDir, &m_tMonInfo.vDir);

	cout << fDist << endl;
	if (fDist < 3.0f)
	{
		BestList.pop_front();
		if (BestList.empty())
		{
			CGameObject_Manger::Get_Instance()->Get_Tile()->Set_MoveTileCheck(false); //다이동하면 타일 꺼줌.
			bAStarCheck = true;
		}
	}
	m_tMonInfo.vPos += m_tMonInfo.vDir * CTime_Manager::Get_Instance()->Get_TimeDelta() * 200;

}
