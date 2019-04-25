#pragma once


// CDia0 对话框

class CDia0 : public CDialogEx
{
	DECLARE_DYNAMIC(CDia0)

public:
	CDia0(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDia0();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 我的CPU使用率进度条
	static CProgressCtrl m_ProGres_CPU;
	//获取CPU内存等信息
	void My_SetCPU_Mem();
	// 我的内存使用率进度条变量
	static CProgressCtrl m_ProGres_Mem;
	//内存清理函数
	void ClearMemory();


	virtual BOOL OnInitDialog();

	static CEdit m_Cpu;
	static CEdit m_Mem;
	afx_msg void OnBnClickedButton1();
	// 各种通知消息
	CEdit m_MyInFo;
};
