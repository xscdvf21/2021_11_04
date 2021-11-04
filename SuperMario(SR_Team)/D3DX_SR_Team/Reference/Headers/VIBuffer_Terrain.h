#pragma once

#ifndef __VIBUFFER_TERRAIN_H__
#define __VIBUFFER_TERRAIN_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
private:
	explicit CVIBuffer_Terrain(_Device pDevice);
	explicit CVIBuffer_Terrain(_Device pDevice, _uint iVertexCountX, _uint iVertexCountZ, _float fVertexItv = 1.f);
	explicit CVIBuffer_Terrain(const CVIBuffer_Terrain& other);
	virtual ~CVIBuffer_Terrain() = default;

	public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void* pArg = nullptr);
	virtual HRESULT Render_VIBuffer() override;
	virtual HRESULT	Change_VertexColor(D3DCOLOR _Color, _uint iIndex = 0) override;

public:
	static CVIBuffer_Terrain* Create(_Device pDevice, _uint iVertexCountX, _uint iVertexCountZ, _float fVertexItv = 1.f);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private:
	_uint	m_iVertexCountX = 0;
	_uint	m_iVertexCountZ = 0;
	_float	m_fVertexInterval = 1.f;

};

END

#endif // !__VIBUFFER_TERRAIN_H__