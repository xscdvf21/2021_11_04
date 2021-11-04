#pragma once

#ifndef __STRUCT_H__
#define __STRUCT_H__

typedef struct tagInfo
{
	float	fX;
	float	fY;
	int		iCX;
	int		iCY;
}INFO;

typedef struct tagBoomInfo
{
	float fX;
	float fY;

}BOOMINFO;

typedef struct tagLinePos
{
	tagLinePos() { ZeroMemory(this, sizeof(tagLinePos)); }
	tagLinePos(float _x, float _y) : fX(_x), fY(_y) {}
	float	fX;
	float	fY;
}LINEPOS;

typedef struct tagLineInfo
{
	tagLineInfo() { ZeroMemory(this, sizeof(tagLineInfo)); }
	tagLineInfo(LINEPOS _tLeftPos, LINEPOS _tRightPos)
		: tLeftPos(_tLeftPos), tRightPos(_tRightPos)
	{

	}
	LINEPOS		tLeftPos;
	LINEPOS		tRightPos;
}LINEINFO;

typedef struct tagFrame
{
	int		iFrameStart;
	int		iFrameEnd;
	int		iFrameScene;
	DWORD	dwTime;
	DWORD	dwFrameTime;
}FRAME;


#endif // !__STRUCT_H__
