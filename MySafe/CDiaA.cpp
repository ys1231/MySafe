// CDiaA.cpp: 实现文件
//

#include "stdafx.h"
#include "MySafe.h"
#include "CDiaA.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include"CMyThread.h"
#include <vector>

// CDiaA 对话框
//给子窗口传递的PID
DWORD g_ProcesID = NULL;

IMPLEMENT_DYNAMIC(CDiaA, CDialogEx)

CDiaA::CDiaA(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CDiaA::~CDiaA()
{
}

void CDiaA::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDiaA, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDiaA::OnNMClickList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDiaA::OnNMRClickList1)
END_MESSAGE_MAP()


// CDiaA 消息处理程序


	//遍历进程的函数
void CDiaA::ShowProcessAndThreads()
{
	// 1. 创建一个进程快照
	HANDLE hSnap = INVALID_HANDLE_VALUE;
	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	// 2. 使用一组API提取出进程快照的内容
	PROCESSENTRY32 ProcInfo = { sizeof(PROCESSENTRY32) };
	if (Process32First(hSnap, &ProcInfo))
	{
		int i = 0;
		do
		{
			CString str;
			str.Format(L"%d", ProcInfo.th32ProcessID);
			m_ListCtrl.InsertItem(i, str);
			m_ListCtrl.SetItemText(i, 1, ProcInfo.szExeFile);
			++i;
		} while (Process32Next(hSnap, &ProcInfo));
	}
	CloseHandle(hSnap);
}
//枚举窗口回调函数
std::vector<CString>g_l_str[3];
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	TCHAR szTitle[MAX_PATH] = { 0 };
	TCHAR szClass[MAX_PATH] = { 0 };
	int nMaxCount = MAX_PATH;

	PTCHAR lpClassName = szClass;
	PTCHAR lpWindowName = szTitle;

	GetWindowText(hwnd, lpWindowName, nMaxCount);
	GetClassName(hwnd, lpClassName, nMaxCount);


	//EnumChildWindows(hwnd, EnumChildProc, lParam);
	if (IsWindowVisible(hwnd))
	{
		if (wcscmp(lpWindowName, L""))
		{
			CString str;
			str.Format(L"%08X", hwnd);
			//str1.Format(L"%s",lpWindowName);
			//str2.Format(L"%s",lpClassName);
			g_l_str[0].push_back(str);
			g_l_str[1].push_back(lpWindowName);
			g_l_str[2].push_back(lpClassName);


		}
	}
	return TRUE;

}
BOOL CDiaA::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// TODO:  在此添加额外的初始化
	//设置标题 列
	m_ListCtrl.InsertColumn(0, L"进程ID", LVCFMT_LEFT,60);
	m_ListCtrl.InsertColumn(1, L"进程名称", LVCFMT_LEFT,100);
	m_ListCtrl.InsertColumn(2, L"窗口句柄", LVCFMT_LEFT,100);
	m_ListCtrl.InsertColumn(3, L"窗口名称", LVCFMT_LEFT,150);
	m_ListCtrl.InsertColumn(4, L"窗口类名", LVCFMT_LEFT,150);
	
	//获取进程ID和名称
	ShowProcessAndThreads();
	/*EnumWindows(EnumWindowsProc, 0);*/
	EnumWindows(EnumWindowsProc, 0);
	int l_size = g_l_str[0].size();
	for(int i=0;i<l_size;++i)
	{
		m_ListCtrl.SetItemText(i, 2, g_l_str[0][i]);
		m_ListCtrl.SetItemText(i, 3, g_l_str[1][i]);
		m_ListCtrl.SetItemText(i, 4, g_l_str[2][i]);
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDiaA::Set_set_set()
{
	m_ListCtrl.DeleteAllItems();
	//获取进程ID和名称
	ShowProcessAndThreads();
	/*EnumWindows(EnumWindowsProc, 0);*/
	EnumWindows(EnumWindowsProc, 0);
	int l_size = g_l_str[0].size();
	for (int i = 0; i < l_size; ++i)
	{
		m_ListCtrl.SetItemText(i, 2, g_l_str[0][i]);
		m_ListCtrl.SetItemText(i, 3, g_l_str[1][i]);
		m_ListCtrl.SetItemText(i, 4, g_l_str[2][i]);
	}
}


void CDiaA::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int had = pNMItemActivate->iItem;
	CString str = m_ListCtrl.GetItemText(had, 0);
	g_ProcesID = _wtoi(str);
	CMyThread Dilg;
	Dilg.DoModal();

	*pResult = 0;
}

//单击右键结束进程
void CDiaA::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int had = pNMItemActivate->iItem;
	int PID = _tstoi(m_ListCtrl.GetItemText(had, 0));
	HANDLE hProcess = OpenProcess(
		PROCESS_ALL_ACCESS,	// 进程对象的所有可能访问权限
		FALSE,				// 句柄释放可以继承
		PID					// 进程ID	
	);
	// 结束进程
	if (TerminateProcess(hProcess, 0)) {
		MessageBox(L"进程关闭成功!");
		Set_set_set();
	}
	else
		MessageBox(L"进程关闭失败!");
	
	

	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
