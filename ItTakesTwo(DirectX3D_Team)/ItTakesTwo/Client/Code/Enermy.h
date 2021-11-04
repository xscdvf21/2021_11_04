#pragma once
#ifndef __ENERMY_H__

#include "GameObject.h"
#include "Client_Define.h"
#include "engine_define.h"
#include "ImageUI.h"

class CEnermy : public CGameObject
{
protected:
	explicit CEnermy(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEnermy() = default;

public:
	SPHERE_INFO		Get_Collider_Info() { return m_tColInfo; }

public:
	void		Add_CollisionList(CGameObject* pGameObject);
	_bool		Check_CollisionList(CGameObject* pGameObject);
	void		Clear_CollisionList();

protected:
	void		Dead_Effect();
	void		Dead_Effect2(_bool Shake);
	void		Hit_Effect(_vec3 vPos, _float fSize);
	void		Hit_Damage(_float fPosY);
	void		Hit_Rim();
	void		Hit_RimLight(const _float& fTimeDelta);

public:
	virtual		void		Tick_Damage(const _float& fTimeDelta);
	void		Set_TickDamage(const _bool& bState, const _int& iDamage, const _bool& bIsFinite = true, const _float& fTickDuration = 10.f, const _float& fTickCool = 0.1f);
	void		Set_TickOff(const _bool& bState);

public:
	virtual void		Set_Damage(_int iHp);
	void				Set_AttAble(_bool bAttAble) { m_bAttAble = bAttAble; }

	_int				Get_Hp() { return m_iHP; }

public:
	_bool		Get_AttAble() { return m_bAttAble; }

protected:
	virtual void Free() override;

protected:
	SPHERE_INFO					m_tColInfo;
	_int						m_iHP = 5;
	_int						m_iHPMax = 100;
	//Hit
	_bool						m_bHit = false;
	_bool						m_bAttAble = false;

	_bool						m_bTickFinite = false; 
	_bool						m_bTick = false;
	_int						m_iTickDamge = 0;
	_float						m_fTickTime = 0.f;
	_float						m_fRimLightTime = 0.f;
	_bool						m_bRimLight = false;

	vector<CGameObject*>		m_vecCollisionList;

protected:
	void		Create_DefaultUI(_float fSize, _float fPosY);
	void		Update_DefaultUI(const _float& fTimeDelta);
	void		Delete_UI();

protected:
	CParticle*	m_pEnermyHp = nullptr;
	CParticle*	m_pEnermyMaxHp = nullptr;
	_float		m_fPosY = 0.f;
};

#define __ENERMY_H__
#endif