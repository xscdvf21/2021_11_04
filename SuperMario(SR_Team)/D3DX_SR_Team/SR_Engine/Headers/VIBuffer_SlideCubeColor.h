#pragma once

#ifndef __VIBUFFER_SLIDECUBECOLOR_H__
#define __VIBUFFER_SLIDECUBECOLOR_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_SlideCubeColor final : public CVIBuffer
{
private :
	explicit CVIBuffer_SlideCubeColor(_Device pDevice);
	explicit CVIBuffer_SlideCubeColor(const CVIBuffer_SlideCubeColor& other);
	virtual ~CVIBuffer_SlideCubeColor() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void* pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;
	virtual HRESULT	Change_VertexColor(D3DCOLOR _Color, _uint iIndex = 0);

public:
	static CVIBuffer_SlideCubeColor* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END

#endif // !__VIBUFFER_SLIDECUBECOLOR_H__