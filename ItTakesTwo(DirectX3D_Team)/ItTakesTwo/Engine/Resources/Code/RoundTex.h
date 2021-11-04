#ifndef RoundTex_h__
#define RoundTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRoundTex : public CVIBuffer
{
private:
	explicit CRoundTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRoundTex(const CRoundTex& rhs);
	virtual ~CRoundTex(void);

public:
	virtual HRESULT Ready_Buffer(void) override;
	virtual void Render_Buffer(void) override;

public:
	static CRoundTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;	

public:
	LPD3DXMESH m_pBuffer;
};

END
#endif // RoundTex_h__
