#include "stdafx.h"
#include "CollisionMgr.h"
#include "Enermy.h"
#include "Player.h"
#include "ChessKing.h"
#include "ChessKnight.h"
#include "Knight.h"
#include "May_Castle.h"
#include "ChessTile.h"
#include "VacuumBomb.h"
#include "Export_Function.h"

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
	Free();
}

void CCollisionMgr::Add_EnermyList(CEnermy* pMonster)
{
	m_vecEnermyList.emplace_back(pMonster);
	//pMonster->AddRef();
}

void CCollisionMgr::Add_EnermyBulletList(CEnermy * pEnermy)
{
	m_vecEnermyBulletList.emplace_back(pEnermy);
}

void CCollisionMgr::Add_PlayerList(CPlayer * pPlayer)
{
	m_vecPlayerList.emplace_back(pPlayer);
	//pPlayer->AddRef();
}

void CCollisionMgr::Add_PlayerWeaponList(CPlayer * pPlayerWeapon)
{
	m_vecPlayerWeaponList.emplace_back(pPlayerWeapon);
	//pPlayerWeapon->AddRef();
}

void CCollisionMgr::Add_PlayerBulletList(CPlayer * pBomb)
{
	m_vecPlayerBulletList.emplace_back(pBomb);
}

void CCollisionMgr::Add_EnermyWeaponList(CEnermy * pEnermy)
{
	m_vecEnermyWeaponList.emplace_back(pEnermy);
}

void CCollisionMgr::Add_PlayerSplashBulletList(CPlayer * pSplashBullet)
{
	m_vecPlayerSplashBulletList.emplace_back(pSplashBullet);
}

void CCollisionMgr::Collision_Player_Monster()
{
	SPHERE_INFO tPlayerCollider, tEnermyCollider;

	for (auto& pPlayer : m_vecPlayerWeaponList)
	{
		tPlayerCollider = pPlayer->Get_Collider_Info();
		for (auto& pEnermy : m_vecEnermyList)
		{
			tEnermyCollider = pEnermy->Get_Collider_Info();

			if (!pPlayer->Get_AttAble())
				continue;

			if ((pPlayer->Check_CollisionList(pEnermy)))
				continue;

			if(Check_Collision_Sphere(tPlayerCollider, tEnermyCollider))
			{


				pEnermy->Set_Damage(-pPlayer->Get_AttDamage());
				pPlayer->Add_CollisionList(pEnermy);

				CPlayer* pMay = (CPlayer*)Engine::Get_GameObject(L"GameObject", L"May");
				pMay->Add_UltiStack(1);
				
			}

		}
	}
}

void CCollisionMgr::Collision_Enermy_Player()
{
	SPHERE_INFO tPlayerCollider, tEnermyCollider;

	for (auto& pPlayer : m_vecPlayerList)
	{
		tPlayerCollider = pPlayer->Get_Collider_Info();
		for (auto& pEnermy : m_vecEnermyBulletList)
		{
			tEnermyCollider = pEnermy->Get_Collider_Info();

			if (Check_Collision_Sphere(tPlayerCollider, tEnermyCollider))
			{
				pEnermy->Set_Dead();
				pPlayer->Set_Hit(HITTYPE::DEFAULT);
			}

		}
	}
}

void CCollisionMgr::Collision_EnermyWeapon_Player()
{
	SPHERE_INFO tPlayerCollider, tEnermyCollider;

	for (auto& pPlayer : m_vecPlayerList)
	{
		tPlayerCollider = pPlayer->Get_Collider_Info();
		for (auto& pEnermy : m_vecEnermyWeaponList)
		{
			tEnermyCollider = pEnermy->Get_Collider_Info();

			if (!pEnermy->Get_AttAble())
				continue;

			if ((pEnermy->Check_CollisionList(pPlayer)))
				continue;

			if(pPlayer->Get_HitAble())
				continue;

			if (Check_Collision_Sphere(tPlayerCollider, tEnermyCollider))
			{
				Select_Target(pPlayer, pEnermy);
			}

		}
	}
}

void CCollisionMgr::Collision_Bullet_Monster()
{
	SPHERE_INFO tPlayerCollider, tEnermyCollider;

	for (auto& pPlayer : m_vecPlayerBulletList)
	{
		tPlayerCollider = pPlayer->Get_Collider_Info();
		for (auto& pEnermy : m_vecEnermyList)
		{
			tEnermyCollider = pEnermy->Get_Collider_Info();

			if (Check_Collision_Sphere(tPlayerCollider, tEnermyCollider))
			{
				
				pPlayer->Set_Dead(true);
				pEnermy->Set_Damage(-pPlayer->Get_AttDamage());
			}

		}
	}
}

