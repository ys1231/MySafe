// CMyTable.cpp: 实现文件
//

#include "stdafx.h"
#include "MySafe.h"
#include "CMyTable.h"


// CMyTable

IMPLEMENT_DYNAMIC(CMyTable, CTabCtrl)

CMyTable::CMyTable()
{

}

CMyTable::~CMyTable()
{
}


BEGIN_MESSAGE_MAP(CMyTable, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMyTable::OnTcnSelchange)
END_MESSAGE_MAP()



// CMyTable 消息处理程序




void CMyTable::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	//获取当前选项卡的序号
	int nNum = GetCurSel();
	switch (nNum)
	{
	case 0:
		m_Dialg[0]->ShowWindow(SW_SHOW);
		m_Dialg[1]->ShowWindow(SW_HIDE);
		m_Dialg[2]->ShowWindow(SW_HIDE);
		m_Dialg[3]->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_Dialg[0]->ShowWindow(SW_HIDE);
		m_Dialg[1]->ShowWindow(SW_SHOW);
		m_Dialg[2]->ShowWindow(SW_HIDE);
		m_Dialg[3]->ShowWindow(SW_HIDE);
		break;
	case 2:
		m_Dialg[0]->ShowWindow(SW_HIDE);
		m_Dialg[1]->ShowWindow(SW_HIDE);
		m_Dialg[2]->ShowWindow(SW_SHOW);
		m_Dialg[3]->ShowWindow(SW_HIDE);
		break;
	case 3:
		m_Dialg[0]->ShowWindow(SW_HIDE);
		m_Dialg[1]->ShowWindow(SW_HIDE);
		m_Dialg[2]->ShowWindow(SW_HIDE);
		m_Dialg[3]->ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}

	*pResult = 0;
}
