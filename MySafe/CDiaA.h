#ifndef CDIAA_H
#define CDIAA_H



// CDiaA 对话框

class CDiaA : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaA)

public:
	CDiaA(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDiaA();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 我的list变量
	CListCtrl m_ListCtrl;
	//遍历进程线程的函数
	void ShowProcessAndThreads();
	//枚举窗口回调函数

	virtual BOOL OnInitDialog();
	void Set_set_set();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

#endif 