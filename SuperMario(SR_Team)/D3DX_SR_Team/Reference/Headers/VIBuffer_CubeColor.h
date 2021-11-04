#pragma once

#ifndef __VIBUFFER_CUBECOLOR_H__
#define __VIBUFFER_CUBECOLOR_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_CubeColor final : public CVIBuffer
{
private:
	explicit CVIBuffer_CubeColor(_Device pDevice);
	explicit CVIBuffer_CubeColor(const CVIBuffer_CubeColor& other);
	virtual ~CVIBuffer_CubeColor() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void* pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;
	virtual HRESULT	Change_VertexColor(D3DCOLOR _Color, _uint iIndex = 0);


public:
	static CVIBuffer_CubeColor* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;



};

END

#endif // !__VIBUFFER_CUBECOLOR_H__