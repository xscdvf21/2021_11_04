#pragma once
#ifndef __PLAYER_H__

#include "GameObject.h"
#include "Engine_Define.h"
#include "Client_Define.h"

class CPlayer : public CGameObject
{
protected:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer() = default;

public:
	SPHERE_INFO		Get_Collider_Info() { return m_tColInfo; };

public:
	void		Add_CollisionList(CGameObject* pGameObject);
	_bool		Check_CollisionList(CGameObject* pGameObject);
	void		Clear_CollisionList();

public:
	void				Set_CutScene(_bool bState) { m_bCutScene = bState; }

	// 히트
	virtual	void		Set_Hit(HITTYPE eType);
	void				Set_HitEnd(_bool bHitEnd) { m_bHitEnd = bHitEnd; }
	void				Set_Damage(_int iHp);
	void				Set_AttDamage(_int iDamage) { m_iDamage = iDamage; }

	// 넉백
	void		Set_KnockBack(_bool bKnockBack) { m_bKnockback = bKnockBack; }
	void		Set_KnockBackDir(_float3 vDir) { m_vKnockBackDir = vDir; }
	
	// 공격이 가능한 상태인지 아닌지
	void		Set_AttAble(_bool bAttAble) { m_bAttAble = bAttAble; }

	void		Add_UltiStack(_int iStack) { m_iHitCount += iStack; }
	void		Clear_HitAble(const _float fTimeDelta);
	
	
public:
	void			Set_SavePoint(_vec3 vSavePoint) { m_vSavePoint = vSavePoint; }

public:
	_bool		Get_AttAble() { return m_bAttAble; }
	_bool		Get_HitAble() { return m_bHitAble; }
	_int		Get_AttDamage() { return m_iDamage; }

protected:
	virtual void Free() override;

protected:
	SPHERE_INFO		m_tColInfo;
	_bool			m_bAttAble = false;
	_bool			m_bHit = false;
	_bool			m_bHitEnd = false;
	_bool			m_bHitAble = false;
	_bool			m_bKnockback = false;
	_bool			m_bCutScene = false;


	_float3			m_vKnockBackDir = _float3(0.0f, 0.0f, 0.0f);

	

	// 플레이어 HP
	_int			m_iHp = 0;
	_int			m_iMaxHp = 0;
	_float			m_fUltiAttTime = 0.f;
	_uint			m_iHitCount = 0;
	_uint			m_iHitCountMax = 20;
	_float			m_fHitClearTime = 0.0f;
	// Recover
	_bool			m_bRecover = false;
	_float			m_fRecoverGoalTime = 5.0f;
	_float			m_fRecoverTime = 0.0f;
	_int			m_iRecoverHp = 1;

	_int			m_iDamage = 1;
	_vec3			m_vSavePoint;
	

	vector<CGameObject*>	m_vecCollisionList;
};

#define __PLAYER_H__
#endif