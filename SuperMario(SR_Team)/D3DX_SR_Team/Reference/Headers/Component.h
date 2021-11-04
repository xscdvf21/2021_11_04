#pragma once
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	explicit CComponent(_Device pDevice);
	explicit CComponent(const CComponent& other);
	virtual ~CComponent() = default;

public :
	virtual HRESULT Ready_Component_Prototype() PURE;
	virtual HRESULT Ready_Component(void* pArg = nullptr) PURE;

public:
	virtual CComponent* Clone(void* pArg = nullptr) PURE;
	virtual void Free() override;

protected:
	_Device  m_pDevice;

};

END

#endif
