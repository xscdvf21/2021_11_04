#ifndef __WEAPON_H__

#include "GameObject.h"
#include "Base.h"
#include "define.h"

BEGIN(Engine)
class CDynamicMesh;
class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CSphere;
END

class CWeapon : public Engine::CGameObject
{
public:
	explicit CWeapon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWeapon(void);

public:
	HRESULT Ready_Object(const _uint& iFlag);
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

public :
	_bool			Get_TimeStop() { return m_bTimeStop; }

private:
	HRESULT			Add_Component(void);
	_bool			Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag, const _float& fTimeDelta);
	void			SetUp_Sphere(void);

private:
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CSphere*		m_pSphere = nullptr;

	const _matrix*			m_pParentBoneMatrix_Sphere[WP_END];
	const _matrix*			m_pParentWorldMatrix_Sphere = nullptr;
	_matrix					m_pFinalMatrix_Sphere[WP_END];

	_matrix					m_pFontMatrix;

	const _matrix*			m_pParentBoneMatrix = nullptr;
	const _matrix*			m_pParentWorldMatrix = nullptr;

	_matrix					m_pWorld; //뼈 위치 수정하기위해서 사용.

	_vec3					m_vDir;
	_bool					m_bColl;
	_uint					m_iFlag = 0;

	_bool					m_bCollCheck = false; //충돌 체크 한번만 하기위해.


	_float					m_iAttackStack = -1;
	_float					m_fDelta = 0.f; //충돌 체크 한번만하기위해 시간 값 저장.
	_float					m_fDel = 1.7; //공격 딜레이.

	_bool					m_bFont = false;
	_float					m_fFontTime = 0.f;

	_vec3					m_vFontPos; //충돌 했을때 폰트 위치.

	_ulong					m_dwRenderCnt = 0;
	_tchar					m_szFPS[256] = L"";


	_bool					m_bTimeStop = false;

	_vec3					m_vJumpPos;

	_float					m_fJumpPowerY = 4.f;
	_float					m_fJumpTime = 0.f;
public:
	static CWeapon*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag);
private:
	virtual void Free(void) override;

};

#define __WEAPON_H__
#endif // !__WEAPON_H__




