#ifndef __SceneChangeSpace_H__
#define __SceneChangeSpace_H__

#include "GameObject.h"
#include "Base.h"
#include "Engine_Define.h"
#include "ImageUI.h"

BEGIN(Engine)

class CTransform;
class CPhysXMesh;

END

class CSceneChangeSpace : public Engine::CGameObject
{
private:
	explicit CSceneChangeSpace(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSceneChangeSpace(void);

	_bool					Fade_Out(const _float& fTimeDelta);

public:
	virtual HRESULT			Ready_Object(void* pArg = nullptr) override;
	virtual _int			Update_Object(const _float& TimeDelta) override;
	virtual _int			LateUpdate_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object(const _int& iIndex) override;


	virtual void*			On_Trigger_Active(void* pTrigger,void* pCaller) override;


	virtual void*			On_Active(void* pCaller) override;

private:
	HRESULT					Add_Component(void* pArg = nullptr);

public:
	void					Set_Next_Scene(SCENEID eScene) { m_eNextScene = eScene; }
	void					Set_Pos(const _vec3& vPos) { m_vPos = vPos; }
	void					Set_Scale(const _float& fX, const _float& fY, const _float& fZ) { m_vScale = { fX,fY,fZ }; }
	void					Set_Scale(const _float& fHalf) { m_vScale = { fHalf,fHalf,fHalf }; }
	void					Set_Next_Scene_StartPos(const _vec3 vPos);
	void					Set_InstanceChange(const _bool& bState=true) { m_bInstance = bState; }

	
	//æ¿ √º¿Œ¡ˆ
	void					Event_VF2(void* pCaller);
	void					Event_TO_SF2(void* pCaller);
	void					Event_TO_SF3(void* pCaller);
	void					Event_TO_SF(void* pCaller);
	void					Event_TO_SB(void* pCaller);
	void					Event_TO_CB(void* pCaller);
public:
	static CSceneChangeSpace* Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENEID eNext, const _vec3& vPos, const _float& fX, const _float& fY, const _float& fZ, void* pArg = nullptr);
	static CSceneChangeSpace* Create(LPDIRECT3DDEVICE9 pGraphicDev, SCENEID eNext, const _vec3& vPos, const _float& fHalf, void* pArg = nullptr);
public:
	CImageUI*				m_pUI = nullptr;
public:

	OBJINFO					m_tObjInfo;
	CTrigger*				m_pTrigger = nullptr;
	SCENEID					m_eNextScene;
	_vec3					m_vPos;
	_vec3					m_vScale;
	_bool					m_bEnd = true;
	_bool					m_bSceneChange[2] = {false,};
	_bool					m_bInstance=false;
	_vec3					m_vNextScenePos;
	_bool					m_bSetScenePos = false;
private:
	virtual void Free(void) override;
};



#endif // TestWall_h__
