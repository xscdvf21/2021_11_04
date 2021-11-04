#ifndef __VACUUMFIELDPART4_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStaticMesh;
class CCalculator;
class CCollider;
class CShader;

END

class CVacuumFieldPart4 : public CGameObject
{
private:
	explicit CVacuumFieldPart4(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVacuumFieldPart4();

public:
	virtual HRESULT Ready_Object(_bool bOpen, _uint iIndex, void* pArg = nullptr);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void	Render_Object(const _int& iIndex = 0) override;

private:
	HRESULT			Add_Component(void* pArg);
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect, const _int& iIndex);

	void			MoveOpen(const _float& fTimeDelta);
	void			MoveClose(const _float& fTimeDelta);

	void			MoveUpEndClose(const _float& fTimeDelta);


	void			All_PartUp();
	void			All_PartDown();
public:

	_bool			Get_MoveOpenCheck() { return m_bMoveOpen; }
	void			Set_MoveOpenCheck(_bool bCheck) { m_bMoveOpen = bCheck; }

	_bool			Get_MoveCloseCheck() { return m_bMoveClose; }
	void			Set_MoveCloseCheck(_bool bCheck) { m_bMoveClose = bCheck; }


private:
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;

private:

	_bool						m_bMoveOpen = false;
	_bool						m_bMoveClose = false;

	_bool						m_bOpen = false; //ó���� ���ڷ� ��������� ������ ����.
	
	_vec3						m_vStartPos; //�̵��Ÿ��� �����ϱ�����, ��ŸƮ ������ �������� ����.
	_vec3						m_vEndPos;

	_uint						m_iIndex;

public:
	static CVacuumFieldPart4*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool bOpen, _uint iIndex, void* pArg = nullptr);
	virtual void Free(void)		override;
};
#define __VACUUMFIELDPART4_H__
#endif

