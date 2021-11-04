#ifndef __BOSS_H__

#include "GameObject.h"
#include "define.h"
#include "Base.h"
#include "Player.h"
#include "BossBullet.h"
#include "BossTimeBullet.h"
BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CNaviMesh;
class CSphere;
END

class CBoss : public CGameObject
{
	
private:
	explicit CBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;


public :
	_bool				Get_Scream() { return m_bBossScream; }
	_bool				Get_PlayerCheck() { return m_bPlayerCheck; }
	_bool				Get_Dead() { return m_bDead; }


	_bool				Get_Victory() { return m_bVictory; }
private:
	HRESULT				Add_Component(void);
	void				SetUp_Sphere(void);
	void				SetUp_AttackSphere(void);
	_bool				PlayerCheck();

	HRESULT				Boss_Move(const _float& fTimeDelta);
	HRESULT				IdleBoss_Move(const _float& fTimeDelta);

	

	
	_bool			Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag, const _float& fTimeDelta);
private :
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CNaviMesh*		m_pNaviMeshCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CSphere*		m_pSphere = nullptr;
	Engine::CSphere*		m_pAttackSphere = nullptr;


	const _matrix*			m_pParentBoneMatrix[SPHERE_END];
	const _matrix*			m_pParentWorldMatrix = nullptr;
	_matrix					m_pFinalMatrix[SPHERE_END]; //�� ���� ���� ��ġ���.



	_uint					m_iBulletMax = 0;
	_uint					m_iAniNum = 0; //���� ����ǰ��ִ� �ִϸ��̼� ��ȣ ����

	_bool					m_bPattern = false; //�÷��̾ �����Ÿ��ȿ� ������ �� ���� ����.
	_bool					m_bColl = false; //�÷��̾ �����ϱ� ���� �ݸ���.
	_bool					m_bAttack = false; //���Ͱ� ���������� �Ǵ�.

	
	_float					m_fCollTime = 0.f;

	_bool					m_bDead = false; //������ ���� ������Ŵ.
	_bool					m_bVictory = false; //������ �¸� UI������.

	
	_uint					iSelect; //� ������ ����� �� ���� ������ �ϱ�����.
	_uint					m_iGroundSelect; //�� ���Ͽ��� ����� �������� �����ϱ�����.
	_uint					iBulletMax = 0; //���׿� �ִ� 3��������.
	_uint					iTimeBulletMax = 0; //Ÿ�� ���׿� ��������.
	_uint					iBulltMax2 = 0; //�Կ��� ������ ���׿� �Ѱ� ������ �ϱ�����. �����Ͽ���.

	_float					m_fGroundTime = 0.f; //�� ���Ͽ��� �ð��� ����.
	_float					m_fRunTime = 0.f; //�����Ͽ��� ������ �����ϱ����� �ڷΰ��� ���� �ð� ��.

	_float					m_fAngle = 0.f; //���̵� ����� ȸ�� �ִ�. 90��.
	_float					m_fDelTime = 0.f; //IdleMove() ����ϴ� �ð���.

	_float					m_fPatternTime = 0.f; //���� ������ ���� �ð� ��.
	_float					m_fFireTime = 0.f; // ���׿� �������� ���� �ð� ��.
	_float					m_fFireTime2 = 0.f; //�����Ͽ���, ���ߴ� �ҷ��� �ð���
	_float					m_fScream = 0.f; //�Ҹ������� �÷��̾� �Ѿ����� �� �ð�.
	_float					m_fScreamShake = 0.f; //�Ҹ������� ���� ī�޶� ����ŷ



	_float					fTurnSpeed = D3DXToRadian(2);
	_float					vAngle;
	_float					m_fMoveDis = 0.f; //�̵� �� �Ÿ� ����.
	_float					m_vPlayerDis = 0.f; //�÷��̾���� �Ÿ�.

	_vec3					m_vCurrentPos; //���� ���̵� ���� ��ó���� ��ġ�� ����.

	

	_bool					m_bTurnCheck = false; //��������� ���� �Ǵ�..
	_bool					m_bTurnAngle = false; //�������� ���� �Ǵ�. Ʈ��� ����.

	_bool					m_bFlyStart = false; //������ �������� ���� �������� �ö󰡴� �ִϸ��̼�.
	_bool					m_bPlayerCheck = false; //�÷��̾ üũ �Ǹ�, �ٽ��Լ� �ȵ����ϱ�����.
	_bool					m_bBossScream = false; //�÷��̾� ó�� ������, �Ҹ������� �׵ڷδ� ���ϰ��� �������ϱ�����.
	
	
	_bool					m_bBossDownSound = false; //���߿��� �ٶ������� ���� ���������� �Ҹ�.
	_bool					m_bBossFlyDown = false; //���߿��� �������� �Ҹ� ������.
	_bool					m_bBossScreamSound = false; //�Ҹ������� ���� �ѹ��� �����ϱ�����.
	_bool					m_bBossGroundRunSound = false; //������ ��� �Ҹ����ٰ� ������ �ȳ����Ϸ���
	_bool					m_bSwingSound = false; //������ ���� �Ҹ� �ѹ��� �����ϱ�����.
	_bool					m_bBossDieBGM = false; //���� ������ �Ŀ� �¸� BGM
	_bool					m_bBossDieSound = false; //������ ������ �Ҹ�.

	_bool					m_bFlyPattern = true; //���� ������ ������. ü��70�� ���ϰ��Ǹ� ���������Ұ�.
	_bool					m_bGroundPattern = false; //ü�� 70���ϰ��Ǹ� �������� ������ ����.
	_bool					m_bDownPattern = true; //ü���� 70���ϰ��Ǹ�, �������� ����.
	_bool					m_bFireSound = false; //���� ���� �� �մ°ſ��� �Ҹ� �ѹ��� ����������.

	_bool					m_bBossAttack0 = false; //�Ѿ� �߻� ���� ���� ���� ������ 0.3������ ����ٲٷ���.
	_bool					m_bBossAttack1 = false; //���� �� ���� ���� ���������� ����ٲٷ���.


	_bool					m_bBossAttack2 = false; //�� ���� �����ϱ� ������ ���� �ѹ� ���ϱ� ���ؼ�.
	_bool					m_bBossAttack3 = false; //�� ���� �������� ������ ���� �ѹ� ���ϱ� ����;

public:
	static CBoss*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void		Free(void) override;



};



#define __BOSS_H__
#endif // !__BOSS_H__



