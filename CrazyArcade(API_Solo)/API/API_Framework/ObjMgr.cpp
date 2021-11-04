#include "stdafx.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "CollisionMgr.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}


CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::Add_Object(CObj* _pObj, OBJID::ID _eID)
{
	m_listObj[_eID].emplace_back(_pObj);
	
}





void CObjMgr::Update()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		auto& iter = m_listObj[i].begin();
		for (; iter != m_listObj[i].end(); )
		{
			int iEvent = (*iter)->Update();
			if (OBJ_DEAD == iEvent)
			{
				SAFE_DELETE(*iter);
				iter = m_listObj[i].erase(iter);
			}
			else
				++iter;
		}
	}

	//왼쪽 파도들과 상호작용
	/*CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::LEFTPADO], m_listObj[OBJID::YELLOWBLOCK]);*/
/*	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::LEFTPADO], m_listObj[OBJID::REDBLOCK]);*/ // 파도와 충돌하면 블럭 부서짐



	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::LEFTPADO], m_listObj[OBJID::PLAYER]); //파도와 플레이어 충돌 시 플레이어 죽음
	CCollisionMgr::Collision_BoomWave(m_listObj[OBJID::LEFTPADO], m_listObj[OBJID::BULLET]); //파도에 물풍선 닿으면 터짐
	//CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::LEFTPADO], m_listObj[OBJID::TOWNBLOCK]);
	//오른쪽 파도들과 상호작용
	/*CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::RIGHTPADO], m_listObj[OBJID::YELLOWBLOCK]);*/
	//CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::RIGHTPADO], m_listObj[OBJID::REDBLOCK]); // 파도와 충돌하면 블럭 부서짐
	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::RIGHTPADO], m_listObj[OBJID::PLAYER]); //파도와 플레이어 충돌 시 플레이어 죽음
	CCollisionMgr::Collision_BoomWave(m_listObj[OBJID::RIGHTPADO], m_listObj[OBJID::BULLET]); //파도에 물풍선 닿으면 터짐
	/*CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::RIGHTPADO], m_listObj[OBJID::TOWNBLOCK]);*/
	//위쪽 파도들과 상호작용
	//CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::UPPADO], m_listObj[OBJID::YELLOWBLOCK]);
	//CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::UPPADO], m_listObj[OBJID::REDBLOCK]); // 파도와 충돌하면 블럭 부서짐
	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::UPPADO], m_listObj[OBJID::PLAYER]); //파도와 플레이어 충돌 시 플레이어 죽음
	CCollisionMgr::Collision_BoomWave(m_listObj[OBJID::UPPADO], m_listObj[OBJID::BULLET]); //파도에 물풍선 닿으면 터짐
	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::UPPADO], m_listObj[OBJID::TOWNBLOCK]);
	//아래쪽 파도들과 상호작용
	//CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::DOWNPADO], m_listObj[OBJID::YELLOWBLOCK]);
	//CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::DOWNPADO], m_listObj[OBJID::REDBLOCK]); // 파도와 충돌하면 블럭 부서짐
	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::DOWNPADO], m_listObj[OBJID::PLAYER]); //파도와 플레이어 충돌 시 플레이어 죽음
	CCollisionMgr::Collision_BoomWave(m_listObj[OBJID::DOWNPADO], m_listObj[OBJID::BULLET]); //파도에 물풍선 닿으면 터짐
	/*CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::DOWNPADO], m_listObj[OBJID::TOWNBLOCK]);*/


	CCollisionMgr::Collision_PadoItem(m_listObj[OBJID::PLAYER], m_listObj[OBJID::BOOMSIZEITEM]); //폭탄 크기 아이템 충돌
	CCollisionMgr::Collision_SpeedItem(m_listObj[OBJID::PLAYER], m_listObj[OBJID::SPEEDITEM]);//스피드 아이템 충돌
	CCollisionMgr::Collision_BoomItem(m_listObj[OBJID::PLAYER], m_listObj[OBJID::BOOMITEM]); //폭탄 아이템 충돌

	CCollisionMgr::Collision_Boom(m_listObj[OBJID::PLAYER], m_listObj[OBJID::BULLET]); //물폭탄이 겹쳐있는지 판단하기 위해
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::YELLOWBLOCK], m_listObj[OBJID::PLAYER]); // 플레이어가 노란블럭 통과못하도록
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::REDBLOCK], m_listObj[OBJID::PLAYER]);		//플레이어가 빨간블럭 통과못하도록
	CCollisionMgr::Collision_BoomAndPlayer(m_listObj[OBJID::BULLET], m_listObj[OBJID::PLAYER]);// 물폭탄과 플레이어가 충돌 시 물풍선을 못 설치하게 하기위해

	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::TOWNBLOCK], m_listObj[OBJID::PLAYER]); //이렇게 쓰면 밀림.
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::HOUSE], m_listObj[OBJID::PLAYER]);     //플레이어가 집을 통과 못하도록
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::TREE], m_listObj[OBJID::PLAYER]);      //플레이어가 나무를 통과 못하도록

	CCollisionMgr::Collision_PadoAndHouse(m_listObj[OBJID::LEFTPADO], m_listObj[OBJID::HOUSE]); //파도와 집이 충돌 시 삭제
	CCollisionMgr::Collision_PadoAndHouse(m_listObj[OBJID::RIGHTPADO], m_listObj[OBJID::HOUSE]); //파도와 집이 충돌 시 삭제
	CCollisionMgr::Collision_PadoAndHouse(m_listObj[OBJID::DOWNPADO], m_listObj[OBJID::HOUSE]); //파도와 집이 충돌 시 삭제
	CCollisionMgr::Collision_PadoAndHouse(m_listObj[OBJID::UPPADO], m_listObj[OBJID::HOUSE]); //파도와 집이 충돌 시 삭제

	CCollisionMgr::Collision_PadoAndHouse(m_listObj[OBJID::LEFTPADO], m_listObj[OBJID::TREE]); //파도와 나무 충돌 시 삭제
	CCollisionMgr::Collision_PadoAndHouse(m_listObj[OBJID::RIGHTPADO], m_listObj[OBJID::TREE]); //파도와 나무 충돌 시 삭제
	CCollisionMgr::Collision_PadoAndHouse(m_listObj[OBJID::DOWNPADO], m_listObj[OBJID::TREE]); //파도와 나무 충돌 시 삭제
	CCollisionMgr::Collision_PadoAndHouse(m_listObj[OBJID::UPPADO], m_listObj[OBJID::TREE]); //파도와 나무 충돌 시 삭제

	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::YELLOWBLOCK], m_listObj[OBJID::MONSTER]);  //몬스터가 노란블럭 통과못하도록
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::REDBLOCK], m_listObj[OBJID::MONSTER]);		//몬스터가 빨간블럭 통과못하도록
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::HOUSE], m_listObj[OBJID::MONSTER]);		//몬스터가 집을 통과 못하도록
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::TREE], m_listObj[OBJID::MONSTER]);			//몬스터가 나무를 통과 못하도록
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::TOWNBLOCK], m_listObj[OBJID::MONSTER]);	//몬스터가 나무박스 통과 못하도록
	CCollisionMgr::Collision_BoomAndPlayer(m_listObj[OBJID::MONSTERBULLET], m_listObj[OBJID::MONSTER]);// 물폭탄과 몬스터 충돌 시 물풍선을 못 설치하게 하기위해


	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::LEFTPADO], m_listObj[OBJID::MONSTER]);
	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::RIGHTPADO], m_listObj[OBJID::MONSTER]);
	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::UPPADO], m_listObj[OBJID::MONSTER]);
	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::DOWNPADO], m_listObj[OBJID::MONSTER]);

	CCollisionMgr::Collision_PadoAndBoss(m_listObj[OBJID::LEFTPADO], m_listObj[OBJID::BOSSMONSTER]); // 파도와 보스 충돌 시 피 깍임
	CCollisionMgr::Collision_PadoAndBoss(m_listObj[OBJID::RIGHTPADO], m_listObj[OBJID::BOSSMONSTER]);
	CCollisionMgr::Collision_PadoAndBoss(m_listObj[OBJID::UPPADO], m_listObj[OBJID::BOSSMONSTER]);
	CCollisionMgr::Collision_PadoAndBoss(m_listObj[OBJID::DOWNPADO], m_listObj[OBJID::BOSSMONSTER]); // 파도와 보스

	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERLEFTPADO], m_listObj[OBJID::SPEEDITEM]);
	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERLEFTPADO], m_listObj[OBJID::BOOMSIZEITEM]); // 파도와 충돌하면 블럭 부서짐
	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERLEFTPADO], m_listObj[OBJID::BOOMITEM]);

	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERRIGHTPADO], m_listObj[OBJID::SPEEDITEM]);
	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERRIGHTPADO], m_listObj[OBJID::BOOMSIZEITEM]); // 파도와 충돌하면 블럭 부서짐
	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERRIGHTPADO], m_listObj[OBJID::BOOMITEM]); // 파도와 충돌하면 블럭 부서짐

	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERDOWNPADO], m_listObj[OBJID::SPEEDITEM]);
	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERDOWNPADO], m_listObj[OBJID::BOOMSIZEITEM]); // 파도와 충돌하면 블럭 부서짐
	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERDOWNPADO], m_listObj[OBJID::BOOMITEM]); // 파도와 충돌하면 블럭 부서짐

	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERUPPADO], m_listObj[OBJID::SPEEDITEM]);
	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERUPPADO], m_listObj[OBJID::BOOMSIZEITEM]); // 파도와 충돌하면 블럭 부서짐
	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERUPPADO], m_listObj[OBJID::BOOMITEM]); // 파도와 충돌하면 블럭 부서짐


	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::MONSTERLEFTPADO], m_listObj[OBJID::PLAYER]);
	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::MONSTERRIGHTPADO], m_listObj[OBJID::PLAYER]);
	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::MONSTERUPPADO], m_listObj[OBJID::PLAYER]);
	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::MONSTERDOWNPADO], m_listObj[OBJID::PLAYER]);

	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::HOUSE], m_listObj[OBJID::MONSTERBULLET]);
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::TREE], m_listObj[OBJID::MONSTERBULLET]);

	
}

