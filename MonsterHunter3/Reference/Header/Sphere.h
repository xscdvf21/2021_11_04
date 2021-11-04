#pragma once
#ifndef __SPHERE_H__


#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CSphere : public CResources
{

private:
	explicit CSphere(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSphere(const CSphere& rhs);
	virtual ~CSphere(void);


public:
	HRESULT Ready_Buffer(const _float & _Radius, const _uint & _Slices, const _uint & _Stack, const LPD3DXMESH  _pMesh = NULL , const LPD3DXBUFFER _pAdjacency = NULL);
	virtual void Render_Buffer(const _matrix* Parent, _vec3 _vScale = { 1.f, 1.f, 1.f }, _vec3 _vPos = { 0.f, 0.f,0.f }, _uint _ii = 0);
	virtual void Render_Buffer(void);

public:
	static CSphere*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float & _Radius, const _uint & _Slices, const _uint & _Stack, const LPD3DXMESH _pMesh = NULL, const LPD3DXBUFFER _pAdjacency = NULL);


	virtual void Free(void) override;
	virtual CComponent* Clone(void) override;


private :
	_float			m_fRadius;
	_uint			m_iSlices;
	_uint			m_iStack;
	LPD3DXMESH		m_pMesh;
	LPD3DXBUFFER	m_pAdjacency;

	LPDIRECT3DTEXTURE9 m_pTextureGreen[2];

};


END

#define __SPHERE_H__
#endif // !__SPHERE_H__




