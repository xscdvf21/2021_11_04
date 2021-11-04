#pragma once

#ifndef __VIBUFFER_ALPHA_EFFECT_ROUND_H__
#define __VIBUFFER_ALPHA_EFFECT_ROUND_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Alpha_Round final : public CVIBuffer
{
private:
	explicit CVIBuffer_Alpha_Round(_Device pDevice);
	explicit CVIBuffer_Alpha_Round(const CVIBuffer_Alpha_Round& other);
	virtual ~CVIBuffer_Alpha_Round() = default;

public:
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void* pArg = nullptr) override;
	virtual HRESULT Render_VIBuffer() override;
	virtual HRESULT	Change_VertexColor(D3DCOLOR _Color, _uint iIndex = 0);


public:
	static CVIBuffer_Alpha_Round* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};

END

#endif // !__VIBUFFER_ALPHA_EFFECT_ROUND_H__