#ifndef __SHEILD_H__

#include "Enermy.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CSheild : public CEnermy
{
private:
	explicit CSheild(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSheild();

public:
	virtual HRESULT Ready_Object(void* pArg = nullptr);
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



private :
	const _matrix*				m_pParentBoneMatrix = nullptr;
	const _matrix*				m_pParentWorldMatrix = nullptr;

	_tchar						m_tParentBone[MAX_PATH] = L"";
public:
	static CSheild*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void Free(void)		override;
};

#define __SHEILD_H__
#endif


