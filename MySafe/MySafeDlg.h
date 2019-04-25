
// MySafeDlg.h: 头文件
//
#ifndef MYSAFEDLG_H
#define MYSAFEDLG_H


#include "CMyTable.h"
#include "CDiaA.h"
#include"CDiaB.h"

// CMySafeDlg 对话框
class CMySafeDlg : public CDialogEx
{
// 构造
public:
	CMySafeDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYSAFE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 我的Table控件
	CMyTable m_MyTable;
	//菜单资源
	CMenu m_Menu;
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	afx_msg void MyShoutDown();
	afx_msg void Exit_Window();
	afx_msg void LookWork_sta();
	afx_msg void Reboot_s();
	afx_msg void Quit_Window();
};


#endif // !MYSAFEDLG_H