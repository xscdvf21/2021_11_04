#ifndef CodyDoll_h__
#define CodyDoll_h__

#include "GameObject.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CCodyDoll : public CGameObject
{
	enum CodyDoll_AnimID 
	{
		Select_Trasition,                         //0	frame:[21]
		Select_Idle,                              //1	frame:[260]
		Select_Both,                              //2	frame:[260]
		CodyDoll_Idle,                            //3	frame:[260]
		CodyDoll_Both,                            //4	frame:[260]
		MayDoll_Idle,                             //5	frame:[260]
		MayDoll_Both,                             //6	frame:[260]
		End
	};

private:
	explicit CCodyDoll(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCodyDoll();

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	CodyDoll_AnimID				m_eAnim;
	const _matrix*				m_pParentBoneMatrix = nullptr;
	const _matrix*				m_pParentWorldMatrix = nullptr;
	_vec3						m_vRot{};

public:
	static CCodyDoll*			Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void Free(void)		override;
};

#endif // CodyDoll_h__


