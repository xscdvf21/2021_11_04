#pragma once
#ifndef __IMAGEUI_H__
#include "UI.h"
#include "Client_Define.h"
USING(Engine)
class CImageUI :public CUI
{
public:
	explicit CImageUI(LPDIRECT3DDEVICE9 pDevice);
	explicit CImageUI(const CImageUI& other);
	virtual ~CImageUI() = default;



	virtual HRESULT Ready_Object(void* pArg =nullptr) override;
	HRESULT Ready_GameObject(void* pArg,_bool bCheck);
	virtual _int Update_Object(const _float& TimeDelta) override;
	virtual _int LateUpdate_Object(const _float& TimeDelta) override;
	virtual void Render_Object(const _int& iIndex = 0) override;
	//virtual HRESULT Ready_GameObject() override;
	//virtual HRESULT Ready_Clone_GameObject(void* pArgs = nullptr) override;
	//virtual _int Update_GameObject(const _float& fDeltaTime) override;
	//virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	//virtual void Render_GameObject(const _int& iIndex)override;
public:

	virtual void Free() override;
public:

	//static CImageUI* Create(LPDIRECT3DDEVICE9 pDevice);
	static CImageUI* Create(LPDIRECT3DDEVICE9 pDevice,  void* pArg = nullptr,_bool bCheck =true);
	virtual HRESULT Add_Component(void* pArg);
	void Set_RenderID(RENDERID eID) { m_eRenderID = eID; }
	void Set_Alpha(const _bool& bState) { m_bAlpha = bState; }
	void Set_Ani(const _bool& bState, const _bool& bRoop = false) { m_bAni = bState;  m_bRoop = bRoop; }
	HRESULT Set_Image(_wstr TextureName);

	void Set_Color(_float Red, _float Green, _float Blue, _float Alpha);
	void Set_Color(_vec4 vColor);
	void Set_Size(const _vec3& vSize);
	void Set_Pos(const _vec3& vPos);
	void Set_AddPos(const _vec3& vPos);
	void Set_RoT(ROTATION eType, const _float& fAngle);
	void Set_Dir(ROTATION eType, const _float& fAngle);
	void Set_Font(_wstr wstrfont) { m_font = wstrfont; }
	void Set_TextureIndex(_int index) { m_iTextureIndex = index; }
	void Set_PassIndex(_uint index) { m_iPass = index; }
	void Set_AniState(_bool bAni, _int x, _int y, _float fSpeed);
	void Set_Amount(_float amount) { m_fAmount = amount; }
	void Set_AddAmount(_float amount) { m_fAmount += amount; }
	void Set_CoolTime(_float Time);
	_float Get_Amount() { return m_fAmount; }
	_vec3 Get_Pos();
	_vec3 Get_Size() { return m_pTransformCom->Get_Scale(); }


	void Change_Color(_float Time);
	void Change_Size(_float Time);
	void Change_Pos(_float Time);

	void Change_Color_To(_float Time, _float Red, _float Green, _float Blue, _float Alpha);
	void Change_Color_From_To(_float Time, _vec4 vFrom, _vec4 vTo);

	void Change_Size_To(_float Time, _vec3 vToSize);
	void Change_Size_From_To(_float Time, _vec3 vFromSize, _vec3 vToSize);

	void Change_Pos_To(_float Time, _vec3 vToPos);
	void Change_Pos_From_To(_float Time, _vec3 vFromPos, _vec3 vToPos);


	void Move_Pos(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta);

	const _bool Is_MoveEnd() { return !m_bChangeStart_Pos; }
	const _bool Is_ColorEnd() { return !m_bChangeStart; }
	const _bool Is_SizeEnd() { return !m_bChangeStart_Size; }

	void Visible_WhileTime(_float Time);


private:
	_bool m_bAlpha = false;
	_bool m_bOrth = false;

	_bool m_bRoop = false;
	_bool m_bAni = false;
	_float m_fFrame = 0.f;

	_vec4 m_vColor = { 0.f, 0.f, 0.f, 0.f };
	_int m_iIndex = 0;
	_wstr m_FilePath;
	_wstr m_font;

	//For.Color
	_vec4 m_vStartColor = { 0.f, 0.f, 0.f, 0.f };
	_vec4 m_vEndColor = { 0.f, 0.f, 0.f, 0.f };
	_float m_fTime = 0.f;
	_float m_fCurTime = 0.f;
	_bool m_bChangeStart = false;

	//For.Size
	_vec3 m_vStartSize = {0.f, 0.f, 0.f};
	_vec3 m_vStartSizeSave = { 0.f, 0.f, 0.f };
	_vec3 m_vEndSize{};
	_float m_fTime_Size = 0.f;
	_float m_fCurTime_Size = 0.f;
	_bool m_bChangeStart_Size = false;

	//For.Translate
	_vec3 m_vStartPos = { 0.f, 0.f, 0.f };
	_vec3 m_vEndPos = { 0.f, 0.f, 0.f };
	_float m_fTime_Pos = 0.f;
	_float m_fCurTime_Pos = 0.f;
	_bool m_bChangeStart_Pos = false;

	_float m_fTime_Visible = 0.f;
	_bool m_bChangeStart_Visible = false;

	_int m_iTextureIndex = 0;
	_vec3 m_vFlatPos = { 0.f, 0.f, 0.f };
	_uint m_iPass = 0;

	//For.Animation
	_int m_iAniIndex = 0;
	_int m_iAniX = 0;
	_int m_iAniY = 0;
	_float m_fAniSpeed = 1.f;
	_float m_fAmount = 0.f;
	_float m_fTimeDelta = 0.f;
	_float m_fCoolTime = 0.f;
	_float m_fCoolDeltaTime = 0.f;



	RENDERID	m_eRenderID = RENDERID::RENDER_UI;
	_uint		m_eSceneID = 0;
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect);
};
#define __IMAGEUI_H__
#endif // !__IMAGEUI_H__

