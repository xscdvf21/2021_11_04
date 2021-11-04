#include "stdafx.h"
#include "RandomMgr.h"


CRandomMgr::CRandomMgr()
{
}

float CRandomMgr::Get_Rand(float _fVal, float _Df)
{
	float fResult = 0;
	return fResult;
}

float CRandomMgr::Get_Rand(float _fMaxVal, float _fMinVal, float _Df, float _Pers)
{
	float fMean = (_Pers * _fMaxVal) + ((1 - _Pers) * _fMinVal);
	float fSize = _fMinVal - _fMaxVal;

	if (_Df > 0)
	{
		return fMean + (1.f - _Df) * fSize;
	}
	else
	{
		return fMean + _Df * fSize;
	}
}
