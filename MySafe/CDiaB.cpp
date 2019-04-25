// CDiaB.cpp: 实现文件
//

#include "stdafx.h"
#include "MySafe.h"
#include "CDiaB.h"
#include "afxdialogex.h"


// CDiaB 对话框

IMPLEMENT_DYNAMIC(CDiaB, CDialogEx)

CDiaB::CDiaB(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_Path_Name(_T(""))
	, m_VS_Path(_T(""))
	, m_VS_Clear(_T(""))
{

}

CDiaB::~CDiaB()
{
}

void CDiaB::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_LIST1, m_List_System);
	//  DDX_Control(pDX, IDC_EDIT1, m_Path_L_J);
	DDX_Text(pDX, IDC_EDIT1, m_Path_Name);
	DDX_Text(pDX, IDC_EDIT2, m_VS_Path);
	DDX_Text(pDX, IDC_EDIT3, m_VS_Clear);
}


BEGIN_MESSAGE_MAP(CDiaB, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDiaB::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDiaB::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDiaB 消息处理程序

//系统垃圾清理
void CDiaB::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str[3] = { L"C:\\Windows\\Temp\\*",L"C:\\Users\\iyue\\AppData\\Local\\Temp\\*",
	L"C:\\Users\\iyue\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\Cache\\*"};
	//清理上次留下的痕迹
	UpdateData(TRUE);
	m_Path_Name.Empty();
	UpdateData(FALSE);

	UpdateData(TRUE);
	for (int i = 0; i <3; i++) {
		FindAndDelete(&str[i]);
	}
	Clear_file();
	UpdateData(FALSE);
	
}

//VS垃圾清理
void CDiaB::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_VS_Path.Empty();
	m_VS_Clear.Empty();
	UpdateData(FALSE);
	UpdateData(TRUE);

	m_VS_Path += L"\\*";
	Clear_VS();
	UpdateData(FALSE);
	m_VS_Path.Empty();
	
	UpdateData(FALSE);

}

void CDiaB::FindAndDelete(CString * Path)
{

	setlocale(LC_ALL, "chs");//注意系统信息中需要输出中文的地方需要使用该函数
	HANDLE hFind;
	WIN32_FIND_DATA w32FileData;

	
	CString FilePath=*Path;
	//FilePath += L"\\*";

	hFind = FindFirstFile(FilePath, &w32FileData);
	//如果没有找到直接退出
	if (hFind == (HANDLE)-1) return ;

	do {
		if (!wcscmp(w32FileData.cFileName, L".") || !wcscmp(w32FileData.cFileName, L".."))//避免遍历隐藏的本目录路径以及父目录路径
			continue;
		//判断是否是文件夹
		if (w32FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			CString FilePathNext;
			//CString str={};
			FilePathNext = FilePath;
			FilePathNext.Delete(FilePathNext.GetLength() - 1);
			FilePathNext += w32FileData.cFileName;
			//m_Path_Name.Format(L"查找到%s\r\n\r\n", FilePathNext);
			FilePathNext += L"\\*";
			
			//递归遍历下一个文件夹
			FindAndDelete(&FilePathNext);
		}
		else
		{
			CString FileName;
			CString str={};
			FileName=FilePath;
			FileName.Delete(FilePath.GetLength() - 1);
	
			FileName+=w32FileData.cFileName;//把文件名追加到文件夹上
			str.Format(L"查找到%s准备删除\r\n\r\n", w32FileData.cFileName);
			m_Path_Name+=str;
			str = L"";
			if(DeleteFile(FileName))
			{
				str.Format(L"%s删除成功\r\n\r\n", w32FileData.cFileName);
				m_Path_Name += str;
			}else
			{
				str = L"";
				str.Format(L"%s删除失败\r\n\r\n", w32FileData.cFileName);
				m_Path_Name += str;
			}

			
		}
	} while (FindNextFile(hFind, &w32FileData));

}

void CDiaB::Clear_file()
{
	//清理运行记录
	SHDeleteKey(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU"));
	m_Path_Name += L"清理运行记录\r\n\r\n";
	//清理回收站垃圾
	SHEmptyRecycleBin(NULL, NULL,
		SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
	m_Path_Name += L"回收站已清理!\r\n";

}

void CDiaB::Clear_VS()
{
	//setlocale(LC_ALL, "chs");//注意系统信息中需要输出中文的地方需要使用该函数
	HANDLE hFind;
	WIN32_FIND_DATA w32FileData;


	CString FilePath = m_VS_Path;
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
			FilePathNext = FilePath;
			FilePathNext.Delete(FilePathNext.GetLength() - 1);
			FilePathNext += w32FileData.cFileName;
			//m_Path_Name.Format(L"查找到%s\r\n\r\n", FilePathNext);
			FilePathNext += L"\\*";
			m_VS_Path = FilePathNext;
			//递归遍历下一个文件夹
			Clear_VS();
		}
		else
		{
			CString FileName={};
			CString str = {};
			FileName = FilePath;
			FileName.Delete(FilePath.GetLength() - 1);

			FileName += w32FileData.cFileName;//把文件名追加到文件夹上
			//str.Format(L"查找到%s准备删除\r\n\r\n", w32FileData.cFileName);
			//m_Path_Name += str;
			LPWSTR FileSuffix = PathFindExtension(FileName);

			if(!lstrcmp(FileSuffix,L".tlog")|| !lstrcmp(FileSuffix, L".obj")||!lstrcmp(FileSuffix, L".log")
				|| !lstrcmp(FileSuffix, L".pch")|| !lstrcmp(FileSuffix, L".ilk")
				|| !lstrcmp(FileSuffix, L".pdb")|| !lstrcmp(FileSuffix, L".idb"))
			{
				str = L"";
				if (DeleteFile(FileName))
				{
					str.Format(L"%s删除成功\r\n\r\n", w32FileData.cFileName);
					m_VS_Clear += str;
					UpdateData(FALSE);
				}
				else
				{
					str = L"";
					str.Format(L"%s删除失败文件被占用或者无权限\r\n\r\n", w32FileData.cFileName);
					m_VS_Clear += str;
					UpdateData(FALSE);
				}
			}

		}
	} while (FindNextFile(hFind, &w32FileData));
}
