#pragma once
#include<vector>
#include<winsvc.h>
using std::vector;

// CDiaE 对话框

class CDiaE : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaE)

public:
	CDiaE(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDiaE();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 我的服务列表
	CListCtrl m_ListService;
	virtual BOOL OnInitDialog();
	//获取服务信息
	void Get_Service();
	//数组保存服务名
	vector<CString>m_ListFileName;
	SC_HANDLE m_hSCM = NULL;

	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
