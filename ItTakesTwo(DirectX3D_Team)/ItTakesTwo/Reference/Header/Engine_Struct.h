#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagVertexColor
	{
		_vec3		vPosition;
		_ulong		dwColor;

	}VTXCOL;

	const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;
	typedef struct tagVertexParticle
	{
		_vec3		vPosition;
		D3DXCOLOR	vColor;
		_float		fSize;

	}VTXPARTICLE;

	const _ulong	FVF_PARTICLE = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0 | D3DFVF_PSIZE;

	typedef struct tagVertexTex
	{
		_vec3		vPosition;		// 12
		_vec3		vNormal;		// 12
		_vec2		vTexUV;			// 8

	}VTXTEX;

	const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexTexScreen
	{
		_vec4		vPosition;		// 12
	//	_vec3		vNormal;		// 12
		_vec2		vTexUV;			// 8

	}VTXTEXSCREEN;

	const _ulong	FVF_TEXSCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1;

	typedef struct tagVertexCube
	{
		_vec3		vPosition;
		_vec3		vTexUV;

	}VTXCUBE;

	const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);	// �ؽ��� UV�� FLOAT�� 3���� ������ ǥ���ϰڴٴ� �ɼ�

	typedef struct tagRoundTexture
	{
		_vec3 vPos;
		_vec3 vScale;
		_float	fAddScale;
		TCHAR szVoxelName[MAX_PATH];
	}ROUNDTEXTURE;

	typedef struct tagVertex_Screen
	{
		_vec4		vPosition;
		_vec2		vTexUV;

	}VTXSCREEN;

	const _ulong FVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1; // RHW ������ ������ ��ġ ���� ǥ���ϴ� �ڷ���


	typedef struct INDEX16
	{
		_ushort		_0;
		_ushort		_1;
		_ushort		_2;

	}INDEX16;

	typedef struct INDEX32
	{
		_ulong		_0;
		_ulong		_1;
		_ulong		_2;

	}INDEX32;

	typedef	struct D3DFRAME_DERIVED : public D3DXFRAME
	{
		_matrix			CombinedTranformationMatrix;	// �θ� ���� ������ ���� ��� ���� �����ϱ� ���� ���

	}D3DXFRAME_DERIVED;

	typedef	struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*		ppTexture;
		LPDIRECT3DTEXTURE9*		ppNmTexture;
		LPDIRECT3DTEXTURE9*		ppMmTexture;
		LPDIRECT3DTEXTURE9*		ppEmTexture;
		LPDIRECT3DTEXTURE9*		ppSmTexture;

		LPDIRECT3DTEXTURE9		pVTFTexture;
		LPD3DXBUFFER			pBoneCombinationBuf;


		LPD3DXMESH				pOriMesh;	// ���� �ε� ������ ���¸� ������ �ִ� �޽� ��ü(�Һ�)
		D3DXATTRIBUTERANGE*		attributeTable;
		_ulong					NumAttributeGroups;
		_ulong					dwNumBones;	// �޽��� ���� ��� ���� ������ ����

		_matrix*				pFrameOffsetMatrix;
		// �ִϸ��̼� ������ ������ ������ ��� ���� 

		_matrix**				ppFrameCombinedMatrix;
		// �� ����ü�� ���� �ִ� CombinedTransformationMatrix�� �ּҰ����� �����ϴ� ������

		_matrix*				pRenderingMatrix;
		// ���������� ����� ���� ����� ����
		// pFrameOffsetMatrix * (*ppFrameCombinedMatrix)�� �����

	}D3DXMESHCONTAINER_DERIVED;

	typedef	struct D3DXMESHCON : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*		ppTexture;

		LPD3DXMESH				pOriMesh;	// ���� �ε� ������ ���¸� ������ �ִ� �޽� ��ü(�Һ�)

		_ulong					dwNumBones;	// �޽��� ���� ��� ���� ������ ����

		_matrix*				pFrameOffsetMatrix;
		// �ִϸ��̼� ������ ������ ������ ��� ���� 

		_matrix**				ppFrameCombinedMatrix;
		// �� ����ü�� ���� �ִ� CombinedTransformationMatrix�� �ּҰ����� �����ϴ� ������

		_matrix*				pRenderingMatrix;
		// ���������� ����� ���� ����� ����
		// pFrameOffsetMatrix * (*ppFrameCombinedMatrix)�� �����

	}D3DXMESHCON;

	// PATH INFORMATION
	typedef struct tagPathInfo
	{
		std::wstring wstrPath = L"";
		std::wstring wstrObjectKey = L"";
		std::wstring wstrStateKey = L"";
		DWORD dwCount = 0;
	}PATHINFO;

	// Terrain Save, Load
	typedef struct tagVecVx
	{
		_vec3 XZScale;
		VTXTEX* pVtx;
		TCHAR	tagTexture[MAX_PATH];
	}VECVTX;

	// Create Tool Object
	typedef struct tagObjectInfo
	{
		_vec3 vPos{};
		_vec3 vScale{};
		_vec3 vAngle{};
		_vec3 vTriggerPos{};
		_vec3 vTriggerScale{};
		_vec3 vTriggerAngle{};
		TCHAR tagMesh[MAX_PATH];
		_uint eMeshType;
		_uint eRenderType;
		_uint eSceneID;
		_tchar* tParentBone;
		_mat* pWorld = nullptr;
		D3DXQUATERNION qQuat;
	}OBJINFO;

	typedef struct tagInfo
	{
		_vec3	vPos{};
		_int	iHp = 0;
		_int	iMaxHp = 0;
		_float	fSpeed = 0.f;
		_float	OPTime = 0.f;
	}OBJECTINFO;

	typedef struct tagNavi
	{
		_tchar	tagFilePath[MAX_PATH];
	}NAVI;

	typedef struct tagUI
	{
		_vec2 PosXY{};
		_vec2 SizeXY{};
		_uint iRender;
		TCHAR tagTexture[MAX_PATH];
		_bool bFade = true;
	}GUI;

	typedef struct tagParticle
	{
		//������ġ
		_vec3 vPos = { 0.f,0.f,0.f };

		//���ӵ�
		_float	fAcc = 0.f;

		//����
		_vec3 vDir = { 0.f,0.f,0.f };

		//�ӵ�
		_float	fVelocity = 0.f;

		//ȸ��
		_vec3 vRot = { 0.f,0.f,0.f };

		//ȸ���ӵ� -���̸� �ݴ�� ȸ��
		_float	fRotateSpeed = 0.f;

		//����ũ��
		_float	fStartSize = 0.f;;
		//����ũ��
		_float	fEndSize = 0.f;

		_vec3	vStartSize = { 0.f,0.f,0.f };
		_vec3	vEndSize = { 0.f,0.f,0.f };

		_float	fCurSize = 0.f;

		D3DXCOLOR	vStartColor = { 0.f,0.f,0.f,0.f };
		D3DXCOLOR	vEndColor = { 0.f,0.f,0.f,0.f };

		D3DXCOLOR	vCurColor = { 0.f,0.f,0.f,0.f };
		_int	iAniIndex = 0;
		_int	iAniX = 0;
		_int	iAniY = 0;
		_float fAniSpeed = 1.f;

		_wstr	tagTextrue = L"";

		_float fLifeTime = 0.f;
		_float fCurrentTime = 0.f;

		_bool bAni = false;
		//Ȱ��ȭ �Ǿ��ִ°�?
		_bool bActive = false;

		_bool bBill = true;
		_bool bGravity = false;
		_bool bRotAdd = false;
		_float fGravityForce = 9.8f;
		_float fAmount = 0.f;
	}PARTICLE;

	typedef struct tagTrampoline
	{
		_vec3 vPos{};
		_vec3 vDst{};
	}TRAM;

	typedef struct tagJelly
	{
		_vec3 vPos{};
		_int iColor;
	}JELLY;

	typedef struct  tagTile
	{
		_vec3 vPos{};
		_uint TileIndex;
	}CHESSTILE;

	typedef struct tagShader
	{
		// RimLight
		_float	m_fRimDist;
		_int	m_iRimPower;
		_vec4	m_vRimColor;
	}SHADER;

	struct FilterGroup
	{
		enum Enum
		{
			ePlayer = (1 << 0),
			eGround = (1 << 1),
			eWall = (1 << 2),
			eHoze = (1 << 3),
			eCody = (1 << 4),
			eMay = (1 << 5),
			eOgre = (1 << 6),
			eStone = (1 << 7),
			eFuse = (1 << 8),
			eDefaultTrigger =(1<<9),
			eHatch = (1 << 10),
			eCamera = (1 << 11),
			eButton = (1 << 12),
			eChessTrigger = (1 << 13),
			eMonster = (1 << 14),
			eArrow = (1 << 15),
			eInteractItem = (1<<16),
			eCrusher = (1<<17),
			eBigFan = (1 << 18),
			eIceOrb = (1 << 19),
			eRideObj = (1 << 20),
			eGravityFloor = (1 << 21),
			eGravityFloorIN = (1 << 22),
			eSpaceTirrger = (1 << 23),
			eRay = (1 << 24),

		};
	};
	struct PxMaterials
	{
		PxMaterials() :
			StaticFriction(0.5f), DynamicFriction(0.5f), Restitution(0.f)
		{

		}
		PxMaterials(const _float& fStatic, const _float& fDynamic, const _float& fRest) :
			StaticFriction(fStatic), DynamicFriction(fDynamic), Restitution(fRest)
		{

		}
		_float StaticFriction;
		_float DynamicFriction;
		_float Restitution;
	};

	typedef struct tagSphereInfo
	{
		_float4x4 matWorld;
		_float fRadius;
	}SPHERE_INFO;
}

#endif // Engine_Struct_h__
