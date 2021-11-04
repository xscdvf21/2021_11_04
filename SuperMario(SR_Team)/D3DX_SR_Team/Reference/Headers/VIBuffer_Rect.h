#pragma once

#ifndef __VIBUFFER_RECT_H__
#define __VIBUFFER_RECT_H__

#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Rect final : public CVIBuffer
{
private :
	explicit CVIBuffer_Rect(_Device pDevice);
	explicit CVIBuffer_Rect(const CVIBuffer_Rect& other);
	virtual ~CVIBuffer_Rect() = default;

public :
	virtual HRESULT Ready_Component_Prototype() override;
	virtual HRESULT Ready_Component(void* pArg = nullptr);
	virtual HRESULT Render_VIBuffer() override;
	virtual HRESULT	Change_VertexColor(D3DCOLOR _Color, _uint iIndex = 0) override;

public:
	static CVIBuffer_Rect* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;



};
END

#endif // !__VIBUFFER_RECT_H__