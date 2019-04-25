
// MySafe.h: PROJECT_NAME 应用程序的主头文件
//



#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#ifndef MYSAFE_H
#define MYSAFE_H
// CMySafeApp:
// 有关此类的实现，请参阅 MySafe.cpp
//

class CMySafeApp : public CWinApp
{
public:
	CMySafeApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CMySafeApp theApp;



#endif // !MYSAFE_H