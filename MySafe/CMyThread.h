#ifndef CMYTHREAD_H
#define CMYTHREAD_H


// CMyThread 对话框

class CMyThread : public CDialogEx
{
	DECLARE_DYNAMIC(CMyThread)

public:
	CMyThread(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyThread();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1_ZI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 进程子窗口list
	CListCtrl m_ZI_ListCtrl;
	//模块名称大小
	void PrintProcessModuleList(DWORD dwPid);
	//线程名称优先级
	void ListProcessThreads(DWORD dwPID);
	virtual BOOL OnInitDialog();
};
#endif