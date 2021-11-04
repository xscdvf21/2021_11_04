#pragma once
#ifndef __GAMEOBJECT_H__

#include "Base.h"
BEGIN(Engine)
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(_Device pDevice);
	explicit CGameObject(const CGameObject& other);
	virtual ~CGameObject() = default;

public:
	class CComponent* Get_Component(const wstring& ComponentTag);

public:
	virtual HRESULT Ready_GameObject_Prototype() = 0;
	virtual HRESULT Ready_GameObject(void* pArg = nullptr) = 0;
	virtual _uint Update_GameObject(_float fDeltaTime) = 0;
	virtual _uint LateUpdate_GameObject(_float fDeltaTime) = 0;
	virtual HRESULT Render_GameObject() = 0;

protected:
	HRESULT Quick_Effect(const wstring& LayerTag, NOWINFO _tNowInfo, const int _EffectType
		, bool _bPosCheck = false, const wstring& PrototypeTag = L"GameObject_Effecter");

public:
	void Set_Dead() { bIsDead = true; }
	_bool Get_Dead() { return bIsDead; }
	void Set_PlayerAndMonster() { bPlayerAndMonster = true; }
	void Set_BulletAndMonster() { bBulletAndMonster = true; }
	void Set_BulletAndPlayer() { bBulletAndPlayer = true; }
	void Set_BattleMonsterAndPlayer() { bBattleMonsterAndPlayer = true; }

protected:
	HRESULT Add_Component(_uint iSceneIndex,
		const wstring& PrototypeTag,
		const wstring& ComponentTag,
		class CComponent** ppOut = nullptr,
		void* pArg = nullptr);

	HRESULT Coin_Create(_uint _CoinCount, _float3 _vPos, _uint iSceneIndex, _uint CurrentScene);

public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;

#pragma region Get
public:
	class CTransform*	Get_Transform() { return m_pTransformCom; }
	bool				Get_FallCheck() { return m_bFallCheck; }
	bool				Get_JumpEvent() { return m_bJumpEvent; }

	_uint				Get_NowSceneType() { return m_tNowInfo.iSceneType; }
	_uint				Get_NowEffectType() { return m_tNowInfo.iEffectType; }

	_float				Get_StartPosY() { return fStartPosY; }

	bool				Get_SinkholeCheck() { return m_bSinkholeCheck; }
#pragma endregion

#pragma region Set
public:
	void				Set_X(_float _fX); 
	void				Set_Z(float _fZ); 
	void				Set_Y(float _fY);

	void				Set_ObjPosition(_float3 _vPos);

	void				Set_StartPosY(_float _fStartPosY) { fStartPosY = _fStartPosY; }

	void				Set_Event(bool _EventCheck) { m_bEvent = _EventCheck; }
	
	void				Set_FallCheck(bool _bFallCheck) { m_bFallCheck = _bFallCheck; }
	void				Set_JumpEvent(bool _bCheck) { m_bJumpEvent = _bCheck; } // 이벤트 체크 기능.

	void				Set_NowInfo(NOWINFO _tNowInfo) { m_tNowInfo = _tNowInfo; }
	void				Set_NowSceneType(_uint _iSceneIndex) { m_tNowInfo.iSceneType = _iSceneIndex; }
	void				Set_NowEffectType(_uint _EffectType) { m_tNowInfo.iEffectType = _EffectType; }

	void				Set_SinkholeCheck(bool _bCheck) { m_bSinkholeCheck = _bCheck; }
#pragma endregion

protected:
	typedef unordered_map<wstring, class CComponent*>  COMPONENTS;
	COMPONENTS				m_Components; //컴포넌트의 복제본을 보관

	FRAME					m_tFrame;

	INFO					m_tInfo;
	class CTransform*		m_pTransformCom = nullptr;
	_Device					m_pDevice;
	_bool					bIsDead;				//사망 트리거 체크용

	_float					fStartPosY = 0;

	bool					m_bJumpEvent = false;
	bool					m_bFallCheck = false;
	bool					m_bEvent = false;
	bool					m_bSinkholeCheck = false;

	//정달영					<- 이거 왜 하필 여기 있어요? (팀장)
	PLAYERINFO              m_tPlayerInfo;
	
	_bool                   bPlayerAndMonster; //플레이어와 몬스터
	_bool                   bBulletAndMonster; //플레이어 총알과 몬스터
	_bool					bBulletAndPlayer;  //몬스터 총알과 플레이어,
	_bool					bBattleMonsterAndPlayer; //배틀 몬스터와 플레이어 충돌시
	_bool					bBillboard; //TRUE면 작동.
	NOWINFO					m_tNowInfo;
	
//상태값 조절용
protected :
	_int m_iTextureIndex;
	_bool m_bEventTrigger01;
	_bool m_bEventTrigger02;
	_bool m_bEventTrigger03;

public:
	void Set_TextureIndex(_int _index) { m_iTextureIndex = _index; }
	_int Get_TextureIndex() { return m_iTextureIndex; }

	_bool Get_EventTrigger01() { return m_bEventTrigger01; }
	_bool Get_EventTrigger02() { return m_bEventTrigger02; }
	_bool Get_EventTrigger03() { return m_bEventTrigger03; }

	void Set_EventTrigger01(_bool _event) { m_bEventTrigger01 = _event; }
	void Set_EventTrigger02(_bool _event) { m_bEventTrigger02 = _event; }
	void Set_EventTrigger03(_bool _event) { m_bEventTrigger03 = _event; }

};
END


#define __GAMEOBJECT_H__
#endif // !__GAMEOBJECT_H__
