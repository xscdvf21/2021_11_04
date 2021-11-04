// MyFormView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyFormView.h"
#include "afxdialogex.h"


// CMyFormView 대화 상자입니다.

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


// CMyFormView 메시지 처리기입니다.
