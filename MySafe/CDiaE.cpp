// CDiaE.cpp: 实现文件
//

#include "stdafx.h"
#include "MySafe.h"
#include "CDiaE.h"
#include "afxdialogex.h"


// CDiaE 对话框

IMPLEMENT_DYNAMIC(CDiaE, CDialogEx)

CDiaE::CDiaE(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{

}

CDiaE::~CDiaE()
{
}

void CDiaE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListService);
}


BEGIN_MESSAGE_MAP(CDiaE, CDialogEx)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDiaE::OnNMDblclkList1)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST1, &CDiaE::OnNMRDblclkList1)
END_MESSAGE_MAP()


// CDiaE 消息处理程序


BOOL CDiaE::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_ListService.InsertColumn(0, L"服务名", LVCFMT_LEFT, 180);
	m_ListService.InsertColumn(1, L"服务状态", LVCFMT_LEFT, 100);
	m_ListService.InsertColumn(2, L"服务类型", LVCFMT_LEFT, 170);
	m_ListService.InsertColumn(3, L"路径信息", LVCFMT_LEFT, 280);
	Get_Service();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
//获取服务信息
void CDiaE::Get_Service()
{
	m_ListFileName.clear();
	//1.打开远程计算机服务控制管理器        //所有权限 
	m_hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//2.第一次调用,获取需要的内存大小
	DWORD l_dwServiceNum = 0;
	DWORD l_dwSize = 0;         //要返回的属性       //服务类型    //所有服务状态		           //需要的大小//缓冲区中服务个数
	EnumServicesStatusEx(m_hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, NULL, 0, &l_dwSize, &l_dwServiceNum, NULL, NULL);
	//3.申请需要的内存,第二次调用
	LPENUM_SERVICE_STATUS_PROCESS l_pEnumService = (LPENUM_SERVICE_STATUS_PROCESS)new char[l_dwSize];
	//4.第二次枚举
	BOOL l_bStatus = FALSE;
	l_bStatus = EnumServicesStatusEx(m_hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, (PBYTE)l_pEnumService, l_dwSize, &l_dwSize, &l_dwServiceNum, NULL, NULL);
	if (l_dwServiceNum) {
		//5.遍历信息
		CString str = {};
		for (int i = 0; i < l_dwServiceNum; ++i)
		{
			//------------获取基础信息----------------
			//服务名
			str.Empty();
			str.Format(L"%s", l_pEnumService[i].lpServiceName);
			m_ListService.InsertItem(i, str);
			m_ListFileName.push_back(str);
			//printf("服务名:%ls \n", str);

			//--------------服务状态----------
			//  已停止 正在运行 正在暂停
			str.Empty();
			switch (l_pEnumService[i].ServiceStatusProcess.dwCurrentState) {

			case SERVICE_STOPPED:
				str = "停止";
				break;
			case SERVICE_START_PENDING:
				str = "服务启动挂起";
				break;
			case SERVICE_STOP_PENDING:
				str = "服务停止挂起";
				break;
			case SERVICE_RUNNING:
				str = "服务运行";
				break;
			case SERVICE_CONTINUE_PENDING:
				str = "服务 继续 挂起";
				break;
			case SERVICE_PAUSE_PENDING:
				str = "服务暂停挂起";
				break;
			case SERVICE_PAUSED:
				str = "服务暂停";
				break;
			default:str = "其他";
				break;
			}
			m_ListService.SetItemText(i, 1, str);
			//-------------服务类型--------------------
			//文件系统驱动服务  驱动服务 独立进程服务
			str.Empty();
			switch (l_pEnumService[i].ServiceStatusProcess.dwServiceType)
			{

			case SERVICE_FILE_SYSTEM_DRIVER:
				str = "该服务是文件系统驱动程序";
				break;
			case SERVICE_WIN32_OWN_PROCESS:
				str = "该服务是独立进程服务";
				break;
			case SERVICE_KERNEL_DRIVER:
				str = "该服务是设备驱动程序";
			default:str = "其他";
				break;
			}
			m_ListService.SetItemText(i, 2, str);

			//------------获取服务的详细信息----------------------
			//打开服务
			SC_HANDLE l_hService = OpenService(m_hSCM,//服务控制管理器的句柄
				l_pEnumService[i].lpServiceName,//服务名
				SERVICE_QUERY_CONFIG			//打开权限
			);
			//第一次调用获取需要的缓冲区大小
			QueryServiceConfig(l_hService, NULL, 0, &l_dwSize);
			//非配内存
			LPQUERY_SERVICE_CONFIG l_pServiceConfig = \
				(LPQUERY_SERVICE_CONFIG)new char[l_dwSize];
			//第二次调用,获取信息
			QueryServiceConfig(l_hService, l_pServiceConfig, l_dwSize, &l_dwSize);

			//通过上面获取到的结构体信息具体得到想要的值

			str.Empty();
			str.Format(L"%s", l_pServiceConfig->lpBinaryPathName);
			m_ListService.SetItemText(i, 3, str);

			//获取路径信息 
			//cout << "路径信息" << l_pServiceConfig->lpBinaryPathName << endl;
		}
	}
	else
		MessageBox(L"以管理员方式打开!!!!!!");
	
}


