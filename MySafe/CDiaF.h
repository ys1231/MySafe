#pragma once

#include"md5.h"
// CDiaF 对话框
#include<vector>
using std::vector;
class CDiaF : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaF)

public:
	CDiaF(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDiaF();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 白名单
	CString m_WhiteName;
	//添加自定义函数接收服务器传来的消息
	void OnReceive(wchar_t sztext[]);
	//添加白名单点击事件(到服务器到数据库)
	afx_msg void OnBnClickedButton3();
	MD5 md5;
	// MD5查杀
	CString m_Md5CS;

	// 病毒特征
	CString m_Virus_Tezhen;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton1();
	// 通知窗口绑定变量
	CString m_TZ;
	// 全路径查杀
	CString m_QuanPath;
	void QuanLuJCX();
	vector<CStringA>m_BingDuShuz;
	afx_msg void OnBnClickedButton2();
};
