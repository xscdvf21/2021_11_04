#pragma once
#include "VIBufferTool.h"

USING(Engine)
class CVIBuffer_PlateTexture :
	public CVIBufferTool
{
private:
	explicit CVIBuffer_PlateTexture(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer_PlateTexture(const CVIBuffer_PlateTexture& other);
	virtual ~CVIBuffer_PlateTexture() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void* pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;


public:
	static CVIBuffer_PlateTexture* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CComponentTool* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

