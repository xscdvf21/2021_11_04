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
	void Set_UnitSelect() { m_bSelect = true; } //���ְ� ���콺Ŀ���� �浹 ������
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

	int					m_iAlpha;  //���� �� ������ ����
	int					m_iSceneNum = 0; // �� �ѹ� ����ġ
	int					static_iAllCheck; // ��� ���¸� ������.
	int					monster_iAllCheck; //���� ������� �����ϱ�����.

	bool				m_bAttaker; //�����ڰ� �������� üũ�ϱ�����, ���� �� �׷��ٶ� �̰Ŷ� ���� �Ѵ� ���̸�, ������.
	bool				m_bUnitMenuCheck; //�޴��� üũ 
	bool				m_bDead;  //��ü ������ ����.
	bool				m_bSelect; 
	bool				m_bCheck; //Unit�� ����.
	bool				m_bUnitSelect; // ���� �������� �����ϰ� �ִ�.
	bool				m_bTileCheck; //������ �̵� ������ Ÿ��. �ѹ��� ����Ϸ��� (���콺 �����Ͱ� �ٸ���ü �������� �� ��ü Ÿ���� ���ϴ°� �ذ�)
	bool				m_bMoveTile; //�̵� ���� �� Ÿ���� ���� �����ֱ� ���� ���
	bool				m_bMonsterTurnCheck;
	bool				m_bUnitSkillCheck; //���ſ��� ���� ��ų�� ����Ͽ����� �Ǵ��ϱ� ����




	vector<_vec3>		m_vVec3Temp;  //���� ��ġ�� ��� ������
public :
	

};