//双击鼠标左键
void CDiaE::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//获取索引
	int n = pNMItemActivate->iItem;

	// 打开www服务。
	SC_HANDLE hSvc = ::OpenService(m_hSCM, m_ListFileName[n],
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL)
	{
		MessageBox(L"打开服务失败");
		//::CloseServiceHandle(m_hSCM);
		return;
	}
	// 获得服务的状态
	SERVICE_STATUS status;
	if (::QueryServiceStatus(hSvc, &status) == FALSE)
	{
		MessageBox(L"获取服务状态失败");
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(m_hSCM);
		return;
	}
	//如果处于停止状态则启动服务，否则停止服务。
	if (status.dwCurrentState == SERVICE_RUNNING)
	{
		// 停止服务
		if (::ControlService(hSvc,
			SERVICE_CONTROL_STOP, &status) == FALSE)
		{
			MessageBox(L"停止服务失败");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(m_hSCM);
			return;
		}
		// 等待服务停止
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_STOPPED)
			{
				MessageBox(L"停止成功");
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(m_hSCM);
				Get_Service();
				return;
			}
		}
	}
	else if (status.dwCurrentState == SERVICE_STOPPED)
	{
		// 启动服务
		if (::StartService(hSvc, NULL, NULL) == FALSE)
		{
			MessageBox(L"启动服务失败");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(m_hSCM);
			return;
		}
		// 等待服务启动
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_RUNNING)
			{
				MessageBox(L"启动服务成功");
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(m_hSCM);
				Get_Service();
				return;
			}
		}
	}
	MessageBox(L"启动失败");
	::CloseServiceHandle(hSvc);
	::CloseServiceHandle(m_hSCM);
	return;


	*pResult = 0;
}

/////////////////////////////////////////////////////////////////

//双击右键关闭进程
void CDiaE::OnNMRDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//获取索引
	int n = pNMItemActivate->iItem;
	// 打开www服务。
	SC_HANDLE hSvc = ::OpenService(m_hSCM, m_ListFileName[n],
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	// 获得服务的状态
	SERVICE_STATUS status;
	if (::QueryServiceStatus(hSvc, &status) == FALSE)
	{
		MessageBox(L"获取服务状态失败");
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(m_hSCM);
		return;
	}
	//如果处于停止状态则启动服务，否则停止服务。
	if (status.dwCurrentState == SERVICE_RUNNING)
	{
		// 停止服务
		if (::ControlService(hSvc,
			SERVICE_CONTROL_STOP, &status) == FALSE)
		{
			MessageBox(L"停止服务失败");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(m_hSCM);
			return;
		}
		// 等待服务停止
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_STOPPED)
			{
				MessageBox(L"停止成功");
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(m_hSCM);
				Get_Service();
				return;
			}
		}
	}


	*pResult = 0;
}
