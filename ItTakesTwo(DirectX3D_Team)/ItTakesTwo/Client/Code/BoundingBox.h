#ifndef BoundingBox_h__
#define BoundingBox_h__

#include "Component.h"

class CBoundingBox : public CComponent
{
private:
	explicit CBoundingBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoundingBox(void);

public:
	const _matrix*		Get_ColMatrix() { return &m_matColMatrix; }
	const _vec3*		Get_Min(void) { return &m_vMin; }
	const _vec3*		Get_Max(void) { return &m_vMax; }
	_float				Get_Dis() { return m_fDis; }
	void				Set_Dis(_float dis) { m_fDis = dis; }
	_vec3				Get_Scale() { return m_vScale; }
	_vec3				Get_Pos() { return m_vPos; }

public:
	void				Set_Scale(_vec3 vScale) { m_vScale = vScale; };
	void				Add_Scale(_vec3 vScale) { m_vScale += vScale; }
	void				Add_ScaleX(_float fScale) { m_vScale.x += fScale; }
	void				Add_ScaleY(_float fScale) { m_vScale.y += fScale; }
	void				Add_ScaleZ(_float fScale) { m_vScale.z += fScale; }
	void				Set_Pos(_vec3 vPos) { m_vPos = vPos; }
	void				Add_Pos(_vec3 vPos) { m_vPos += vPos; }
	void				Move_Pos(_vec3 vDir, const _float& fTimeDelta, _float fSpeed);
	void				Set_Angle(_vec3 vAngle) { m_vAngle = vAngle; }

public:
	HRESULT				Ready_Collider(_vec3 vScale, _vec4 argb);
	void				Render_Collider(_matrix pParentMatrix, _bool debug, LPD3DXEFFECT & pEffect);
	void				Render_Collider(_vec3 vPos, _bool debug, LPD3DXEFFECT & pEffect);
	void				Render_Collider(_bool debug, LPD3DXEFFECT & pEffect);
	
private:
	_vec3				m_vMin, m_vMax;
	_matrix				m_matColMatrix;

	LPDIRECT3DDEVICE9	m_pGraphicDev;
	LPDIRECT3DTEXTURE9	m_pTexture;

public:
	LPD3DXMESH			m_pBuffer;
	_float				m_fDis;
	_vec3				m_vScale;
	_vec3				m_vPos;
	_vec3				m_vAngle = {0.0f, 0.0f, 0.0f};

public:
	static CBoundingBox*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vScale, _vec4 argb);
	virtual CComponent*	Clone(void* pArg = nullptr) override;
	virtual void		Free(void);

};

#endif // Collider_h__
