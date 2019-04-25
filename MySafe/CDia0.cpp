// CDia0.cpp: 实现文件
//

#include "stdafx.h"
#include "MySafe.h"
#include "CDia0.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <psapi.h>
CProgressCtrl CDia0::m_ProGres_CPU;
CProgressCtrl CDia0::m_ProGres_Mem;
CEdit CDia0::m_Cpu;
CEdit CDia0::m_Mem;



////////////////////////////////////////////////

//windows 内存 使用率
DWORD getWin_MemUsage() {
	MEMORYSTATUS ms;
	::GlobalMemoryStatus(&ms);//当前系统内存使用率
	return ms.dwMemoryLoad;
}

//2.获取windowsCPU使用率
//
//__int64 CompareFileTime(FILETIME time1, FILETIME time2)
//{
//	__int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime;
//	__int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime;
//
//	return (b - a);
//}
////WIN CPU使用情况
double FILETIME2Double(const _FILETIME& fileTime)
{
	return double(fileTime.dwHighDateTime*4.294967296e9) + double(fileTime.dwLowDateTime);
}
////保存CPU使用率的全局变量
//__int64 g_cpu = 0;
//
//void getWin_CpuUsage() {
//	HANDLE hEvent;//临时句柄
//	BOOL res;
//	FILETIME preidleTime;
//	FILETIME prekernelTime;
//	FILETIME preuserTime;
//	FILETIME idleTime;
//	FILETIME kernelTime;
//	FILETIME userTime;
//
//	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
//	preidleTime = idleTime;
//	prekernelTime = kernelTime;
//	preuserTime = userTime;
//
//	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL); // 初始值为 nonsignaled ，并且每次触发后自动设置为nonsignaled
//
//	while (true) {
//
//		res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
//
//		__int64 idle = CompareFileTime(preidleTime, idleTime);
//		__int64 kernel = CompareFileTime(prekernelTime, kernelTime);
//		__int64 user = CompareFileTime(preuserTime, userTime);
//
//		 int l_cpu= (kernel + user - idle) * 100 / (kernel + user+1);
//		//__int64 cpuidle = (idle) * 100 / (kernel + user);
//		//cout << "CPU利用率:" << cpu << "%" << " CPU空闲率:" << cpuidle << "%" << endl;
//		 if (l_cpu > 100||l_cpu<5)break;
//		 CDia0::m_ProGres_CPU.SetPos(l_cpu);
//		 CString str;
//		 str.Format(L"%d %%", l_cpu);
//		 CDia0::m_Cpu.SetWindowTextW(str);
//
//		preidleTime = idleTime;
//		prekernelTime = kernelTime;
//		preuserTime = userTime;
//		WaitForSingleObject(hEvent, 500);
//	}
//}
// 计算CUP使用清空
void GetCpuUsage()
{
	//		  空闲时间  内核时间   用户时间
	_FILETIME  idleTime, kernelTime, userTime;
	// 获取时间
	GetSystemTimes(&idleTime, &kernelTime, &userTime);

	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	// 等待 1000 毫秒,使用内核对象等待会更精确
	WaitForSingleObject(hEvent, 1000);

	// 获取新的时间
	_FILETIME newIdleTime, newKernelTime, newUserTime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);

	// 将各个时间转换
	double dOldIdleTime = FILETIME2Double(idleTime);
	double dNewIdleTime = FILETIME2Double(newIdleTime);
	double dOldKerneTime = FILETIME2Double(kernelTime);
	double dNewKerneTime = FILETIME2Double(newKernelTime);
	double dOldUserTime = FILETIME2Double(userTime);
	double dNewUserTime = FILETIME2Double(newUserTime);

	// 计算出使用率
	int MyCUP = (int)(100.0 - (dNewIdleTime - dOldIdleTime) /
		(dNewKerneTime - dOldKerneTime + dNewUserTime - dOldUserTime) * 100.0);

	if (MyCUP > 100) return ;
	CDia0::m_ProGres_CPU.SetPos(MyCUP);
	CString str;
	str.Format(L"%d %%", MyCUP);
	CDia0::m_Cpu.SetWindowTextW(str);
	return;
}

