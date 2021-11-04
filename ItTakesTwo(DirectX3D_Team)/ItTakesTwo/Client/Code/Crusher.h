#ifndef __CRUSHER_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CCrusher : public CGameObject
{
	enum animID {
		PlayRoom_Castle_Dungeon_BridgeCollapse_ToyCrusher,//0	frame:[346]
		PlayRoom_Castle_Dungeon_CrusherIntro_ToyCrusher,//1	frame:[121]
		PlayRoom_Castle_Dungeon_CrushPlayers_ToyCrusher,//2	frame:[46]
		ToyCrusher_FacePoses,                     //3	frame:[75]
		ToyCrusher_MH,                            //4	frame:[33]
		ToyCrusher_Move_Fwd,                      //5	frame:[19]
		ToyCrusher_Smash,                         //6	frame:[25]
		End
	};

	enum SoundID
	{
		BGM,
		StartCry,
		DeadSound,
		Sound_End
	};
private:
	explicit CCrusher(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCrusher();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;

	
public :
	void				Add_HitPoint() { m_iHitPointCount++; }

	void				Set_Move(_bool bMove) { m_bMove = bMove; }
	void				Set_Dead(_bool bDead) { m_bDeadAni = bDead; }
private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	void				Move(const _float& fTimeDelta);
	void				Dead(const _float& fTimeDelta);
	void				DeadMove(const _float& fTimeDelta, const _float& fMoveTime, _vec3 vStartPos, _vec3 vEndPos);

private:
	Engine::CDynamicMesh*			m_pMeshCom = nullptr;
	Engine::CCalculator*			m_pCalculatorCom = nullptr;
	Engine::CCollider*				m_pColliderCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;

private:
	_bool							m_bMove;
	_bool							m_bDeadAni;
	_bool							m_bMoveStart;
	_bool							m_bMoveReady;
	_uint							m_iAniIndex;

private :
	_uint							m_iHitPointCount;

private :
	_bool							m_bSound[Sound_End];

private: // DeadMove()
	_float							m_fCurTime;
	_float							m_fMoveTime;

	_vec3							m_vStartPos;
	_vec3							m_vEndPos;

	_vec3							m_vMoveStartPos;
	_vec3							m_vMoveEndPos;

	_bool							m_bCameraFinger = false;
	_bool							m_bDeadMove;
	CLight*							m_pLight[3];

public:
	static CCrusher*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void					Free(void)		override;


};


#define __CRUSHER_H__
#endif

