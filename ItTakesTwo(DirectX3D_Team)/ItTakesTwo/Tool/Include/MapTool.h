#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMapTool ��ȭ �����Դϴ�.

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void	OnBnClickedTerrainCreate();
	afx_msg void	OnLbnSelchangeTextureList();
	virtual BOOL	OnInitDialog();
	afx_msg void	OnDropFiles(HDROP hDropInfo);
	afx_msg void	OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void	OnEnChangeHeight();
	afx_msg void	OnEnChangeRange();
	afx_msg void	OnBnClickedTerrainSave();
	afx_msg void	OnBnClickedTerrainLoad();
	void			HorizontalScroll();

public:
	void			Update_MapTool();

public:
	int				m_TerrainX;
	int				m_TerrainZ;
	float			m_TerrainScale;
	CButton			m_MouseOption[7];
	bool			bCheck = true;
	CListBox		m_TextureList;
	TCHAR			m_Texture[MAX_PATH];

	CSliderCtrl		m_fHeightCtrl;
	CSliderCtrl		m_fRangeCtrl;
	CEdit			m_Height;
	CEdit			m_Range;
	float			m_Mul;
	float			m_fHeight;
	float			m_fRange;
	int				m_Round1Edge2Flat3;
	unsigned int	m_TextureIndex;
};
