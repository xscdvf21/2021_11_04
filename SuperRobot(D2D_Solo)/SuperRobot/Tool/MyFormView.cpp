// MyFormView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyFormView.h"
#include "afxdialogex.h"


// CMyFormView ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMyFormView, CFormView)

CMyFormView::CMyFormView(CWnd* pParent /*=NULL*/)
	: CFormView(IDD_MYFORMVIEW)
{

}

CMyFormView::~CMyFormView()
{
}

void CMyFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyFormView, CFormView)
END_MESSAGE_MAP()


// CMyFormView �޽��� ó�����Դϴ�.
