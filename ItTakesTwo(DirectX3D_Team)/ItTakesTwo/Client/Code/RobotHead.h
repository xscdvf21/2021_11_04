#ifndef __RobotHead_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CDynamicMesh;
class CShader;

END



class CRobotHead : public Engine::CGameObject
{
public:
	explicit CRobotHead(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRobotHead();

public:
	virtual HRESULT Ready_Object(_uint iBodyCount, void* pArg = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;


private:
	HRESULT			Add_Component(void* pArg = nullptr);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);


private:
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;


public:
	static CRobotHead*		Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr);
	virtual void Free(void)		override;

};



#define __RobotHead_H__
#endif


