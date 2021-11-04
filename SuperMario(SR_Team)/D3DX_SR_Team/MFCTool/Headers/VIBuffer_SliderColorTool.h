
#pragma once

#ifndef __VIBUFFER_SLIDERColorHan_H__
#define __VIBUFFER_SLIDERColorHan_H__

#include "VIBufferTool.h"

USING(Engine)
class CVIBuffer_SliderColor final : public CVIBufferTool
{
private:
	explicit CVIBuffer_SliderColor(_Device pDevice);
	explicit CVIBuffer_SliderColor(const CVIBuffer_SliderColor& other);
	virtual ~CVIBuffer_SliderColor() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void* pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;
	virtual HRESULT	Change_VertexColor(D3DCOLOR _Color, _uint iIndex = 0);

public:
	static CVIBuffer_SliderColor* Create(_Device pDevice);
	virtual CComponentTool* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};


#endif // !__VIBUFFER_SLIDERColorHan_H__
