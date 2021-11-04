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

	//���� �ĵ���� ��ȣ�ۿ�
	/*CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::LEFTPADO], m_listObj[OBJID::YELLOWBLOCK]);*/
/*	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::LEFTPADO], m_listObj[OBJID::REDBLOCK]);*/ // �ĵ��� �浹�ϸ� �� �μ���



	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::LEFTPADO], m_listObj[OBJID::PLAYER]); //�ĵ��� �÷��̾� �浹 �� �÷��̾� ����
	CCollisionMgr::Collision_BoomWave(m_listObj[OBJID::LEFTPADO], m_listObj[OBJID::BULLET]); //�ĵ��� ��ǳ�� ������ ����
	//CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::LEFTPADO], m_listObj[OBJID::TOWNBLOCK]);
	//������ �ĵ���� ��ȣ�ۿ�
	/*CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::RIGHTPADO], m_listObj[OBJID::YELLOWBLOCK]);*/
	//CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::RIGHTPADO], m_listObj[OBJID::REDBLOCK]); // �ĵ��� �浹�ϸ� �� �μ���
	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::RIGHTPADO], m_listObj[OBJID::PLAYER]); //�ĵ��� �÷��̾� �浹 �� �÷��̾� ����
	CCollisionMgr::Collision_BoomWave(m_listObj[OBJID::RIGHTPADO], m_listObj[OBJID::BULLET]); //�ĵ��� ��ǳ�� ������ ����
	/*CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::RIGHTPADO], m_listObj[OBJID::TOWNBLOCK]);*/
	//���� �ĵ���� ��ȣ�ۿ�
	//CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::UPPADO], m_listObj[OBJID::YELLOWBLOCK]);
	//CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::UPPADO], m_listObj[OBJID::REDBLOCK]); // �ĵ��� �浹�ϸ� �� �μ���
	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::UPPADO], m_listObj[OBJID::PLAYER]); //�ĵ��� �÷��̾� �浹 �� �÷��̾� ����
	CCollisionMgr::Collision_BoomWave(m_listObj[OBJID::UPPADO], m_listObj[OBJID::BULLET]); //�ĵ��� ��ǳ�� ������ ����
	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::UPPADO], m_listObj[OBJID::TOWNBLOCK]);
	//�Ʒ��� �ĵ���� ��ȣ�ۿ�
	//CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::DOWNPADO], m_listObj[OBJID::YELLOWBLOCK]);
	//CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::DOWNPADO], m_listObj[OBJID::REDBLOCK]); // �ĵ��� �浹�ϸ� �� �μ���
	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::DOWNPADO], m_listObj[OBJID::PLAYER]); //�ĵ��� �÷��̾� �浹 �� �÷��̾� ����
	CCollisionMgr::Collision_BoomWave(m_listObj[OBJID::DOWNPADO], m_listObj[OBJID::BULLET]); //�ĵ��� ��ǳ�� ������ ����
	/*CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::DOWNPADO], m_listObj[OBJID::TOWNBLOCK]);*/


	CCollisionMgr::Collision_PadoItem(m_listObj[OBJID::PLAYER], m_listObj[OBJID::BOOMSIZEITEM]); //��ź ũ�� ������ �浹
	CCollisionMgr::Collision_SpeedItem(m_listObj[OBJID::PLAYER], m_listObj[OBJID::SPEEDITEM]);//���ǵ� ������ �浹
	CCollisionMgr::Collision_BoomItem(m_listObj[OBJID::PLAYER], m_listObj[OBJID::BOOMITEM]); //��ź ������ �浹

	CCollisionMgr::Collision_Boom(m_listObj[OBJID::PLAYER], m_listObj[OBJID::BULLET]); //����ź�� �����ִ��� �Ǵ��ϱ� ����
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::YELLOWBLOCK], m_listObj[OBJID::PLAYER]); // �÷��̾ ����� ������ϵ���
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::REDBLOCK], m_listObj[OBJID::PLAYER]);		//�÷��̾ ������ ������ϵ���
	CCollisionMgr::Collision_BoomAndPlayer(m_listObj[OBJID::BULLET], m_listObj[OBJID::PLAYER]);// ����ź�� �÷��̾ �浹 �� ��ǳ���� �� ��ġ�ϰ� �ϱ�����

	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::TOWNBLOCK], m_listObj[OBJID::PLAYER]); //�̷��� ���� �и�.
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::HOUSE], m_listObj[OBJID::PLAYER]);     //�÷��̾ ���� ��� ���ϵ���
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::TREE], m_listObj[OBJID::PLAYER]);      //�÷��̾ ������ ��� ���ϵ���

	CCollisionMgr::Collision_PadoAndHouse(m_listObj[OBJID::LEFTPADO], m_listObj[OBJID::HOUSE]); //�ĵ��� ���� �浹 �� ����
	CCollisionMgr::Collision_PadoAndHouse(m_listObj[OBJID::RIGHTPADO], m_listObj[OBJID::HOUSE]); //�ĵ��� ���� �浹 �� ����
	CCollisionMgr::Collision_PadoAndHouse(m_listObj[OBJID::DOWNPADO], m_listObj[OBJID::HOUSE]); //�ĵ��� ���� �浹 �� ����
	CCollisionMgr::Collision_PadoAndHouse(m_listObj[OBJID::UPPADO], m_listObj[OBJID::HOUSE]); //�ĵ��� ���� �浹 �� ����

	CCollisionMgr::Collision_PadoAndHouse(m_listObj[OBJID::LEFTPADO], m_listObj[OBJID::TREE]); //�ĵ��� ���� �浹 �� ����
	CCollisionMgr::Collision_PadoAndHouse(m_listObj[OBJID::RIGHTPADO], m_listObj[OBJID::TREE]); //�ĵ��� ���� �浹 �� ����
	CCollisionMgr::Collision_PadoAndHouse(m_listObj[OBJID::DOWNPADO], m_listObj[OBJID::TREE]); //�ĵ��� ���� �浹 �� ����
	CCollisionMgr::Collision_PadoAndHouse(m_listObj[OBJID::UPPADO], m_listObj[OBJID::TREE]); //�ĵ��� ���� �浹 �� ����

	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::YELLOWBLOCK], m_listObj[OBJID::MONSTER]);  //���Ͱ� ����� ������ϵ���
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::REDBLOCK], m_listObj[OBJID::MONSTER]);		//���Ͱ� ������ ������ϵ���
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::HOUSE], m_listObj[OBJID::MONSTER]);		//���Ͱ� ���� ��� ���ϵ���
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::TREE], m_listObj[OBJID::MONSTER]);			//���Ͱ� ������ ��� ���ϵ���
	CCollisionMgr::Collision_RectEx(m_listObj[OBJID::TOWNBLOCK], m_listObj[OBJID::MONSTER]);	//���Ͱ� �����ڽ� ��� ���ϵ���
	CCollisionMgr::Collision_BoomAndPlayer(m_listObj[OBJID::MONSTERBULLET], m_listObj[OBJID::MONSTER]);// ����ź�� ���� �浹 �� ��ǳ���� �� ��ġ�ϰ� �ϱ�����


	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::LEFTPADO], m_listObj[OBJID::MONSTER]);
	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::RIGHTPADO], m_listObj[OBJID::MONSTER]);
	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::UPPADO], m_listObj[OBJID::MONSTER]);
	CCollisionMgr::Collision_PadoAndPlayer(m_listObj[OBJID::DOWNPADO], m_listObj[OBJID::MONSTER]);

	CCollisionMgr::Collision_PadoAndBoss(m_listObj[OBJID::LEFTPADO], m_listObj[OBJID::BOSSMONSTER]); // �ĵ��� ���� �浹 �� �� ����
	CCollisionMgr::Collision_PadoAndBoss(m_listObj[OBJID::RIGHTPADO], m_listObj[OBJID::BOSSMONSTER]);
	CCollisionMgr::Collision_PadoAndBoss(m_listObj[OBJID::UPPADO], m_listObj[OBJID::BOSSMONSTER]);
	CCollisionMgr::Collision_PadoAndBoss(m_listObj[OBJID::DOWNPADO], m_listObj[OBJID::BOSSMONSTER]); // �ĵ��� ����

	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERLEFTPADO], m_listObj[OBJID::SPEEDITEM]);
	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERLEFTPADO], m_listObj[OBJID::BOOMSIZEITEM]); // �ĵ��� �浹�ϸ� �� �μ���
	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERLEFTPADO], m_listObj[OBJID::BOOMITEM]);

	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERRIGHTPADO], m_listObj[OBJID::SPEEDITEM]);
	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERRIGHTPADO], m_listObj[OBJID::BOOMSIZEITEM]); // �ĵ��� �浹�ϸ� �� �μ���
	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERRIGHTPADO], m_listObj[OBJID::BOOMITEM]); // �ĵ��� �浹�ϸ� �� �μ���

	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERDOWNPADO], m_listObj[OBJID::SPEEDITEM]);
	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERDOWNPADO], m_listObj[OBJID::BOOMSIZEITEM]); // �ĵ��� �浹�ϸ� �� �μ���
	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERDOWNPADO], m_listObj[OBJID::BOOMITEM]); // �ĵ��� �浹�ϸ� �� �μ���

	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERUPPADO], m_listObj[OBJID::SPEEDITEM]);
	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERUPPADO], m_listObj[OBJID::BOOMSIZEITEM]); // �ĵ��� �浹�ϸ� �� �μ���
	CCollisionMgr::Collision_PadoAndBlock(m_listObj[OBJID::MONSTERUPPADO], m_listObj[OBJID::BOOMITEM]); // �ĵ��� �浹�ϸ� �� �μ���


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
