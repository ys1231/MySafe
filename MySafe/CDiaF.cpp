// CDiaF.cpp: 实现文件
//

#include "stdafx.h"
#include "MySafe.h"
#include "CDiaF.h"
#include "afxdialogex.h"
#include"MySock.h"


// CDiaF 对话框
HANDLE g_Hwnd = {};
extern MySock g_sock;

IMPLEMENT_DYNAMIC(CDiaF, CDialogEx)

CDiaF::CDiaF(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG6, pParent)
	
	, m_WhiteName(_T(""))
	, m_Md5CS(_T(""))
	, m_Virus_Tezhen(_T(""))
	, m_TZ(_T(""))
	, m_QuanPath(_T(""))
{

}

CDiaF::~CDiaF()
{
}

void CDiaF::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT7, m_ListWhite);
	DDX_Text(pDX, IDC_EDIT7, m_WhiteName);
	DDX_Text(pDX, IDC_EDIT2, m_Md5CS);
	DDX_Text(pDX, IDC_EDIT8, m_Virus_Tezhen);
	DDX_Text(pDX, IDC_EDIT4, m_TZ);
	DDX_Text(pDX, IDC_EDIT5, m_QuanPath);
}


BEGIN_MESSAGE_MAP(CDiaF, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3, &CDiaF::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDiaF::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CDiaF::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDiaF::OnBnClickedButton2)
END_MESSAGE_MAP()

//添加自定义函数接收服务器传来的消息
void CDiaF::OnReceive(wchar_t sztext[])
{

	My_Msg msg = { 0 };
	memcpy(&msg, sztext, sizeof(My_Msg));
	MessageBox(msg.szMsgBuff);

}
// CDiaF 消息处理程序

//添加白名单点击事件(到服务器到数据库)
void CDiaF::OnBnClickedButton3()
{
	g_Hwnd = m_hWnd;
	UpdateData(TRUE);
	//初始化通知数据
	m_TZ.Empty();
	//计算MD5
	
	char str[40] = {};
	//判断路径是否为空
	if (m_WhiteName.IsEmpty()) {

		CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY,
			_T("Describe Files (*.dll)|*.dll|(*.exe)|*.exe|All Files (*.*)|*.*||"), NULL);
		if (dlgFile.DoModal())
		{
			//获取到文件路径
			m_WhiteName = dlgFile.GetPathName();
		}
		else
		{
			m_TZ += L"没有获取到文件路径";
			UpdateData(FALSE);
			return;
		}
	}
	wsprintfA(str, "%S", m_WhiteName);

	//放到字符指针里面
	char*str_md5=md5.GetFilePath(str);
	//打开本地数据库进行对比
	
	FILE* l_file=fopen("BaiMD.txt", "a");
	if (!l_file)
	{
		m_TZ += L"白名单添加失败失败!\r\n打不开文件\r\n";
		fclose(l_file);
		m_WhiteName.Empty();
		UpdateData(FALSE);
		return ;
	}
	int n=fprintf(l_file, "%s\n", str_md5);
	//判断文件是否写入成功
	if (n < 0)
		m_TZ += L"白名单添加失败失败";
	else 
		m_TZ += L"白名单添加成功";
	//关闭文件
	fclose(l_file);
	m_WhiteName.Empty();
	UpdateData(FALSE);
	
	//wchar_t l_White_Md5[40] = {};
	//wsprintfW(l_White_Md5, L"%S", str_md5);
	//My_Msg msg = {};

	//msg.msgtype = 1;//添加白名单
	//wcscpy(msg.szMsgBuff, l_White_Md5);
	//msg.msglens = 40*2;
	//g_sock.Send((void*)&msg, sizeof(msg));
	// TODO: 在此添加控件通知处理程序代码
}

//添加病毒特征
void CDiaF::OnBnClickedButton4()
{
	UpdateData(TRUE);
	//初始化通知数据
	m_TZ.Empty();
	//计算MD5

	char str[40] = {};
	//判断路径是否为空
	if (m_Virus_Tezhen.IsEmpty()) {
		
		CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY,
			_T("Describe Files (*.dll)|*.dll|(*.exe)|*.exe|All Files (*.*)|*.*||"), NULL);
		if (dlgFile.DoModal())
		{
			//获取到文件路径
			m_Virus_Tezhen = dlgFile.GetPathName();
		}
		else
		{
			m_TZ += L"没有获取到文件路径";
			UpdateData(FALSE);
			return;
		}
	}
	wsprintfA(str, "%S", m_Virus_Tezhen);

	char*str_md5 = md5.GetFilePath(str);
	//打开本地病毒数据库进行添加

	FILE* l_file = fopen("Virusbank.txt", "a");
	if (!l_file)
	{
		m_TZ += L"黑名单添加失败\r\n,打不开文件\r\n";
		fclose(l_file);
		m_Virus_Tezhen.Empty();
		UpdateData(FALSE);
		return;
	}
	//fseek(l_file, SEEK_SET);
	int n = fprintf(l_file,"%s\n", str_md5);
	//判断文件是否写入成功
	if (n < 0)
		m_TZ += L"黑名单添加失败失败";
	else 
		m_TZ += L"黑名单添加成功";
	//关闭文件
	fclose(l_file);
	m_Virus_Tezhen.Empty();
	UpdateData(FALSE);
	// TODO: 在此添加控件通知处理程序代码
}

