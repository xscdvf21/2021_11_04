#ifndef Player_h__
#define Player_h__

#include "GameObject.h"
#include "Base.h"
#include "define.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CNaviMesh;
class CSphere;

END

class CPlayer : public Engine::CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

public :
	_bool			Get_AttackCheck();
	_uint			Get_AttackStack();
	_bool			Get_Dash();
	_bool			Get_Inventory() { return m_bInventory; }




	void			Set_FallDown(_bool _bFallDown) { m_bFallDown = _bFallDown; }


private:
	HRESULT			Add_Component(void);
	void			Key_Input(const _float& fTimeDelta);
	void			MouseMove(const _float& fTimeDelta);
	void			Attack(const _float& fTimeDelta);
	void			Dash(const _float& fTimeDelta);
	void			Turn(_uint iKey);
	void			Default();
	void			SetUp_Sphere(void);
	void			SetUp_OnTerrain(void);
	_vec3			PickUp_OnTerrain(void);

	
	void			Inventory();
	HRESULT			WindEffectCreate();





	void			FallDown(const _float& fTimeDelta);
	void			PlayAni(const _float& fTimeDelta); //�ִϸ��̼� ��ſ����� �ð��������ϱ�����.




private:
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CNaviMesh*		m_pNaviMeshCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CSphere*		m_pSphere = nullptr;

	const _matrix*			m_pParentBoneMatrix_Sphere[PLAYER_END];
	const _matrix*			m_pParentWorldMatrix_Sphere = nullptr;
	_matrix					m_pFinalMatrix_Sphere[PLAYER_END];


	_bool					m_bBuffOn = false;
	_bool					m_bAttackSound32 = false; //���� ���� �ѹ����� �鸮���ϱ�����.
	_bool					m_bAttackSound31 = false;
	_bool					m_bAttackSound30 = false;


	_bool					m_bAttackCheck = false; //���������� �ƴ��� üũ.
	_bool					m_bTimeStop = false;



#pragma region �⺻�ʿ� 

	_vec3					m_vDir;
	_float					m_fRAngle = 0;
	_float					m_fLAngle = 0;
	_float					m_fAngle = 0;
	_float					m_fMoveSpeed = 5.f;

	_uint					m_iSelect = 0;
	_uint					m_iAniSet = 0;
	

	_uint					m_iCurrentAttack = 0; //�ð� �ʱ�ȭ�� ���� ���Ӱ��� ���� ������.
	_float					m_iTimeDelta = 0; //���� ������ ���� ������ �ð� ����.
	_float					m_fTimeDeltaSound = 0.f; //���� ����� ���� �ð��� ����
	_uint					m_iAttackStack = 0; //���� ������ ���� ����.


	_uint					m_iCurrentCharge = 0; //���� ���� ���Ӱ��� ���� ������
	_uint					m_iChargeAttackStack = 0; //���� ���� ����.


	_bool					m_bNormalAttackCheck = false;
	_bool					m_bChargeAttackCheck = false;
	_bool					m_bTurnCheck = false; //��������� ���� �Ǵ�..
	_bool					m_bTurnAngle = false; //�������� ���� �Ǵ�. Ʈ��� ����.
	_bool					m_bDash = false;
	_bool					m_bDashStart = true;
	_bool					m_bFallDown = false; //�ٿ� ������ �ٸ��� ���ϰ�.
	_bool					m_bFallDownOne = false; //�ٿ� ���ϴ� �ִϸ��̼� �ѹ��� ����ϱ�����.

	_bool					m_bInventory = false;
	


	_vec3					vCameraLook;
	_vec3					vPlayerLook;
	_vec3					vPlayerRight;
	_vec3					vPlayerLeft;
	_vec3					vPlayerBack;
	_vec3					vPlayerPos;
	_vec3					vCameraPos;
	_vec3					vRightLook;
	_vec3					vLeftLook;
	_vec3					vRightBack;
	_vec3					vLeftBack;

	_vec3					vDir; //�÷��̾�� ī�޶��� �Ÿ�.
	_vec3					vCameraRight;
	_vec3					vCameraLeft;
	_vec3					vCameraBack;

	_vec3					vCameraEye;
	_vec3					vCameraAt;
	_vec3					vCameraUp;




	
	
	_float					fTurnSpeed = D3DXToRadian(3);

	_uint					iKeySelect = 0; //������ Ű ����.
	_float					vAngle;

#pragma endregion
	
public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;

};

#endif // Player_h__
