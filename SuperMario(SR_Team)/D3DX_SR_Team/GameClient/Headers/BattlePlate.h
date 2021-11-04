#pragma once

#ifndef __BATTLEPLATE_TEMPLAET_H__
#define __BATTLEPLATE_TEMPLAET_H__

#include "GameObject.h"


class CMonster;
class CSuperMonster;

USING(Engine)

enum class EBattlePlateEffect
{
	CIRCLE, SLIDE, LONG, CUBE, NONE
};

//그러고보니 몬스터 죽었을때 처리 어떡함????? <- 해결함

enum class EPosisition
{
	NONE, LEFT, RIGHT, UP, DOWN
};

enum class EModeType
{
	ROTATE, SLIDE
};

//무기 종류
enum class EWeaponType
{
	BOOTS, HAMMER, DEFAULT
};

enum class EBattleState
{
	//회전모드, 타겟선택 = 무기선택, 공격, 적 공격, 회전 랜덤 뿌리기, 게임종료, 빠져나가기
	ROTATEMODE, TARGETSELECTMODE, ATTACK, ENEMYATTACK,REROLL, GAMESET, RELEASE
};

//template <typename T>
class CBattlePlate : public CGameObject
{

//기본 게임오브젝트 세팅
private :
	explicit CBattlePlate(_Device pDevice);				//얘가 디바이스 쓸 일이...?
	explicit CBattlePlate(const CBattlePlate& other);
	virtual ~CBattlePlate() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) override;
	virtual _uint Update_GameObject(_float fDeltaTime) override;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

public:
	static CBattlePlate* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private:
	HRESULT Add_Component();




/// <summary>
/// 아래 개선부
/// </summary>

//함수부
public:
// UI용. 얘를 씬에서 호출해서 UI 에 던져주면 됨
	_uint Get_RemainCircle();					//현재까지 남은 링 회전 수
	_uint Get_RemainAttackCount();				//현재까지 남은 공격회수
	_float Get_RemainTimer();					//현재까지 남은 시간

	 
public:
//씬에서 정보를 확인하기위한 용도 (enum값들)
	EBattleState Get_BattleState() { return eBattleState; }
	EPosisition Get_Position() { return eAPos; }

public:
//외부에서 인풋받는 함수
	HRESULT Add_Monster(const wstring& _pMonsterObject, _uint posY, _uint posX);		//한마리씩 추가하는 함수
	HRESULT Add_Monster_Group(const wstring& _pMonsterObject, _uint posY, _uint posX);		//한 타입 추가시 4마리씩 들어감.
	void Set_CamCol(int a);			//카메라 어디까지 돌아갔는지 기록용.

private :
//내부 연산용 (마스터레벨)
	HRESULT Control_Plate(_float fDeltaTime);		//퍼즐 조립 마스터
	HRESULT Search_Target();						//어느 적 그룹을 때릴건지
	HRESULT Hit_Target();							//Hit Target에서 나머지 둘을 호출함.

	_bool	Caculate_Position();					//판정 체크
	void Update_BattlePlateVisual(_float fDeltaTime);	//비주얼부분 업데이터

private:
//배틀 진행
//A. 퍼즐 선택
	void Select_Spin_Circle();
	void Select_Spin_Slide();

//B. 퍼즐 이동
	void Move_Spin_Circle();
	void Move_Spin_Slide();

//C. 공격 방식 선택 & 공격	
	void Hit_Boots();			//부츠공격
	void Hit_Hammer();			//해머공격

//D. 적의 공격!
	void Enemy_Turn(float fDeltaTime);			//기본 공격페이즈
	_uint Enemy_Attack_Phase(float fDeltaTime);		//적 공격
	void Enemy_Attack(float fDeltaTime);

//E. 게임이 끝났어요!
	void Battle_Result();

private:
//내부 연산용 (서포트함수 - 있어도 되고 없어도 되는거)
	void Copy_PlateState(CSuperMonster* (*dst)[12], CSuperMonster* (*origin)[12]);				//링 돌아가기 용, origin to dst
	void Clear_PlateState(CSuperMonster* (*dst)[12]);


//변수들
private:
//내부 변수들 (핵심변수)
//[m_iRow = posY][m_iCol = posX] <- 헷깔리지 말것

	CSuperMonster* arrBattlePlate[4][12];		//몬스터 타입의 포인터를 가진... <- 보스전에서는 자체 아이템타입 가져아됨.
	//CSuperMonster* arrBattlePlateOrigin[4][12];	//뒤로 돌아가기 구현 용

	_uint m_iRow;		//가로줄
	_uint m_iCol;		//세로줄
	int m_iCamCol;

private:
//내부 변수들 (외부로 나가거나, 상태 조절 하는 애들)
	//
	float m_fRemainTimer;		//남은 시간
	_uint m_iRemainCount;		//남은 링 카운트
	_uint m_iRemainMonster;		//남은 몬스터 수 (공격 회수, 링 카운트 회수에 영향)
	_uint m_iRemainAttackCount;	//남은 공격가능회수


	_bool m_bGameStart;			//배틀 시작했니?
	_bool m_bSelectMoveCircle;	//움직일 링 선택했는가.
	
	_uint m_iRemainEnemyBattle;
	float m_fDefenceFlowTime;	//디펜스중 흐르는 시간
	float m_fDefenceTime;		//디펜스 가능 시간
	_bool m_bMonsterTurn;		//적 공격 시간!
	_bool m_bNowAttack;			//현재 공격중인가?


	EBattleState eBattleState;					//현재 플레이 모드 상태
	EModeType eBattleMode;						//현재 조작 모드 상태 (회전 vs 슬라이드)
	EPosisition	eAPos;							//선택한 방향
	EWeaponType eWeapon;						//무슨 무기를 골랐는가.
	EBattlePlateEffect eBattleEffect;			//어떤 이펙트가 출력중이어야 하는가

private:
	//내부 변수들 (딱히 없어도 되는애들)
	_bool m_bStopTimerEnable;		//시간채우기 모드인가

	float m_fNeedTime;				//도주에 필요한 시간
	float m_fNowNeedTimer;			//현재까지 누르고 있는 시간.
	
private:
//이펙트용 객체 포인터
	class CBattlePlateEffect* pBattlePlateCircle=nullptr;
	class CBattlePlateEffect* pBattlePlateSlide=nullptr;
	class CBattlePlateEffect* pBattlePlateSelectLine=nullptr;
	class CBattlePlateEffect* pBattlePlateSelectCube=nullptr;

//기본 외부 객체 포인터
	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;

/// <summary>
/// 개선부 끝
/// </summary>

};



#endif // !__BATTLEPLATE_TEMPLAET_H__
