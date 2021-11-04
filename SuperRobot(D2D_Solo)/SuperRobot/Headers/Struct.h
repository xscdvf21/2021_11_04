#pragma once

typedef struct tagTexInfo
{
	LPDIRECT3DTEXTURE9 pTexture; // hbitmap  �� ���� ����. �ؽ��� ������ �����ϱ� ���� �İ�ü. 
	D3DXIMAGE_INFO tImageInfo; // �̹����� ������ ������ ���� ����ü. 
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

	D3DXVECTOR3 vPos;  //���� ��ġ.
	D3DXVECTOR3 vSize; //���� ������
	_vec3		vDir;  //�Ÿ����.


	int		iEnergy;	//���� ���
	int		iMaxEnergy;	//���� �ִ���
	int		iArmor;		//���� ����
	int		iHp;		//���� ü��
	int		iMaxHp;     //���� ü��
	int		iExp;		//���� �������ġ
	int		iMaxExp;	//���� ������ ����ġ
	int		iLevel;		//���� ����
	int		iAttack;	//���� ���ݷ�
	int		iMove;		//���� �̵���

	int		iRange;		//���� �����Ÿ�

	TCHAR	szUnitName[64] = L""; //������ ��ü�̸�
	TCHAR	szName[64] = L"";	//������ ���Ϸ� �̸�
	TCHAR	szToking[64] = L""; //������ ���.
	TCHAR	szToking2[64] = L""; //������ �����.
	TCHAR	szWeaponName[64] = L""; //���� �̸�



	bool	bTurn;		//������ ��

}INFO;



typedef struct tagMonsterInfo
{

	D3DXVECTOR3 vPos;  //���� ��ġ.
	D3DXVECTOR3 vSize; //���� ������
	_vec3		vDir;

	
	int		iEnergy;	//���� ���
	int		iArmor;		//���� ����
	int		iHp;		//���� ü��
	int		iMaxHp;     //���� ü��
	int		iExp;		//���� �������ġ
	int		iMaxExp;	//���� ������ ����ġ
	int		iLevel;		//���� ����
	int		iAttack;	//���� ���ݷ�
	int		iMove;		//���� �̵���
	int		iGold;		//������ ���(����)
	TCHAR	szUnitName[64] = L""; //������ ��ü�̸�
	TCHAR	szName[64] = L"";	//������ ���Ϸ� �̸�
	TCHAR	szToking[64] = L""; //������ ���ݴ��.
	TCHAR	szToking2[64] = L""; //������ �����.
	bool	bTurn;		//������ ��

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