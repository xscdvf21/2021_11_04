#pragma once

#ifndef __VIBUFFER_CUBEINVISIBLE_H__
#define __VIBUFFER_CUBEINVISIBLE_H__


#include "VIBuffer.h"

BEGIN(Engine)

class CVIBuffer_CubeInvisible :	public CVIBuffer
{
public:
	explicit CVIBuffer_CubeInvisible(_Device pDevice);
	explicit CVIBuffer_CubeInvisible(const CVIBuffer_CubeInvisible& other);
	virtual ~CVIBuffer_CubeInvisible() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void* pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;
	virtual HRESULT	Change_VertexColor(D3DCOLOR _Color, _uint iIndex = 0);

public:
	static CVIBuffer_CubeInvisible* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};

END

#endif // !__VIBUFFER_CUBEINVISIBLE_H__
