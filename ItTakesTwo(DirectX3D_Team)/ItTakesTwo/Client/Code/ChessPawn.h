#ifndef __CHESSPAWN_H__

#include "Enermy.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CChessPawn : public CEnermy
{

	enum animID {
		Enemy_PlayRoom_ChessBishop_Anticipation_mh,//0	frame:[61]
		Enemy_PlayRoom_ChessBishop_Anticipation_mh_Max,//1	frame:[61]
		Enemy_PlayRoom_ChessBishop_Anticipation_Start,//2	frame:[46]
		Enemy_PlayRoom_ChessBishop_Attack_mh,     //3	frame:[13]
		Enemy_PlayRoom_ChessBishop_Attack_Start,  //4	frame:[31]
		Enemy_PlayRoom_ChessBishop_mh,            //5	frame:[2]
		Enemy_PlayRoom_ChessBishop_Play_Move,     //6	frame:[51]
		Enemy_PlayRoom_ChessBishop_Summon,        //7	frame:[91]
		Enemy_PlayRoom_ChessPawn_Anticipation_mh, //8	frame:[71]
		Enemy_PlayRoom_ChessPawn_Anticipation_mh_Max,//9	frame:[41]
		Enemy_PlayRoom_ChessPawn_Anticipation_Start,//10	frame:[46]
		Enemy_PlayRoom_ChessPawn_Attack_mh,       //11	frame:[13]
		Enemy_PlayRoom_ChessPawn_Attack_Start,    //12	frame:[21]
		Enemy_PlayRoom_ChessPawn_mh,              //13	frame:[71]
		Enemy_PlayRoom_ChessPawn_Play_Move,       //14	frame:[61]
		Enemy_PlayRoom_ChessPawn_Summon,          //15	frame:[61]
		Enemy_PlayRoom_ChessRook_Anticipation_mh, //16	frame:[61]
		Enemy_PlayRoom_ChessRook_Anticipation_Start,//17	frame:[46]
		Enemy_PlayRoom_ChessRook_Attack_Crash,    //18	frame:[6]
		Enemy_PlayRoom_ChessRook_Attack_mh,       //19	frame:[13]
		Enemy_PlayRoom_ChessRook_Attack_Start,    //20	frame:[9]
		Enemy_PlayRoom_ChessRook_mh,              //21	frame:[2]
		Enemy_PlayRoom_ChessRook_Play_Move,       //22	frame:[42]
		Enemy_PlayRoom_ChessRook_Slam_Anticipation_mh,//23	frame:[71]
		Enemy_PlayRoom_ChessRook_Slam_Anticipation_mh_Max,//24	frame:[71]
		Enemy_PlayRoom_ChessRook_Slam_Anticipation_Start,//25	frame:[46]
		Enemy_PlayRoom_ChessRook_Slam_Attack_mh,  //26	frame:[13]
		Enemy_PlayRoom_ChessRook_Slam_Attack_Start,//27	frame:[21]
		Enemy_PlayRoom_ChessRook_Summon,          //28	frame:[91]
		End
	};

	enum SoundID
	{
		Ready0,Ready1, Attack,
		Soudn_End
	};

private:
	explicit CChessPawn(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CChessPawn();

public:
	virtual HRESULT Ready_Object(_uint iTileIndex, _uint iUnitName);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			Move(const _float& fTimeDelta);
public:
	void			Set_Damage(_int iDamage) override;
private:
	void			Dead_State(const _float& fTimeDelta);
	void			PhysicsCreate();
private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

	_bool						m_bCodySelect = false;
	_bool						m_bMaySelect = false;

	_float						m_fTurnSpeed = D3DXToRadian(2); //턴 스피드
	_float						m_fAngle;	 //플레이어와의 각도

	CGameObject*				m_pTargetPlayer = nullptr;

	_uint						m_iAniNum = 0; //현재 재생되고있는 애니메이션 번호 저장
	_float						m_fAttackTime = 0.f; //어택 딜레이 주기 위해.
	_float						m_fMoveStart = 0.f; //공격 스타트 모션과 이동 모션 타이밍 맞추려고.

	_uint						m_iUnitName;

private :
	_bool						m_bSound[Soudn_End] = { false };

private: // EFFECT FUNCTION
	void	Effect_Attack();

private: // EFFECT VARIABLE
	_int	m_iAttackEffect = 0;
	_bool	m_bAttackEffect = false;

public:
	static CChessPawn*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iTileIndex , _uint iUnitName);
	virtual void Free(void)		override;
};

#define __CHESSPAWN_H__
#endif