//单文件MD5 开始查杀点击事件
void CDiaF::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//初始化通知数据
	m_TZ.Empty();

	char str[40] = {};
	//判断路径是否为空
	if (m_Md5CS.IsEmpty()) {

		CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY,
			_T("Describe Files (*.dll)|*.dll|(*.exe)|*.exe|All Files (*.*)|*.*||"), NULL);
		if (dlgFile.DoModal())
		{
			//获取到文件路径
			m_Md5CS = dlgFile.GetPathName();
		}
		else
		{
			m_TZ += L"没有获取到文件路径";
			UpdateData(FALSE);
			return;
		}
	}
		wsprintfA(str, "%S", m_Md5CS);

	//放到字符指针里面
	char*str_md5 = md5.GetFilePath(str);
	//打开本地数据库进行对比

	FILE* l_file = fopen("Virusbank.txt", "r");
	if (!l_file)
	{
		m_TZ += L"病毒库打开失败!\r\n是否没有创建\r\n";
		fclose(l_file);
		m_Md5CS.Empty();
		UpdateData(FALSE);
		return;
	}
	//读取所有病毒MD5数据
	//临时变量
	vector<CStringA>l_BingDuShuz;
	
	while (!feof(l_file)) {
		char l_bingdu[40] = {};
		fscanf(l_file, "%s",l_bingdu);
		if(strcmp(l_bingdu,""))
		l_BingDuShuz.push_back(l_bingdu);
	}
	//开始和取出的文件做对比
	for (int i = 0; i < l_BingDuShuz.size();i++) {
		if (!strcmp(str_md5, l_BingDuShuz[i])) {
			DeleteFile(m_Md5CS);
			m_TZ += L"是病毒已删除!\r\n";

		}
	}
	
	//关闭文件
	fclose(l_file);
	m_Md5CS.Empty();
	UpdateData(FALSE);


}

//全路径查杀
void CDiaF::OnBnClickedButton2()
{
	UpdateData(TRUE);
	//初始化通知数据
	m_TZ.Empty();
	//计算MD5

	char str[40] = {};
	//判断路径是否为空
	if (m_QuanPath.IsEmpty()) {

		CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY,
			_T("Describe Files (*.dll)|*.dll|(*.exe)|*.exe|All Files (*.*)|*.*||"), NULL);
		if (dlgFile.DoModal())
		{
			//获取到文件路径
			m_QuanPath = dlgFile.GetPathName();
		}
		else
		{
			m_TZ += L"没有获取到文件路径";
			UpdateData(FALSE);
			return;
		}
	}
	m_QuanPath += L"\\*";

	FILE* l_file = fopen("Virusbank.txt", "r");
	if (!l_file)
	{
		m_TZ += L"病毒库打开失败!\r\n是否没有创建\r\n";
		fclose(l_file);
		m_Md5CS.Empty();
		UpdateData(FALSE);
		return;
	}
	//读取所有病毒MD5数据
	//临时变量
	
	while (!feof(l_file)) {
		char l_bingdu[40] = {};
		fscanf(l_file, "%s", l_bingdu);
		if (strcmp(l_bingdu, ""))
			m_BingDuShuz.push_back(l_bingdu);
	}
	


	QuanLuJCX();
	

	
	m_QuanPath.Empty();
	UpdateData(FALSE);


	// TODO: 在此添加控件通知处理程序代码
}

void CDiaF::QuanLuJCX()
{
	//setlocale(LC_ALL, "chs");//注意系统信息中需要输出中文的地方需要使用该函数
	HANDLE hFind;
	WIN32_FIND_DATA w32FileData;

	CString FilePath = m_QuanPath;
	//FilePath += L"\\*";

	hFind = FindFirstFile(FilePath, &w32FileData);
	//如果没有找到直接退出
	if (hFind == (HANDLE)-1) return;

	do {
		if (!wcscmp(w32FileData.cFileName, L".") || !wcscmp(w32FileData.cFileName, L".."))//避免遍历隐藏的本目录路径以及父目录路径
			continue;
		//判断是否是文件夹
		if (w32FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			CString FilePathNext;
			//CString str={};
			FilePathNext = FilePath; -
				FilePathNext.Delete(FilePathNext.GetLength() - 1);
			FilePathNext += w32FileData.cFileName;
			//m_Path_Name.Format(L"查找到%s\r\n\r\n", FilePathNext);
			FilePathNext += L"\\*";
			m_QuanPath = FilePathNext;
			//递归遍历下一个文件夹
			QuanLuJCX();
		}
		else
		{
			CString FileName = {};
			CString str = {};
			FileName = FilePath;
			FileName.Delete(FilePath.GetLength() - 1);

			FileName += w32FileData.cFileName;//把文件名追加到文件夹上
			char str1[40] = {};
			wsprintfA(str1, "%S", FileName);
			char *str2=md5.GetFilePath(str1);
			//开始和取出的文件做对比
			for (int i = 0; i < m_BingDuShuz.size(); i++) {
				if (!strcmp(str2, m_BingDuShuz[i])) {
					DeleteFile(FileName);
					m_TZ += w32FileData.cFileName;
					m_TZ += L"是病毒已删除!\r\n";

				}
			}
			

		}
	} while (FindNextFile(hFind, &w32FileData));
}
