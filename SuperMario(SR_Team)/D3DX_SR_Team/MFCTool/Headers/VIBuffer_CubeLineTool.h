#pragma once

#ifndef __VIBUFFER_CUBELINE_H__
#define __VIBUFFER_CUBELINE_H__

#include "VIBufferTool.h"

USING(Engine)

class CVIBuffer_CubeLineTool final : public CVIBufferTool
{
	private:
	explicit CVIBuffer_CubeLineTool(_Device pDevice);
	explicit CVIBuffer_CubeLineTool(const CVIBuffer_CubeLineTool& other);
	virtual ~CVIBuffer_CubeLineTool() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void* pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;


public:
	static CVIBuffer_CubeLineTool* Create(_Device pDevice);
	virtual CComponentTool* Clone(void* pArg = nullptr) override;
	virtual void Free() override;



};


#endif // !__VIBUFFER_CUBELINE_H__