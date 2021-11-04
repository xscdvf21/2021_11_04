#ifndef __SPACETUBE_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CShader;

END

enum TubeColor
{
	TubeColor_Blue,
	TubeColor_Orange,
	TubeColor_Purple,
	TubeColor_End
};

class CSpaceTube : public CGameObject
{
private:
	explicit CSpaceTube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpaceTube();

public:
	virtual HRESULT		Ready_Object(void* pArg = nullptr, TubeColor _eColor = TubeColor_Blue);
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual _int		LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void		Render_Object(const _int& iIndex = 0) override;


public :
	void				Set_SizeChange() { m_bSizeChange = true; }
private:
	HRESULT				Add_Component(void* pArg = nullptr, TubeColor _eColor = TubeColor_Blue);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void				Move(const _float& fTimeDelta);
	void				SizeChange(const _float& fTimeDelta);

private:
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CShader*				m_pShaderCom = nullptr;

private :
	CTrigger*						m_pTrigger = nullptr;
	TubeColor						m_eColor;

	_bool							m_bSizeChange = false;
	_bool							m_bSoundStart = false;
public:
	static CSpaceTube*			Create(LPDIRECT3DDEVICE9 pGraphicDev, void* pArg = nullptr, TubeColor _eColor = TubeColor_Blue);
	virtual void				Free(void)		override;

};

#define __SPACETUBE_H__
#endif


