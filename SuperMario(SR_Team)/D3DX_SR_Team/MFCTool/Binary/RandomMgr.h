#pragma once
//#include <random>
//#include <iostream>

class CRandomMgr
{

public:
	CRandomMgr();
	virtual ~CRandomMgr() = default;

public:
	float Get_Rand(float _fVal, float _Df);
	float Get_Rand(float _fMaxVal, float _fMinVal, float _Df, float _Pers);
};

