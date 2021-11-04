
// ToolView.h : CToolView Ŭ������ �������̽�
//

#pragma once

#include "Base.h"
#include "Engine_Define.h"
#include "Layer.h"
#include "ToolTerrain.h"
#include "ToolCamera.h"
#include "ToolObject.h"
#include "ToolNavi.h"

BEGIN(Engine)

class CGraphicDevice;
class CManagement;

END


class CToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

	// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

	// �۾��Դϴ�.
public:

	// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// �����Դϴ�.
public:
	virtual ~CToolView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent); // Update

public:
	HRESULT		SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev, Engine::CManagement** ppManagementClass);
	HRESULT		Resource_Loading();

public:
	HRESULT		Ready_Environment_Layer(const _tchar*  pLayerTag);
	HRESULT		Ready_Terrain_Layer(const _tchar * pLayerTag);
	HRESULT		Ready_GameObject_Layer(const _tchar*  pLayerTag);
	HRESULT		Ready_LightInfo(void);

public:
	void		KeyInput(_float fTimeDelta);
	void		Mouse_Picking();
	void		ObjectMove(_float fTimeDelta);

	_bool		RayPicking(_vec3* vPos, const _vec3* vtxpos, _ulong vtxcnt, _vec3 raypos, _vec3 raydir);
	_bool		Translation_ToLocalSpace(HWND hWnd, _uint iWinCX, _uint iWinCY, CCamera * pCamera, _matrix * pWorldMatrix);
	_bool		Translation_ToWorldSpace(HWND hWnd, _uint iWinCX, _uint iWinCY, CCamera * pCamera);

public:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	Engine::CGraphicDevice*	m_pDeviceClass;
	Engine::CManagement*	m_pManagementClass;
	HWND					g_hWnd;
	_float					m_fTime;
	_bool					m_bWireFrame = false;
	_bool					m_bNavi = false;
	_vec3					m_vRayPos;
	_vec3					m_vRayDir;
	_vec3					m_vMouse;
	_vec3					m_vFirst, m_vSecond, m_vThird;


	D3DXPLANE				m_tPlane;
public:
	map<_wstr, CLayer*>		m_mapLayer;
};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
{
	return reinterpret_cast<CToolDoc*>(m_pDocument);
}
#endif

