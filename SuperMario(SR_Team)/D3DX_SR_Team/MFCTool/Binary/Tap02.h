#pragma once

#include "afxwin.h"
#include "afxcmn.h"

// CTap02 ��ȭ �����Դϴ�.

class CTap02 : public CDialog
{
	DECLARE_DYNAMIC(CTap02)

public:
	CTap02(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTap02();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAP02 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	void Key_State();
	virtual BOOL OnInitDialog();

	D3DXVECTOR3		m_vObjPoint;
	D3DXVECTOR3		m_vPreObjPoint;
	float			m_fMovingSize;

	afx_msg void OnBnClickedButtonCubeload();
	afx_msg void OnBnClickedButtonMonsterload();
	afx_msg void OnBnClickedButtonEtcload();
	afx_msg void OnBnClickedButtonInstall();
	afx_msg void OnBnClickedButtonSaveall();
	afx_msg void OnEnChangeEditObjectpoint();
	CEdit m_eObjectPoint;

	
};
