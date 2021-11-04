#pragma once
#include "Base.h"

USING(Engine)
class CComponentTool :
	public CBase
{
protected:
	explicit CComponentTool(LPDIRECT3DDEVICE9 pDevice);
	explicit CComponentTool(const CComponentTool& other);
	virtual ~CComponentTool() = default;

public:
	virtual HRESULT Ready_Component_Prototype() = 0;
	virtual HRESULT Ready_Component(void* pArg = nullptr) = 0;

public:
	virtual CComponentTool* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;

protected:
	LPDIRECT3DDEVICE9 m_pDevice;
};

