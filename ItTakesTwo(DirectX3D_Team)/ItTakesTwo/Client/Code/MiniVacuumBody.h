#ifndef __MINIVACUUMBODY_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END


class CMiniVacuumBody : public Engine::CGameObject
{
public:
	explicit CMiniVacuumBody(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMiniVacuumBody();

public:
	virtual HRESULT Ready_Object(_float fDis, void* pArg = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	const _matrix*			m_pParentBoneMatrix = nullptr;
	const _matrix*			m_pParentWorldMatrix = nullptr;

	_vec3					m_vDir;

	_float					m_fDis; //각 호스 마다의 거리.

public:
	static CMiniVacuumBody*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _float fDis, void* pArg = nullptr);
	virtual void Free(void)		override;


	
};


#define __MINIVACUUMBODY_H__
#endif

