#pragma once

#ifndef __VIBUFFER_CUBETEXTURE_H__
#define __VIBUFFER_CUBETEXTURE_H__

#include "VIBufferTool.h"

USING(Engine)

class CVIBuffer_CubeTextureTool final : public CVIBufferTool
{
	private:
	explicit CVIBuffer_CubeTextureTool(_Device pDevice);
	explicit CVIBuffer_CubeTextureTool(const CVIBuffer_CubeTextureTool & other);
	virtual ~CVIBuffer_CubeTextureTool() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void* pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;


public:
	static CVIBuffer_CubeTextureTool * Create(_Device pDevice);
	virtual CComponentTool* Clone(void* pArg = nullptr) override;
	virtual void Free() override;



};


#endif // !__VIBUFFER_CUBETEXTURE_H__