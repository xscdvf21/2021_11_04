#ifndef Calculator_h__
#define Calculator_h__

#include "Component.h"

BEGIN(Engine)

class CTerrainTex;
class CTransform;
class CGameObject;

class ENGINE_DLL CCalculator : public CComponent
{
private:
	typedef	struct tagOBB
	{
		_vec3		vPoint[8];
		_vec3		vCenter;
		_vec3		vProjAxis[3];	// �������� �� ������ ���� ������ ���� ����
		_vec3		vAxis[3];		// �ڽ��� ������ �� ����

	}OBB;

private:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCalculator(const CCalculator& rhs);
	virtual ~CCalculator(void);

public:
	HRESULT		Ready_Calculator(void);
	_float		Compute_HeightOnTerrain(const _vec3* pPos, const _vec3* pTerrainVtxPos, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxIdx = 1);
	_vec3		Picking_OnTerrain(HWND hWnd, const CTerrainTex* pTerrainBufferCom, const CTransform* pTerrainTransformCom);
	
	_bool		Collision_AABB(const _vec3* pDestMin, const _vec3* pDestMax, const _matrix* pDestWorld,
							   const _vec3* pSourMin, const _vec3* pSourMax, const _matrix* pSourWorld);

	_bool		Collision_OBB(const _vec3* pDestMin, const _vec3* pDestMax, const _matrix* pDestWorld,
							  const _vec3* pSourMin, const _vec3* pSourMax, const _matrix* pSourWorld);

	_bool		Collision_Sphere(CGameObject* _Dst, CGameObject* _Src);

private:
	void		Set_Point(OBB* pObb, const _vec3* pMin, const _vec3* pMax);
	void		Set_Axis(OBB* pObb);

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
public:
	static CCalculator*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone(void);
private:
	virtual void Free(void);

};

END
#endif // Calculator_h__
