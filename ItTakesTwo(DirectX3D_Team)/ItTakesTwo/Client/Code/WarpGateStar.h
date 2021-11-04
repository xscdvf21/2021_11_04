#ifndef __WARPGATESTAR_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CShader;

END

class CWarpGateStar : public CGameObject
{

	typedef enum WarpGateStar
	{	
		GATESTAR_1,
		GATESTAR_2,
		GATESTAR_3,
		GATESTAR_4,
		GATESTAR_5,
		GATESTAR_End
	}GATESTAR;
private:
	explicit		 CWarpGateStar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			~CWarpGateStar();

	void Light(const _float & fTimeDelta);
	bool m_bBegin = false;
public:
	virtual HRESULT Ready_Object(void * pArg, _uint Index = 0);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;


	virtual void* On_Active(void* pCaller) override;

private:
	HRESULT			Add_Component(void* pArg);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex, const _int& iMeshNum = 100);

	void			Set_INFO(void* pArg);
public:
	void			Set_Light(_uint  _StarIndex) { m_bLight[_StarIndex] = true; }
	_bool			Get_Light(_uint  _StarIndex) { return m_bLight[_StarIndex]; }
private:
	Engine::CStaticMesh*		m_pMeshCom[GATESTAR_End];
	Engine::CTransform*			m_pTransformCom[GATESTAR_End];
	Engine::CShader*			m_pShaderCom = nullptr;
private:
	_uint						m_iIndex = 0;
	_bool						m_bLight[GATESTAR_End] = { false };
	_int						m_iPass = 9;
	_float						m_fLightTime = 0.f;

public:
	static CWarpGateStar*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void * pArg, _uint Index = 0);
	virtual void				Free(void)		override;
};


#define __WARPGATESTAR_H__
#endif

