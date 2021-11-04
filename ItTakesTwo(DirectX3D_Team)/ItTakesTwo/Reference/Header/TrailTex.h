#ifndef TrailTex_h__
#define TrailTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTrailTex : public CVIBuffer
{
private:
	explicit CTrailTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTrailTex(const CTrailTex& rhs);
	virtual ~CTrailTex(void);

public:
	virtual HRESULT Ready_Buffer(void) override;
	virtual void Render_Buffer(void) override;

public:
	void	Set_Vertices(vector<_vec3> vecStart, vector<_vec3> vecEnd);

private:
	VTXTEX*		m_pVertexSword = NULL;
	INDEX16*	m_pIndexSword = NULL;
	_vec3		m_vStart;
	_vec3		m_vEnd;

public:
	static CTrailTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone(void* pArg = nullptr) override;

private:
	virtual void		Free(void);

};


END
#endif // TrailTex_h__
