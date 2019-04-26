// MySock.cpp: 实现文件
//

#include "stdafx.h"
#include "MySafe.h"
#include "MySock.h"
#include"CDiaF.h"

// MySock
extern HANDLE g_Hwnd;
MySock::MySock()
{
}

MySock::~MySock()
{
}

void MySock::InitSock()
{
	AfxSocketInit();
	Create();
	if (!Connect(L"127.0.0.1", 12345))
	{
		MessageBoxA(nullptr, "连接服务器失败!", nullptr, 0);
		ExitProcess(-1);
	}
}


// MySock 成员函数


void MySock::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	wchar_t s[40] = { 0 };
	Receive(s, sizeof(s));

	CDiaF *Dlg = (CDiaF*)g_Hwnd;
	Dlg->OnReceive(s);
	CSocket::OnReceive(nErrorCode);
}

MySock g_sock;