#ifndef NormalTerrainTex_h__
#define NormalTerrainTex_h__


#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CNormalTerrainTex : public CVIBuffer
{
private:
	explicit CNormalTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNormalTerrainTex(const CNormalTerrainTex& rhs);
	virtual ~CNormalTerrainTex(void);

public:
	HRESULT Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	HRESULT Ready_Clone_Buffer(void* pArg = nullptr);
	virtual void Render_Buffer(void) override;

public:
	void				Copy_Indices(INDEX32* pIndex, const _ulong& dwTriCnt);

	HRESULT				Set_Terrain(void* pArg = nullptr);

private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;
	_int				m_iCount = 0;

public:
	static CNormalTerrainTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	virtual CComponent* Clone(void* pArg = nullptr) override;

public:
	virtual void Free(void) override;
};
END


#endif // NormalTerrainTex_h__
