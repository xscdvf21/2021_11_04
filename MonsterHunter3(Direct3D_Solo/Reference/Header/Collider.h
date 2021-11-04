#ifndef Collider_h__
#define Collider_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider : public CComponent
{
private:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCollider(void);

public:
	const _matrix*			Get_ColMatrix() { return &m_matColMatrix; }
	const _vec3*			Get_Min(void) { return &m_vMin; }
	const _vec3*			Get_Max(void) { return &m_vMax; }

public:
	HRESULT		Ready_Collider(const _vec3* pPos, const _ulong& dwVtxCnt, const _ulong& dwStride);
	void		Render_Collider(COLTYPE eType, const _matrix* pColliderMatrix);

private:
	_vec3		m_vMin, m_vMax;
	_matrix		m_matColMatrix;

#ifdef _DEBUG
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	LPDIRECT3DTEXTURE9			m_pTexture[COL_END];
	
#endif

public:
	static CCollider*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _ulong& dwVtxCnt, const _ulong& dwStride);
	virtual CComponent*		Clone(void);
	virtual void			Free(void);

};

END
#endif // Collider_h__
