#include "Randoms.h"


USING(Engine)
uniform_real_distribution<_float> CRandoms::distribution{ 0,0 };
uniform_int_distribution<_int> CRandoms::idistribution{ 0,0 };
mt19937 CRandoms::engine{};
_Binder<_Unforced, uniform_real_distribution<_float>&, mt19937&> CRandoms::fgenerator = bind(CRandoms::distribution, CRandoms::engine);
_Binder<_Unforced, uniform_int_distribution<_int>&, mt19937&> CRandoms::igenerator = bind(CRandoms::idistribution, CRandoms::engine);

CRandoms::CRandoms()
{
}


CRandoms::~CRandoms()
{
}
