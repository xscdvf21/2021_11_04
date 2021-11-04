// ToolPhyXNavi.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PhyXNaviTool.h"
#include "afxdialogex.h"



IMPLEMENT_DYNAMIC(CPhyXNaviTool, CDialogEx)

CPhyXNaviTool::CPhyXNaviTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TOOLPHYXNAVI, pParent)
{

}

CPhyXNaviTool::~CPhyXNaviTool()
{
}

void CPhyXNaviTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPhyXNaviTool, CDialogEx)
END_MESSAGE_MAP()


