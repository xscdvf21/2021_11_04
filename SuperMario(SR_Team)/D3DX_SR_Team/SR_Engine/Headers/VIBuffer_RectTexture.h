#pragma once

#ifndef __VIBUFFER_RECTTEXTURE_H__
#define __VIBUFFER_RECTTEXTURE_H__

#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_RectTexture final : public CVIBuffer
{
private:
	explicit CVIBuffer_RectTexture(_Device pDevice);
	explicit CVIBuffer_RectTexture(const CVIBuffer_RectTexture& other);
	virtual ~CVIBuffer_RectTexture() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void* pArg = nullptr);
	virtual HRESULT Render_VIBuffer() override;
	virtual HRESULT	Change_VertexColor(D3DCOLOR _Color, _uint iIndex = 0) override;

public:
	static CVIBuffer_RectTexture* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;


};
END

#endif // !__VIBUFFER_RECT_H__