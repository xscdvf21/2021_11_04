#ifndef TriggerFactory_h__
#define TriggerFactory_h__

#include "Export_Function.h"
#include "Trigger.h"

static HRESULT CreateBoxTrigger(PxScene* pScene,const _wstr& tagName,const _vec3& vPos,const _float& fXHalf, const _float& fYHalf, const _float& fZHalf)
{
	CTrigger::Create();
}
#endif // TriggerFactory_h__