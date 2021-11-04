#pragma once

typedef struct tagTexInfo
{
	LPDIRECT3DTEXTURE9 pTexture; // hbitmap  과 같은 역할. 텍스쳐 한장을 제어하기 위한 컴객체. 
	D3DXIMAGE_INFO tImageInfo; // 이미지의 정보를 가지고 있을 구조체. 
}TEXINFO;
typedef struct tagTile
{

	D3DXVECTOR3 vPos; 
	D3DXVECTOR3 vSize; 
	BYTE		byDrawID; 
	BYTE		byOption; 

}TILE;

typedef struct tagGround
{
	DWORD		dwIndex;
	DWORD		dwParentIndex;
	_vec3		vPos;
	_vec3		vSize;
	int			iIndex;
	BYTE		byOption;
}GROUND;


typedef struct tagUnitInfo
{
#ifdef _AFX
	CString strName; 
#else
	wstring strName;
#endif

	int		iAtt;
	int		iMaxHp; 
	BYTE	byClass;
	BYTE	byItem;
}UNITINFO;
typedef struct tagInfo
{

	D3DXVECTOR3 vPos;  //유닛 위치.
	D3DXVECTOR3 vSize; //유닛 사이즈
	_vec3		vDir;  //거리계산.


	int		iEnergy;	//유닛 기력
	int		iMaxEnergy;	//유닛 최대기력
	int		iArmor;		//유닛 방어력
	int		iHp;		//현재 체력
	int		iMaxHp;     //유닛 체력
	int		iExp;		//유닛 현재경험치
	int		iMaxExp;	//유닛 레벨업 경험치
	int		iLevel;		//유닛 레벨
	int		iAttack;	//유닛 공격력
	int		iMove;		//유닛 이동력

	int		iRange;		//무기 사정거리

	TCHAR	szUnitName[64] = L""; //유닛의 기체이름
	TCHAR	szName[64] = L"";	//유닛의 파일럿 이름
	TCHAR	szToking[64] = L""; //유닛의 대사.
	TCHAR	szToking2[64] = L""; //유닛의 방어대사.
	TCHAR	szWeaponName[64] = L""; //무기 이름



	bool	bTurn;		//유닛의 턴

}INFO;



typedef struct tagMonsterInfo
{

	D3DXVECTOR3 vPos;  //유닛 위치.
	D3DXVECTOR3 vSize; //유닛 사이즈
	_vec3		vDir;

	
	int		iEnergy;	//유닛 기력
	int		iArmor;		//유닛 방어력
	int		iHp;		//현재 체력
	int		iMaxHp;     //유닛 체력
	int		iExp;		//유닛 현재경험치
	int		iMaxExp;	//유닛 레벨업 경험치
	int		iLevel;		//유닛 레벨
	int		iAttack;	//유닛 공격력
	int		iMove;		//유닛 이동력
	int		iGold;		//유닛의 골드(몸값)
	TCHAR	szUnitName[64] = L""; //유닛의 기체이름
	TCHAR	szName[64] = L"";	//유닛의 파일럿 이름
	TCHAR	szToking[64] = L""; //유닛의 공격대사.
	TCHAR	szToking2[64] = L""; //유닛의 방어대사.
	bool	bTurn;		//유닛의 턴

}MONSTERINFO;

typedef struct tagFrame
{
	float fFrameStart; 
	float fFrameEnd; 
}FRAME;
typedef struct tagPathInfo
{
	wstring wstrPath = L""; 
	wstring wstrObjectKey = L""; 
	wstring wstrStateKey = L""; 
	DWORD dwCount = 0; 

}PATHINFO;