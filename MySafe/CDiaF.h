#pragma once


// CDiaF 对话框

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
};
