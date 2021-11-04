#ifndef Torch_h__
#define Torch_h__


#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CTexture;
class CShader;

END


class CTorch : public Engine::CGameObject
{
private:
	explicit CTorch(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTorch();

public:
	virtual HRESULT Ready_Object(void* pArg);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CTexture*			m_pTexture = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

public:
	static CTorch*				Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg);
	virtual void Free(void)		override;
};


#endif // Torch_h__

