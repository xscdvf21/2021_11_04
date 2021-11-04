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
	_matrix					m_pFinalMatrix[SPHERE_END]; //구 생성 최종 위치행렬.



	_uint					m_iBulletMax = 0;
	_uint					m_iAniNum = 0; //현재 재생되고있는 애니메이션 번호 저장

	_bool					m_bPattern = false; //플레이어가 일정거리안에 들어왔을 시 패턴 시작.
	_bool					m_bColl = false; //플레이어를 공격하기 위한 콜리전.
	_bool					m_bAttack = false; //몬스터가 공격중인지 판단.

	
	_float					m_fCollTime = 0.f;

	_bool					m_bDead = false; //죽으면 모든걸 정지시킴.
	_bool					m_bVictory = false; //죽으면 승리 UI를위해.

	
	_uint					iSelect; //어떤 패턴을 사용할 것 인지 결정를 하기위해.
	_uint					m_iGroundSelect; //땅 패턴에서 어떤것을 쓸것인지 결정하기위해.
	_uint					iBulletMax = 0; //메테오 최대 3개까지만.
	_uint					iTimeBulletMax = 0; //타임 메테오 갯수제한.
	_uint					iBulltMax2 = 0; //입에서 나가는 메테오 한개 나가게 하기위해. 땅패턴에서.

	_float					m_fGroundTime = 0.f; //땅 패턴에서 시간값 저장.
	_float					m_fRunTime = 0.f; //땅패턴에서 앞으로 돌진하기전에 뒤로가기 위한 시간 값.

	_float					m_fAngle = 0.f; //아이들 무브시 회전 최대. 90도.
	_float					m_fDelTime = 0.f; //IdleMove() 사용하는 시간값.

	_float					m_fPatternTime = 0.f; //공격 패턴을 위한 시간 값.
	_float					m_fFireTime = 0.f; // 메테오 공격텀을 위한 시간 값.
	_float					m_fFireTime2 = 0.f; //땅패턴에서, 멈추는 불렛등 시간값
	_float					m_fScream = 0.f; //소리지르기 플레이어 넘어지게 할 시간.
	_float					m_fScreamShake = 0.f; //소리지르기 동안 카메라 쉐이킹



	_float					fTurnSpeed = D3DXToRadian(2);
	_float					vAngle;
	_float					m_fMoveDis = 0.f; //이동 한 거리 누적.
	_float					m_vPlayerDis = 0.f; //플레이어와의 거리.

	_vec3					m_vCurrentPos; //공중 아이들 상태 맨처음의 위치값 저장.

	

	_bool					m_bTurnCheck = false; //어느쪽으로 돌지 판단..
	_bool					m_bTurnAngle = false; //언제까지 돌지 판단. 트루면 왼쪽.

	_bool					m_bFlyStart = false; //보스가 공중전을 위해 공중으로 올라가는 애니메이션.
	_bool					m_bPlayerCheck = false; //플레이어가 체크 되면, 다시함수 안돌게하기위해.
	_bool					m_bBossScream = false; //플레이어 처음 만나면, 소리지르고 그뒤로는 패턴공격 나오게하기위해.
	
	
	_bool					m_bBossDownSound = false; //공중에서 다떨어지고 땅에 떨어졌을때 소리.
	_bool					m_bBossFlyDown = false; //공중에서 떨어질때 소리 지르게.
	_bool					m_bBossScreamSound = false; //소리지르는 사운드 한번만 나게하기위해.
	_bool					m_bBossGroundRunSound = false; //땅패턴 계속 소리나다가 끝날때 안나게하려고
	_bool					m_bSwingSound = false; //땅패턴 스윙 소리 한번만 나게하기위해.
	_bool					m_bBossDieBGM = false; //보스 다죽은 후에 승리 BGM
	_bool					m_bBossDieSound = false; //보스가 죽을때 소리.

	_bool					m_bFlyPattern = true; //공중 패턴을 시작함. 체력70퍼 이하가되면 떨어지게할것.
	_bool					m_bGroundPattern = false; //체력 70이하가되면 떨어지고 땅패턴 시작.
	_bool					m_bDownPattern = true; //체력이 70이하가되면, 떨어지는 패턴.
	_bool					m_bFireSound = false; //공중 패턴 불 뿜는거에서 소리 한번만 나게학위해.

	_bool					m_bBossAttack0 = false; //총알 발사 보스 공중 패턴 나오기 0.3초전에 방향바꾸려고.
	_bool					m_bBossAttack1 = false; //공중 전 돌진 패턴 나오기전에 방향바꾸려고.


	_bool					m_bBossAttack2 = false; //땅 패턴 돌진하기 직전에 방향 한번 구하기 위해서.
	_bool					m_bBossAttack3 = false; //땅 패턴 날개공격 직전에 방향 한번 구하기 위해;

public:
	static CBoss*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void		Free(void) override;



};



#define __BOSS_H__
#endif // !__BOSS_H__



