#pragma once
#ifndef CollisionMgr_h__
#define CollisionMgr_h__

#include "Base.h"
#include "Engine_Define.h"
#include "Client_Define.h"


BEGIN(Engine)

class CGameObject;

END
class CCollisionMgr : public CBase
{
	DECLARE_SINGLETON(CCollisionMgr)

public:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr();

public:
	void		Add_EnermyList(class CEnermy* pEnermy);
	void		Add_EnermyBulletList(class CEnermy* pEnermy);
	void		Add_PlayerList(class CPlayer* pPlayer);
	void		Add_PlayerWeaponList(class CPlayer* pPlayerWeapon);
	void		Add_PlayerBulletList(class CPlayer* pBomb);
	void		Add_EnermyWeaponList(class CEnermy* pEnermy);
	void		Add_PlayerSplashBulletList(class CPlayer* pSplashBullet);

	void		Collision_Player_Monster();
	void		Collision_Enermy_Player();
	void		Collision_EnermyWeapon_Player();
	void		Collision_Bullet_Monster();
	void		Collision_SplashBullet_Monster();
	void		Clear_List();

	
	vector<class CEnermy*>*		Get_EnermyList(); 
	vector<class CPlayer*>*		Get_PlayerList() { return &m_vecPlayerList; }
private:
	_bool		Check_Collision_Sphere(SPHERE_INFO tPlayerCollider, SPHERE_INFO tEnermyCollider);
	void		Select_Target(class CPlayer* pPlayer, class CEnermy* pEnermy);
	

public:
	virtual void Free() override;

private:
	vector<class CPlayer*>		m_vecPlayerList;
	vector<class CPlayer*>		m_vecPlayerBulletList;
	vector<class CPlayer*>		m_vecPlayerWeaponList;
	vector<class CPlayer*>		m_vecPlayerSplashBulletList;

	vector<class CEnermy*>		m_vecEnermyList;
	vector<class CEnermy*>		m_vecEnermyBulletList;
	vector<class CEnermy*>		m_vecEnermyWeaponList;
		
};

#endif // CollisionMgr_h__