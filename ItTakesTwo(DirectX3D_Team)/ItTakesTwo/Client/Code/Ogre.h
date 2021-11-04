#ifndef __OGRE_H__

#include "Enermy.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END
//������ �̰� ���
//���߿� �浹�ϸ鼭 �ؾ��ҵ�.
//ToyOgre_Hole_LeftHand_Hurt,               //9	frame:[26]
//ToyOgre_Hole_LeftHand_Hurt_Death,         //10	frame:[41]
//ToyOgre_Hole_LeftHand_Hurt_mh,            //11	frame:[37]
//ToyOgre_Hole_RightHand_Hurt,              //14	frame:[26]
//ToyOgre_Hole_RightHand_Hurt_Death,        //15	frame:[41]
//ToyOgre_Hole_RightHand_Hurt_mh,           //16	frame:[37]

class COgre : public CEnermy
{
	enum animID {
		PlayRoom_Castle_Dungeon_Outro_ToyOgre,    //0	frame:[308]
		ToyOgre_Charge,                           //1	frame:[19]
		ToyOgre_Charge_Hit,                       //2	frame:[27]
		ToyOgre_Charge_Start,                     //3	frame:[13]
		ToyOgre_Dizzy,                            //4	frame:[81]
		ToyOgre_Dizzy_Recover,                    //5	frame:[26]
		ToyOgre_HitReaction,                      //6	frame:[11]
		ToyOgre_mh,                               //7	frame:[61]
		ToyOgre_Hole_Enter,                       //8	frame:[31]
		ToyOgre_Hole_LeftHand_Hurt,               //9	frame:[26]
		ToyOgre_Hole_LeftHand_Hurt_Death,         //10	frame:[41]
		ToyOgre_Hole_LeftHand_Hurt_mh,            //11	frame:[37]
		ToyOgre_Hole_LeftHand_Recover,            //12	frame:[32]
		ToyOgre_Hole_mh,                          //13	frame:[37]
		ToyOgre_Hole_RightHand_Hurt,              //14	frame:[26]
		ToyOgre_Hole_RightHand_Hurt_Death,        //15	frame:[41]
		ToyOgre_Hole_RightHand_Hurt_mh,           //16	frame:[37]
		ToyOgre_Hole_RightHand_Recover,           //17	frame:[32]
		ToyOgre_Telegraph_Enter,                  //18	frame:[9]
		ToyOgre_Telegraph_mh,                     //19	frame:[29]
		ToyOgre_Turn_Left,                        //20	frame:[30]
		ToyOgre_Turn_Right,                       //21	frame:[30]
		ToyOgre_Walk,                             //22	frame:[21]
		End
	};

	enum SoundID
	{
		BGM, Sound_Recover, Sound_Stun, Sound_Run, Sound_RunReady,
		Sound_End
	};

private:
	explicit COgre(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~COgre();

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

public:
	void			Set_Damage(_int iDamage) override;
private:
	HRESULT			Add_Component(void* pArg = nullptr);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			Move(const _float& fTimeDelta);

private :
	void			StunSound();
public:
	void			Set_Stun(_bool _bStun) { m_bStun = _bStun; }
	_bool			Get_Stun() { return m_bStun; }

	void			Set_HatchOpne(_bool _bHatch) { m_bHatchOpen = _bHatch; }
	void			Add_StoneCol() { m_iStoneCol++; }

	_uint			Get_StoneCol() { return m_iStoneCol; }
	_float			Get_HatchDir() { return m_fHatchDir; }

private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;
private:
	_bool						m_bSound[Sound_End] = { false };
	_uint						m_iSound[Sound_End] = { 0 };

	_float						m_fSoundTime = 0.f;
private :
	
	_bool						m_bCodySelect = false;
	_bool						m_bMaySelect = false;

	CGameObject*				m_pTargetPlayer = nullptr;


	_float						m_fTurnSpeed = D3DXToRadian(10); //�� ���ǵ�
	_float						m_fAngle;	 //�÷��̾���� ����


	_uint						m_iRandom;
	_uint						m_iAniNum = 0; //���� ����ǰ��ִ� �ִϸ��̼� ��ȣ ����

	_float						m_fCurTime = 0.0f;
	_float						m_fPreTime = 0.0f;

	_uint						m_iSelect = 0; //����.
	_float						m_fPaturnTime = 0.f; //���� �ð�.


	_float						m_fMoveSpeed = 0.f;

private : //ó���� �Ÿ� ����ؼ� ���� �ݱ� ����.
	void						CloseDoor();

	_uint						m_iPlayerCount; //�÷��̾ �Ѵ� ������ ���Դ��� Ȯ���ϱ� ����.
	_bool						m_bCloseDoor;
	_bool						m_bMayCheck;
	_bool						m_bCodyCheck;

private : //�� ������ ���� ù ������ �����ֱ� ����.
	_bool						m_bFirstPattern;

private : //�� ������ ���� ���� ����.
	void						Run(const _float& fTimeDelta, CGameObject* _pTarGet);

	_float						m_fRunReadyTime;
	_float						m_fRunReadyTurnSpeed; //�߱��� �ϸ鼭 �� �ӵ�
	_float						m_fRunSpeed;

	_bool						m_bRun;
	_bool						m_bRunReady;
	_bool						m_bRunning;
private ://���� �� ����
	void						Stun(const _float& fTimeDelta);
	void						StunDamage(const _float& fTimeDelta);

	_uint						m_iStoneCount;
	_uint						m_iStoneCountMax;

	_float						m_fStunTime;
	_float						m_fStoneCreateTime;
	_float						m_fStunDamageRadius;
	_int						m_iStunDamage;


	_bool						m_bStun;
	_bool						m_bBack;

	_uint						m_iKnightCount;
	_uint						m_iKnightCountMax;

	_uint						m_iArbalistCount;
	_uint						m_iArbalistCountMax;

private : //���� �� ��Ŀ����.
	void						Recover(const _float& fTimeDelta);
	_bool						m_bRecover;
private : //Ÿ�� ���ؼ� ���� ��.
	void						JumpTurn(const _float& fTimeDelta, CGameObject* _pTarget);
	
	_float						m_fJumpStartTime;		//�� �ҋ� ���� �ҋ� ������ ���������� ������
	_float						m_fJumpTurnSpeed; //�� ���ǵ�
	
	_bool						m_bJumpTurn = false;
private : //���� �� �߿� ���� ���� �����ϱ�����.
	void						Jump(const _float& fTimeDelta);

	_bool						m_bJump;
	_bool						m_bJumpStart;
	_bool						m_bTest = false;

	_float						m_fJumpPower;
	_float						m_fJumpTime;
	_float						m_fJumpHeight;

private :
	void						DeathJump(const _float& fTimeDelta);
	
	_bool						m_bDeathJump = false;
	_bool						m_bDeathJumpStart = true;
	
	_float						m_fDeathJumpPower = 10.f;
	_float						m_fDeathJumpTime = 0.f; 
	_float						m_fDeathJumpHeight = 0.f;

private :
	_uint						m_iStoneCol; //����� �浹 ���.

private : //��ġ�� �����ִ��� 
	void						HatchDir();
	void						HatchJump(const _float& fTimeDelta);
	
	_bool						m_bHatchOpen;
	_bool						m_bHatchStart;
	_bool						m_bHatchHold;
	_bool						m_bHit;
	_bool						m_bDeadAni;

	_float						m_fHatchTime;
	_float						m_fHactHeight;

	_float						m_fHatchDir;

	_matrix						matWorld4;
public:
	static	COgre*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void				Free(void)		override;
	
};


#define __OGRE_H__
#endif //__OGRE_H__