void CCollisionMgr::Collision_SplashBullet_Monster()
{
	SPHERE_INFO tPlayerCollider, tEnermyCollider;

	for (auto& pPlayer : m_vecPlayerSplashBulletList)
	{
		tPlayerCollider = pPlayer->Get_Collider_Info();
		for (auto& pEnermy : m_vecEnermyList)
		{
			tEnermyCollider = pEnermy->Get_Collider_Info();

			if (Check_Collision_Sphere(tPlayerCollider, tEnermyCollider))
			{
				pPlayer->Set_Dead(true);
			}

		}
	}
}

_bool CCollisionMgr::Check_Collision_Sphere(SPHERE_INFO tPlayerCollider, SPHERE_INFO tEnermyCollider)
{
	_float3 vPlayerColliderPos, vEnermyColliderPos;
	_float fDiffRadius, fDiffPos;
	vPlayerColliderPos = _float3(tPlayerCollider.matWorld._41, tPlayerCollider.matWorld._42, tPlayerCollider.matWorld._43);
	vEnermyColliderPos = _float3(tEnermyCollider.matWorld._41, tEnermyCollider.matWorld._42, tEnermyCollider.matWorld._43);

	// 거리
	_float3 vDis = vPlayerColliderPos - vEnermyColliderPos;
	fDiffPos = fabs(D3DXVec3Length(&vDis));
	fDiffRadius = tPlayerCollider.fRadius + tEnermyCollider.fRadius;

	if (fDiffPos <= fDiffRadius)
		return true;

	return false;
}

void CCollisionMgr::Select_Target(class CPlayer* pPlayer, class CEnermy* pEnermy)
{
	_float3 vPlayerPos, vEnermyPos;

	// 체스킹일때 판정
	if (typeid(*pEnermy) == typeid(CChessKing))
	{
		pPlayer->Set_Hit(HITTYPE::CHESSBOSS);
		pPlayer->Set_Damage(-1);

		// 플레이어가 메이일 경우
		if (typeid(*pPlayer) == typeid(CMayCastle))
		{
			CTransform* pMayTransform = (CTransform*)Get_Component(L"GameObject", L"May", L"Com_Transform", ID_DYNAMIC);
			NULL_CHECK_RETURN(pMayTransform, );

			CTransform* pChessKingTransform = (CTransform*)Get_Component(L"GameObject", L"Chess_King", L"Com_Transform", ID_DYNAMIC);
			NULL_CHECK_RETURN(pChessKingTransform, );

			vPlayerPos = pMayTransform->Get_Position();
			vEnermyPos = pChessKingTransform->Get_Position();

			_float3 vDir = vPlayerPos - vEnermyPos;
			vDir.y = 0.0f;
			D3DXVec3Normalize(&vDir, &vDir);

			pPlayer->Set_KnockBackDir(vDir);
			pEnermy->Add_CollisionList(pPlayer);
		}
		else
		{
			CTransform* pMayTransform = (CTransform*)Get_Component(L"GameObject", L"Cody", L"Com_Transform", ID_DYNAMIC);
			NULL_CHECK_RETURN(pMayTransform, );

			CTransform* pChessKingTransform = (CTransform*)Get_Component(L"GameObject", L"Chess_King", L"Com_Transform", ID_DYNAMIC);
			NULL_CHECK_RETURN(pChessKingTransform, );

			vPlayerPos = pMayTransform->Get_Position();
			vEnermyPos = pChessKingTransform->Get_Position();

			_float3 vDir = vPlayerPos - vEnermyPos;
			vDir.y = 0.0f;
			D3DXVec3Normalize(&vDir, &vDir);

			pPlayer->Set_KnockBackDir(vDir);
			pEnermy->Add_CollisionList(pPlayer);
		}

	}
	else if (typeid(*pEnermy) == typeid(CChessKnight))
	{
		pPlayer->Set_Hit(HITTYPE::CHESSBOSS);
		pPlayer->Set_Damage(-1);

		//if (typeid(*pPlayer) == typeid(CMayCastle))
		//{
			_float3 vDir = CChessTile::GetInstance()->Get_ChessKnightDir();
			vDir.y = 0.0f;
			D3DXVec3Normalize(&vDir, &vDir);

			pPlayer->Set_KnockBackDir(vDir);
			pEnermy->Add_CollisionList(pPlayer);
		//}
	}
	// 나머지
	else
	{
		pPlayer->Set_Damage(-1);
		pPlayer->Set_Hit(HITTYPE::DEFAULT);
		pEnermy->Add_CollisionList(pPlayer);
	}
}

void CCollisionMgr::Clear_List()
{
	m_vecEnermyBulletList.clear();
	m_vecEnermyWeaponList.clear();
	m_vecEnermyList.clear();
	m_vecPlayerList.clear();
	m_vecPlayerWeaponList.clear();
	m_vecPlayerBulletList.clear();
	m_vecPlayerSplashBulletList.clear();
}

vector<CEnermy*>* CCollisionMgr::Get_EnermyList()
{
	return &m_vecEnermyList;
}

void CCollisionMgr::Free()
{
}
