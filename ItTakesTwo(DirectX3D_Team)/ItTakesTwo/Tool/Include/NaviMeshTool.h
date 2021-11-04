#pragma once
#include "afxwin.h"
#include "Engine_Define.h"
#include "ToolNavi.h"


// CNaviMeshTool 대화 상자입니다.

class CNaviMeshTool : public CDialog
{
	DECLARE_DYNAMIC(CNaviMeshTool)

public:
	CNaviMeshTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNaviMeshTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERATOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL	OnInitDialog();
	afx_msg void	OnLbnSelchangeNaviMeshList();
	afx_msg void	OnBnClickedNaviMeshLoad();
	afx_msg void	OnBnClickedNaviMeshSave();
	afx_msg void	OnBnClickedCreateGroup();
	afx_msg void	OnBnClickedDeleteGroup();
	afx_msg void	OnBnClickedDeleteCell();
	afx_msg void	OnLbnSelchangeGroupList();
	afx_msg void	OnBnClickedCellStateChange();
	void			HorizontalScroll();

public:
	void			Update_NaviMeshTool();

public:
	CButton			m_MouseState[5];
	CButton			m_CellState[2];
	CButton			m_Lock;
	CListBox		m_NaviList;
	CString			m_FirstX;
	CString			m_FirstY;
	CString			m_FirstZ;
	CString			m_SecondX;
	CString			m_SecondY;
	CString			m_SecondZ;
	CString			m_ThirdX;
	CString			m_ThirdY;
	CString			m_ThirdZ;
	int				m_iMouseState;
	int				m_iPickCount;
	int				m_CellIndex;
	int				m_CellGroupIndex;
	TCHAR			m_CellText[MAX_PATH];
	TCHAR			m_CellGroupText[MAX_PATH];
	CListBox		m_GroupList;
	unsigned int	m_eCellState;
	bool			m_bLock;
	float m_fPosY;
	afx_msg void OnBnClickedButton9();
};