DWORD CALLBACK ThreadProc(LPVOID pArg)
{
	while (true) {
		/*getWin_CpuUsage();*/
	 GetCpuUsage();

	}
	return 0;
}
DWORD CALLBACK ThreadProc1(LPVOID pArg)
{
	
	while (true)
	{
		//+++======================获取windows 内存使用率==================+++//
		DWORD dwValue = getWin_MemUsage();
		//printf("内存使用率为 %d %% \n", dwValue);
		CDia0::m_ProGres_Mem.SetPos(dwValue);
		CString str;
		str.Format(L"%d %%", dwValue);
		CDia0::m_Mem.SetWindowTextW(str);
		Sleep(1000);
	}
	return 0;
}

// CDia0 对话框

IMPLEMENT_DYNAMIC(CDia0, CDialogEx)

CDia0::CDia0(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)

{

}

CDia0::~CDia0()
{
}

void CDia0::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProGres_CPU);
	DDX_Control(pDX, IDC_PROGRESS2, m_ProGres_Mem);

	DDX_Control(pDX, IDC_EDIT3, m_Cpu);
	DDX_Control(pDX, IDC_EDIT4, m_Mem);
	DDX_Control(pDX, IDC_EDIT6, m_MyInFo);
}


BEGIN_MESSAGE_MAP(CDia0, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDia0::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDia0 消息处理程序

//获取设置显示CPU内存等信息
void CDia0::My_SetCPU_Mem()
{
	DWORD tid = 0;
	// 创建一个线程
	CreateThread(NULL, 0, ThreadProc,0, 0, &tid);
	HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, tid);
	DWORD tid1 = 0;
	// 创建一个线程
	CreateThread(NULL, 0, ThreadProc1,0, 0, &tid);
	HANDLE hThread1 = OpenThread(THREAD_SUSPEND_RESUME, FALSE, tid1);


}
// 内存清理
void CDia0::ClearMemory()
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return;
	}

	PROCESSENTRY32 ProcessInfo;
	ProcessInfo.dwSize = sizeof(ProcessInfo);
	int count = 0;

	//获取系统中第一个进程的信息  
	BOOL Status = Process32First(hProcessSnap, &ProcessInfo);
	while (Status)
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE,
			ProcessInfo.th32ProcessID);
		if (hProcess)
		{
			//设置进程工作区大小
			SetProcessWorkingSetSize(hProcess, -1, -1);
			//尽可能多的将指定进程的页面从工作区移除
			EmptyWorkingSet(hProcess);
			CloseHandle(hProcess);
		}
		//获取下一个进程的信息  
		Status = Process32Next(hProcessSnap, &ProcessInfo);
	}
}

BOOL CDia0::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//m_ProGres_CPU.SetRange(0,100);
	My_SetCPU_Mem();
	//m_ProGres_CPU.SetPos(80);
		


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



	


void CDia0::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//1.获取当前的内存状态
	MEMORYSTATUSEX stcMemStatusEx = { 0 };
	stcMemStatusEx.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&stcMemStatusEx);
	//用实际物理内存－去可用空闲的内存就是已经使用的内存大小
	DOUBLE preUsedMem = (DOUBLE)(stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys)/1024/1024;
	CString str;
	str.Format(L"当前内存:%.2f MB %s", preUsedMem,L"\r\n开始清理内存");
	m_MyInFo.SetWindowTextW(str);
	//2.清理内存
	ClearMemory();
	str += L"\r\n清理完成";
	m_MyInFo.SetWindowTextW(str);
	//获取清理后的信息
	GlobalMemoryStatusEx(&stcMemStatusEx);
	//用实际物理内存－去可用空闲的内存就是已经使用的内存大小
	DOUBLE afterClearUsedMem = (DOUBLE)(stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys)/1024/1024;
	CString temp;
	temp.Format(L"\r\n清理后大小%.2f MB", afterClearUsedMem);
	str +=temp;
	m_MyInFo.SetWindowTextW(str);
	
}
