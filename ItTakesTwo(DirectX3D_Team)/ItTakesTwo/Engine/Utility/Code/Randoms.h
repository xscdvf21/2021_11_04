#pragma once
#ifndef Randoms_h__
#define Randoms_h__
#include "Engine_Define.h"
BEGIN(Engine)
class  ENGINE_DLL CRandoms
{
public:
	CRandoms();
	~CRandoms();
public: // operator	

	const _float operator () (const _float& Min, const _float& Max)
	{
		std::mt19937 Tengine((unsigned int)time(NULL) + (unsigned int)(rand() * 10000000000));  // MT19937 ���� ����(seed�� ����)	
		engine = Tengine;
		std::uniform_real_distribution<_float> temp(Min, Max);
		distribution = temp;
		fgenerator = bind(distribution, engine); // ���� ���� �Լ�
		return fgenerator();
	}
	const _int operator () (const _int& Min, const _int& Max)
	{
		std::mt19937 Tengine((unsigned int)time(NULL) + (unsigned int)(rand()* 100000));  // MT19937 ���� ����(seed�� ����)	
		engine = Tengine;
		std::uniform_int_distribution<_int> temp(Min, Max);
		idistribution = temp;
		igenerator = bind(idistribution, engine); // ���� ���� �Լ�
		return igenerator();
	}


public:

	static std::mt19937 engine;
	static std::uniform_real_distribution<_float> distribution;
	static std::uniform_int_distribution<_int> idistribution;
	static std::_Binder<std::_Unforced, std::uniform_real_distribution<_float>&, std::mt19937&> fgenerator;
	static std::_Binder<std::_Unforced, std::uniform_int_distribution<_int>&, std::mt19937&> igenerator;
};
END
#endif // Randoms_h__

