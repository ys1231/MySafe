#pragma once

// MySock 命令目标


typedef struct _MY_MSG1 {
	int msgtype;//消息类型
	int msglens;//消息大小
	wchar_t szMsgBuff[40] = { 0 };//消息内容
}My_Msg;
class MySock : public CSocket
{
public:
	MySock();
	virtual ~MySock();
	void InitSock();
	virtual void OnReceive(int nErrorCode);
};


