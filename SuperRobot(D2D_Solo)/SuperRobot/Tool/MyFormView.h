#pragma once


// CMyFormView ��ȭ �����Դϴ�.

class CMyFormView : public CFormView
{
	DECLARE_DYNAMIC(CMyFormView)

public:
	CMyFormView(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMyFormView();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORMVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
