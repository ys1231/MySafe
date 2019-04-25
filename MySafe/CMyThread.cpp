// CMyThread.cpp: 实现文件
//

#include "stdafx.h"
#include "MySafe.h"
#include "CMyThread.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <psapi.h>


//引用父窗口的PID
extern DWORD g_ProcesID;
// CMyThread 对话框

IMPLEMENT_DYNAMIC(CMyThread, CDialogEx)

CMyThread::CMyThread(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1_ZI, pParent)
{

}

CMyThread::~CMyThread()
{
}

void CMyThread::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ZI_ListCtrl);
}


BEGIN_MESSAGE_MAP(CMyThread, CDialogEx)
END_MESSAGE_MAP()

//获取进程模块
void CMyThread::PrintProcessModuleList(DWORD dwPid)
{
	
	HANDLE hSnap = INVALID_HANDLE_VALUE;
	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,dwPid);

	MODULEENTRY32 moduleInfo = { sizeof(MODULEENTRY32) };
	if (Module32First(hSnap, &moduleInfo))
	{
		int i = 0;
		do
		{
			//CString str;
			//str.Format(L"%d", moduleInfo.modBaseAddr);
			//m_Name.push_back(str);
			//m_Size.push_back(moduleInfo.szModule);
			m_ZI_ListCtrl.InsertItem(i, moduleInfo.szModule);
			//m_ZI_ListCtrl.SetItemText(i, 1, str);
			++i;
		} while (Module32Next(hSnap, &moduleInfo));
	}
	CloseHandle(hSnap);

}
//获取线程信息
void CMyThread::ListProcessThreads(DWORD dwPID)
{
	
	//初始化为无效的窗口句柄
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;
	//创建快照
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return;
	//s设置输入参数
	te32.dwSize = sizeof(THREADENTRY32);
	//开始获取信息
	if (!Thread32First(hThreadSnap, &te32))
	{
		CloseHandle(hThreadSnap);
		return;
	}
	int i = 0;
	if (te32.th32OwnerProcessID == dwPID)
		do{
			CString str;
			CString str1;
			str.Format(L"%d", te32.th32ThreadID);
			str1.Format(L"%d", te32.tpBasePri);
			m_ZI_ListCtrl.SetItemText(i, 1,str);
			m_ZI_ListCtrl.SetItemText(i, 2,str1);
			
			++i;
		} while (Thread32Next(hThreadSnap, &te32));

	CloseHandle(hThreadSnap);
}

BOOL CMyThread::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	m_ZI_ListCtrl.InsertColumn(0, L"模块名字", LVCFMT_LEFT, 80);
	//m_ZI_ListCtrl.InsertColumn(1, L"模块内存", LVCFMT_LEFT, 100);
	m_ZI_ListCtrl.InsertColumn(1, L"线程ID", LVCFMT_LEFT, 60);
	m_ZI_ListCtrl.InsertColumn(2, L"线程优先级", LVCFMT_LEFT, 80);
	
	PrintProcessModuleList(g_ProcesID);
	ListProcessThreads(g_ProcesID);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
