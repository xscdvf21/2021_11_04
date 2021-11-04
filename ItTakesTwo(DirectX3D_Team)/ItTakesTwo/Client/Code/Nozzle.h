#ifndef __Nozzle_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END



class CNozzle : public Engine::CGameObject
{
public:
	explicit CNozzle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNozzle();

public:
	virtual HRESULT Ready_Object(_uint iBodyCount, void* pArg = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;
	void			Set_TargetCamera(CCamera* pCamera) { m_pTargetCamera = pCamera; }

private:
	HRESULT			Add_Component(void* pArg = nullptr);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);


private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;
private:
	_vec3						m_vStartPos;
	CCamera*					m_pTargetCamera= nullptr;
public:
	static CNozzle*		Create(LPDIRECT3DDEVICE9 pGraphicDev,CCamera* pCamera, void* pArg = nullptr);
	virtual void Free(void)		override;

};



#define __Nozzle_H__
#endif


