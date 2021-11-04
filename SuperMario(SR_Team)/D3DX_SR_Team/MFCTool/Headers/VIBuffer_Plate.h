#pragma once
#include "VIBufferTool.h"

USING(Engine)
class CVIBuffer_Plate :
	public CVIBufferTool
{
private:
	explicit CVIBuffer_Plate(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer_Plate(const CVIBuffer_Plate& other);
	virtual ~CVIBuffer_Plate() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void* pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;

public:
	static CVIBuffer_Plate* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponentTool* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

