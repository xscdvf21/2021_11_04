#pragma once


// CPhyXNaviTool ��ȭ �����Դϴ�.

class CPhyXNaviTool : public CDialogEx
{
	DECLARE_DYNAMIC(CPhyXNaviTool)

public:
	CPhyXNaviTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPhyXNaviTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOOLPHYXNAVI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
