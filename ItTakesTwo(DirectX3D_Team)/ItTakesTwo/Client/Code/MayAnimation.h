#pragma once
#ifndef __MAYANIMATION_H__

#include "May.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Client_Define.h"

BEGIN(Engine)

class CDynamicMesh;

END

class CMayAnimation : public CBase
{
	DECLARE_SINGLETON(CMayAnimation)

};

#define __MAYANIMATION_H__
#endif