#pragma once

#ifndef __VIBUFFER_CUBETEXTURE_H__
#define __VIBUFFER_CUBETEXTURE_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_CubeTexture final : public CVIBuffer
{
private:
	explicit CVIBuffer_CubeTexture(_Device pDevice);
	explicit CVIBuffer_CubeTexture(const CVIBuffer_CubeTexture& other);
	virtual ~CVIBuffer_CubeTexture() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void* pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;
	virtual HRESULT	Change_VertexColor(D3DCOLOR _Color, _uint iIndex = 0);

public:
	static CVIBuffer_CubeTexture* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;


};

END

#endif // !__VIBUFFER_CUBETEXTURE_H__