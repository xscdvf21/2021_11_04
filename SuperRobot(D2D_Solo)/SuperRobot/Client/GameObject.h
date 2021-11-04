#pragma once
class CTile;
class CGameObject abstract
{
public:
	explicit CGameObject();
	virtual ~CGameObject();
public:
	virtual HRESULT Ready_GameObject()PURE;
	virtual int		Update_GameObject()PURE;
	virtual void	LateUpdate_GameObject()PURE;
	virtual void	Render_GameObject()PURE;
	virtual void	Release_GameObject()PURE;

public : //enum


public : //Set
	void Set_UnitSelect() { m_bSelect = true; } //유닛과 마우스커서가 충돌 했을떄
	void Set_Pos(_vec3 _vPos) { m_tInfo.vPos = _vPos; }
	void Set_Dead(bool _bDead) { m_bDead = _bDead; }
	void Set_iSceneNum(int _iSceneNum) { m_iSceneNum = _iSceneNum; }
	void Set_static_iAllCheck(int _iAllCheck) { static_iAllCheck = _iAllCheck; }
	void Set_UnitCheck(bool _UnitCheck) { m_tInfo.bTurn = _UnitCheck; }
	void Set_MonsterCheck(bool _MonsterCheck) { m_tMonInfo.bTurn = _MonsterCheck; }
	void Set_UnitInfo(INFO _INFO) { m_tInfo = _INFO; }
	void Set_MonsterInfo(MONSTERINFO _MONSTERINFO) { m_tMonInfo = _MONSTERINFO; }
	void Set_MonsterHP(MONSTERINFO _MonsterHP) { m_tMonInfo.iHp = _MonsterHP.iHp; }
	void Set_UnitHP(INFO _UNITHP) { m_tInfo.iHp = _UNITHP.iHp; }
	void Set_UnitMaxHP(INFO _UNITMAX) { m_tInfo.iHp = _UNITMAX.iMaxHp; }
	void Set_UnitEnergy(INFO _Energy) { m_tInfo.iEnergy = _Energy.iMaxEnergy; }
	void Set_TextureUnitKey(TCHAR* _TextureKey) { m_pTextureUnit = _TextureKey; }
	void Set_TextureMonsterKey(TCHAR* _TextureKey) { m_pTextureMonster = _TextureKey; }
	void Set_UnitTurn(bool _bTurn) { m_tInfo.bTurn = _bTurn; }
	void Set_MonsterTurn(bool _bTurn) { m_tMonInfo.bTurn = _bTurn; }
	void Set_TileCheck(bool _bTile) { m_bTileCheck = _bTile; }
	void Set_MoveTileCheck(bool _bTile) { m_bMoveTile = _bTile; }
	void Set_Attacker(bool _Attacker) { m_bAttaker = _Attacker; }
	void Set_monster_iAllCheck(int _iAllCheck) { monster_iAllCheck = _iAllCheck; }
	void Set_TurnDelay(float _TurnDelay) { m_fTurnDelay = _TurnDelay; }
	void Set_MonsterTurnCheck(bool _MonTurn) { m_bMonsterTurnCheck = _MonTurn; }
	void Set_UnitSkillCheck(bool _Skill) { m_bUnitSkillCheck = _Skill; }
	
public : //Get
	const INFO&	Get_Info() const { return m_tInfo; }
	const MONSTERINFO& Get_MonsterInfo() const { return m_tMonInfo; }
	const TEXINFO* Get_TexInfo() const { return pTexInfo; }
	const vector<GROUND*>& Get_vecTemp() const{ return vecTemp; }
	const TCHAR* Get_TextureKey() const { return m_pTextureKey; }
	TCHAR* Get_TextureUnitKey()  { return m_pTextureUnit; }
	TCHAR* Get_TextureMonsterKey() { return m_pTextureMonster; }
	const bool& Get_TileCheck() { return m_bTileCheck; }
	const bool& Get_MoveTileCheck() { return m_bMoveTile; }
	const bool&	Get_Attacker() { return m_bAttaker; }
protected:
	INFO				m_tInfo;
	MONSTERINFO			m_tMonInfo;
	const TEXINFO*		pTexInfo;

	CTile*				m_pTile;
	vector<GROUND*>		vecTemp;

	TCHAR*				m_pTextureKey;
	TCHAR*				m_pTextureUnit;
	TCHAR*				m_pTextureMonster;
	TCHAR*				m_pTextureWeapon;

	float				m_fTurnDelay;

	int					m_iAlpha;  //알파 값 조정을 위함
	int					m_iSceneNum = 0; // 씬 넘버 스위치
	int					static_iAllCheck; // 모든 상태를 제어함.
	int					monster_iAllCheck; //몬스터 순서대로 조종하기위해.

	bool				m_bAttaker; //공격자가 누구인지 체크하기위해, 전투 씬 그려줄때 이거랑 같이 둘다 참이면, 공격자.
	bool				m_bUnitMenuCheck; //메뉴바 체크 
	bool				m_bDead;  //객체 죽일지 말지.
	bool				m_bSelect; 
	bool				m_bCheck; //Unit의 상태.
	bool				m_bUnitSelect; // 내가 이유닛을 조종하고 있다.
	bool				m_bTileCheck; //유닛의 이동 가능한 타일. 한번만 담게하려고 (마우스 포인터가 다른객체 지나가면 그 객체 타일을 구하는걸 해결)
	bool				m_bMoveTile; //이동 가능 한 타일을 먼저 보여주기 위해 사용
	bool				m_bMonsterTurnCheck;
	bool				m_bUnitSkillCheck; //정신에서 유닛 스킬을 사용하였는지 판단하기 위해




	vector<_vec3>		m_vVec3Temp;  //이전 위치를 잠시 보관함
public :
	

};

