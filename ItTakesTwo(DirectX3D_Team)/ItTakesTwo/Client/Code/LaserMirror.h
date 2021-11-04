#ifndef __LASERMIRROR_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CCalculator;
class CCollider;
class CShader;

END

enum LaserMirrorType
{
	Mirror_Default,
	Mirror_Gravity,
	Mirror_HitPossible,
	Mirror_Hit,
	Mirror_HitEnd,
	Mirror_MovePossible,
	Mirror_End
};

class CLaserMirror : public CGameObject
{
	enum MoveID
	{
		Move_Up,
		Move_Down,
		AllMove_Up,
		Move_End

	};

	enum MeshID
	{
		Mesh_Glass,
		Mesh_Mirror,
		Mesh_BrokeGlass,
		Mesh_Sphere,
		Mesh_Cover,
		Mesh_End
	};

private:
	explicit CLaserMirror(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLaserMirror();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr, LaserMirrorType _eType = Mirror_Default);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;
public:
	const _bool			Check_Hit();
	void				Effect_Explosive();
public:
	//SET
	void				Set_MoveUp(_bool _bool) { m_bMove[Move_Up] = _bool; }
	void				Set_MoveDown(_bool _bool) { m_bMove[Move_Down] = _bool; }
	void				Set_AllMove() { m_bMove[AllMove_Up] = true; }
	void				Set_State(LaserMirrorType eState, _bool bState) { m_bState[eState] = bState; }

	
	//GET
	_bool				Get_State(LaserMirrorType eState) { return m_bState[eState]; }
private:
	HRESULT				Add_Component(void* pArg = nullptr);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex, const _int& iMeshNum = 100);

private:
	void				MoveUp(const _float& fTimeDelta);
	void				MoveDown(const _float& fTimeDelta); 
	void				AllMoveUp(const _float& fTimeDelta);

	void				TriggerCreate();
private:
	Engine::CStaticMesh*			m_pMeshCom[Mesh_End];
	Engine::CTransform*				m_pTransformCom[Mesh_End];
	Engine::CCalculator*			m_pCalculatorCom = nullptr;
	Engine::CCollider*				m_pColliderCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;

private :
	LaserMirrorType					m_eType;
	_bool							m_bCreateTrigger;
	CTrigger*						m_pTrigger = nullptr;
private:
	_bool							m_bState[Mirror_End]{};
	_bool							m_bMove[Move_End]{};

private:
	_bool							m_bMoveUpDir = false;
	_bool							m_bMoveDownDir = false;
	_bool							m_bAllMoveDir = false;
	_bool							m_bWindLaserMove = false;


	_vec3							m_vMoveUpStartPos[Mesh_End];
	_vec3							m_vMoveDownStartPos[Mesh_End];

	_float							m_fMoveUpDir = 5.f;
	_float							m_fMoveDownDir = 5.f;

	_float							m_fMoveDir = 0.f;
	_float							m_fCoverMoveDir = 0.f;


	_float							m_fAllUpDir;
	
	_float							m_fHeight;
public:
	static CLaserMirror*			Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr, LaserMirrorType _eType = Mirror_Default);
	virtual void					Free(void)		override;

};
#define __LASERMIRROR_H__
#endif

