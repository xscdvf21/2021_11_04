#pragma once
#ifndef __ButtonUI_H__
#include "UI.h"
USING(Engine)
class CButtonUI :public CUI
{
public:
	explicit CButtonUI(LPDIRECT3DDEVICE9 pDevice);
	explicit CButtonUI(const CButtonUI& other);
	virtual ~CButtonUI() = default;




	virtual HRESULT Ready_Object(void* pArg = nullptr) override;
	virtual _int Update_Object(const _float& TimeDelta) override;
	virtual _int LateUpdate_Object(const _float& TimeDelta) override;
	virtual void Render_Object(const _int& iIndex = 0) override;

public:

	virtual void Free() override;
public:
	static CButtonUI* Create(LPDIRECT3DDEVICE9 pDevice,_bool bClickable = true,_bool bEnd =false);
	virtual HRESULT Add_Component();

	void Set_Clickable(const _bool& bState) { m_bClickable = bState; }
	void Set_Alpha(const _bool& bState) { m_bAlpha = bState; }
	//void Set_NickName(const _wstr& tagName) { m_tagNickName = tagName; }
	void Set_End(const _bool& bState =true) { m_bEnd = bState; }
	void Click_Check();
	HRESULT Set_Image(_wstr TextureName);

	void Set_Color(_float Red, _float Green, _float Blue, _float Alpha);
	void Set_Color(_vec4 vColor);
	void Set_Size(const _vec3& vSize);
	void Set_Pos(const _vec3& vPos);

	void Set_Font(_wstr wstrfont) { m_font = wstrfont; }
	void Change_Color(_float Time);
	void Change_Size(_float Time);
	void Change_Pos(_float Time);

	void Change_Color_To(_float Time, _float Red, _float Green, _float Blue, _float Alpha);
	void Change_Color_From_To(_float Time, _vec4 vFrom, _vec4 vTo);

	void Change_Size_To(_float Time, _vec3 vToSize);
	void Change_Size_From_To(_float Time, _vec3 vFromSize, _vec3 vToSize);

	void Change_Pos_To(_float Time, _vec3 vToPos);
	void Change_Pos_From_To(_float Time, _vec3 vFromPos, _vec3 vToPos);	

	void Visible_WhileTime(_float Time);
private:
	_vec4 m_vColor;
	_int m_iIndex = 0;
	_wstr m_FilePath;
	_wstr m_font;
	//For.Color
	_vec4 m_vStartColor{};
	_vec4 m_vEndColor{};
	_float m_fTime = 0.f;
	_float m_fCurTime =0.f;
	_bool m_bChangeStart = false;
	//For.Size
	_vec3 m_vStartSize{};
	_vec3 m_vEndSize{};
	_float m_fTime_Size = 0.f;
	_float m_fCurTime_Size = 0.f;
	_bool m_bChangeStart_Size = false;
	//For.Translate
	_vec3 m_vStartPos{};
	_vec3 m_vEndPos{};
	_float m_fTime_Pos = 0.f;
	_float m_fCurTime_Pos = 0.f;
	_bool m_bChangeStart_Pos = false;

	_float m_fTime_Visible = 0.f;
	_bool m_bChangeStart_Visible = false;

	_bool m_bHover = false;
	_bool m_bClick = false;

	_bool m_bClickable = true;
	_bool m_bAlpha = false;


	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	_bool m_bEnd;
};
#define __ButtonUI_H__
#endif // !__ButtonUI_H__