void CObjMgr::Late_Update()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for (auto& pObj : m_listObj[i])
		{
			pObj->Late_Update();
			
			if(m_listObj[i].empty())
				break;

			RENDERID::ID eID = pObj->Get_GroupID();
			m_listRender[eID].emplace_back(pObj);
		}
	}
}

void CObjMgr::Render(HDC _DC)
{
	for (int i = 0; i < RENDERID::END; ++i)
	{
		if (RENDERID::OBJECT == i)
			m_listRender[i].sort(CompareY<CObj*>);

		for (auto& pObj : m_listRender[i])
			pObj->Render(_DC);

		m_listRender[i].clear();
	}
}

void CObjMgr::Release()
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for_each(m_listObj[i].begin(), m_listObj[i].end(), Safe_Delete<CObj*>);
		m_listObj[i].clear();
	}
}

CObj* CObjMgr::Get_Target(CObj* _pObj, OBJID::ID _eID)
{
	if (m_listObj[_eID].empty())
		return nullptr;

	float fRealDis = 0.f;
	CObj* pTarget = nullptr;


	for (auto& pObj : m_listObj[_eID])
	{
		float fX = _pObj->Get_Info().fX - pObj->Get_Info().fX;
		float fY = _pObj->Get_Info().fY - pObj->Get_Info().fY;
		float fDis = sqrtf(fX * fX + fY * fY);

		if (fRealDis > fDis || !pTarget)
		{
			pTarget = pObj;
			fRealDis = fDis;
		}
	}

	return pTarget;
}

void CObjMgr::Delete_ID(OBJID::ID _eID)
{
	for_each(m_listObj[_eID].begin(), m_listObj[_eID].end(), Safe_Delete<CObj*>);
	m_listObj[_eID].clear();
}
