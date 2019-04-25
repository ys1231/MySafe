#pragma once
#include <clocale>


// CDiaB 对话框

class CDiaB : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaB)

public:
	CDiaB(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDiaB();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	//查找文件清理垃圾
	void FindAndDelete(CString*Path);
	//清理各种记录 
	void Clear_file();
	// 垃圾// 垃圾
//	CEdit m_Path_L_J;
	CString m_Path_Name;
	// Vs工程目录
	CString m_VS_Path;
	CString m_VS_Clear;

	void Clear_VS();
};
