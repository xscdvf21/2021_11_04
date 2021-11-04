#pragma once

#ifndef __VIBUFFER_CUBECOLOR_H__
#define __VIBUFFER_CUBECOLOR_H__

#include "VIBufferTool.h"

USING(Engine)

class CVIBuffer_CubeColorTool final : public CVIBufferTool
{
	private:
	explicit CVIBuffer_CubeColorTool(_Device pDevice);
	explicit CVIBuffer_CubeColorTool(const CVIBuffer_CubeColorTool& other);
	virtual ~CVIBuffer_CubeColorTool() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void* pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;
	virtual HRESULT	Change_VertexColor(D3DCOLOR _Color, _uint iIndex = 0);


public:
	static CVIBuffer_CubeColorTool* Create(_Device pDevice);
	virtual CComponentTool* Clone(void* pArg = nullptr) override;
	virtual void Free() override;



};


#endif // !__VIBUFFER_CUBECOLOR_H__