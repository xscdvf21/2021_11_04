#pragma once
#ifndef __ENGINE_STRUCT_H__
#define __ENGINE_STRUCT_H__
//구조 바꿀까?

typedef struct tagVertexMixed
{
	_float3 vPosition;
	_uint iColor;
	_float2 vUV; // UV좌표.

}VTX_MIX;

const _uint VTX_MIX_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0 | D3DFVF_TEX1;

typedef struct tagVertexColor
{
	_float3 vPosition;
	_uint iColor;
}VTX_COLOR;

const _uint VTX_COLOR_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

typedef struct tagVertexTexture
{
	_float3 vPosition;
	_float2 vUV; // UV좌표.
}VTX_TEXTURE;

const _uint VTX_TEXTURE_FVF = D3DFVF_XYZ | D3DFVF_TEX1/* | D3DFVF_TEXCOORDSIZE2(0)*/;

typedef struct tagVertexCubeColor
{
	_float3 vPosition;
	_uint iColor;
}VTX_CUBECOLOR;

const _uint VTX_CUBECOLOR_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;


typedef struct tagVertexCubeTexture
{
	_float3 vPosition;
	_float3 vUV; // UV좌표.
}VTX_CUBETEXTURE;

const _uint VTX_CUBETEXTURE_FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

typedef struct tagVertexCubeInvisible
{
	_float3 vPosition;
}VTX_CUBEINV;

const _uint VTX_CUBEINVISIBLE_FVF = D3DFVF_XYZ | D3DFVF_XYZ;

typedef struct tagInfo8
{
	WORD _1, _2, _3;
}INDEX8;

typedef struct tagIndex16
{
	WORD _1, _2, _3; /* 폴리곤을 구성할 버텍스들의 "배열 인덱스"를 시계방향(디폴트) 또는 반시계방향으로 보관 */
}INDEX16;

typedef struct tagIndex32
{
	DWORD _1, _2, _3;
}INDEX32;


//한승현.
typedef struct tagFrame
{
	int		iSpriteStart = 0;
	int		iSpriteEnd = 0;
	int		iSpriteNow = 0;

	float	fFrameSpeed = 0;
	float	fFrameMax = 0.f;
	float	fFrameStart = 0.f;

	float	fEventStart = 0.f;
	float	fEventNow = 0.f;
	float	fEventMax = 0.f;
	float	fEventSpeed = 0.f;
	bool	bEventCheck = false;

	DWORD    dwTime;
	DWORD    dwFrameTime;

}FRAME;

typedef struct tagInfo
{
	float	fX = 0;
	float	fY = 0;
	int		iCX = 0;
	int		iCY = 0;

}INFO;

typedef struct tagPlayer
{
	int		iHp;
	int		iMaxHp;
	int		iCoin;
	int		iPaper;
	int		iAttack;

}PLAYERINFO;

typedef struct tagMonster
{
	int		iHp;
	int		iMaxHp;
	int		iAttack;

}MONSTERINFO;

typedef struct tagEffect // 이펙트
{
	_uint			iEffectType;
	D3DXVECTOR3		vPos;

}EFFECTINFO;

typedef struct tagINFO
{
	_uint		iPrototypeType;
	_uint		iSceneType;
	_int		iEffectType;
	_float3		vPos;
	_float3		vDir;

}NOWINFO;


#endif