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
	const _matrix*		Get_ColMatrix() { return &m_matColMatrix; }
	const _vec3*		Get_Min(void) { return &m_vMin; }
	const _vec3*		Get_Max(void) { return &m_vMax; }
	_float				Get_Dis() { return m_fDis; }
	void				Set_Dis(_float dis) { m_fDis = dis; }

public:
	HRESULT				Ready_Collider(_float fScale, _vec4 argb);
	void				Render_Collider(_matrix pParentMatrix, _bool debug);
	void				Render_Collider(_matrix pParentMatrix, _bool debug, LPD3DXEFFECT & pEffect);
private:
	_vec3				m_vMin, m_vMax;
	_matrix				m_matColMatrix;

	LPDIRECT3DDEVICE9	m_pGraphicDev;
	LPDIRECT3DTEXTURE9	m_pTexture;

public:
	LPD3DXMESH			m_pBuffer;
	_float				m_fDis;

public:
	static CCollider*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fScale, _vec4 argb);
	virtual CComponent*	Clone(void* pArg = nullptr) override;
	virtual void		Free(void);

};

END
#endif // Collider_h__
