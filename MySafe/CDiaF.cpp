// CDiaF.cpp: 实现文件
//

#include "stdafx.h"
#include "MySafe.h"
#include "CDiaF.h"
#include "afxdialogex.h"


// CDiaF 对话框

IMPLEMENT_DYNAMIC(CDiaF, CDialogEx)

CDiaF::CDiaF(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG6, pParent)
{

}

CDiaF::~CDiaF()
{
}

void CDiaF::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDiaF, CDialogEx)
END_MESSAGE_MAP()


// CDiaF 消息处理程序
