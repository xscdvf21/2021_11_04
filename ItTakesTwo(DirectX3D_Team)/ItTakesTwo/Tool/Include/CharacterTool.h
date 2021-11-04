#pragma once
#include "afxwin.h"
#include "Base.h"
#include "Engine_Define.h"


// CCharacterTool ��ȭ �����Դϴ�.

class CCharacterTool : public CDialog
{
	DECLARE_DYNAMIC(CCharacterTool)

public:
	CCharacterTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCharacterTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARACTERTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void	OnBnClickedCreateObject();
	afx_msg void	OnLbnSelchangeObjectList();
	afx_msg void	OnBnClickedObjectListSave();
	afx_msg void	OnBnClickedObjectListLoad();
	afx_msg void	OnBnClickedSelectDelete();
	afx_msg void	OnBnClickedSelectCreate();
	afx_msg void	OnBnClickedScalePosSet();
	afx_msg void	OnBnClickedAddMap();
	virtual BOOL	OnInitDialog();
	void			HorizontalScroll();

public:
	void			Update_ObjectTool();

public:
	CButton			m_MeshType[2];
	CButton			m_Alpha[2];
	CButton			m_SelectPos[3];
	CListBox		m_ObjectList;
	_uint			m_eMeshType;
	_uint			m_eRenderType;
	_uint			m_CheckPos;
	float			m_ScaleX;
	float			m_ScaleY;
	float			m_ScaleZ;
	float			m_PositionX;
	float			m_PositionY;
	float			m_PositionZ;
	int				m_TextureIndex;
	TCHAR			m_Texture[MAX_PATH];
	CString			m_Map;

	vector<_wstr>	m_vecObject;
};
