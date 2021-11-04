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

//�׷����� ���� �׾����� ó�� ���????? <- �ذ���

enum class EPosisition
{
	NONE, LEFT, RIGHT, UP, DOWN
};

enum class EModeType
{
	ROTATE, SLIDE
};

//���� ����
enum class EWeaponType
{
	BOOTS, HAMMER, DEFAULT
};

enum class EBattleState
{
	//ȸ�����, Ÿ�ټ��� = ���⼱��, ����, �� ����, ȸ�� ���� �Ѹ���, ��������, ����������
	ROTATEMODE, TARGETSELECTMODE, ATTACK, ENEMYATTACK,REROLL, GAMESET, RELEASE
};

//template <typename T>
class CBattlePlate : public CGameObject
{

//�⺻ ���ӿ�����Ʈ ����
private :
	explicit CBattlePlate(_Device pDevice);				//�갡 ����̽� �� ����...?
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
/// �Ʒ� ������
/// </summary>

//�Լ���
public:
// UI��. �긦 ������ ȣ���ؼ� UI �� �����ָ� ��
	_uint Get_RemainCircle();					//������� ���� �� ȸ�� ��
	_uint Get_RemainAttackCount();				//������� ���� ����ȸ��
	_float Get_RemainTimer();					//������� ���� �ð�

	 
public:
//������ ������ Ȯ���ϱ����� �뵵 (enum����)
	EBattleState Get_BattleState() { return eBattleState; }
	EPosisition Get_Position() { return eAPos; }

public:
//�ܺο��� ��ǲ�޴� �Լ�
	HRESULT Add_Monster(const wstring& _pMonsterObject, _uint posY, _uint posX);		//�Ѹ����� �߰��ϴ� �Լ�
	HRESULT Add_Monster_Group(const wstring& _pMonsterObject, _uint posY, _uint posX);		//�� Ÿ�� �߰��� 4������ ��.
	void Set_CamCol(int a);			//ī�޶� ������ ���ư����� ��Ͽ�.

private :
//���� ����� (�����ͷ���)
	HRESULT Control_Plate(_float fDeltaTime);		//���� ���� ������
	HRESULT Search_Target();						//��� �� �׷��� ��������
	HRESULT Hit_Target();							//Hit Target���� ������ ���� ȣ����.

	_bool	Caculate_Position();					//���� üũ
	void Update_BattlePlateVisual(_float fDeltaTime);	//���־�κ� ��������

private:
//��Ʋ ����
//A. ���� ����
	void Select_Spin_Circle();
	void Select_Spin_Slide();

//B. ���� �̵�
	void Move_Spin_Circle();
	void Move_Spin_Slide();

//C. ���� ��� ���� & ����	
	void Hit_Boots();			//��������
	void Hit_Hammer();			//�ظӰ���

//D. ���� ����!
	void Enemy_Turn(float fDeltaTime);			//�⺻ ����������
	_uint Enemy_Attack_Phase(float fDeltaTime);		//�� ����
	void Enemy_Attack(float fDeltaTime);

//E. ������ �������!
	void Battle_Result();

private:
//���� ����� (����Ʈ�Լ� - �־ �ǰ� ��� �Ǵ°�)
	void Copy_PlateState(CSuperMonster* (*dst)[12], CSuperMonster* (*origin)[12]);				//�� ���ư��� ��, origin to dst
	void Clear_PlateState(CSuperMonster* (*dst)[12]);


//������
private:
//���� ������ (�ٽɺ���)
//[m_iRow = posY][m_iCol = posX] <- ����� ����

	CSuperMonster* arrBattlePlate[4][12];		//���� Ÿ���� �����͸� ����... <- ������������ ��ü ������Ÿ�� �����Ƶ�.
	//CSuperMonster* arrBattlePlateOrigin[4][12];	//�ڷ� ���ư��� ���� ��

	_uint m_iRow;		//������
	_uint m_iCol;		//������
	int m_iCamCol;

private:
//���� ������ (�ܺη� �����ų�, ���� ���� �ϴ� �ֵ�)
	//
	float m_fRemainTimer;		//���� �ð�
	_uint m_iRemainCount;		//���� �� ī��Ʈ
	_uint m_iRemainMonster;		//���� ���� �� (���� ȸ��, �� ī��Ʈ ȸ���� ����)
	_uint m_iRemainAttackCount;	//���� ���ݰ���ȸ��


	_bool m_bGameStart;			//��Ʋ �����ߴ�?
	_bool m_bSelectMoveCircle;	//������ �� �����ߴ°�.
	
	_uint m_iRemainEnemyBattle;
	float m_fDefenceFlowTime;	//���潺�� �帣�� �ð�
	float m_fDefenceTime;		//���潺 ���� �ð�
	_bool m_bMonsterTurn;		//�� ���� �ð�!
	_bool m_bNowAttack;			//���� �������ΰ�?


	EBattleState eBattleState;					//���� �÷��� ��� ����
	EModeType eBattleMode;						//���� ���� ��� ���� (ȸ�� vs �����̵�)
	EPosisition	eAPos;							//������ ����
	EWeaponType eWeapon;						//���� ���⸦ ����°�.
	EBattlePlateEffect eBattleEffect;			//� ����Ʈ�� ������̾�� �ϴ°�

private:
	//���� ������ (���� ��� �Ǵ¾ֵ�)
	_bool m_bStopTimerEnable;		//�ð�ä��� ����ΰ�

	float m_fNeedTime;				//���ֿ� �ʿ��� �ð�
	float m_fNowNeedTimer;			//������� ������ �ִ� �ð�.
	
private:
//����Ʈ�� ��ü ������
	class CBattlePlateEffect* pBattlePlateCircle=nullptr;
	class CBattlePlateEffect* pBattlePlateSlide=nullptr;
	class CBattlePlateEffect* pBattlePlateSelectLine=nullptr;
	class CBattlePlateEffect* pBattlePlateSelectCube=nullptr;

//�⺻ �ܺ� ��ü ������
	class CVIBuffer* m_pVIBufferCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;

/// <summary>
/// ������ ��
/// </summary>

};



#endif // !__BATTLEPLATE_TEMPLAET_H__
