
// MySafeDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MySafe.h"
#include "MySafeDlg.h"
#include "afxdialogex.h"
#include "CDia0.h"
#include"CDiaD.h"
#include"CDiaE.h"
#pragma region

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void Exit_Windows();
//	afx_msg void Look_WorkStat();
//	afx_msg void Reboot_Window();
//	afx_msg void Quit_Window();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
	
	
	
END_MESSAGE_MAP()


// CMySafeDlg 对话框



CMySafeDlg::CMySafeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYSAFE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMySafeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MYTAB, m_MyTable);
}

BEGIN_MESSAGE_MAP(CMySafeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_32772, &CMySafeDlg::MyShoutDown)
	ON_COMMAND(ID_32776, &CMySafeDlg::Exit_Window)
	ON_COMMAND(ID_32775, &CMySafeDlg::LookWork_sta)
	ON_COMMAND(ID_32773, &CMySafeDlg::Reboot_s)
	ON_COMMAND(ID_32774, &CMySafeDlg::Quit_Window)
END_MESSAGE_MAP()


// CMySafeDlg 消息处理程序

BOOL CMySafeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//EnableShutDownPriv();
	//加载菜单资源
	m_Menu.LoadMenuW(IDR_MENU1);
	this->SetMenu(&m_Menu);

	m_MyTable.InsertItem(0, L"系统信息");
	m_MyTable.InsertItem(1, L"任务管理");
	m_MyTable.InsertItem(2, L"垃圾清理");
	m_MyTable.InsertItem(3, L"PE文件解析");
	m_MyTable.InsertItem(4, L"服务管理");
	//给子窗口指针赋值
	m_MyTable.m_Dialg[0] = new CDia0();
	m_MyTable.m_Dialg[1] = new CDiaA();
	m_MyTable.m_Dialg[2] = new CDiaB();
	m_MyTable.m_Dialg[3] = new CDiaD();
	m_MyTable.m_Dialg[4] = new CDiaE();
	//创建子窗口
	m_MyTable.m_Dialg[0]->Create(IDD_DIALOG3, &m_MyTable);
	m_MyTable.m_Dialg[1]->Create(IDD_DIALOG1, &m_MyTable);
	m_MyTable.m_Dialg[2]->Create(IDD_DIALOG2, &m_MyTable);
	m_MyTable.m_Dialg[3]->Create(IDD_DIALOG4, &m_MyTable);
	m_MyTable.m_Dialg[4]->Create(IDD_DIALOG5, &m_MyTable);
	//控制窗口大小
	CRect rc;
	m_MyTable.GetClientRect(rc);
	rc.DeflateRect(2, 23, 2, 2);
	m_MyTable.m_Dialg[0]->MoveWindow(rc);
	m_MyTable.m_Dialg[1]->MoveWindow(rc);
	m_MyTable.m_Dialg[2]->MoveWindow(rc);
	m_MyTable.m_Dialg[3]->MoveWindow(rc);
	m_MyTable.m_Dialg[4]->MoveWindow(rc);
	//显示第一个 子窗口
	m_MyTable.m_Dialg[0]->ShowWindow(SW_SHOW);
	m_MyTable.m_Dialg[1]->ShowWindow(SW_HIDE);
	m_MyTable.m_Dialg[2]->ShowWindow(SW_HIDE);
	m_MyTable.m_Dialg[3]->ShowWindow(SW_HIDE);
	m_MyTable.m_Dialg[4]->ShowWindow(SW_HIDE);












	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMySafeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMySafeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMySafeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMySafeDlg::OnOK()
{
}

BOOL GetWindowsPower()
{
	HANDLE hProcess = NULL;
	HANDLE hToken = NULL;
	LUID uID = { 0 };
	TOKEN_PRIVILEGES stToken_Privileges = { 0 };

	hProcess = GetCurrentProcess();  //获取当前应用程序进程句柄

	if (!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))  //打开当前进程的访问令牌句柄(OpenProcessToken函数调用失败返回值为零)
		return FALSE;

	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &uID))  //获取权限名称为"SeShutdownPrivilege"的LUID(LookupPrivilegeValue函数调用失败返回值为零)
		return FALSE;

	stToken_Privileges.PrivilegeCount = 1;  //欲调整的权限个数
	stToken_Privileges.Privileges[0].Luid = uID;  //权限的LUID
	stToken_Privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;  //权限的属性,SE_PRIVILEGE_ENABLED为使能该权限

	if (!AdjustTokenPrivileges(hToken, FALSE, &stToken_Privileges, sizeof stToken_Privileges, NULL, NULL))  //调整访问令牌里的指定权限(AdjustTokenPrivileges函数调用失败返回值为零)
		return FALSE;

	if (GetLastError() != ERROR_SUCCESS)  //查看权限是否调整成功
		return FALSE;

	CloseHandle(hToken);
	return TRUE;
}


void CMySafeDlg::MyShoutDown()
{
	// TODO: 在此添加命令处理程序代码
	GetWindowsPower();
	ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, NULL);
	//ReSetWindows(EWX_SHUTDOWN, true);//关机

}



void CMySafeDlg::Exit_Window()
{
	// TODO: 在此添加命令处理程序代码
	GetWindowsPower();
	ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, NULL);
}


void CMySafeDlg::LookWork_sta()
{
	// TODO: 在此添加命令处理程序代码
	GetWindowsPower();
	LockWorkStation();
}


void CMySafeDlg::Reboot_s()
{
	// TODO: 在此添加命令处理程序代码
	GetWindowsPower();
	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, NULL);
}


void CMySafeDlg::Quit_Window()
{
	// TODO: 在此添加命令处理程序代码
	//AfxGetMainWnd()->CloseWindow();
	AfxGetApp()->m_pMainWnd->SendMessage(WM_CLOSE);
}
