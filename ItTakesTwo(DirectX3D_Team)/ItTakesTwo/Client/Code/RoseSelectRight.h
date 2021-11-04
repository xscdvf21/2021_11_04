#ifndef RoseSelectRight_h__
#define RoseSelectRight_h__

#include "GameObject.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END


class CRoseSelectRight : public CGameObject
{
	enum Rose_AnimID 
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
	explicit CRoseSelectRight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRoseSelectRight();

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

public:
	void			Set_Animation(_uint eID) { m_eAnim = (Rose_AnimID)eID; }

private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:
	Rose_AnimID m_eAnim;

public:
	static CRoseSelectRight*	Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);

public:
	virtual void Free(void)		override;
};

#endif // RoseSelect_h__


