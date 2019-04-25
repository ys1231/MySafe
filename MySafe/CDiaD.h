#pragma once


// CDiaD 对话框

class CDiaD : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaD)

public:
	CDiaD(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDiaD();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CString m_FilePath = _T("");
	//临时存地址
	char m_FilePathA[MAX_PATH] = {};
	//文件缓冲区
	char* m_pBuf = NULL;
	//保存Nt头
	PIMAGE_NT_HEADERS m_pNt = NULL;

	//将文件读取到内存中
	void ReadFileToMemory();
	//判断是否是PE文件
	BOOL IsPeFile();
	//RVA --> FOA
	DWORD RVAtoFOA(DWORD dwRVA);
	//获取pe信息
	void Get_PE_InFo();
	//获取导出表信息
	BOOL Get_EXPORT_Directory();
	////动态显示是什么表
	//CString m_Show_IDC_GEZB;

	afx_msg void OnDropFiles(HDROP hDropInfo);
	// 入口点
	CString m_RuKouDian;
	// 镜像基址
	CString m_JxJizhi;
	// 代码基址
	CString m_DaiMaJizhi;
	// 区段数目
	CString m_QuDuanShuMu;
	// 区段信息列表
	CListCtrl m_List_Quduan;
	virtual BOOL OnInitDialog();
	// 目录列表
	CListCtrl m_Directory;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	// 模块信息
	CListCtrl m_ModeInfo;
	//清空列表
	void Clear_m_ModeInfo();
	// 模块详细信息
	CListCtrl m_Mode_XiangX_Info;
	//清空列表
	void Clear_m_Mode_XiangX_Info();
};
